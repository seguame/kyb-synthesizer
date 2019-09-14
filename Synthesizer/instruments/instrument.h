#pragma once

#include "envelope.h"

namespace synth {

	enum {
		OSC_SINE, OSC_SQUARE, OSC_TRIANGLE, OSC_SAW_ANA, OSC_SAW_DIG, OSC_NOISE, OSC_LENGTH
	};

	struct Note {
		int id;		//position in scale
		double on;	//time note was activated
		double off;	//time note was deactivated
		bool active;
		int channel;

		Note() {
			id = 0;
			on = 0.0;
			off = 0.0;
			active = false;
			channel = 0;
		}
	};

	struct Instrument {
		double dVolume;
		synth::sEnvelopeADSR env;

		virtual double sound(const double dTime, Note n, bool &bNoteFinished) = 0;
	};


	//////////////////////////////////////////////////////////////////////////////
	// Utils!

	//Converts freqyebcy [Hz] to angular velocity
	double w(double dHertz) {
		return dHertz * 2.0 * PI;
	}

	//////////////////////////////////////////////////////////////////////////////
	// Multi-Function Oscillator

	double oscillator(double dHertz, double dTime, int nType = OSC_SINE, double dLFOHertz = 0.0, double dLFOAmplitude = 0.0) {
		double dFreq = w(dHertz)  * dTime + dLFOAmplitude * dHertz * sin(w(dLFOHertz) * dTime);
		switch (nType)
		{
		case OSC_SINE:
			return sin(dFreq);

		case OSC_SQUARE:
			return sin(dFreq) > 0.0 ? 1.0 : -1.0;

		case OSC_TRIANGLE:
			return asin(sin(dFreq)) * (2.0 / PI);

		case OSC_SAW_ANA: // (analogue / warm / slow)  
		{
			double dOutput = 0.0;
			for (double n = 1.0; n < 40.0; n++) {
				dOutput += (sin(n * dFreq)) / n;
			}
			return dOutput * (2.0 / PI);
		}

		case OSC_SAW_DIG: //(optimized / harsh / fast)
			return (2.0 / PI) * (dHertz * PI * fmod(dTime, 1.0 / dHertz) - (PI / 2.0));

		case OSC_NOISE: // Pseudorandom noise
			return 2.0 * ((double)rand() / (double)RAND_MAX) - 1.0;

		default:
			return 0.0;
		}
	}

	//////////////////////////////////////////////////////////////////////////////
	// Scale to Frequency conversion

	const int SCALE_DEFAULT = 0;

	double scale(const int nNoteID, const int nScaleID = SCALE_DEFAULT) {
		switch (nScaleID) {
		case SCALE_DEFAULT: default:
			return 256 * pow(1.0594630943592952645618252949463, nNoteID);
		}
	}
}

