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


Screen::Screen(const Log& _log) : log(_log) {
	::initscr();          /* Start curses mode          */
    getmaxyx(stdscr, rows, cols);
    std::cout << rows << ", " << cols << endl;
    ::noecho();
    ::keypad(stdscr, TRUE);   // for KEY_UP, KEY_DOWN

    printBuf.resize(rows);
}

Screen::~Screen() {
	::endwin();           /* End curses mode        */
}

void Screen::drawLog(size_t startLine, size_t lineOffset) {
	::clear();
	for(size_t i = 0; i < rows; ++i) {
		size_t line = startLine + i;
		printBuf.at(i) = log.getLine(line,cols,lineOffset);
		::move(i,0);
		::addstr(printBuf.at(i).c_str());
	}
	::refresh();          /* Print it on to the real screen */
}

std::string Screen::getInputLine() {
	char c;
	std::string ret;
	do {
	 c = ::getch();            /* Wait for user input */
	 ret += c;
	} while(c != '\n');
	return ret;
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
