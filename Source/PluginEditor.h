/*
  ==============================================================================

    This file contains the basic framework code for a JUCE plugin editor.

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>
#include "PluginProcessor.h"
#include "Dial.h"
#include "AudioOscilloscope.h"


//==============================================================================
/**
*/


class SwellerAudioProcessorEditor  : public juce::AudioProcessorEditor,
                                    private juce::Timer
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
    void timerCallback() override;
    
    SwellerAudioProcessor& audioProcessor;
    juce::UndoManager& undoManager;
    
    Dial dryDial;
    Dial wetDial;
    Dial feedbackDial;
    Dial phaseDial;
    Dial cutoffDial;
    Dial qDial;
    
    AudioOscilloscope drySignalScope;
    AudioOscilloscope wetSignalScope;
 
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (SwellerAudioProcessorEditor)
};




