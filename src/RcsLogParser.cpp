//============================================================================
// Name        : RcsLogParser.cpp
// Author      : Tomas Szabo
// Version     :
// Copyright   : Free stuff
// Description : Hello World in C++, Ansi-style
//============================================================================

#include <iostream>
#include <string>
#include <Screen.h>
#include <State.h>
#include <Log.h>
#include <Logger.h>

#include <cassert>
#include <thread>

using namespace std;
using namespace std::chrono;

void acceleratedInc(int& val, int num, int scaling = 1);
void acceleratedDec(int& val, int num, int scaling = 1);

Logger logger(".debug_log");

int main(int argc, char* argv[]) {
	re2::StringPiece s[10];

	Settings settings;
	Log log(settings);

	if(!log.map(argv[1])) {
		return 1;
	}

	for(auto tokenizer : settings.getTokenizers()) {
		cout << "iteration" << endl;
		cout << tokenizer->toString() << endl;
	}

	cout << "before getTriLogTokens" << endl;
	log.getTriLogTokens(100,s);

}

void guiLoop(Screen& screen,State& state)  {
	logger.registerClient(" gui");
	while(state.running)
	{
		if(!screen.log.areLineNumbersParsed()) {
			screen.log.scanForLines(INT_MAX,10000);
		}
//		else if(!screen.areLinesScannedForWidths()) {
//			screen.scanForWidths(25000);
//		}
		else {
			usleep(10000);
		}
		screen.drawLog();
	}
}

int main2(int argc, char* argv[]) {
	if(argc != 2) {
		cout << "Usage: rcs_log_parser <logfile>" << endl;
		exit(1);
	}

	logger.registerClient("main");

	Settings settings;

	Log log(settings);

	if(!log.map(argv[1])) {
		return 1;
	}

	State currentState;
	Screen screen(log,currentState);

	// Spawn the gui thread
    thread gui(guiLoop,std::ref(screen),std::ref(currentState));

	int numSameInput = 0;
	int lastInput = 0;
	int input = 0;
	while((input = ::getch()) != 'q') {
		if(lastInput == input) ++numSameInput;
		else numSameInput = 0;

		switch(input) {
			case KEY_UP:
				acceleratedDec(currentState.currLine,numSameInput);
			break;
			case KEY_DOWN:
				acceleratedInc(currentState.currLine,numSameInput);
			break;
//			case KEY_LEFT:
//				if(currentState.lineOffset >= 10) currentState.lineOffset -= 10;
//			break;
//			case KEY_RIGHT:
//				currentState.lineOffset += 10;
//			break;
			case KEY_PPAGE:
			{
				acceleratedDec(currentState.currLine,numSameInput,screen.getRows());
			}
			break;
			case KEY_NPAGE:
				acceleratedInc(currentState.currLine,numSameInput,screen.getRows());
			break;
			case 's':
			case '/':
			{
				::addstr("search> ");
				currentState.search = screen.getInputLine();
				int foundLine = log.searchForLineContaining(currentState.currLine,currentState.search);
				if(foundLine != log.getNumLines()) {
					currentState.currLine = foundLine;
				}
			}
			break;
			case 'n':
			case KEY_ENTER:
			{
				int foundLine = log.searchForLineContaining(currentState.currLine+1,currentState.search);
				if(foundLine != log.getNumLines()) {
					currentState.currLine = foundLine;
				}
			}
			break;
			case KEY_END:
			{
				log.lineAt(INT_MAX);
				currentState.currLine = INT_MAX;
			}
			break;
			case KEY_HOME:
			{
				currentState.currLine = 0;
			}
			break;
			case 'l':
				::addstr("line> ");
				currentState.currLine = screen.getInputInteger();
				break;
			case KEY_IC:
				currentState.filtered = !currentState.filtered;
				break;
			case '1':
				currentState.format ^= TriFormatMask::line;
				break;
			case '2':
				currentState.format ^= TriFormatMask::time;
				break;
			case '3':
				currentState.format ^= TriFormatMask::timeDiff;
				break;
			case '4':
				currentState.format ^= TriFormatMask::card;
				break;
			case '5':
				currentState.format ^= TriFormatMask::traceLevel;
				break;
			case '6':
				currentState.format ^= TriFormatMask::cpuId;
				break;
			case '7':
				currentState.format ^= TriFormatMask::process;
				break;
			case '8':
				currentState.format ^= TriFormatMask::traceObj;
				break;
			case '9':
				currentState.format ^= TriFormatMask::fileAndLine;
				break;
			case '0':
				currentState.format ^= TriFormatMask::msg;
				break;
			case KEY_RESIZE:
				logger.log("Triggering resize");
				screen.updateSize();
		}
		lastInput = input;
	}

	currentState.running = false;

    gui.join();

	return 0;
}

void acceleratedInc(int& val, int num, int scaling) {
	int toInc;

	if ( num > 100 ) {
		toInc = scaling * 5;
	} else if ( num > 50 ) {
		toInc = scaling * 2;
	} else {
		toInc = scaling;
	}

	val += toInc;
}

void acceleratedDec(int& val, int num, int scaling) {
	int toDec;

	if ( num > 100 ) {
		toDec = scaling * 5;
	} else if ( num > 50 ) {
		toDec = scaling * 2;
	} else {
		toDec = scaling;
	}

	if(toDec > val) val = 0;
	else val -= toDec;
}
