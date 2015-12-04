/*
 * Screen.cpp
 *
 *  Created on: Oct 31, 2015
 *      Author: Tomas Szabo
 *
 *  This program is free software: you can redistribute it and/or modify
 *  it under the terms of the GNU Affero General Public License as published by
 *  the Free Software Foundation, either version 3 of the License, or
 *  (at your option) any later version.
 *
 *  This program is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU Affero General Public License for more details.
 *
 *  You should have received a copy of the GNU Affero General Public License
 *  along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */

#include "Screen.h"
#include <iostream>
#include <Logger.h>
#include <cassert>

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

Screen::Screen(LogView* _logView, State& _state, Settings& _settings) : logView(_logView), currentState(_state), settings(_settings) {
	::initscr();          /* Start curses mode          */
	::noecho();
	::keypad(stdscr, TRUE);   // for KEY_UP, KEY_DOWN
	//::nodelay(stdscr, TRUE);
	if(has_colors() == FALSE)
	{
		endwin();
		printf("Your terminal does not support color\n");
		exit(1);
	}
	start_color();			/* Start color 			*/
	use_default_colors();
	init_pair(1, COLOR_RED, -1);
	updateSize();
}

void Screen::setLogView(LogView* newLogView) {
	logView = newLogView;
}

Screen::~Screen() {
	::move(0,0);
	::clear();
	::endwin();           /* End curses mode        */
}

void Screen::printToken(StringLiteral _token, int formatIndex, bool printSeparatorPrefix) {
	StringLiteral token{_token};
	const TokenDefinition& tokenDefinition = settings.getTokenDefinition(formatIndex);
	int ypos;
	int xpos;
	if(printSeparatorPrefix) {
		attron(COLOR_PAIR(1));
		::addstr("|");
		attroff(COLOR_PAIR(1));
	}
	getyx(stdscr, ypos, xpos);

	assert(xpos < cols);
	// maximum possible chars to print
	int charsToPrint = tokenDefinition.getWidth();

	// limit to available space
	if(charsToPrint > cols - xpos) {
		charsToPrint = cols - xpos;
	}

	// pad with blanks if not long enough
	int blanksToPrint = 0;
	if(token.getLength() < charsToPrint) {
		blanksToPrint = charsToPrint - token.getLength();
	}
	// Trim from either end depending on crop alignment
	else if(token.getLength() > charsToPrint) {
		if(tokenDefinition.getCrop() == Alignment::LEFT) {
			token.trimFromStart(token.getLength() - charsToPrint);
		}
		else {
			token.trimFromEnd(token.getLength() - charsToPrint);
		}
	}

	if(tokenDefinition.getAlignment() == Alignment::RIGHT) {
		for(int k = 0; k < blanksToPrint; ++k) {
			::addch(' ');
		}
		for(int k = 0; k < charsToPrint && k < token.getLength(); ++k) {
			::addch(token[k]);
		}
	}
	else {
		for(int k = 0; k < charsToPrint && k < token.getLength(); ++k) {
			::addch(token[k]);
		}
		for(int k = 0; k < blanksToPrint; ++k) {
			::addch(' ');
		}
	}
}

void Screen::printToken(StringLiteral token) {
	int ypos;
	(void)ypos; // supress not used
	int xpos;
	getyx(stdscr, ypos, xpos);
	if( (currentState.tokenVisible[0]) != 0) {
		attron(COLOR_PAIR(1));
		::addstr("|");
		attroff(COLOR_PAIR(1));
	}

	int charsToPrint = cols - xpos; // maximum possible chars to print
	if(token.getLength() < charsToPrint) charsToPrint = token.getLength();
	for(int k = 0; k < charsToPrint; ++k) {
		::addch(token[k]);
	}
}

void Screen::printLine(int line) {
	if( (currentState.tokenVisible[0]) != 0) {
		::printw("%5d",line);
	}
}

