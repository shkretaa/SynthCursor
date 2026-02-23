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
    float t = ofGetElapsedTimef();
    
    // --- Base color depends on wave type (1/2/3)
    float baseHue = 0;
    if (currentWave == 0) baseHue = 180; // Sine = cool blue
    if (currentWave == 1) baseHue = 30;  // Square = warm orange
    if (currentWave == 2) baseHue = 300; // Saw = purple/pink
    
    // Mouse influences brightness/saturation
    float sat = ofMap(ofGetMouseX(), 0, ofGetWidth(), 90, 255, true);
    float bri = ofMap(ofGetMouseY(), 0, ofGetHeight(), 255, 80, true);
    
    // Animate hue slightly over time
    float hue = fmod(baseHue + t * 25.0f, 255.0f);
    
    ofColor bg;
    bg.setHsb(hue, sat * 0.35f, bri * 0.20f);
    ofBackground(bg);
    
    // --- Text UI
    ofSetColor(255);
    ofDrawBitmapString("Press 1: Sine | 2: Square | 3: Saw", 20, 20);
    
    string names[] = {"Sine", "Square", "Sawtooth"};
    ofDrawBitmapString("Current Wave: " + names[currentWave], 20, 40);
    ofDrawBitmapString("Frequency: " + ofToString(currentFreq, 2) + " Hz", 20, 60);
    
    // --- Frequency meter (right side)
    float meterH = ofMap(currentFreq, 100, 2000, 0, ofGetHeight() - 40, true);
    ofNoFill();
    ofSetColor(255);
    ofDrawRectangle(ofGetWidth() - 40, 20, 20, ofGetHeight() - 40);
    
    ofFill();
    ofColor meter; meter.setHsb(hue, sat, 255);
    ofSetColor(meter);
    ofDrawRectangle(ofGetWidth() - 40, ofGetHeight() - 20 - meterH, 20, meterH);
    
    // --- Mouse “orb” + trail
    ofVec2f m(ofGetMouseX(), ofGetMouseY());
    
    // Orb that orbits slightly
    float r = 12 + 6 * sin(t * 4);
    ofColor orb; orb.setHsb(hue, sat, 255);
    ofSetColor(orb);
    ofDrawCircle(m.x + cos(t * 6) * 8, m.y + sin(t * 6) * 8, r);
    
    // Draw trail with fade
    for (int i = 0; i < (int)trail.size(); i++) {
        float a = ofMap(i, 0, trail.size()-1, 180, 0, true);
        ofColor c; c.setHsb(fmod(hue + i * 2, 255.0f), sat, 255);
        c.a = a;
        ofSetColor(c);
        ofDrawCircle(trail[i].x, trail[i].y, ofMap(i, 0, trail.size()-1, 10, 2, true));
    }
    
    // --- Moving wave visualization (slow scroll)
    ofNoFill();
    ofSetColor(255);
    
    float speed = 0.8f;      // smaller = slower
    float freqVisual = 0.05f;
    float ampVisual  = 50.0f;
    
    ofBeginShape();
    for (int i = 0; i < ofGetWidth(); i++) {
        float x = i;
        float y = ofGetHeight() / 2;
        
        float phase = i * freqVisual + t * speed;
        
        if (currentWave == 0) {
            y += sin(phase) * ampVisual;
        }
        else if (currentWave == 1) {
            y += (sin(phase) > 0 ? ampVisual : -ampVisual);
        }
        else if (currentWave == 2) {
            float p = fmod(phase, TWO_PI) / TWO_PI;
            float saw = (p * 2.0f) - 1.0f;
            y += saw * ampVisual;
        }
        
        ofVertex(x, y);
    }
    ofEndShape(false);
}

void ofApp::mouseMoved(int x, int y) {
    trail.push_front(ofVec2f(x, y));
    if (trail.size() > maxTrail) {
        trail.pop_back();
    }
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
