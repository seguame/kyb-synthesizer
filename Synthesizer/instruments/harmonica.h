#pragma once

#include "instrument.h"

namespace synth {

	struct Harmonica : public synth::Instrument
	{
		Harmonica() {
			env.dAttackTime = 0.05;
			env.dDecayTime = 1.0;
			env.dSustainAmplitude = 0.95;
			env.dReleaseTime = 0.1;

			dVolume = 1.0;
		}

		virtual double sound(const double dTime, synth::Note n, bool &bNoteFinished)
		{
			double dAmplitude = synth::envelope(dTime, env, n.on, n.off);
			if (dAmplitude <= 0.0) {
				bNoteFinished = true;
			}

			double dSound =
				+ 1.00 * synth::oscillator(n.on - dTime, synth::scale(n.id), synth::OSC_SQUARE, 5.0, 0.001)
				+ 0.50 * synth::oscillator(n.on - dTime, synth::scale(n.id + 12), synth::OSC_SQUARE)
				+ 0.05 * synth::oscillator(n.on - dTime, synth::scale(n.id + 24), synth::OSC_NOISE);

			return dAmplitude * dSound * dVolume;
		}
	};
}