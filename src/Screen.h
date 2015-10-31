/*
 * Screen.h
 *
 *  Created on: Oct 31, 2015
 *      Author: jolt
 */

#ifndef SCREEN_H_
#define SCREEN_H_

#include <ncurses.h>
#include <string>

class Screen {
public:
	Screen();
	virtual ~Screen();

	void print(const std::string str);
	void print(const char* str);

	void println(const std::string str);
	void println(const char* str);

	void refresh();

	std::string getInput();
};

#endif /* SCREEN_H_ */
