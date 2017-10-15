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

#ifndef PARSINGEXCEPTION_H_
#define PARSINGEXCEPTION_H_

#include <exception>
#include <string>

class ParsingException : public std::exception {
public:
	ParsingException(std::string _message) : message{_message}, pos{nullptr} {
	}

	ParsingException(std::string fileName, int lineNr, int linePos, std::string _message) : pos{nullptr}{
		message += fileName;
		message += ":";
		message += std::to_string(lineNr);
		message += ":";
		message += std::to_string(linePos);
		message += "\tparse error:\t";
		message += _message;
	}

	ParsingException(std::string _message, const char* _pos) : message{_message}, pos{_pos} {
	}

	virtual ~ParsingException() {}

	virtual const char* what() const noexcept {
		return message.c_str();
	}

	bool hasStringPos() const {
		return pos != nullptr;
	}

	const char* getStringPos() const {
		return pos;
	}

private:
	std::string message;
	const char* pos;
};

#endif /* PARSINGEXCEPTION_H_ */
