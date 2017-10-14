/*
 * History.h
 *
 *  Created on: Dec 2, 2015
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

#ifndef HISTORY_H_
#define HISTORY_H_

#include <list>
#include <string>

class History {
public:
	History(std::string filePath);
	virtual ~History();

	std::string getPrevEntry();
	std::string getNextEntry();
	void addEntry(std::string entry);
	void moveCurrentToEnd();
	void resetPosition();
private:
	void readHistoryFile();
	void writeHistoryToFile();

	std::string filePath;

	std::list<std::string> storage;
	std::list<std::string>::iterator pos;
};

#endif /* HISTORY_H_ */
