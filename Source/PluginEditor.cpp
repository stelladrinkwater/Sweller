#include "PluginProcessor.h"
#include "PluginEditor.h"
#include "MyColours.h"
#include "ParamIDs.h"

 
SwellerAudioProcessorEditor::SwellerAudioProcessorEditor (SwellerAudioProcessor& p,
                                                                juce::AudioProcessorValueTreeState& state,
                                                                juce::UndoManager& um)
    : AudioProcessorEditor (&p), audioProcessor (p), undoManager (um),
      dryDial  (*state.getParameter (ParamIDs::dry_ratio),  &um),
      wetDial (*state.getParameter (ParamIDs::wet_ratio),   &um),
      feedbackDial  (*state.getParameter (ParamIDs::feedback_ratio), &um),
      phaseDial   (*state.getParameter (ParamIDs::phase_shift_mix),  &um),
      cutoffDial (*state.getParameter (ParamIDs::phase_shift_cutoff),     &um),
      qDial   (*state.getParameter (ParamIDs::phase_shift_q),       &um)
{
    setWantsKeyboardFocus (true);
    setSize (740, 380);
    
    // Set labels
    dryDial.setLabelText     ("dry");
    wetDial.setLabelText     ("wet");
    feedbackDial.setLabelText("fbk");
    phaseDial.setLabelText   ("phs");
    cutoffDial.setLabelText  ("cut");
    qDial.setLabelText       ("res");

    // Set intervals
    dryDial.setInterval      (1.0f);
    dryDial.setFineInterval  (0.1f);

    wetDial.setInterval      (1.0f);
    wetDial.setFineInterval  (0.1f);

    feedbackDial.setInterval     (1.0f);
    feedbackDial.setFineInterval (0.1f);

    phaseDial.setInterval        (1.0f);
    phaseDial.setFineInterval    (0.1f);

    cutoffDial.setInterval       (100.0f);  // Adjust as appropriate for your Hz range
    cutoffDial.setFineInterval   (10.0f);

    qDial.setInterval            (0.5f);
    qDial.setFineInterval        (0.05f);

    // Make visible
    addAndMakeVisible (dryDial);
    addAndMakeVisible (wetDial);
    addAndMakeVisible (feedbackDial);
    addAndMakeVisible (phaseDial);
    addAndMakeVisible (cutoffDial);
    addAndMakeVisible (qDial);
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
    const int dialWidth = 80;
    const int dialHeight = 95;
    const int spacing = 10;
    const int startX = 10;
    const int startY = 10;

    dryDial.setBounds     (startX + 0 * (dialWidth + spacing), startY + 0 * (dialHeight + spacing), dialWidth, dialHeight);
    wetDial.setBounds     (startX + 1 * (dialWidth + spacing), startY + 0 * (dialHeight + spacing), dialWidth, dialHeight);
    feedbackDial.setBounds(startX + 2 * (dialWidth + spacing), startY + 0 * (dialHeight + spacing), dialWidth, dialHeight);

    phaseDial.setBounds   (startX + 0 * (dialWidth + spacing), startY + 1 * (dialHeight + spacing), dialWidth, dialHeight);
    cutoffDial.setBounds  (startX + 1 * (dialWidth + spacing), startY + 1 * (dialHeight + spacing), dialWidth, dialHeight);
    qDial.setBounds       (startX + 2 * (dialWidth + spacing), startY + 1 * (dialHeight + spacing), dialWidth, dialHeight);
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

