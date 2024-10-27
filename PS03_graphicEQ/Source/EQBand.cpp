/*
  ==============================================================================

    EQBand.cpp
    Created: 26 Oct 2024 7:41:15pm
    Author:  David Matthew Welch

  ==============================================================================
*/

#include "EQBand.h"
#include <JuceHeader.h>

EQBand::EQBand()
{
    addAndMakeVisible(bandSlider);
    
    bandSlider.setSliderStyle(juce::Slider::SliderStyle::LinearVertical);
    
    bandSlider.setRange(.01, 2.0, .01);
    bandSlider.setValue(1.0);
    
    bandSlider.addListener(this);
    
    gain = 1.0;
}

void EQBand::resized()
{
    bandSlider.setBounds(getLocalBounds());
}

void EQBand::initializeVTS(juce::AudioProcessorValueTreeState& vts){
    gainAttachment = std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment>(vts, "GAIN_PARAM_ID", bandSlider);
}

void EQBand::prepare (float frequency, int sampleRate, float gain){
//    set the arguments to each band's member variables
    this->frequency = frequency;
    this->sampleRate = sampleRate;
    this->gain = gain;
//    set the coefficients for each band's IIR filter
    auto coefficients = juce::IIRCoefficients::makePeakFilter(sampleRate, frequency, 1.0f, gain);
//    ensure we our number of filters matches the audio channels in the event of stereo/higher multichannel
    filter.resize(numChannels);
//    set coefficients for each filter within the vector
    for(auto& f : filter){
        f.setCoefficients(coefficients);
    }
}

void EQBand::process(juce::dsp::AudioBlock<float>& block)
{
    numChannels = block.getNumChannels();
    
}
