#include <iostream>
#include <vector>
#include <algorithm>
#include <mutex>
#include <cstdio>

#include "olcNoiseMaker.h"
#include "instruments/harmonica.h"
#include "instruments/bell.h"

double dMasterVolume = 0.1;

synth::Instrument* voice1 = nullptr;
synth::Instrument* voice2 = nullptr;
std::vector<synth::Note> vecNotes;
std::mutex muxNotes;


double MakeNoise(int nChannel, double dTime);

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

	voice1 = new synth::Harmonica();
	voice2 = new synth::Bell();

	sound.SetUserFunction(MakeNoise);

	// Sit in loop, capturing keyboard state changes and modify
	// synthesizer output accordingly
	int nCurrentKey = -1;
	bool bKeyPressed = false;
	while (true) {

		bKeyPressed = false;
		for (int k = 0; k < 16; k++) {

			short nKeyState = GetAsyncKeyState((unsigned char)("ZSXCFVGBNJMK\xbcL\xbe\xbf"[k]));

			double dTimeNow = sound.GetTime();

			// Check if note already exists in currently playing notes
			muxNotes.lock();
			auto noteFound = std::find_if(vecNotes.begin(), vecNotes.end(), [&k](synth::Note const& item) { 
				return item.id == k; 
			});

			// Note not found in vector
			if (noteFound == vecNotes.end()){
				if (nKeyState & 0x8000){
					// Key has been pressed so create a new note
					synth::Note n;
					n.id = k;
					n.on = dTimeNow;
					n.channel = 1;
					n.active = true;

					// Add note to vector
					vecNotes.emplace_back(n);
				} else {
					// Note not in vector, but key has been released...
					// ...nothing to do
				}
			} else {
				// Note exists in vector
				if (nKeyState & 0x8000) {
					// Key is still held, so do nothing
					if (noteFound->off > noteFound->on) {
						// Key has been pressed again during release phase
						noteFound->on = dTimeNow;
						noteFound->active = true;
					}
				} else {
					// Key has been released, so switch off
					if (noteFound->off < noteFound->on) {
						noteFound->off = dTimeNow;
					}
				}
			}
			muxNotes.unlock();
		}

		wcout << "\rNotes: " << vecNotes.size() << "    ";
	}

	return EXIT_SUCCESS;
}



typedef bool(*lambda)(synth::Note const& item);
template<class T>
void safe_remove(T &v, lambda f) {
	auto n = v.begin();
	while (n != v.end()) {
		if (!f(*n)) {
			n = v.erase(n);
		}
		else {
			++n;
		}
	}
}

//Used to generate sound waves, returning an amplitude (-1.0 to +1.0) as a function of time
double MakeNoise(int nChannel, double dTime) {

	unique_lock<std::mutex> lm(muxNotes);
	double dMixedOutput = 0.0;

	for (auto &n : vecNotes) {
		bool bNoteFinished = false;
		double dSound = 0;
		
		if (n.channel == 2) {
			dSound = voice1->sound(dTime, n, bNoteFinished);
		}

		if (n.channel == 1) {
			dSound = voice2->sound(dTime, n, bNoteFinished)  * 0.5;
		}

		dMixedOutput += dSound;

		if (bNoteFinished && n.off > n.on) {
			n.active = false;
		}
	}

	safe_remove<std::vector<synth::Note>>(vecNotes, [](synth::Note const& item) { 
		return item.active; 
	});


	return dMixedOutput * 0.2;
}