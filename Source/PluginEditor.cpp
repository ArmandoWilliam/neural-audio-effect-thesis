#include "PluginProcessor.h"
#include "PluginEditor.h"

NeuralAmpAudioProcessorEditor::NeuralAmpAudioProcessorEditor (NeuralAmpAudioProcessor& p)
    : AudioProcessorEditor (&p), audioProcessor (p)
{
    setSize (400, 200);
}

NeuralAmpAudioProcessorEditor::~NeuralAmpAudioProcessorEditor() {}

void NeuralAmpAudioProcessorEditor::paint (juce::Graphics& g)
{
    g.fillAll (getLookAndFeel().findColour (juce::ResizableWindow::backgroundColourId));
    g.setColour (juce::Colours::white);
    g.setFont (juce::FontOptions (20.0f));
    g.drawFittedText ("NeuralAmp v0.1", getLocalBounds(), juce::Justification::centred, 1);
}

void NeuralAmpAudioProcessorEditor::resized() {}