void Screen::drawLog() {
	logger.log("entering with currLine = " + to_string(currentState.currLine));

	// Find starting line and how many lines should be drawn
	int numLinesToPrint = rows - 1;
	if(currentState.currLine > (logView->getNumLines() - rows + 1)) {
		// One more line to take into account the bottom line
		currentState.currLine = logView->getNumLines() - rows + 1;
		if(currentState.currLine < 0) {
			currentState.currLine = 0;
		}
	}
	if(logView->getNumLines() < numLinesToPrint) {
		numLinesToPrint = logView->getNumLines();
	}
	if(currentState != lastDrawnState) {
		logView->getLine(currentState.currLine);
		if(currentState.currLine > logView->getNumLines()) {
			currentState.forceUpdate = true;
			return drawLog();
		}
		logger.log("entering for-loop with currline " + to_string(currentState.currLine));
		::erase();

		for(int i = 0; i < numLinesToPrint; ++i) {
			int tokensPrinted{0};
			int logLine  =currentState.currLine + i;
			int lineNumber = logView->getLineNumber(logLine);
			::move(i,0);
			if(currentState.tokenVisible[0]) {
				printLine(lineNumber);
				tokensPrinted++;
			}

			if(currentState.tokenized) {
				std::string** tokens = logView->getLogTokens(logLine);
				if(tokens != nullptr) {
					for(int j = 0; j < 9; ++j) {
						int formatIndex = j+1;
						if(currentState.tokenVisible[j+1]) {
							printToken(StringLiteral{*(tokens[j])},formatIndex,tokensPrinted != 0);
							tokensPrinted++;
						}
					}
					continue;
				}
			}
			//printToken(logView->getLine(line,cols,currentState.lineOffset));
			printToken(StringLiteral{logView->getLine(logLine)});
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

std::string Screen::getInputLine(History* history) {
	int start_x, start_y, x, y;
	getyx(stdscr, start_y, start_x);

	int c;
	std::string input;
	while((c = ::getch()) != '\n') {
		getyx(stdscr, y, x);
		int inputSize = (int)input.size();
		int cursorPos = x - start_x;

		switch(c) {
		case 27: //ESC
		{
			::move(start_y,start_x);
			fillRestOfLine(' ');
			::move(start_y,start_x);
			return std::string{};
		}
		break;
		case KEY_HOME:
		case 545: // Ctrl+Left
		case 1: // Ctrl+a
		{
			x = start_x;
		}
		break;
		case KEY_END:
		case 560: // Ctrl+Right
		case 5: // Ctrl+e
		{
			x = start_x + inputSize;
		}
		break;
		case KEY_UP:
		{
			if(history) {
				std::string entry = history->getPrevEntry();
				input = entry;
				inputSize = input.size();
				x = start_x + inputSize;
			}
		}
		break;
		case KEY_DOWN:
		{
			if(history) {
				std::string entry = history->getNextEntry();
				input = entry;
				inputSize = input.size();
				x = start_x + inputSize;
			}
		}
		break;
		case KEY_LEFT:
		{
			if(x > start_x)
				x--;
		}
		break;
		case KEY_RIGHT:
		{
			if(cursorPos < inputSize)
				x++;
		}
		break;
		case KEY_BACKSPACE:
		{
			if(cursorPos > 0 && cursorPos <= inputSize) {
				input.erase(cursorPos-1,1);
				x--;
			}
		}
		break;
		case KEY_DC:
		{
			int inputSize = (int)input.size();

			if(cursorPos >= 0 && cursorPos < inputSize) {
				input.erase(cursorPos,1);
			}
		}
		break;
		// Ctrl+k
		case 11:
		{
			int inputSize = (int)input.size();

			if(cursorPos >= 0 && cursorPos < inputSize) {
				input.erase(cursorPos);
			}
		}
		break;
		default: // everything in reasonable ascii territory
		{
			if(c >= ' ' && c <= '~') {
				if(history) history->resetPosition();
				input.insert(cursorPos,1,c);
				x++;
			}
//			ret += std::to_string(c);
//			ret += " ";
		}

		}

		::move(start_y,start_x);
		fillRestOfLine(' ');
		::move(start_y,start_x);
		::printw(input.c_str());
		::move(y,x);
	}
	if(history && !input.empty()) history->addEntry(input);
	return input;
}

void Screen::fillRestOfLine(int c) {
	int ypos, xpos;
	getyx(stdscr, ypos, xpos);
	for(int pos{xpos}; pos < cols; ++pos) {
		::addch(c);
	}
	::move(ypos,xpos);
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
