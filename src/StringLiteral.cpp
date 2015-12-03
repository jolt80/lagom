/*
 * StringLiteral.cpp
 *
 *  Created on: Nov 2, 2015
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
 *
 */

#include <StringLiteral.h>
#include <sstream>
#include <stdint.h>
#include <iostream>
#include <cassert>

using namespace std;

const char* StringLiteral::getStr() const
{
  return str;
}

const char* StringLiteral::getStrEnd() const
{
  return str + strLen;
}


int StringLiteral::getLength() const
{
  return strLen;
}

const char StringLiteral::operator [](int i) const {
	const char* pos = str+i;
	return *pos;
}

bool StringLiteral::operator<( const StringLiteral & rhs ) const
{
  if(this->strLen < rhs.strLen) return true;
  else return strncmp(str,rhs.str,strLen) < 0;
}

bool StringLiteral::operator==( const char * compareStr ) const
{
  return 0 == strncmp(str,compareStr,strLen);
}

bool StringLiteral::operator==( const std::string& compareStr ) const
{
  return 0 == strncmp(str,compareStr.c_str(),strLen);
}

bool StringLiteral::operator==( const StringLiteral& compareStr ) const
{
  return strLen == compareStr.strLen && 0 == strncmp(str,compareStr.str,strLen);
}

bool StringLiteral::operator!=( const char * compareStr ) const
{
  return !(*this == compareStr);
}

bool StringLiteral::operator!=( const std::string& compareStr ) const
{
  return !(*this == compareStr);
}

bool StringLiteral::operator!=( const StringLiteral& compareStr ) const
{
  return !(*this == compareStr);
}

std::string StringLiteral::toString() const
{
  return std::string(str,strLen);
}

StringLiteral& StringLiteral::operator=( const StringLiteral& other )
{
  this->str = other.str;
  this->strLen = other.strLen;
  return *this;
}


StringLiteral& StringLiteral::operator=( const char * other )
{
  this->str = other;
  this->strLen = strlen(other);
  return *this;
}

StringLiteral StringLiteral::subString( int start, int length) const
{
  if(start + length <= strLen)
  {
    return StringLiteral{str+start,length};
  }
  else
  {
    return StringLiteral{};
  }
}

StringLiteral StringLiteral::subString( std::pair<int,int> startAndEnd) const {
	assert(startAndEnd.second >= startAndEnd.first);
	return subString(startAndEnd.first, startAndEnd.second - startAndEnd.first);
}

int StringLiteral::findFirstOf(char charToFind)const
{
	for(int i{0}; i < strLen; ++i) {
		if(str[i] == charToFind) return i;
	}
	return strLen;
}

std::pair<int,int> StringLiteral::findFirstAndSecondOf(char charToFind) const
{
	int first = strLen;
	int second = strLen;
	int i;
	for(i = 0; i < strLen; ++i) {
		if(str[i] == charToFind) {
			first = i;
			break;
		}
	}
	for(; i < strLen; ++i) {
		if(str[i] == charToFind) {
			second = i;
			break;
		}
	}
	return std::pair<int,int>(first,second);
}


bool StringLiteral::startsWith(const StringLiteral& compareStr) const
{
  if(compareStr.strLen > strLen)
  {
    return false;
  }

  return 0 == strncmp(str,compareStr.str,compareStr.strLen);
}

int StringLiteral::contains(std::string containsStr) const {
	int searchSize = containsStr.size();
	int searchIndex = 0;
	for(int i{0}; i < strLen; ++i) {
		if(str[i] == containsStr[searchIndex]) {
			searchIndex++;
			if(searchIndex == searchSize) return true;
		} else {
			searchIndex = 0;
		}
	}
	return false;
}

int StringLiteral::containsCaseInsensitive(std::string containsStr) const {
	int searchSize = containsStr.size();
	int searchIndex = 0;
	for(int i{0}; i < strLen; ++i) {
		// Sloppy comparison but probably pretty efficient
		if((str[i] | 32 ) == (containsStr[searchIndex] | 32)) {
			searchIndex++;
			if(searchIndex == searchSize) return true;
		} else {
			searchIndex = 0;
		}
	}
	return false;
}

bool StringLiteral::empty() const
{
  return strLen == 0;
}

void StringLiteral::clear()
{
  str = nullptr;
  strLen = 0;
}


bool StringLiteral::trimFromStart(int offset)
{
  if(offset > strLen)
  {
    str = nullptr;
    strLen = 0;
    return false;
  }
  else
  {
    str += offset;
    strLen -= offset;
    return true;
  }
}

bool StringLiteral::trimFromEnd(int offset)
{
  if(offset > strLen)
  {
    str = nullptr;
    strLen = 0;
    return false;
  }
  else
  {
    strLen -= offset;
    return true;
  }
}

int32_t StringLiteral::toInt() const
{
  std::stringstream sstr(this->toString());
  int32_t ret;

  sstr >> ret;

  return ret;
}

/**
 * Not bad performance, but might be good to experiment a bit more with this.
 */
std::size_t StringLiteral::hash() const
{
  const char* input = str;
  std::size_t result = 0x55555555;

  for (int index = 0; index < strLen; ++index) {
    result ^= *input++;
    result = result << 5;
  }

  return result;
}

std::ostream& operator<<(std::ostream& os, const StringLiteral& stringLiteral)
{
  os << stringLiteral.toString();
  return os;
}
