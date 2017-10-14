/*
 * Logger.h
 *
 *  Created on: Nov 8, 2015
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

#ifndef LOGGER_H_
#define LOGGER_H_

#include <map>
#include <mutex>
#include <fstream>
#include <thread>

class Logger : public std::ostream {
public:
	std::mutex mutex;
	std::ofstream of;

	Logger(const char* logFileName);
	virtual ~Logger();

	void log(std::string);

	void registerClient(std::string name);

    template<typename T>
    Logger& operator<< (const T& data)
    {
//        of << data;

        return *this;
    }

private:
	std::map<std::thread::id,std::string> clients;
};

#endif /* LOGGER_H_ */
