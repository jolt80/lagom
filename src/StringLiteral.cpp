#include <StringLiteral.h>
#include <sstream>
#include <stdint.h>

const char* StringLiteral::getStr() const
{
  return str;
}

const char* StringLiteral::getStrEnd() const
{
  return str + strLen;
}


size_t StringLiteral::getLength() const
{
  return strLen;
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

StringLiteral StringLiteral::subString( size_t start, size_t length) const
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

size_t StringLiteral::findFirstOf(char charToFind)const
{
  const char* found = strchr(str, charToFind);
  if(nullptr == found)
  {
    return strLen;
  }
  else
  {
    //lint is wrong here, found is not going to be null
    //lint -esym(613, found)
    return found - str;
  }
}

bool StringLiteral::startsWith(const StringLiteral& compareStr) const
{
  if(compareStr.strLen > strLen)
  {
    return false;
  }

  return 0 == strncmp(str,compareStr.str,compareStr.strLen);
}

size_t StringLiteral::contains(std::string containsStr) const {
	size_t searchSize = containsStr.size();
	size_t searchIndex = 0;
	for(size_t i{0}; i < strLen; ++i) {
		if(str[i] == containsStr[searchIndex]) {
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


bool StringLiteral::trimFromStart(size_t offset)
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

bool StringLiteral::trimFromEnd(size_t offset)
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

  for (uint32_t index = 0; index < strLen; ++index) {
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
