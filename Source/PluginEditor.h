#pragma once
#include <JuceHeader.h>
#include "PluginProcessor.h"

class NeuralAmpAudioProcessorEditor : public juce::AudioProcessorEditor
{
public:
    NeuralAmpAudioProcessorEditor (NeuralAmpAudioProcessor&);
    ~NeuralAmpAudioProcessorEditor() override;

    void paint (juce::Graphics&) override;
    void resized() override;

private:
    NeuralAmpAudioProcessor& audioProcessor;
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (NeuralAmpAudioProcessorEditor)
    std::unique_ptr<juce::AudioProcessorValueTreeState::SliderAttachment> sliderAttachment;
    juce::Slider gainSlider;
    juce::Label gainLabel;
};