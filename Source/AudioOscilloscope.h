#pragma once

#include <JuceHeader.h>

class AudioOscilloscope : public juce::Component,
                          private juce::Timer
{
public:
    AudioOscilloscope()
    {
        startTimerHz(30); // Refresh rate
    }

    ~AudioOscilloscope() override
    {
        stopTimer();
    }

    void paint(juce::Graphics& g) override
    {
        g.fillAll(juce::Colours::black);
        g.setColour(lineColor);
        
        const float width = (float)getWidth();
        const float height = (float)getHeight();
        const float midHeight = height * 0.5f;
        
        // Draw horizontal center line
        g.drawLine(0, midHeight, width, midHeight, 0.5f);
        
        if (waveform.getNumSamples() < 2)
            return;
            
        // Scale to fit width
        const float xScale = width / (waveform.getNumSamples() - 1);
        
        juce::Path path;
        path.startNewSubPath(0, midHeight - waveform.getSample(0, 0) * midHeight);
        
        for (int i = 1; i < waveform.getNumSamples(); ++i)
        {
            path.lineTo(i * xScale,
                        midHeight - waveform.getSample(0, i) * midHeight);
        }
        
        g.strokePath(path, juce::PathStrokeType(1.5f));
    }

    void processBlock(const juce::AudioBuffer<float>& buffer)
    {
        // Store the samples for display
        const auto numSamples = buffer.getNumSamples();
        const auto numChannels = buffer.getNumChannels();
        
        // Create appropriate size buffer (mono or mix down to mono)
        if (waveform.getNumSamples() != numSamples || waveform.getNumChannels() != 1)
            waveform.setSize(1, numSamples);
            
        if (numChannels == 1)
        {
            // Just copy the samples
            waveform.copyFrom(0, 0, buffer, 0, 0, numSamples);
        }
        else
        {
            // Mix down to mono
            waveform.clear();
            for (int channel = 0; channel < numChannels; ++channel)
                waveform.addFrom(0, 0, buffer, channel, 0, numSamples, 1.0f / numChannels);
        }
    }
    
    void setColor(juce::Colour color)
    {
        lineColor = color;
        repaint();
    }

private:
    void timerCallback() override
    {
        repaint();
    }
    
    juce::AudioBuffer<float> waveform;
    juce::Colour lineColor = juce::Colours::lightgreen;
};
