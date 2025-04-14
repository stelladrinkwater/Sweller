#include "PluginProcessor.h"
#include "PluginEditor.h"
#include "ParamIDs.h"
 
static juce::AudioProcessorValueTreeState::ParameterLayout createParameterLayout()
{
    juce::AudioProcessorValueTreeState::ParameterLayout layout;
 
    layout.add (std::make_unique<juce::AudioParameterFloat> (juce::ParameterID { ParamIDs::dry_ratio, 1 },
                                                             "dry",
                                                             juce::NormalisableRange<float> (0.0f, 1.0f, 0.01f, 0.405f),
                                                             0.5f,
                                                             juce::String(),
                                                             juce::AudioProcessorParameter::genericParameter,
                                                             [](float value, int) {
                                                                return juce::String (value, 1) + " %"; },
                                                             nullptr));
    
    layout.add (std::make_unique<juce::AudioParameterFloat> (juce::ParameterID { ParamIDs::wet_ratio, 1 },
                                                             "wet",
                                                             juce::NormalisableRange<float> (0.0f, 1.0f, 0.01f, 0.405f),
                                                             0.5f,
                                                             juce::String(),
                                                             juce::AudioProcessorParameter::genericParameter,
                                                             [](float value, int) {
                                                                return juce::String (value, 1) + " %"; },
                                                             nullptr));
    
    layout.add (std::make_unique<juce::AudioParameterFloat> (juce::ParameterID { ParamIDs::feedback_ratio, 1 },
                                                             "feedback",
                                                             juce::NormalisableRange<float> (0.0f, 1.0f, 0.01f, 1.0f),
                                                             0.1f,
                                                             juce::String(),
                                                             juce::AudioProcessorParameter::genericParameter,
                                                             [](float value, int) {
                                                                return juce::String (value, 1) + " %"; },
                                                             nullptr));
    
    layout.add (std::make_unique<juce::AudioParameterFloat> (juce::ParameterID { ParamIDs::phase_shift_mix, 1 },
                                                             "phase shift mix",
                                                             juce::NormalisableRange<float> (0.0f, 1.0f, 0.01f, 0.405f),
                                                             0.1f,
                                                             juce::String(),
                                                             juce::AudioProcessorParameter::genericParameter,
                                                             [](float value, int) {
                                                                return juce::String (value, 1) + " %"; },
                                                             nullptr));
    
    layout.add (std::make_unique<juce::AudioParameterFloat> (juce::ParameterID { ParamIDs::phase_shift_cutoff, 1 },
                                                             "phase shift cutoff",
                                                             juce::NormalisableRange<float> (0.0f, 15000.0f, 0.01f, 1.0f),
                                                             10000.0f,
                                                             juce::String(),
                                                             juce::AudioProcessorParameter::genericParameter,
                                                             [](float value, int) {
                                                                return juce::String (value, 1) + " Hz"; },
                                                             nullptr));
    
    layout.add (std::make_unique<juce::AudioParameterFloat> (juce::ParameterID { ParamIDs::phase_shift_q, 1 },
                                                             "phase shift q",
                                                             juce::NormalisableRange<float> (1.0f, 10.0f, 0.01f, 1.0f),
                                                             5.0f,
                                                             juce::String(),
                                                             juce::AudioProcessorParameter::genericParameter,
                                                             [](float value, int) {
                                                                return juce::String (value, 1) + " Q"; },
                                                             nullptr));
 
    return layout;
}

 
//==============================================================================
SwellerAudioProcessor::SwellerAudioProcessor()
#ifndef JucePlugin_PreferredChannelConfigurations
     : AudioProcessor (BusesProperties()
                     #if ! JucePlugin_IsMidiEffect
                      #if ! JucePlugin_IsSynth
                       .withInput  ("Input",  juce::AudioChannelSet::stereo(), true)
                      #endif
                       .withOutput ("Output", juce::AudioChannelSet::stereo(), true)
                     #endif
                       ),
#endif
       apvts (*this, &undoManager, "Parameters", createParameterLayout())
{
    for (RNBO::ParameterIndex i = 0; i < rnboObject.getNumParameters(); ++i)
    {
        RNBO::ParameterInfo info;
        rnboObject.getParameterInfo (i, &info);
 
        if (info.visible)
        {
            auto paramID = juce::String (rnboObject.getParameterId (i));
 
            // Each apvts parameter id and range must be the same as the rnbo param object's.
            // If you hit this assertion then you need to fix the incorrect id in ParamIDs.h.
            jassert (apvts.getParameter (paramID) != nullptr);
 
            // If you hit these assertions then you need to fix the incorrect apvts
            // parameter range in createParameterLayout().
            jassert (info.min == apvts.getParameterRange (paramID).start);
            jassert (info.max == apvts.getParameterRange (paramID).end);
 
            apvtsParamIdToRnboParamIndex[paramID] = i;
 
            apvts.addParameterListener (paramID, this);
            rnboObject.setParameterValue (i, apvts.getRawParameterValue (paramID)->load());
        }
    }
}
 
