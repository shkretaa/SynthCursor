#include "ofApp.h"

void ofApp::setup() {
	ofBackground(20, 20, 20);
	currentWave = 0;
	
	// Setup sound stream: 2 output channels, 0 input channels, 44100 sample rate
	ofSoundStreamSettings settings;
	settings.setOutListener(this);
	settings.sampleRate = 44100;
	settings.numOutputChannels = 2;
	settings.bufferSize = 512;
	ofSoundStreamSetup(settings);
}

void ofApp::update() {
	// Map Mouse X to frequency (20Hz to 2000Hz)
	currentFreq = ofMap(mouseX, 0, ofGetWidth(), 20, 2000, true);
	
	// Map Mouse Y to volume (0.0 to 0.5)
	float vol = ofMap(mouseY, 0, ofGetHeight(), 0.5, 0.0, true);

	// Update the oscillators
	sine.setFrequency(currentFreq);
	sine.amplitude = vol;
	
	square.setFrequency(currentFreq);
	square.amplitude = vol;

	saw.setFrequency(currentFreq);
	saw.amplitude = vol;
}

void ofApp::draw() {
	ofSetColor(255);
	ofDrawBitmapString("Press 1: Sine | 2: Square | 3: Saw", 20, 20);
	
	string names[] = {"Sine", "Square", "Sawtooth"};
	ofDrawBitmapString("Current Wave: " + names[currentWave], 20, 40);
	ofDrawBitmapString("Frequency: " + ofToString(currentFreq) + " Hz", 20, 60);

	// Simple Visualization: Drawing the wave shape
	ofNoFill();
	ofBeginShape();
	for (int i = 0; i < ofGetWidth(); i++) {
		float x = i;
		float y = ofGetHeight()/2;
		if (currentWave == 0) y += sin(i * 0.05) * 50;
		if (currentWave == 1) y += (sin(i * 0.05) > 0 ? 50 : -50);
		if (currentWave == 2) y += ( (i % 50) / 25.0 - 1.0) * 50;
		ofVertex(x, y);
	}
	ofEndShape();
}

void ofApp::audioOut(ofSoundBuffer & buffer) {
	for (size_t i = 0; i < buffer.getNumFrames(); i++) {
		float sample = 0;

		if (currentWave == 0) {
			sample = sine.getSample();
			sine.updatePhase();
		} else if (currentWave == 1) {
			sample = square.getSample();
			square.updatePhase();
		} else if (currentWave == 2) {
			sample = saw.getSample();
			saw.updatePhase();
		}

		buffer[i * buffer.getNumChannels()] = sample;     // Left Speaker
		buffer[i * buffer.getNumChannels() + 1] = sample; // Right Speaker
	}
}

void ofApp::keyPressed(int key) {
	if (key == '1') currentWave = 0;
	if (key == '2') currentWave = 1;
	if (key == '3') currentWave = 2;
}
