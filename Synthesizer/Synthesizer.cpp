#include <iostream>
#include <cstdio>

#include "olcNoiseMaker.h"

atomic<double> dFrequencyOutput = 0.0;			// dominant hertz frequency of instrument, i.e. the note
double dOctaveBaseFrequency = 110.0; // A2		// frequency of octave represented by keyboard
double d12thRootOf2 = pow(2.0, 1.0 / 12.0);		// assuming western 12 notes per ocatve
double dMasterVolume = 0.5;

double MakeNoise(double dTime);

int main() {

	std::wcout << "First try!" << std::endl;

	//Get all sound hardware
	std::vector<wstring> devices = olcNoiseMaker<short>::Enumerate();

	//Show what we got
	for (auto device : devices) {
		std::wcout << "Found Output Device: " << device << std::endl;
	}

	// Display a keyboard
	std::wcout << std::endl <<
		"|   |   |   |   |   | |   |   |   |   | |   | |   |   |   |" << std::endl <<
		"|   | S |   |   | F | | G |   |   | J | | K | | L |   |   |" << std::endl <<
		"|   |___|   |   |___| |___|   |   |___| |___| |___|   |   |__" << std::endl <<
		"|     |     |     |     |     |     |     |     |     |     |" << std::endl <<
		"|  Z  |  X  |  C  |  V  |  B  |  N  |  M  |  ,  |  .  |  /  |" << std::endl <<
		"|_____|_____|_____|_____|_____|_____|_____|_____|_____|_____|" << std::endl << std::endl;

	//Create the sound machine with selected device
	olcNoiseMaker<short> sound(devices[0], 44100, 1, 8, 512);

	sound.SetUserFunction(MakeNoise);

	while (true) {

		bool bKeyPressed = false;

		for (int k = 0; k < 15; k++) {
			//piano like keyboard
			if (GetAsyncKeyState((unsigned char)("ZSXCFVGBNJMK\xbcL\xbe\xbf"[k])) & 0x8000) {

				//every key [k] is part of an octave
				dFrequencyOutput = dOctaveBaseFrequency * pow(d12thRootOf2, k);
				wcout << "\rNote On : " << dFrequencyOutput << "Hz";
				bKeyPressed = true;
			}
		}

		if (!bKeyPressed)
		{
			dFrequencyOutput = 0.0;
		}
	}

	return EXIT_SUCCESS;
}

double MakeNoise(double dTime) {

	double dOutput = sin(dFrequencyOutput * 2 * 3.14159 * dTime);

	if (dOutput > 0.0) return 0.2;
	else return -0.2;
	//return dOutput * dMasterVolume;
}