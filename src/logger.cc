/*
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

#include <chrono>
#include <ctime>
#include <iomanip>
#include <thread>
#include "auto_lock.h"
#include "logger.h"

using namespace std;
using namespace std::chrono;

Logger logger(".debug_log");

Logger::Logger(const char* logFileName) {
//	of.open (logFileName, std::ofstream::out | std::ofstream::app);
}

Logger::~Logger() {
//	of.flush();
//	of.close();
}

void Logger::log(std::string msg) {
	AutoLock lock(mutex);
//	high_resolution_clock::time_point tp = high_resolution_clock::now();
//	time_t now_c = system_clock::to_time_t(tp);
//	auto duration = tp.time_since_epoch();
//	auto millis = std::chrono::duration_cast<std::chrono::milliseconds>(duration).count() % 1000;
//
//	string name = clients.at(std::this_thread::get_id());

	//<< std::put_time(std::localtime(&now_c),"%H:%M:%S:")
//	of << "- " << millis << " - " << name << " - " << msg << endl;
//	of.flush();
}

void Logger::registerClient(std::string name) {
	//AutoLock lock(mutex);
	//clients.insert(std::pair<std::thread::id,std::string>(std::this_thread::get_id(),name));
}
