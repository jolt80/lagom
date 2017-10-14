/*
 * LogViewFactory.h
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

#ifndef LOGVIEWREPOSITORY_H_
#define LOGVIEWREPOSITORY_H_

#include <list>

#include "filtered_log_view.h"

class LogViewRepository {
public:
	LogViewRepository(Log& _log) : log(_log) {};
	virtual ~LogViewRepository();

	LogView* getFilteredLogView(std::string pattern);

	static bool isMultiplePattern(std::string& pattern);

	std::string getLastErrorMessage();
private:
	std::string errorMessage;
	std::vector<int>* buildVectorOfMatchingLines(std::string pattern);
	std::list<std::string> splitMultiplePattern(std::string& pattern);

	Log& log;
	std::map<std::string,FilteredLogView*> filteredViews;
};

#endif /* LOGVIEWREPOSITORY_H_ */
