#include <iostream>
#include <cstdio>

#include "olcNoiseMaker.h"

double MakeNoise(double dTime);

int main() {

	std::wcout << "First try!" << std::endl;

	//Get all sound hardware
	std::vector<wstring> devices = olcNoiseMaker<short>::Enumerate();

	//Show wat we got
	for (auto device : devices) {
		std::wcout << "Found Output Device: " << device << std::endl;
	}

	//Create the sound machine with selected device
	olcNoiseMaker<short> sound(devices[0], 44100, 1, 8, 512);

	sound.SetUserFunction(MakeNoise);

	while (true) {

	}


	return EXIT_SUCCESS;
}

double MakeNoise(double dTime) {

	double hertz = 440.0;

	return 0.5 * sin(hertz * 2 * 3.14159 * dTime);
}