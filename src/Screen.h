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

	void printToken(re2::StringPiece token, int formatIndex);
	void drawLog();

	void refresh();

	std::string getInputLine();
	int getInputInteger();
	int getInput();

	bool areLinesScannedForWidths() const;
	void scanForWidths(long maxDuration = 2000000);

protected:
	std::array<int,12> maxWidth{ { 12,13,12,12,3,30,30,30,1000,1000,1000,1000 } };

	State lastDrawnState;
	re2::StringPiece s[20];
	int lastLineScannedForWidths = 0;

	bool isNumberOrLetter(int c) const;
	bool updateNeeded() const;

	int rows;
	int cols;
};

#endif /* SCREEN_H_ */
