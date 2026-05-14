#pragma once
#include <JuceHeader.h>
#include "PluginProcessor.h"

class NeuralGuitarAudioProcessorEditor : public juce::AudioProcessorEditor
{
public:
    NeuralGuitarAudioProcessorEditor (NeuralGuitarAudioProcessor&);
    ~NeuralGuitarAudioProcessorEditor() override;

    void paint (juce::Graphics&) override;
    void resized() override;

private:
    NeuralGuitarAudioProcessor& audioProcessor;
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (NeuralGuitarAudioProcessorEditor)
    juce::Slider gainSlider;
    juce::Label gainLabel;
    juce::Slider toneSlider;
    juce::Label toneLabel;

    std::unique_ptr<juce::AudioProcessorValueTreeState::SliderAttachment> sliderAttachment;
    std::unique_ptr<juce::AudioProcessorValueTreeState::SliderAttachment> toneAttachment;
};