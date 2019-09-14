#pragma once

#include "instrument.h"

struct harmonica : instrument
{
	harmonica() {
		env.dAttackTime = 0.05;
		env.dDecayTime = 1.0;
		env.dSustainAmplitude = 0.95;
		env.dReleaseTime = 0.1;
	}

	double sound(double dTime, double dFrequency) {
		double dOutput = env.GetAmplitude(dTime) * (
			+ 1.0 * oscillator(dFrequency, dTime, OSC_SQUARE, 5.0, 0.001)
			+ 0.5 * oscillator(dFrequency * 1.5, dTime, OSC_SQUARE)
			+ 0.5 * oscillator(dFrequency * 2.5, dTime, OSC_SQUARE)
			+ 0.05 * oscillator(0, dTime, OSC_NOISE)
			);

		return dOutput;
	}
};