/*
 * Screen.cpp
 *
 *  Created on: Oct 31, 2015
 *      Author: jolt
 */

#include "Screen.h"
#include <iostream>
#include <Logger.h>

extern Logger logger;

using namespace std;
using namespace std::chrono;

int Screen::getRows() const {
	return rows;
}

int Screen::getCols() const {
	return cols;
}

void Screen::updateSize() {
	getmaxyx(stdscr, rows, cols);
	//currentState.forceUpdate = true;
}

Screen::Screen(const Log& _log, State& _state) : log(_log), currentState(_state) {
	::initscr();          /* Start curses mode          */
	::noecho();
	::keypad(stdscr, TRUE);   // for KEY_UP, KEY_DOWN
	if(has_colors() == FALSE)
	{
		endwin();
		printf("Your terminal does not support color\n");
		exit(1);
	}
	start_color();			/* Start color 			*/
	use_default_colors();
	init_pair(1, COLOR_RED, -1);
	lastDrawnState.format = 0;
	updateSize();
}

Screen::~Screen() {
	::move(0,0);
	::clear();
	::endwin();           /* End curses mode        */
}

void Screen::printToken(re2::StringPiece token, int formatIndex) {
	int ypos;
	int xpos;
	if(formatIndex > 1) {
		attron(COLOR_PAIR(1));
		::addstr("|");
		attroff(COLOR_PAIR(1));
	}
	getyx(stdscr, ypos, xpos);

	int charsToPrint = cols - xpos;
	if(token.size() < charsToPrint) charsToPrint = token.size();

	for(int k = 0; k < charsToPrint; ++k) {
		::addch(token[k]);
	}
	::move(ypos,xpos + charsToPrint);
}

void Screen::printToken(re2::StringPiece token) {
	int ypos;
	int xpos;
	getyx(stdscr, ypos, xpos);

	int charsToPrint = cols - xpos;
	if(token.size() < charsToPrint) charsToPrint = token.size();
	for(int k = 0; k < charsToPrint; ++k) {
		::addch(token[k]);
	}
}


void Screen::drawLog() {
	logger.log("entering with currLine = " + to_string(currentState.currLine));

	// Find starting line and how many lines should be drawn
	int numLinesToPrint = rows - 1;
	if(currentState.currLine > (log.getNumLines() - rows + 1)) {
		// One more line to take into account the bottom line
		currentState.currLine = log.getNumLines() - rows + 1;
		if(currentState.currLine < 0) {
			currentState.currLine = 0;
		}
	}
//	if(log.getNumLines() < numLinesToPrint) {
//		numLinesToPrint = log.getNumLines();
//	}

	logger.log("currLine = " + to_string(currentState.currLine));

	if(currentState != lastDrawnState) {
		::clear();
		uint32_t formatMask;

		for(int i = 0; i < numLinesToPrint; ++i) {
			int line = currentState.currLine + i;
			::move(i,0);
			if( (currentState.format & TriFormatMask::line) != 0) {
				::printw("%d",line);
				attron(COLOR_PAIR(1));
				::addstr("|");
				attroff(COLOR_PAIR(1));
			}

			if(currentState.filtered) {
				if(log.getTriLogTokens(line,s)) {
					for(int j = 0; j < 9; ++j) {
						int formatIndex = j+1;
						formatMask = 1 << formatIndex;
						if( (currentState.format & formatMask) != 0) {
							re2::StringPiece token = s[j];
							printToken(token,formatIndex);
						}
					}
					continue;
				}
			}
			printToken(log.getLine(line,cols,currentState.lineOffset));
		}
		::move(rows - 1,0);
		::addstr(string(cols,' ').c_str());
		::move(rows - 1,0);
		::addch(':');
		::refresh();          /* Print it on to the real screen */
		lastDrawnState = currentState;
		lastDrawnState.forceUpdate = false;
	}
}

std::string Screen::getInputLine() {
	int c;
	std::string ret;
	while((c = ::getch()) != '\n') {
		if(c >= ' ' && c <= '~') {
			::addch(c);
			ret += (char)c;
		}
		else if( c == KEY_BACKSPACE) {
			int inputSize = ret.size();
			if(inputSize > 0)
			{
				--inputSize;
				ret.resize(inputSize);
				int ypos, xpos;
				getyx(stdscr, ypos, xpos);
				xpos--;
				::move(ypos,xpos);
				::addch(' ');
				::move(ypos,xpos);
			}
		}
	}
	return ret;
}

int Screen::getInputInteger() {
	int c;
	std::string ret;
	while((c = ::getch()) != '\n') {
		if(c >= '0' && c <= '9') {
			::addch(c);
			ret += (char)c;
		}
		else if( c == KEY_BACKSPACE) {
			int inputSize = ret.size();
			if(inputSize > 0)
			{
				--inputSize;
				ret.resize(inputSize);
				int ypos, xpos;
				getyx(stdscr, ypos, xpos);
				xpos--;
				::move(ypos,xpos);
				::addch(' ');
				::move(ypos,xpos);
			}
		}
	}
	return std::atoi(ret.c_str());
}



bool Screen::isNumberOrLetter(int c) const {
	if(c >= '0' && c <= '9') return true;
	if(c >= 'A' && c <= 'Z') return true;
	if(c >= 'a' && c <= 'z') return true;
	return false;
}


int Screen::getInput() {
	return ::getch();            /* Wait for user input */
}

void Screen::refresh() {
	::refresh();
}

void Screen::print(const std::string str) {
	::printw(str.c_str());  /* Print string          */
}

void Screen::print(const char* str) {
	::printw(str);  /* Print string          */
}

void Screen::println(const std::string str) {
	std::string print(str + '\n');
	::printw(print.c_str());  /* Print string          */
}

void Screen::println(const char* str) {
	std::string print(str);
	print += '\n';
	::printw(print.c_str());  /* Print string          */
}

bool Screen::areLinesScannedForWidths() const {
	return lastLineScannedForWidths == log.getNumLines();
}

void Screen::scanForWidths(long maxDuration) {
	logger << "enter scanForWidths\n";

	int startLine = lastLineScannedForWidths;
	auto start = high_resolution_clock::now();

	for(; lastLineScannedForWidths < log.getNumLines(); ++lastLineScannedForWidths) {
		// Regex matching a TRI Log line
		if(log.getTriLogTokens(lastLineScannedForWidths,s)) {
			for(int i = 0; i < 12; ++i)
			{
				if(currentState.width[i] < s[i].size() && s[i].size() <= maxWidth[i]) currentState.width[i] = s[i].size();
			}
		}

		if(lastLineScannedForWidths % 1000 == 0) {
			auto end = high_resolution_clock::now();
			auto duration = end - start;
			if(duration_cast<microseconds>(duration).count() > maxDuration) break;
		}
	}

	auto end = high_resolution_clock::now();
	auto duration = end - start;

	logger << "scanned " << lastLineScannedForWidths - startLine << " for widths in " << duration_cast<microseconds>(duration).count() << " us\n";

}
