/*
 * Screen.cpp
 *
 *  Created on: Oct 31, 2015
 *      Author: jolt
 */

#include "Screen.h"
#include <iostream>

using namespace std;

size_t Screen::getRows() const {
	return rows;
}

size_t Screen::getCols() const {
	return cols;
}

void Screen::updateSize() {
	getmaxyx(stdscr, rows, cols);
    printBuf.resize(rows);
}

Screen::Screen(const Log& _log) : log(_log) {
	::initscr();          /* Start curses mode          */
    ::noecho();
    ::keypad(stdscr, TRUE);   // for KEY_UP, KEY_DOWN
    updateSize();
}

Screen::~Screen() {
	::endwin();           /* End curses mode        */
}

void Screen::drawLog(size_t startLine, size_t lineOffset) {
	::clear();
	size_t numLinesToPrint = rows - 1;
	for(size_t i = 0; i < numLinesToPrint; ++i) {
		size_t line = startLine + i;
		printBuf.at(i) = log.getLine(line,cols,lineOffset);
		::move(i,0);
		::printw("[%d] %s",line,printBuf.at(i).c_str());
	}
	::move(numLinesToPrint,0);
	::addstr(string(cols,' ').c_str());
	::move(numLinesToPrint,0);
	::addch(':');
	::refresh();          /* Print it on to the real screen */
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
			size_t inputSize = ret.size();
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
