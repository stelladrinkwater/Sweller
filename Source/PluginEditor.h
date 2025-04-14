/*
  ==============================================================================

    This file contains the basic framework code for a JUCE plugin editor.

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>
#include "PluginProcessor.h"

//==============================================================================
/**
*/


class SwellerAudioProcessorEditor  : public juce::AudioProcessorEditor
{
public:
    SwellerAudioProcessorEditor (SwellerAudioProcessor& p,
                                    juce::AudioProcessorValueTreeState& state,
                                    juce::UndoManager& um);
 
    ~SwellerAudioProcessorEditor() override;
 
    void paint (juce::Graphics&) override;
    void resized() override;
 
    bool keyPressed (const juce::KeyPress& key) override;
 
private:
    SwellerAudioProcessor& audioProcessor;
    juce::UndoManager& undoManager;
 
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (SwellerAudioProcessorEditor)
};
