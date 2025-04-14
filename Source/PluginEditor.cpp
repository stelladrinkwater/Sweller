#include "PluginProcessor.h"
#include "PluginEditor.h"
#include "MyColours.h"
 
SwellerAudioProcessorEditor::SwellerAudioProcessorEditor (SwellerAudioProcessor& p,
                                                                juce::AudioProcessorValueTreeState& state,
                                                                juce::UndoManager& um)
    : AudioProcessorEditor (&p), audioProcessor (p), undoManager (um)
{
    setWantsKeyboardFocus (true);
    setSize (440, 280);
}
 
SwellerAudioProcessorEditor::~SwellerAudioProcessorEditor()
{
}
 
void SwellerAudioProcessorEditor::paint (juce::Graphics& g)
{
    g.fillAll (MyColours::black);
}
 
void SwellerAudioProcessorEditor::resized()
{
}
 
bool SwellerAudioProcessorEditor::keyPressed (const juce::KeyPress& key)
{
    const auto cmdZ = juce::KeyPress { 'z', juce::ModifierKeys::commandModifier, 0 };
 
    if (key == cmdZ && undoManager.canUndo())
    {
        undoManager.undo();
        return true;
    }
 
    const auto cmdShiftZ = juce::KeyPress { 'z', juce::ModifierKeys::commandModifier
                                                 | juce::ModifierKeys::shiftModifier, 0 };
 
    if (key == cmdShiftZ && undoManager.canRedo())
    {
        undoManager.redo();
        return true;
    }
 
    return false;
}

