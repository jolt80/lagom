/*
 * Screen.h
 *
 *  Created on: Oct 31, 2015
 *      Author: jolt
 */

#ifndef SCREEN_H_
#define SCREEN_H_

#include <Log.h>
#include <ncurses.h>
#include <string>

class Screen {
public:
	Screen(const Log& _log);
	virtual ~Screen();

	size_t getRows() const;
	size_t getCols() const;

	void print(const std::string str);
	void print(const char* str);

	void println(const std::string str);
	void println(const char* str);

	void drawLog(size_t startLine, size_t lineOffset = 0);

	void refresh();

	std::string getInputLine();
	int getInput();

protected:
	const Log& log;

	size_t rows;
	size_t cols;

	std::vector<std::string> printBuf;
};

#endif /* SCREEN_H_ */
