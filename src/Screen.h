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
#include <chrono>
#include <Settings.h>


class Screen {
public:
	Log& log;
	State& currentState;
	Settings& settings;

	Screen(Log& _log, State& _state, Settings& _settings);
	virtual ~Screen();

	int getRows() const;
	int getCols() const;

	void updateSize();

	void print(const std::string str);
	void print(const char* str);

	void println(const std::string str);
	void println(const char* str);

	void printToken(StringLiteral token, int formatIndex, bool printSeparatorPrefix);
	void printToken(StringLiteral token);
	void printLine(int line);

	void drawLog();

	void refresh();

	std::string getInputLine();
	int getInputInteger();
	int getInput();

protected:
	State lastDrawnState;

	bool isNumberOrLetter(int c) const;
	bool updateNeeded() const;

	int rows;
	int cols;
};

#endif /* SCREEN_H_ */
