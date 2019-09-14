#pragma once

#include "instrument.h"

namespace synth {

	struct Bell : public synth::Instrument
	{
		Bell() {
			env.dAttackTime = 0.01;
			env.dDecayTime = 1.0;
			env.dStartAmplitude = 1.0;
			env.dSustainAmplitude = 0.0;
			env.dReleaseTime = 1.0;

			dVolume = 1.0;
		}

		virtual double sound(const double dTime, synth::Note n, bool &bNoteFinished) {

			double dAmplitude = synth::envelope(dTime, env, n.on, n.off);
			if(dAmplitude <= 0.0) {
				bNoteFinished = true;
			}

			double dSound = 
				+ 1.0	* synth::oscillator(n.on - dTime, synth::scale(n.id + 12), synth::OSC_SINE, 5.0, 0.001)
				+ 0.50	* synth::oscillator(n.on - dTime, synth::scale(n.id + 24))
				+ 0.25	* synth::oscillator(n.on - dTime, synth::scale(n.id + 36));

			return dAmplitude * dSound * dVolume;
		}
	};

	struct Bell_Square : Instrument
	{
		Bell_Square() {
			env.dAttackTime = 0.01;
			env.dDecayTime = 0.5;
			env.dStartAmplitude = 1.0;
			env.dSustainAmplitude = 0.8;
			env.dReleaseTime = 1.0;

			dVolume = 1.0;
		}

		virtual double sound(const double dTime, synth::Note n, bool &bNoteFinished) {

			double dAmplitude = synth::envelope(dTime, env, n.on, n.off);
			if(dAmplitude <= 0.0) {
				bNoteFinished = true;
			}

			double dSound =
				+1.0	* synth::oscillator(n.on - dTime, synth::scale(n.id), synth::OSC_SQUARE, 5.0, 0.001)
				+ 0.50	* synth::oscillator(n.on - dTime, synth::scale(n.id + 12))
				+ 0.25	* synth::oscillator(n.on - dTime, synth::scale(n.id + 24));

			return dAmplitude * dSound * dVolume;
		}
	};
}

