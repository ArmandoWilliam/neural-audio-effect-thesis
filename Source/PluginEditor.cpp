#include "PluginProcessor.h"
#include "PluginEditor.h"

NeuralGuitarAudioProcessorEditor::NeuralGuitarAudioProcessorEditor (NeuralGuitarAudioProcessor& p)
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

    toneLabel.setJustificationType(juce::Justification::centred);
    toneSlider.setSliderStyle(juce::Slider::RotaryVerticalDrag);
    toneSlider.setTextBoxStyle(juce::Slider::TextBoxBelow, true, 50, 50);
    addAndMakeVisible(toneSlider);
    toneLabel.setText("Tone", juce::dontSendNotification);
    addAndMakeVisible(toneLabel);
    sliderAttachment = std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment>(audioProcessor.apvts, "tone", toneSlider);
}

NeuralGuitarAudioProcessorEditor::~NeuralGuitarAudioProcessorEditor() {}

void NeuralGuitarAudioProcessorEditor::paint (juce::Graphics& g)
{
    g.fillAll (getLookAndFeel().findColour (juce::ResizableWindow::backgroundColourId));
    g.setColour (juce::Colours::white);
    g.setFont (juce::FontOptions (20.0f));
    g.drawFittedText ("NeuralGuitar v0.1", getLocalBounds().removeFromTop(30), juce::Justification::centred, 1);
}

void NeuralGuitarAudioProcessorEditor::resized() {
    gainSlider.setBounds(150, 50, 100, 100);
    gainLabel.setBounds(150, 30, 100, 20);
}