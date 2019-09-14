#pragma once

#include "instrument.h"

struct bell : instrument
{
	bell() {
		env.dAttackTime = 0.01;
		env.dDecayTime = 1.0;
		env.dStartAmplitude = 1.0;
		env.dSustainAmplitude = 0.0;
		env.dReleaseTime = 1.0;
	}

	double sound(double dTime, double dFrequency) {
		double dOutput = env.GetAmplitude(dTime) * (
			+1.0 * oscillator(dFrequency, dTime, OSC_SINE, 5.0, 0.001)
			+ 0.5 * oscillator(dFrequency * 2.0, dTime, OSC_SINE)
			+ 0.25 * oscillator(dFrequency * 3.0, dTime, OSC_SINE)
			);

		return dOutput;
	}
};