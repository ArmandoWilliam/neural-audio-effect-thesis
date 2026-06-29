#pragma once
#include <JuceHeader.h>
#include "RTNeuralLSTM.h"

class NeuralGuitarAudioProcessor : public juce::AudioProcessor
{
public:
    NeuralGuitarAudioProcessor();
    ~NeuralGuitarAudioProcessor() override;
    juce::AudioProcessorValueTreeState apvts;
    juce::LinearSmoothedValue<float> gain  { 0.05f };

    static juce::AudioProcessorValueTreeState::ParameterLayout createParameterLayout() {
        // crea i parametri e li restituisce
        return {
            std::make_unique<AudioParameterFloat> (ParameterID { "gain",  1 }, "Gain",  NormalisableRange<float> (-24.0f, 24.0f, 0.01f), 0.0f),
            std::make_unique<AudioParameterFloat> (ParameterID { "tone",  2 }, "Tone",  NormalisableRange<float> (0.0f, 20000.0f, 0.01f), 4000.0f),
            std::make_unique<AudioParameterFloat> (ParameterID { "qfactor",  3 }, "qFactor",  NormalisableRange<float> (0.1f, 10.0f, 0.01f), 0.707f),
            std::make_unique<AudioParameterFloat> (ParameterID { "drive",  4 }, "drive",  NormalisableRange<float> (1.0f, 50.0f, 1.0f), 1.0f),
        };
    }

    void prepareToPlay (double sampleRate, int samplesPerBlock) override;
    void releaseResources() override;
    bool isBusesLayoutSupported (const BusesLayout& layouts) const override;
    void processBlock (juce::AudioBuffer<float>&, juce::MidiBuffer&) override;

    juce::AudioProcessorEditor* createEditor() override;
    bool hasEditor() const override { return true; }

    const juce::String getName() const override { return "NeuralGuitar"; }
    bool acceptsMidi() const override  { return false; }
    bool producesMidi() const override { return false; }
    bool isMidiEffect() const override { return false; }
    double getTailLengthSeconds() const override { return 0.0; }

    int getNumPrograms() override { return 1; }
    int getCurrentProgram() override { return 0; }
    void setCurrentProgram (int) override {}
    const juce::String getProgramName (int) override { return {}; }
    void changeProgramName (int, const juce::String&) override {}

    void getStateInformation (juce::MemoryBlock& destData) override;
    void setStateInformation (const void* data, int sizeInBytes) override;

    RT_LSTM LSTM;
    // RT_LSTM LSTM_left;
    // RT_LSTM LSTM_right;

    const char* char_filename = "";
    juce::File json_file;
private:
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(NeuralGuitarAudioProcessor)
    std::atomic<float>* gainValue;
    std::atomic<float>* toneValue;
    std::atomic<float>* qFactor;
    std::atomic<float>* drive;
    //Infinite Inpulse Response filter to simulte a low pass
    juce::IIRFilter preIIRFilter;
    // juce::IIRFilter preIIRFilterLeft;
    // juce::IIRFilter preIIRFilterRight;
    juce::IIRFilter postIIRFilter;
    // juce::IIRFilter postIIRFilterLeft;
    // juce::IIRFilter postIIRFilterRight;
};