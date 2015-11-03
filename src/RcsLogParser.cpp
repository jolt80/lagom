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

using namespace std;

void acceleratedInc(size_t& val, int num, size_t scaling = 1);
void acceleratedDec(size_t& val, int num, size_t scaling = 1);

int main(int argc, char* argv[]) {
	Log log;
	if(!log.map(argv[1])) {
		return 1;
	}

	Screen screen(log);

	size_t currLine = 0;
	size_t lineOffset = 0;

	std::string search;

	screen.drawLog(currLine);
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
				size_t foundLine = log.searchForLineContaining(currLine,search);
				if(foundLine != log.getNumLines()) {
					currLine = foundLine;
				}
			}
			break;
			case 'n':
			case KEY_ENTER:
			{
				size_t foundLine = log.searchForLineContaining(currLine+1,search);
				if(foundLine != log.getNumLines()) {
					currLine = foundLine;
				}
			}
			break;

			case KEY_RESIZE:
				screen.updateSize();
		}

		if(currLine + screen.getRows() > log.getNumLines()) currLine = log.getNumLines() - screen.getRows();
		screen.drawLog(currLine,lineOffset);
		screen.refresh();
		lastInput = input;
	}

	return 0;
}

void acceleratedInc(size_t& val, int num, size_t scaling) {
	size_t toInc;

	if ( num > 100 ) {
		toInc = scaling * 10;
	} else if ( num > 50 ) {
		toInc = scaling * 5;
	} else {
		toInc = scaling;
	}

	val += toInc;
}

void acceleratedDec(size_t& val, int num, size_t scaling) {
	size_t toDec;

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
