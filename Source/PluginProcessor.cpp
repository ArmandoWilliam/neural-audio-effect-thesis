#include "PluginProcessor.h"
#include "PluginEditor.h"

NeuralGuitarAudioProcessor::NeuralGuitarAudioProcessor()
    : AudioProcessor (BusesProperties()
        .withInput  ("Input",  juce::AudioChannelSet::mono(), true)
        .withOutput ("Output", juce::AudioChannelSet::mono(), true)), 
        apvts(*this, nullptr, "apvts", createParameterLayout()), 
        gainValue(apvts.getRawParameterValue("gain")),
        toneValue(apvts.getRawParameterValue("tone")),
        qFactor(apvts.getRawParameterValue("qfactor")), 
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
                    // LSTM_left.load_json(jsonPath.toRawUTF8());
                    // LSTM_right.load_json(jsonPath.toRawUTF8());
                    // LSTM_left.reset();
                    // LSTM_right.reset();
                }
            }

    }
    catch (const std::exception& e) {
        std::cout << e.what();
    }

    gain.reset(sampleRate, 0.05);

    //configure the filter
    float frequencyValue = toneValue->load();
    postIIRFilter.setCoefficients(juce::IIRCoefficients::makeLowPass(sampleRate, frequencyValue, *qFactor));
    // postIIRFilterLeft.setCoefficients(juce::IIRCoefficients::makeLowPass(sampleRate, frequencyValue, *qFactor));
    // postIIRFilterRight.setCoefficients(juce::IIRCoefficients::makeLowPass(sampleRate, frequencyValue, *qFactor));
}
void NeuralGuitarAudioProcessor::releaseResources() {}

bool NeuralGuitarAudioProcessor::isBusesLayoutSupported (const BusesLayout& layouts) const
{
    if (layouts.getMainOutputChannelSet() != juce::AudioChannelSet::mono())
        return false;

    if (layouts.getMainOutputChannelSet() != layouts.getMainInputChannelSet())
        return false;

    return true;
}

void NeuralGuitarAudioProcessor::processBlock (juce::AudioBuffer<float>& buffer, juce::MidiBuffer&)
{
    juce::ScopedNoDenormals noDenormals;
    auto totalNumInputChannels  = getTotalNumInputChannels();
    auto totalNumOutputChannels = getTotalNumOutputChannels();  
    
    bool isNeural = neuralOn->load() > 0.5f;

    // mute input channels not linked to output
    for (auto i = totalNumInputChannels; i < totalNumOutputChannels; ++i)
        buffer.clear (i, 0, buffer.getNumSamples());

    float dBValue = gainValue->load();
    // convert the dBValue in a linear scale to use it in the Knob SmoothedValue
    float linearValue = juce::Decibels::decibelsToGain(dBValue);

    // load the drive value
    float driveValue = drive->load();
    
    //laod the frequency value
    float frequencyValue = toneValue->load();
    
    // set a target value where the final gain will stabilize
    gain.setTargetValue(linearValue);

    // set the coefficients of the pre-filters
    preIIRFilter.setCoefficients(juce::IIRCoefficients::makeLowPass(this->getSampleRate(), frequencyValue, *qFactor));
    // preIIRFilterLeft.setCoefficients(juce::IIRCoefficients::makeLowPass(this->getSampleRate(), frequencyValue, *qFactor));
    // preIIRFilterRight.setCoefficients(juce::IIRCoefficients::makeLowPass(this->getSampleRate(), frequencyValue, *qFactor));  
    
    // set the coefficients of the post-filters
    postIIRFilter.setCoefficients(juce::IIRCoefficients::makeLowPass(this->getSampleRate(), frequencyValue, *qFactor));
    // postIIRFilterLeft.setCoefficients(juce::IIRCoefficients::makeLowPass(this->getSampleRate(), frequencyValue, *qFactor));
    // postIIRFilterRight.setCoefficients(juce::IIRCoefficients::makeLowPass(this->getSampleRate(), frequencyValue, *qFactor));


    // *********************************************
    // LSTM -> Gain -> Pre-Filter -> Drive -> Post-Filter
    // *********************************************

    

    if (isNeural) {

        // temporary buffer to store the processed audio, I need to create a copy todo the forward propagation
        juce::AudioBuffer<float> tempBuffer(1, buffer.getNumSamples());

        LSTM.process(buffer.getReadPointer(0), tempBuffer.getWritePointer(0), buffer.getNumSamples());
        // LSTM_left.process(buffer.getReadPointer(0), tempBuffer.getWritePointer(0), buffer.getNumSamples());
        // LSTM_right.process(buffer.getReadPointer(1), tempBuffer.getWritePointer(1), buffer.getNumSamples());

        buffer.copyFrom(0, 0, tempBuffer, 0, 0, buffer.getNumSamples());
        // buffer.copyFrom(0, 0, tempBuffer, 0, 0, buffer.getNumSamples());
        // buffer.copyFrom(1, 0, tempBuffer, 1, 0, buffer.getNumSamples());
    }

    // for (auto i = 0; i < totalNumInputChannels; ++i) {
    float *smoothedValuePointer = buffer.getWritePointer(0);
    for (auto j = 0; j < buffer.getNumSamples(); ++j) {
        // Gain
        smoothedValuePointer[j] *= gain.getNextValue();

        // Pre-Filter
        smoothedValuePointer[j] = preIIRFilter.processSingleSampleRaw(smoothedValuePointer[j]);
        // if (i == 0)
        // left channel
            // smoothedValuePointer[j] = preIIRFilterLeft.processSingleSampleRaw(smoothedValuePointer[j]);
        // else
        // right channel
            // smoothedValuePointer[j] = preIIRFilterRight.processSingleSampleRaw(smoothedValuePointer[j]);

        if (!isNeural)
        {
            //Drive
            smoothedValuePointer[j] *= driveValue;
            smoothedValuePointer[j] = std::tanh(smoothedValuePointer[j]);
        }
        

        // Post-Filter
        smoothedValuePointer[j] = postIIRFilter.processSingleSampleRaw(smoothedValuePointer[j]);
        // if (i == 0)
            // smoothedValuePointer[j] = postIIRFilterLeft.processSingleSampleRaw(smoothedValuePointer[j]);
        // else
            // smoothedValuePointer[j] = postIIRFilterRight.processSingleSampleRaw(smoothedValuePointer[j]);
    }
    // }
    

    
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