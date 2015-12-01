/*
 * Log.h
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

#ifndef LOG_H_
#define LOG_H_

#include <Settings.h>
#include <string>
#include <vector>
#include <climits>
#include <StringLiteral.h>
#include <string>

#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/mman.h>
#include <chrono>
#include <mutex>

#include <re2/re2.h>

struct Line {
	StringLiteral contents;
	bool tokenized = false;
	std::string** tokens = nullptr;

	Line(char* start, char* end) : contents{start,end}, tokens{nullptr} {}
	Line(const Line&) =delete;
	Line(Line&&) =delete;
	~Line() {
		if(tokens != nullptr) {
			for(int i{0}; i < 9; ++i) {
				delete tokens[i];
			}
			delete[] tokens;
		}
	}
};


class Log {
public:
	Log(Settings& _settings);
	virtual ~Log();

	bool map(std::string fileName);
	bool unmap();

	bool areLineNumbersParsed() const;
	bool areAllLinesTokenized();
	int getNumLines() const;

	int searchForLineContaining(int startLine, std::string search);

	re2::StringPiece getLine(int index);
	std::string** getLogTokens(int index);

	std::string toString() const;

	void scanForLines(int index, long maxDuration = 2000000);
	int tokenizeLines(int index, long maxDuration = 2000000);
protected:
	Line& lineAt(int index);
	void tokenizeLine(Line& line);
	void scanForLinesNotLocked(int index, long maxDuration = 2000000);

	mutable std::mutex mutex;

	int numLines;
	Settings& settings;
	char *fileStart;
	char* fileEnd;

	std::vector<Line*> lines;
};

#endif /* LOG_H_ */
