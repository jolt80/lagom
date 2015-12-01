/*
 * Screen.h
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

#ifndef SCREEN_H_
#define SCREEN_H_

#include <State.h>
#include <ncurses.h>
#include <string>
#include <re2/stringpiece.h>
#include <chrono>
#include <Settings.h>
#include <LogView.h>

class Screen {
public:
	LogView* logView;
	State& currentState;
	Settings& settings;

	Screen(LogView* _logView, State& _state, Settings& _settings);
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

	void setLogView(LogView* newLogView);

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
