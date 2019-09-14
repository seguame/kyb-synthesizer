#pragma once

// Amplitude (Attack, Decay, Sustain, Release) Envelope
// amplitude modulation of output to give texture, i.e. the timbre
struct sEnvelopeADSR
{
	double dAttackTime;
	double dDecayTime;
	double dSustainAmplitude;
	double dReleaseTime;
	double dStartAmplitude;
	double dTriggerOffTime;
	double dTriggerOnTime;
	bool bNoteOn;

	sEnvelopeADSR()
	{
		dAttackTime = 0.100;
		dDecayTime = 0.01;
		dStartAmplitude = 1.0;
		dSustainAmplitude = 0.8;
		dReleaseTime = 0.200;
		dTriggerOffTime = 0.0;
		dTriggerOnTime = 0.0;
		bNoteOn = false;
	}

	// Call when key is pressed
	void NoteOn(double dTimeOn)
	{
		dTriggerOnTime = dTimeOn;
		bNoteOn = true;
	}

	// Call when key is released
	void NoteOff(double dTimeOff)
	{
		dTriggerOffTime = dTimeOff;
		bNoteOn = false;
	}

	// Get the correct amplitude at the requested point in time
	double GetAmplitude(double dTime)
	{
		double dAmplitude = 0.0;
		double dLifeTime = dTime - dTriggerOnTime;

		if (bNoteOn)
		{
			if (dLifeTime <= dAttackTime)
			{
				// In attack Phase - approach max amplitude
				dAmplitude = (dLifeTime / dAttackTime) * dStartAmplitude;
			}

			if (dLifeTime > dAttackTime && dLifeTime <= (dAttackTime + dDecayTime))
			{
				// In decay phase - reduce to sustained amplitude
				dAmplitude = ((dLifeTime - dAttackTime) / dDecayTime) * (dSustainAmplitude - dStartAmplitude) + dStartAmplitude;
			}

			if (dLifeTime > (dAttackTime + dDecayTime))
			{
				// In sustain phase - dont change until note released
				dAmplitude = dSustainAmplitude;
			}
		}
		else
		{
			// Note has been released, so in release phase
			dAmplitude = ((dTime - dTriggerOffTime) / dReleaseTime) * (0.0 - dSustainAmplitude) + dSustainAmplitude;
		}

		// Amplitude should not be negative
		if (dAmplitude <= 0.0001)
		{
			dAmplitude = 0.0;
		}

		return dAmplitude;
	}
};