cmake_minimum_required(VERSION 3.22)

project(NeuralGuitar VERSION 0.1.0)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

# Add JUCE as a subdirectory
add_subdirectory(JUCE ${CMAKE_BINARY_DIR}/JUCE)

# Add RTNeural as a subdirectory
add_subdirectory(RTNeural)

# Define the plugin
juce_add_plugin(NeuralGuitar
    COMPANY_NAME "ArmandoWilliamDev"
    BUNDLE_ID com.armandowilliamdev.neuralguitar
    PLUGIN_MANUFACTURER_CODE Armt
    PLUGIN_CODE Namp
    FORMATS AU VST3 Standalone
    PRODUCT_NAME "NeuralGuitar"
    IS_SYNTH FALSE
    NEEDS_MIDI_INPUT FALSE
    NEEDS_MIDI_OUTPUT FALSE
    COPY_PLUGIN_AFTER_BUILD TRUE
)

juce_add_binary_data(NeuralGuitarData
    SOURCES
        Assets/pedal.png
)

# Gen the header JuceHeader.h
juce_generate_juce_header(NeuralGuitar)

# Sources files
target_sources(NeuralGuitar PRIVATE
    Source/PluginProcessor.cpp
    Source/PluginProcessor.h
    Source/PluginEditor.cpp
    Source/PluginEditor.h
    Source/RTNeuralLSTM.cpp
    Source/RTNeuralLSTM.h
)

# Definitions of standard compilations in JUCE
target_compile_definitions(NeuralGuitar PUBLIC
    JUCE_WEB_BROWSER=0
    JUCE_USE_CURL=0
    JUCE_VST3_CAN_REPLACE_VST2=0
)

# Link the necessary JUCE modules
target_link_libraries(NeuralGuitar PRIVATE
    juce::juce_audio_utils
    juce::juce_dsp
    juce::juce_audio_plugin_client
    PUBLIC
    juce::juce_recommended_config_flags
    juce::juce_recommended_lto_flags
    juce::juce_recommended_warning_flags
    RTNeural
    NeuralGuitarData
)

target_include_directories(NeuralGuitar PRIVATE
    RTNeural
)