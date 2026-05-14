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
    toneSlider.setTextBoxStyle(juce::Slider::TextBoxBelow, true, 80, 20);
    addAndMakeVisible(toneSlider);
    toneLabel.setText("Tone", juce::dontSendNotification);
    addAndMakeVisible(toneLabel);
    toneAttachment = std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment>(audioProcessor.apvts, "tone", toneSlider);

    qLabel.setJustificationType(juce::Justification::centred);
    qSlider.setSliderStyle(juce::Slider::RotaryVerticalDrag);
    qSlider.setTextBoxStyle(juce::Slider::TextBoxBelow, true, 50, 50);
    addAndMakeVisible(qSlider);
    qLabel.setText("Q", juce::dontSendNotification);
    addAndMakeVisible(qLabel);
    qAttachment = std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment>(audioProcessor.apvts, "qfactor", qSlider);
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
    gainSlider.setBounds(200, 50, 100, 100);
    gainLabel.setBounds(200, 30, 100, 20);

    toneSlider.setBounds(100, 50, 100, 100);
    toneLabel.setBounds(100, 30, 100, 20);

    qSlider.setBounds(0, 50, 100, 100);
    qLabel.setBounds(0, 30, 80, 20);
}