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
#include <re2/stringpiece.h>

namespace TriFormatMask {
	static const uint32_t line                {0b1}; // 1
	static const uint32_t time               {0b10}; // 2
	static const uint32_t timeDiff          {0b100}; // 3
	static const uint32_t card             {0b1000}; // 4
	static const uint32_t traceLevel      {0b10000}; // 5
	static const uint32_t cpuId          {0b100000}; // 6
	static const uint32_t process       {0b1000000}; // 7
	static const uint32_t traceObj     {0b10000000}; // 8
	static const uint32_t fileAndLine {0b100000000}; // 9
	static const uint32_t msg        {0b1000000000}; // 0
}

class Screen {
public:
	uint32_t format = UINT_MAX;

	Screen(const Log& _log);
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
	const Log& log;
	re2::StringPiece s[20];

	bool isNumberOrLetter(int c) const;

	int rows;
	int cols;

	std::vector<std::string> printBuf;
};

#endif /* SCREEN_H_ */
