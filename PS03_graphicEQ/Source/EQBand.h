/*
  ==============================================================================

    EQBand.h
    Created: 26 Oct 2024 7:41:30pm
    Author:  David Matthew Welch

  ==============================================================================
*/

#pragma once
#include <JuceHeader.h>

class EQBand : public juce::Component, public juce::Slider::Listener {
public:
    EQBand();
    void resized() override;
    
    void initializeVTS(juce::AudioProcessorValueTreeState& vts);
    void prepare(float frequency, int sampleRate, float gain);
    void process(juce::dsp::AudioBlock<float>& block);
    void reset();
private:
    std::vector<juce::IIRFilter> filter;
    std::unique_ptr<juce::AudioProcessorValueTreeState::SliderAttachment> gainAttachment;
    int numChannels, sampleRate;
    float frequency, gain;
    juce::Slider bandSlider;
    juce::Label bandLabel;
};




