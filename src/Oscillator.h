#pragma once
#include "ofMain.h"

// The Parent Class
class Oscillator {
public:
	float frequency;
	float phase;
	float phaseStep;
	float amplitude;
	float sampleRate;

	Oscillator() {
		phase = 0;
		amplitude = 0.5;
		sampleRate = 44100;
	}

	// A virtual function: Child classes will fill this in with their own math
	virtual float getSample() = 0;

	void setFrequency(float f) {
		frequency = f;
		phaseStep = (frequency / sampleRate);
	}

	void updatePhase() {
		phase += phaseStep;
		if (phase > 1.0) phase -= 1.0;
	}
};

// Child Class 1: Sine Wave
class SineOsc : public Oscillator {
public:
	float getSample() override {
		return sin(phase * TWO_PI) * amplitude;
	}
};

// Child Class 2: Square Wave
class SquareOsc : public Oscillator {
public:
	float getSample() override {
		return (phase < 0.5 ? 1.0 : -1.0) * amplitude;
	}
};

// Child Class 3: Sawtooth Wave
class SawOsc : public Oscillator {
public:
	float getSample() override {
		return (phase * 2.0 - 1.0) * amplitude;
	}
};
