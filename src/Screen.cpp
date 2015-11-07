/*
 * Screen.cpp
 *
 *  Created on: Oct 31, 2015
 *      Author: jolt
 */

#include "Screen.h"
#include <iostream>

using namespace std;

int Screen::getRows() const {
	return rows;
}

int Screen::getCols() const {
	return cols;
}

void Screen::updateSize() {
	getmaxyx(stdscr, rows, cols);
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

void Screen::drawLog(int startLine, bool filtered, int lineOffset) {

	if(currentState != lastDrawnState) {
		::clear();
		int numLinesToPrint = rows - 1;
		uint32_t formatMask;

		for(int i = 0; i < numLinesToPrint; ++i) {
			int line = startLine + i;
			::move(i,0);
			if(filtered) {
				if(log.getTriLogTokens(line,s)) {
					if( (currentState.format & TriFormatMask::line) != 0) {
						::printw("%d",line);
					}
					for(int j = 0; j < 11; ++j) {
						formatMask = 1 << (j+1);
						if( (currentState.format & formatMask) != 0) {
							attron(COLOR_PAIR(1));
							::addstr("|");
							attroff(COLOR_PAIR(1));
							for(auto character : s[j]) {
								::addch(character);
							}
						}
					}
				}
				else {
					::addstr(log.getLine(line,cols,lineOffset).c_str());
				}
			}
			else {
				::addstr(log.getLine(line,cols,lineOffset).c_str());
			}
		}
		::move(numLinesToPrint,0);
		::addstr(string(cols,' ').c_str());
		::move(numLinesToPrint,0);
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
