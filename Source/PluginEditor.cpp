#include "PluginProcessor.h"
#include "PluginEditor.h"

NeuralGuitarAudioProcessorEditor::NeuralGuitarAudioProcessorEditor (NeuralGuitarAudioProcessor& p)
    : AudioProcessorEditor (&p), audioProcessor (p)
{
    pedalImage = juce::ImageCache::getFromMemory(BinaryData::pedal_png, BinaryData::pedal_pngSize);
    qSlider.setVisible(true);
    qLabel.setVisible(true);
    toneSlider.setVisible(true);
    toneLabel.setVisible(true);
    driveSlider.setVisible(true);
    driveLabel.setVisible(true);
    
    setSize (660, 450);
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

    addAndMakeVisible(neuralToggle);
    neuralAttachment = std::make_unique<juce::AudioProcessorValueTreeState::ButtonAttachment>(audioProcessor.apvts, "neuralOn", neuralToggle);
    neuralToggle.onClick = [this]() { updateControlStates(); };
    neuralToggle.setLookAndFeel(&toggleSwitchLNF);

    neuralOffLabel.setText("ON", juce::dontSendNotification);
    neuralOffLabel.setJustificationType(juce::Justification::centredRight);
    addAndMakeVisible(neuralOffLabel);

    neuralOnLabel.setText("OFF", juce::dontSendNotification);
    neuralOnLabel.setJustificationType(juce::Justification::centredLeft);
    addAndMakeVisible(neuralOnLabel);

    neuralLabel.setText("NEURAL", juce::dontSendNotification);
    neuralLabel.setJustificationType(juce::Justification::centred);
    addAndMakeVisible(neuralLabel);

    updateControlStates();
}

NeuralGuitarAudioProcessorEditor::~NeuralGuitarAudioProcessorEditor() 
{
    neuralToggle.setLookAndFeel(nullptr);
}

void NeuralGuitarAudioProcessorEditor::updateControlStates()
{
    bool isNeural = neuralToggle.getToggleState();
    driveSlider.setEnabled(!isNeural);
    driveLabel.setEnabled(!isNeural);

    auto accent = juce::Colour(0xfff2c019);
    auto dim    = juce::Colours::grey;

    neuralOnLabel.setColour(juce::Label::textColourId, isNeural ? dim : accent);
    neuralOffLabel.setColour(juce::Label::textColourId, isNeural ? accent : dim);
}

void NeuralGuitarAudioProcessorEditor::paint (juce::Graphics& g)
{
    g.fillAll (getLookAndFeel().findColour (juce::ResizableWindow::backgroundColourId));
    g.setColour (juce::Colours::white);
    g.setFont (juce::FontOptions (24.0f, juce::Font::bold));
    g.drawText ("NeuralGuitar", 30, 25, 300, 30, juce::Justification::left);

    g.setColour (juce::Colours::grey);
    g.setFont (juce::FontOptions (15.0f));
    g.drawText ("SD-1 Overdrive", 30, 55, 300, 20, juce::Justification::left);

    if (pedalImage.isValid())
    {
        int imgW = 220;   // larghezza desiderata, da tarare
        int imgH = 320;   // altezza desiderata, da tarare
        int x = (getWidth()  - imgW) / 2;
        int y = (getHeight() - imgH) / 2;

        g.setOpacity(0.12f);   // basso = watermark tenue, da tarare
        g.drawImage(pedalImage, x, y, imgW, imgH,
                    0, 0, pedalImage.getWidth(), pedalImage.getHeight());
        g.setOpacity(1.0f);    // ripristina, se no i testi dopo restano trasparenti
    }
}

void NeuralGuitarAudioProcessorEditor::resized() {

    neuralToggle.setBounds(540, 35, 60, 30);

    toneSlider.setBounds(40, 150, 120, 120);
    toneLabel.setBounds(40, 125, 120, 20);
    gainSlider.setBounds(500, 150, 120, 120);
    gainLabel.setBounds(500, 125, 120, 20);

    neuralLabel.setBounds(540, 10, 60, 20);
    neuralOnLabel.setBounds(440, 35, 50, 30);
    neuralOffLabel.setBounds(605, 35, 50, 30);

    qSlider.setBounds(40, 320, 120, 120);
    qLabel.setBounds(40, 295, 120, 20);
    driveSlider.setBounds(500, 320, 120, 120);
    driveLabel.setBounds(500, 295, 120, 20);
}