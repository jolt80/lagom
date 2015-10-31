/*
 * Screen.cpp
 *
 *  Created on: Oct 31, 2015
 *      Author: jolt
 */

#include "Screen.h"
#include <iostream>

using namespace std;

Screen::Screen() {
	::initscr();          /* Start curses mode          */
}

Screen::~Screen() {
	::endwin();           /* End curses mode        */
}

void Screen::refresh() {
	::refresh();          /* Print it on to the real screen */
}

std::string Screen::getInput() {
	char c;
	std::string ret;
	do {
	 c = ::getch();            /* Wait for user input */
	 ret += c;
	} while(c != '\n');
	return ret;
}

void Screen::print(const std::string str) {
	::printw(str.c_str());  /* Print string          */
	refresh();
}

void Screen::print(const char* str) {
	::printw(str);  /* Print string          */
	refresh();
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
