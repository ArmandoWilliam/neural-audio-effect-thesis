#include "PluginProcessor.h"
#include "PluginEditor.h"

NeuralGuitarAudioProcessor::NeuralGuitarAudioProcessor()
    : AudioProcessor (BusesProperties()
        .withInput  ("Input",  juce::AudioChannelSet::stereo(), true)
        .withOutput ("Output", juce::AudioChannelSet::stereo(), true)), 
        apvts(*this, nullptr, "apvts", createParameterLayout()), 
        gainValue(apvts.getRawParameterValue("gain")),
        toneValue(apvts.getRawParameterValue("tone"))
{
    
}

NeuralGuitarAudioProcessor::~NeuralGuitarAudioProcessor() {}

// Called when DAW changes sample rate or audio is stopped and played again
void NeuralGuitarAudioProcessor::prepareToPlay (double sampleRate, int sampleRatePerBlock) {
    gain.reset(sampleRate, 0.05);

    //configure the filter
    float frequencyValue = toneValue->load();
    IIRFilterLeft.setCoefficients(juce::IIRCoefficients::makeLowPass(sampleRate, frequencyValue, qFactor));
    IIRFilterRight.setCoefficients(juce::IIRCoefficients::makeLowPass(sampleRate, frequencyValue, qFactor));
}
void NeuralGuitarAudioProcessor::releaseResources() {}

bool NeuralGuitarAudioProcessor::isBusesLayoutSupported (const BusesLayout& layouts) const
{
    if (layouts.getMainOutputChannelSet() != juce::AudioChannelSet::mono()
     && layouts.getMainOutputChannelSet() != juce::AudioChannelSet::stereo())
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

    // mute input channels not linked to output
    for (auto i = totalNumInputChannels; i < totalNumOutputChannels; ++i)
        buffer.clear (i, 0, buffer.getNumSamples());

    float dBValue = gainValue->load();
    float linearValue = juce::Decibels::decibelsToGain(dBValue);
    // set a target value where the final gain will stabilize
    gain.setTargetValue(linearValue);

    for (auto i = 0; i < totalNumInputChannels; ++i) {
        float *smoothedValuePointer = buffer.getWritePointer(i);
        for (auto j = 0; j < buffer.getNumSamples(); ++j) {
            smoothedValuePointer[j] *= gain.getNextValue();
        }
    }

    float frequencyValue = toneValue->load();
    DBG("Tone frequency: " + juce::String(frequencyValue));
    IIRFilterLeft.setCoefficients(juce::IIRCoefficients::makeLowPass(this->getSampleRate(), frequencyValue, qFactor));
    IIRFilterRight.setCoefficients(juce::IIRCoefficients::makeLowPass(this->getSampleRate(), frequencyValue, qFactor));

    float *channelDataPointer = buffer.getWritePointer(0);
    // filter on the channel after the gain
    IIRFilterLeft.processSamples(channelDataPointer, buffer.getNumSamples());
    channelDataPointer = buffer.getWritePointer(1);
    IIRFilterRight.processSamples(channelDataPointer, buffer.getNumSamples());
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