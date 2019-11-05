/*
  ==============================================================================

    This file was auto-generated!

  ==============================================================================
*/

#include "MainComponent.h"
#include <Windows.h>

//==============================================================================
MainComponent::MainComponent()
{
    // Make sure you set the size of the component after
    // you add any child components.
    setSize (800, 600);
	m_slider.setRange(100.0, 25000.0);
	m_slider.setValue(600.0);
	addAndMakeVisible(m_slider);

    // Some platforms require permissions to open input channels so request that here
    if (RuntimePermissions::isRequired (RuntimePermissions::recordAudio)
        && ! RuntimePermissions::isGranted (RuntimePermissions::recordAudio))
    {
        RuntimePermissions::request (RuntimePermissions::recordAudio,
                                     [&] (bool granted) { if (granted)  setAudioChannels (2, 2); });
    }
    else
    {
        // Specify the number of input and output channels that we want to open
        setAudioChannels (0, 2);
    }
}

MainComponent::~MainComponent()
{
    // This shuts down the audio device and clears the audio source.
    shutdownAudio();
}

//==============================================================================
void MainComponent::prepareToPlay (int samplesPerBlockExpected, double sampleRate)
{
	m_sample_rate = sampleRate; // audio device wants sample per second
	m_currently_processing_sample = 0;
    // This function will be called when the audio device is started, or when
    // its settings (i.e. sample rate, block size, etc) are changed.

    // You can use this function to initialise any resources you might need,
    // but be careful - it will be called on the audio thread, not the GUI thread.

    // For more details, see the help for AudioProcessor::prepareToPlay()
}

void MainComponent::getNextAudioBlock (const AudioSourceChannelInfo& bufferToFill)
{
	std::stringstream ss;
	ss << "Num samples: " << bufferToFill.numSamples << " Start sample : " << bufferToFill.startSample << std::endl;
	OutputDebugString(ss.str().c_str());

    // Your audio-processing code goes here!

    // For more details, see the help for AudioProcessor::getNextAudioBlock()

    // Right now we are not producing any data, in which case we need to clear the buffer
    // (to prevent the output of random noise)
    bufferToFill.clearActiveBufferRegion();
	double freq = m_slider.getValue(); // Hz

	for (size_t i = 0; i < bufferToFill.numSamples; ++i) // how many sample device want in this func call
	{
		double sampleShift = (MathConstants<double>::pi * freq) / m_sample_rate;
		
		float sample = static_cast<float>(std::sin(sampleShift * m_currently_processing_sample));

		bufferToFill.buffer->setSample(0, bufferToFill.startSample + i, sample);
		bufferToFill.buffer->setSample(1, bufferToFill.startSample + i, sample);

		m_currently_processing_sample = m_currently_processing_sample + 1;
	}

}


void MainComponent::releaseResources()
{
    // This will be called when the audio device stops, or when it is being
    // restarted due to a setting change.

    // For more details, see the help for AudioProcessor::releaseResources()
}

//==============================================================================
void MainComponent::paint (Graphics& g)
{
    // (Our component is opaque, so we must completely fill the background with a solid colour)
    //g.fillAll (Colour(10,25,136));
	g.fillAll(Colours::lightblue);
	g.setColour(Colours::darkblue);

	g.setFont(20.0f);
	g.drawText("Hello, World!", getLocalBounds(), Justification::centred, true);

    // You can add your drawing code here!
}

void MainComponent::resized()
{
    // This is called when the MainContentComponent is resized.
    // If you add any child components, this is where you should
    // update their positions.
	m_slider.setBounds(0, 0, 200, 400);
}
