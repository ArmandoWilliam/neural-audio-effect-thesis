#include "PluginProcessor.h"
#include "PluginEditor.h"

NeuralAmpAudioProcessor::NeuralAmpAudioProcessor()
    : AudioProcessor (BusesProperties()
        .withInput  ("Input",  juce::AudioChannelSet::stereo(), true)
        .withOutput ("Output", juce::AudioChannelSet::stereo(), true)), 
        apvts(*this, nullptr, "apvts", createParameterLayout()), 
        gainValue(apvts.getRawParameterValue("gain"))
{
    
}

NeuralAmpAudioProcessor::~NeuralAmpAudioProcessor() {}

// Called when DAW changes sample rate ora audio is stopped and played again
void NeuralAmpAudioProcessor::prepareToPlay (double sampleRate, int sampleRatePerBlock) {
    gain.reset(sampleRate, 0.05);
}
void NeuralAmpAudioProcessor::releaseResources() {}

bool NeuralAmpAudioProcessor::isBusesLayoutSupported (const BusesLayout& layouts) const
{
    if (layouts.getMainOutputChannelSet() != juce::AudioChannelSet::mono()
     && layouts.getMainOutputChannelSet() != juce::AudioChannelSet::stereo())
        return false;

    if (layouts.getMainOutputChannelSet() != layouts.getMainInputChannelSet())
        return false;

    return true;
}

void NeuralAmpAudioProcessor::processBlock (juce::AudioBuffer<float>& buffer, juce::MidiBuffer&)
{
    juce::ScopedNoDenormals noDenormals;
    auto totalNumInputChannels  = getTotalNumInputChannels();
    auto totalNumOutputChannels = getTotalNumOutputChannels();

    for (auto i = totalNumInputChannels; i < totalNumOutputChannels; ++i)
        buffer.clear (i, 0, buffer.getNumSamples());

    float dBValue = gainValue->load();
    float linearValue = juce::Decibels::decibelsToGain(dBValue);
    // set a target value where the final gain will stabilize
    gain.setTargetValue(linearValue);
    DBG (linearValue);

    for (auto i = 0; i < totalNumInputChannels; ++i) {
        float *smoothedValuePointer = buffer.getWritePointer(i);
        for (auto j = 0; j < buffer.getNumSamples(); ++j) {
            smoothedValuePointer[j] *= gain.getNextValue();
        }
    }
}

juce::AudioProcessorEditor* NeuralAmpAudioProcessor::createEditor()
{
    return new NeuralAmpAudioProcessorEditor (*this);
}

void NeuralAmpAudioProcessor::getStateInformation (juce::MemoryBlock&) {}
void NeuralAmpAudioProcessor::setStateInformation (const void*, int) {}

juce::AudioProcessor* JUCE_CALLTYPE createPluginFilter()
{
    return new NeuralAmpAudioProcessor();
}