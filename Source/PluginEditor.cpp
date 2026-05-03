#include "PluginProcessor.h"
#include "PluginEditor.h"

NeuralAmpAudioProcessorEditor::NeuralAmpAudioProcessorEditor (NeuralAmpAudioProcessor& p)
    : AudioProcessorEditor (&p), audioProcessor (p)
{
    setSize (400, 200);
    gainLabel.setJustificationType(juce::Justification::centred);
    gainSlider.setSliderStyle(juce::Slider::RotaryVerticalDrag);
    gainSlider.setTextBoxStyle(juce::Slider::TextBoxBelow, true, 80, 20);
    addAndMakeVisible(gainSlider);
    gainLabel.setText("Gain", juce::dontSendNotification);
    addAndMakeVisible(gainLabel);
    sliderAttachment = std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment>(audioProcessor.apvts, "gain", gainSlider);
}

NeuralAmpAudioProcessorEditor::~NeuralAmpAudioProcessorEditor() {}

void NeuralAmpAudioProcessorEditor::paint (juce::Graphics& g)
{
    g.fillAll (getLookAndFeel().findColour (juce::ResizableWindow::backgroundColourId));
    g.setColour (juce::Colours::white);
    g.setFont (juce::FontOptions (20.0f));
    g.drawFittedText ("NeuralAmp v0.1", getLocalBounds().removeFromTop(30), juce::Justification::centred, 1);
}

void NeuralAmpAudioProcessorEditor::resized() {
    gainSlider.setBounds(150, 50, 100, 100);
    gainLabel.setBounds(150, 30, 100, 20);
}