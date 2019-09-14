#pragma once
namespace synth {

	struct Envelope {
		virtual double Amplitude(const double dTime, const double dTimeOn, const double dTimeOff) = 0;
	};

	// Amplitude (Attack, Decay, Sustain, Release) Envelope
	// amplitude modulation of output to give texture, i.e. the timbre
	struct sEnvelopeADSR : public Envelope {
		double dAttackTime;
		double dDecayTime;
		double dSustainAmplitude;
		double dReleaseTime;
		double dStartAmplitude;

		sEnvelopeADSR() {
			dAttackTime = 0.100;
			dDecayTime = 0.1;
			dStartAmplitude = 1.0;
			dSustainAmplitude = 1.0;
			dReleaseTime = 0.2;
		}

		// Get the correct amplitude at the requested point in time
		virtual double Amplitude(const double dTime, const double dTimeOn, const double dTimeOff) {
			double dAmplitude = 0.0;
			double dReleaseAmplitude = 0.0;

			if (dTimeOn > dTimeOff) // Note is on
			{
				double dLifeTime = dTime - dTimeOn;

				if (dLifeTime <= dAttackTime) {
					// In attack Phase - approach max amplitude
					dAmplitude = (dLifeTime / dAttackTime) * dStartAmplitude;
				}

				if (dLifeTime > dAttackTime && dLifeTime <= (dAttackTime + dDecayTime)) {
					// In decay phase - reduce to sustained amplitude
					dAmplitude = ((dLifeTime - dAttackTime) / dDecayTime) * (dSustainAmplitude - dStartAmplitude) + dStartAmplitude;
				}

				if (dLifeTime > (dAttackTime + dDecayTime)) {
					// In sustain phase - dont change until note released
					dAmplitude = dSustainAmplitude;
				}
			}
			else // Note is off
			{
				double  dLifeTime = dTimeOff - dTimeOn;

				if (dLifeTime <= dAttackTime) {
					dReleaseAmplitude = (dLifeTime / dAttackTime) * dStartAmplitude;
				}

				if (dLifeTime > dAttackTime && dLifeTime <= (dAttackTime + dDecayTime)) {
					dReleaseAmplitude = ((dLifeTime - dAttackTime) / dDecayTime) * (dSustainAmplitude - dStartAmplitude) + dStartAmplitude;
				}

				if (dLifeTime > (dAttackTime + dDecayTime)) {
					dReleaseAmplitude = dSustainAmplitude;
				}

				// Note has been released, so in release phase
				dAmplitude = ((dTime - dTimeOff) / dReleaseTime) * (0.0 - dReleaseAmplitude) + dReleaseAmplitude;
			}

			// Amplitude should not be negative
			if (dAmplitude <= 0.000) {
				dAmplitude = 0.0;
			}

			return dAmplitude;
		}
	};

	double envelope(const double dTime, Envelope &env, const double dTimeOn, const double dTimeOff) {
		return env.Amplitude(dTime, dTimeOn, dTimeOff);
	}
}