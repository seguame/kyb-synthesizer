#include <iostream>
#include <cstdio>

#include "olcNoiseMaker.h"
#include "instruments/harmonica.h"

atomic<double> dFrequencyOutput = 0.0;			// dominant hertz frequency of instrument, i.e. the note
double dOctaveBaseFrequency = 220.0; // A2		// frequency of octave represented by keyboard
double d12thRootOf2 = pow(2.0, 1.0 / 12.0);		// assuming western 12 notes per ocatve
double dMasterVolume = 0.1;

instrument* voice = nullptr;

double MakeNoise(double dTime);

int main() {

	//Get all sound hardware
	vector<wstring> devices = olcNoiseMaker<short>::Enumerate();

	//Show what we got
	for (auto device : devices) {
		wcout << "Found Output Device: " << device << endl;
	}

	// Display a keyboard
	wcout << endl <<
		"|   |   |   |   |   | |   |   |   |   | |   | |   |   |   |" << endl <<
		"|   | S |   |   | F | | G |   |   | J | | K | | L |   |   |" << endl <<
		"|   |___|   |   |___| |___|   |   |___| |___| |___|   |   |__" << endl <<
		"|     |     |     |     |     |     |     |     |     |     |" << endl <<
		"|  Z  |  X  |  C  |  V  |  B  |  N  |  M  |  ,  |  .  |  /  |" << endl <<
		"|_____|_____|_____|_____|_____|_____|_____|_____|_____|_____|" << endl << endl;

	//Create the sound machine with selected device
	olcNoiseMaker<short> sound(devices[0], 44100, 1, 8, 512);

	voice = new harmonica();

	sound.SetUserFunction(MakeNoise);

	// Sit in loop, capturing keyboard state changes and modify
	// synthesizer output accordingly
	int nCurrentKey = -1;
	bool bKeyPressed = false;
	while (true) {
		bKeyPressed = false;
		for (int k = 0; k < 16; k++) {
			//piano like keyboard
			if (GetAsyncKeyState((unsigned char)("ZSXCFVGBNJMK\xbcL\xbe\xbf"[k])) & 0x8000) {
				if (nCurrentKey != k) {

					//every key [k] is part of an octave
					dFrequencyOutput = dOctaveBaseFrequency * pow(d12thRootOf2, k);
					voice->env.NoteOn(sound.GetTime());
					wcout << "\rNote On : " << sound.GetTime() << "s " << dFrequencyOutput << "Hz";
					nCurrentKey = k;
				}
				bKeyPressed = true;
			}
		}

		if (!bKeyPressed)
		{
			if (nCurrentKey != -1) {
				wcout << "\rNote Off: " << sound.GetTime() << "s                        ";
				voice->env.NoteOff(sound.GetTime());
				nCurrentKey = -1;
			}
		}
	}

	return EXIT_SUCCESS;
}

//Used to generate sound waves, returning an amplitude (-1.0 to +1.0) as a function of time
double MakeNoise(double dTime) {
	double dOutput = voice->sound(dTime, dFrequencyOutput);
	return dOutput * dMasterVolume;
}