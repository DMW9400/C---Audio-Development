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
class PS03_graphicEQAudioProcessorEditor  : public juce::AudioProcessorEditor
{
public:
    PS03_graphicEQAudioProcessorEditor (PS03_graphicEQAudioProcessor&);
    ~PS03_graphicEQAudioProcessorEditor() override;

    //==============================================================================
    void paint (juce::Graphics&) override;
    void resized() override;

private:
    // This reference is provided as a quick way for your editor to
    // access the processor object that created it.
    PS03_graphicEQAudioProcessor& audioProcessor;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (PS03_graphicEQAudioProcessorEditor)
};