SwellerAudioProcessor::~SwellerAudioProcessor()
{
}
 
//==============================================================================
const juce::String SwellerAudioProcessor::getName() const
{
    return JucePlugin_Name;
}
 
bool SwellerAudioProcessor::acceptsMidi() const
{
   #if JucePlugin_WantsMidiInput
    return true;
   #else
    return false;
   #endif
}
 
bool SwellerAudioProcessor::producesMidi() const
{
   #if JucePlugin_ProducesMidiOutput
    return true;
   #else
    return false;
   #endif
}
 
bool SwellerAudioProcessor::isMidiEffect() const
{
   #if JucePlugin_IsMidiEffect
    return true;
   #else
    return false;
   #endif
}
 
double SwellerAudioProcessor::getTailLengthSeconds() const
{
    return 0.0;
}
 
int SwellerAudioProcessor::getNumPrograms()
{
    return 1;   // NB: some hosts don't cope very well if you tell them there are 0 programs,
                // so this should be at least 1, even if you're not really implementing programs.
}
 
int SwellerAudioProcessor::getCurrentProgram()
{
    return 0;
}
 
void SwellerAudioProcessor::setCurrentProgram (int index)
{
    juce::ignoreUnused (index);
}
 
const juce::String SwellerAudioProcessor::getProgramName (int index)
{
    juce::ignoreUnused (index);
    return {};
}
 
void SwellerAudioProcessor::changeProgramName (int index, const juce::String& newName)
{
    juce::ignoreUnused (index, newName);
}
 
//==============================================================================
void SwellerAudioProcessor::prepareToPlay (double sampleRate, int samplesPerBlock)
{
    rnboObject.prepareToProcess (sampleRate, static_cast<size_t> (samplesPerBlock));
}
 
void SwellerAudioProcessor::releaseResources()
{
    // When playback stops, you can use this as an opportunity to free up any
    // spare memory, etc.
}
 
#ifndef JucePlugin_PreferredChannelConfigurations
bool SwellerAudioProcessor::isBusesLayoutSupported (const BusesLayout& layouts) const
{
  #if JucePlugin_IsMidiEffect
    juce::ignoreUnused (layouts);
    return true;
  #else
    // This is the place where you check if the layout is supported.
    // In this template code we only support mono or stereo.
    // Some plugin hosts, such as certain GarageBand versions, will only
    // load plugins that support stereo bus layouts.
    if (layouts.getMainOutputChannelSet() != juce::AudioChannelSet::mono()
     && layouts.getMainOutputChannelSet() != juce::AudioChannelSet::stereo())
        return false;
 
    // This checks if the input layout matches the output layout
   #if ! JucePlugin_IsSynth
    if (layouts.getMainOutputChannelSet() != layouts.getMainInputChannelSet())
        return false;
   #endif
 
    return true;
  #endif
}
#endif
 
void SwellerAudioProcessor::processBlock (juce::AudioBuffer<float>& buffer, juce::MidiBuffer& midiMessages)
{
    juce::ignoreUnused (midiMessages);
    auto bufferSize = buffer.getNumSamples();
 
    rnboObject.prepareToProcess (getSampleRate(),
                                 static_cast<size_t> (bufferSize));
 
    rnboObject.process (buffer.getArrayOfWritePointers(),
                        static_cast<RNBO::Index> (buffer.getNumChannels()),
                        buffer.getArrayOfWritePointers(),
                        static_cast<RNBO::Index> (buffer.getNumChannels()),
                        static_cast<RNBO::Index> (bufferSize));
}
 
//==============================================================================
bool SwellerAudioProcessor::hasEditor() const
{
    return true; // (change this to false if you choose to not supply an editor)
}
 
juce::AudioProcessorEditor* SwellerAudioProcessor::createEditor()
{
    return new SwellerAudioProcessorEditor (*this, apvts, undoManager);
    // return new juce::GenericAudioProcessorEditor (*this);
}
 
//==============================================================================
void SwellerAudioProcessor::getStateInformation (juce::MemoryBlock& destData)
{
    juce::MemoryOutputStream mos (destData, true);
    apvts.state.writeToStream (mos);
}
 
void SwellerAudioProcessor::setStateInformation (const void* data, int sizeInBytes)
{
    auto tree = juce::ValueTree::readFromData (data, static_cast<size_t> (sizeInBytes));
 
    if (tree.isValid())
        apvts.replaceState (tree);
}
 
void SwellerAudioProcessor::parameterChanged (const juce::String& parameterID, float newValue)
{
    rnboObject.setParameterValue (apvtsParamIdToRnboParamIndex[parameterID], newValue);
}
 
//==============================================================================
// This creates new instances of the plugin..
juce::AudioProcessor* JUCE_CALLTYPE createPluginFilter()
{
    return new SwellerAudioProcessor();
    // return new juce::GenericAudioProcessorEditor (*this);
}
