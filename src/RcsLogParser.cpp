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
#include <Log.h>

#include <cassert>

using namespace std;

void acceleratedInc(int& val, int num, int scaling = 1);
void acceleratedDec(int& val, int num, int scaling = 1);

int main(int argc, char* argv[]) {
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

	Screen screen(log);

	int currLine = 0;
	int lineOffset = 0;

	std::string search;

	bool filtered = false;

	screen.drawLog(currLine,filtered);
	screen.refresh();

	int numSameInput = 0;
	int lastInput = 0;
	int input = 0;
	while((input = ::getch()) != 'q') {
		if(lastInput == input) ++numSameInput;
		else numSameInput = 0;

		switch(input) {
			case KEY_UP:
				acceleratedDec(currLine,numSameInput);
			break;
			case KEY_DOWN:
				acceleratedInc(currLine,numSameInput);
			break;
			case KEY_LEFT:
				if(lineOffset >= 10) lineOffset -= 10;
			break;
			case KEY_RIGHT:
				lineOffset += 10;
			break;
			case KEY_PPAGE:
			{
				acceleratedDec(currLine,numSameInput,screen.getRows());
			}
			break;
			case KEY_NPAGE:
				acceleratedInc(currLine,numSameInput,screen.getRows());
			break;
			case 's':
			case '/':
			{
				::addstr("search> ");
				search = screen.getInputLine();
				int foundLine = log.searchForLineContaining(currLine,search);
				if(foundLine != log.getNumLines()) {
					currLine = foundLine;
				}
			}
			break;
			case 'n':
			case KEY_ENTER:
			{
				int foundLine = log.searchForLineContaining(currLine+1,search);
				if(foundLine != log.getNumLines()) {
					currLine = foundLine;
				}
			}
			break;
			case KEY_END:
			{
				log.getLine(UINT_MAX);
				currLine = log.getNumLines();
			}
			break;
			case KEY_HOME:
			{
				currLine = 0;
			}
			break;
			case KEY_IC:
				filtered = !filtered;
				break;
			case '1':
				screen.format ^= TriFormatMask::line;
				break;
			case '2':
				screen.format ^= TriFormatMask::time;
				break;
			case '3':
				screen.format ^= TriFormatMask::timeDiff;
				break;
			case '4':
				screen.format ^= TriFormatMask::card;
				break;
			case '5':
				screen.format ^= TriFormatMask::traceLevel;
				break;
			case '6':
				screen.format ^= TriFormatMask::cpuId;
				break;
			case '7':
				screen.format ^= TriFormatMask::process;
				break;
			case '8':
				screen.format ^= TriFormatMask::traceObj;
				break;
			case '9':
				screen.format ^= TriFormatMask::fileAndLine;
				break;
			case '0':
				screen.format ^= TriFormatMask::msg;
				break;
			case KEY_RESIZE:
				screen.updateSize();
		}

		if(currLine + screen.getRows() > log.getNumLines()) currLine = log.getNumLines() - screen.getRows();
		screen.drawLog(currLine,filtered,lineOffset);
		screen.refresh();
		lastInput = input;
	}

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
