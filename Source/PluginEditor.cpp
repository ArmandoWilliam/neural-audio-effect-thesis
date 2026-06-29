#include "PluginProcessor.h"
#include "PluginEditor.h"

NeuralGuitarAudioProcessorEditor::NeuralGuitarAudioProcessorEditor (NeuralGuitarAudioProcessor& p)
    : AudioProcessorEditor (&p), audioProcessor (p)
{
    qSlider.setVisible(true);
    qLabel.setVisible(true);
    toneSlider.setVisible(true);
    toneLabel.setVisible(true);
    driveSlider.setVisible(true);
    driveLabel.setVisible(true);
    
    setSize (400, 350);
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

    driveLabel.setJustificationType(juce::Justification::centred);
    driveSlider.setSliderStyle(juce::Slider::RotaryVerticalDrag);
    driveSlider.setTextBoxStyle(juce::Slider::TextBoxBelow, true, 50, 50);
    addAndMakeVisible(driveSlider);
    driveLabel.setText("Drive", juce::dontSendNotification);
    addAndMakeVisible(driveLabel);
    driveAttachment = std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment>(audioProcessor.apvts, "drive", driveSlider);
    
}

NeuralGuitarAudioProcessorEditor::~NeuralGuitarAudioProcessorEditor() {}

void NeuralGuitarAudioProcessorEditor::paint (juce::Graphics& g)
{
    g.fillAll (getLookAndFeel().findColour (juce::ResizableWindow::backgroundColourId));
    g.setColour (juce::Colours::white);
    g.setFont (juce::FontOptions (20.0f));
    g.drawFittedText ("NeuralGuitar v1.0", getLocalBounds().removeFromTop(30), juce::Justification::centred, 1);
}

void NeuralGuitarAudioProcessorEditor::resized() {
    toneSlider.setBounds(50, 50, 120, 120);
    toneLabel.setBounds(50, 30, 120, 20);
    gainSlider.setBounds(230, 50, 120, 120);
    gainLabel.setBounds(230, 30, 120, 20);

    qSlider.setBounds(55, 210, 100, 100);
    qLabel.setBounds(55, 190, 80, 20);
    driveSlider.setBounds(265, 210, 100, 100);
    driveLabel.setBounds(265, 190, 80, 20);

}