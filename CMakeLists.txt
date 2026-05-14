cmake_minimum_required(VERSION 3.22)

project(NeuralGuitar VERSION 0.1.0)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

# Aggiungi JUCE come subdirectory
add_subdirectory(/Users/armando/GIT/JUCE ${CMAKE_BINARY_DIR}/JUCE)

# Definisci il plugin
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

# Genera l'header JuceHeader.h
juce_generate_juce_header(NeuralGuitar)

# I sorgenti del plugin
target_sources(NeuralGuitar PRIVATE
    Source/PluginProcessor.cpp
    Source/PluginProcessor.h
    Source/PluginEditor.cpp
    Source/PluginEditor.h
)

# Definizioni di compilazione standard JUCE
target_compile_definitions(NeuralGuitar PUBLIC
    JUCE_WEB_BROWSER=0
    JUCE_USE_CURL=0
    JUCE_VST3_CAN_REPLACE_VST2=0
)

# Link dei moduli JUCE necessari
target_link_libraries(NeuralGuitar PRIVATE
    juce::juce_audio_utils
    juce::juce_dsp
    juce::juce_audio_plugin_client
    PUBLIC
    juce::juce_recommended_config_flags
    juce::juce_recommended_lto_flags
    juce::juce_recommended_warning_flags
)