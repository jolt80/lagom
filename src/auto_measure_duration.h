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

#ifndef AUTOMEASUREDURATION_H_
#define AUTOMEASUREDURATION_H_

#include <chrono>
#include <iostream>

class AutoMeasureDuration {
public:
	AutoMeasureDuration(std::ostream& _os, const std::string& _message) : os(_os), message(_message) {
		start = std::chrono::high_resolution_clock::now();
	}
	~AutoMeasureDuration() {
		end = std::chrono::high_resolution_clock::now();
		auto duration = end - start;

		//os << message << " took " << std::chrono::duration_cast<std::chrono::milliseconds>(duration).count() << " ms\n";
	}

private:
	std::ostream &os;
	std::string message;
	std::chrono::time_point<std::chrono::high_resolution_clock> start;
	std::chrono::time_point<std::chrono::high_resolution_clock> end;
};

#endif /* AUTOMEASUREDURATION_H_ */
