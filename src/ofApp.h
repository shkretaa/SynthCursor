#pragma once
#include "ofMain.h"
#include "Oscillator.h" // Include our waves

class ofApp : public ofBaseApp {
public:
	void setup();
	void update();
	void draw();
	void audioOut(ofSoundBuffer & buffer); // This is where the sound is made
	void keyPressed(int key);

	// Composition: ofApp "HAS" these objects
	SineOsc sine;
	SquareOsc square;
	SawOsc saw;

	int currentWave; // 0 for Sine, 1 for Square, 2 for Saw
	float currentFreq;
};
