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

void acceleratedInc(int& val, int num, int scaling = 1);
void acceleratedDec(int& val, int num, int scaling = 1);

Logger logger(".debug_log");

void guiLoop(Screen& screen,State& state)  {
	logger.registerClient(" gui");
	while(state.running)
	{
		if(!screen.log.areLineNumbersParsed()) {
			screen.log.scanForLines(INT_MAX,9500);
		}
		else if(!screen.areLinesScannedForWidths()) {
			screen.scanForWidths(9500);
		}
		else {
			usleep(10000);
		}
		screen.drawLog();
	}
}

int main(int argc, char* argv[]) {
	logger.registerClient("main");

	const char* timeStr = "\\[\\d{4}-\\d{2}-\\d{2}\\s*(\\d{1,2}:\\d{1,2}:\\d{1,2}\\.\\d{3}).*?\\]";
	const char* timeDiffStr = "\\((\\+\\d+\\.\\d+)\\)";
	const char* card = "(\\w+)";
	const char* traceLevel = "com_ericsson_triobjif:(.*?):";
	const char* cpuId = "{ cpu_id = (\\w+) },";
	const char* processAndObjIf = "\\{ processAndObjIf = \"(.*?)\\((.*?)\\)\",";
	const char* fileAndLine = "fileAndLine = \".*?(\\w+\\.\\w+:\\d+)\",";
	const char* msg = "msg = \"\\s*(.*)\" }";
	const char* separator = ".*?";

	std::string triRegex;

	triRegex += timeStr;
	triRegex += separator;
	triRegex += timeDiffStr;
	triRegex += separator;
	triRegex += card;
	triRegex += separator;
	triRegex += traceLevel;
	triRegex += separator;
	triRegex += cpuId;
	triRegex += separator;
	triRegex += processAndObjIf;
	triRegex += separator;
	triRegex += fileAndLine;
	triRegex += separator;
	triRegex += msg;
	triRegex += separator;

	Log log(triRegex);
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
			case KEY_LEFT:
				if(currentState.lineOffset >= 10) currentState.lineOffset -= 10;
			break;
			case KEY_RIGHT:
				currentState.lineOffset += 10;
			break;
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
				currentState.currLine = log.getNumLines() - screen.getRows();
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
		toInc = scaling * 10;
	} else if ( num > 50 ) {
		toInc = scaling * 5;
	} else {
		toInc = scaling;
	}

	val += toInc;
}

void acceleratedDec(int& val, int num, int scaling) {
	int toDec;

	if ( num > 100 ) {
		toDec = scaling * 10;
	} else if ( num > 50 ) {
		toDec = scaling * 5;
	} else {
		toDec = scaling;
	}

	if(toDec > val) val = 0;
	else val -= toDec;
}
