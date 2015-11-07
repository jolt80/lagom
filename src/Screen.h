/*
 * Screen.h
 *
 *  Created on: Oct 31, 2015
 *      Author: jolt
 */

#ifndef SCREEN_H_
#define SCREEN_H_

#include <Log.h>
#include <State.h>
#include <ncurses.h>
#include <string>
#include <re2/stringpiece.h>

class Screen {
public:
	const Log& log;
	State& currentState;

	Screen(const Log& _log, State& _state);
	virtual ~Screen();

	int getRows() const;
	int getCols() const;

	void updateSize();

	void print(const std::string str);
	void print(const char* str);

	void println(const std::string str);
	void println(const char* str);

	void drawLog(int startLine, bool filtered, int lineOffset = 0);

	void refresh();

	std::string getInputLine();
	int getInput();

protected:
	State lastDrawnState;
	re2::StringPiece s[20];

	bool isNumberOrLetter(int c) const;
	bool updateNeeded() const;

	int rows;
	int cols;
};

#endif /* SCREEN_H_ */
