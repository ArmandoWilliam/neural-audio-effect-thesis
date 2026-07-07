#include "PluginProcessor.h"
#include "PluginEditor.h"

NeuralGuitarAudioProcessor::NeuralGuitarAudioProcessor()
    : AudioProcessor (BusesProperties()
        .withInput  ("Input",  juce::AudioChannelSet::stereo(), true)
        .withOutput ("Output", juce::AudioChannelSet::stereo(), true)), 
        apvts(*this, nullptr, "apvts", createParameterLayout()), 
        gainValue(apvts.getRawParameterValue("gain")),
        toneValue(apvts.getRawParameterValue("tone")),
        drive(apvts.getRawParameterValue("drive")),
        neuralOn(apvts.getRawParameterValue("neuralOn"))
{
    
}

NeuralGuitarAudioProcessor::~NeuralGuitarAudioProcessor() {}

// Called when DAW changes sample rate or audio is stopped and played again
void NeuralGuitarAudioProcessor::prepareToPlay (double sampleRate, int sampleRatePerBlock) {

    File userAppDataDirectory = File::getSpecialLocation(File::userDocumentsDirectory).getChildFile(JucePlugin_Manufacturer).getChildFile(JucePlugin_Name);
    File path = userAppDataDirectory.getFullPathName();

    try {
        if (userAppDataDirectory.isDirectory()) {
                juce::Array<juce::File> results;
                userAppDataDirectory.findChildFiles(results, juce::File::findFiles, false, "*.json");
                if (results.size() > 0) {
                    json_file = results[0];
                    juce::String jsonPath = json_file.getFullPathName();
                    LSTM.load_json(jsonPath.toRawUTF8());
                    LSTM.reset();
                }
            }

    }
    catch (const std::exception& e) {
        std::cout << e.what();
    }

    gain.reset(sampleRate, 0.05);

    //configure the filter
    float frequencyValue = toneValue->load();
    float qFromTone = 0.01f + (frequencyValue - 500.0f) * (1.49f / 6500.0f);
    postIIRFilter.setCoefficients(juce::IIRCoefficients::makeLowPass(sampleRate, frequencyValue, qFromTone));
}
void NeuralGuitarAudioProcessor::releaseResources() {}

bool NeuralGuitarAudioProcessor::isBusesLayoutSupported (const BusesLayout& layouts) const
{
    auto in  = layouts.getMainInputChannelSet();
    auto out = layouts.getMainOutputChannelSet();

    if (in != juce::AudioChannelSet::mono() && in != juce::AudioChannelSet::stereo())
        return false;
    if (out != juce::AudioChannelSet::mono() && out != juce::AudioChannelSet::stereo())
        return false;

    if (in.size() > out.size())
        return false;

    return true;
}

void NeuralGuitarAudioProcessor::processBlock (juce::AudioBuffer<float>& buffer, juce::MidiBuffer&)
{
    juce::ScopedNoDenormals noDenormals;
    auto totalNumInputChannels  = getTotalNumInputChannels();
    auto totalNumOutputChannels = getTotalNumOutputChannels(); 
    constexpr float softG = 1.5f;
    const float softNorm = 1.0f / std::asinh(softG);
    
    bool isNeural = neuralOn->load() > 0.5f;

    // mute input channels not linked to output
    // for (auto i = totalNumInputChannels; i < totalNumOutputChannels; ++i)
    //    buffer.clear (i, 0, buffer.getNumSamples());

    float dBValue = gainValue->load();
    // convert the dBValue in a linear scale to use it in the Knob SmoothedValue
    float linearValue = juce::Decibels::decibelsToGain(dBValue);

    // load the drive value
    float driveValue = drive->load();
    
    //laod the frequency value
    float frequencyValue = toneValue->load();

    float qFromTone = 0.01f + (frequencyValue - 500.0f) * (1.49f / 6500.0f);
    
    // set a target value where the final gain will stabilize
    gain.setTargetValue(linearValue);

    // set the coefficients of the pre-filters
    preIIRFilter.setCoefficients(juce::IIRCoefficients::makeLowPass(this->getSampleRate(), frequencyValue, qFromTone));  
    
    // set the coefficients of the post-filters
    postIIRFilter.setCoefficients(juce::IIRCoefficients::makeLowPass(this->getSampleRate(), frequencyValue, qFromTone));


    // *********************************************
    // LSTM -> Gain -> Pre-Filter -> Drive -> Post-Filter
    // *********************************************

    

    if (isNeural) {

        // temporary buffer to store the processed audio, I need to create a copy todo the forward propagation
        juce::AudioBuffer<float> tempBuffer(1, buffer.getNumSamples());

        LSTM.process(buffer.getReadPointer(0), tempBuffer.getWritePointer(0), buffer.getNumSamples());

        buffer.copyFrom(0, 0, tempBuffer, 0, 0, buffer.getNumSamples());

    }

    float *smoothedValuePointer = buffer.getWritePointer(0);
    for (auto j = 0; j < buffer.getNumSamples(); ++j) {

        if (!isNeural) {
            // Transistor Saturation, inside the SD-1 schematic, always applies
            smoothedValuePointer[j] = std::asinh(softG * smoothedValuePointer[j]) * softNorm;
        }

        if (!isNeural)
        {
            //Drive
            smoothedValuePointer[j] *= driveValue;
            smoothedValuePointer[j] = std::tanh(smoothedValuePointer[j]);
        }

        // Pre-Filter
        smoothedValuePointer[j] = preIIRFilter.processSingleSampleRaw(smoothedValuePointer[j]);

        // Gain
        smoothedValuePointer[j] *= gain.getNextValue();

        // Post-Filter
        // smoothedValuePointer[j] = postIIRFilter.processSingleSampleRaw(smoothedValuePointer[j]);

    }

    for (int ch = 1; ch < totalNumOutputChannels; ++ch)
        buffer.copyFrom(ch, 0, buffer, 0, 0, buffer.getNumSamples());

    
}

juce::AudioProcessorEditor* NeuralGuitarAudioProcessor::createEditor()
{
    return new NeuralGuitarAudioProcessorEditor (*this);
}

void NeuralGuitarAudioProcessor::getStateInformation (juce::MemoryBlock&) {}
void NeuralGuitarAudioProcessor::setStateInformation (const void*, int) {}

juce::AudioProcessor* JUCE_CALLTYPE createPluginFilter()
{
    return new NeuralGuitarAudioProcessor();
}