/*
 * Copyright (c) 2018 Tomas Szabo
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in all
 * copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
 * SOFTWARE.
 */

#ifndef STRINGLITERAL_H_
#define STRINGLITERAL_H_

#include <re2/stringpiece.h>
#include <cstring>
#include <functional>
#include <iostream>
#include <string>
#include <string>
#include <utility>

namespace lagom {

/**
 *
 * Wrapper class for a const char*.
 *
 * Fulfills requirements for functioning as key in std container classes.
 *
 * BEWARE!
 *
 * If constructed dynamically in code from a const char* or std::string
 * (ie not used as static const object) this class is only a wrapper for the
 * char buffer used in the constructor, it will never allocate any memory for
 * storing a copy of the string, therefore it will cause undefined behviour if
 * used after the char buffer used in the constructor is deleted.
 *
 */
class StringLiteral {
 public:
  /**
   * Constructors.
   */
  constexpr StringLiteral()
      : str{nullptr}
      , strLen{0} {}
  constexpr StringLiteral(const char* str_, size_t strLen_)
      : str{str_}
      , strLen{strLen_} {}

  // Construct a StringLiteral from an std::string or a const char*
  StringLiteral(std::string& string)
      : str{string.c_str()}
      , strLen{string.size()} {}
  StringLiteral(const char* str_)
      : str{str_}
      , strLen{strlen(str_)} {}
  StringLiteral(const char* startPtr, const char* endPtr)
      : str{startPtr}
      , strLen{size_t(endPtr - startPtr)} {}
  StringLiteral(re2::StringPiece sp)
      : str{sp.data()}
      , strLen{sp.size()} {}

  inline re2::StringPiece toStringPiece() const { return re2::StringPiece{str, strLen}; }

  const char* getStr() const;
  const char* getStrEnd() const;
  int getLength() const;

  /**
   * This is the operator used by std containers to sort key objects.
   * It has to provide strict weak ordering:
   *
   * Properties of strict weak ordering:
   *
   * irreflexive: r(x, x) == false
   * asymmetric: r(x, y) == true implies r(y, x) == false
   * transitive:  r(x, y) == true and r(y, z) == true implies r(x, z) == true
   * incomparability: r(x, y) == false and r(y, x) == false and r(y, z) == false and r(z, y) == false implies r(x, z) ==
   * false and r(z, x) == false
   *
   *  - Ordering by string length.
   *  - For equally long strings the ordering that strcmp provides (it fulfills the strict weak ordering criteria).
   */
  bool operator<(const StringLiteral& rhs) const;

  /**
   * Equality operators for some useful types.
   */
  bool operator==(const char* compareStr) const;
  bool operator==(const std::string& compareStr) const;
  bool operator==(const StringLiteral& compareStr) const;
  bool operator!=(const char* compareStr) const;
  bool operator!=(const std::string& compareStr) const;
  bool operator!=(const StringLiteral& compareStr) const;
  const char operator[](int i) const;

  StringLiteral& operator=(const StringLiteral& other);
  StringLiteral& operator=(const char* other);

  /**
   * Create a substring from this StringLiteral.
   *
   *  start  = offset from the beginning of the string
   *  length = the length of the new string
   *
   *  Returns:
   *    - StringLiteral with the substring if start and length are valid
   *    - Empty StringLiteral if combination of start and length is invalid
   */
  StringLiteral subString(int start, size_t length) const;
  StringLiteral subString(std::pair<int, int> startAndEnd) const;

  /**
   * Find first occurence of character in StringLiteral
   *
   *  Returns
   *   - offset from start of string
   *   - length of string if not found
   */
  int findFirstOf(char charToFind) const;
  std::pair<int, int> findFirstAndSecondOf(char charToFind) const;
  int contains(std::string containsStr) const;
  int containsCaseInsensitive(std::string containsStr) const;

  /**
   * Returns true if the StringLiteral starts with a specific string.
   */
  bool startsWith(const StringLiteral& compareStr) const;

  bool empty() const;
  void clear();

  bool trimFromStart(size_t offset);
  bool trimFromEnd(size_t offset);

  void trimWhitespaceFromStart();

  /**
   * Attempt to convert the contents to an integer.
   */
  int toInt() const;

  /**
   * Not bad performance, but might be good to experiment a bit more with this.
   */
  std::size_t hash() const;

  std::string toString() const;

 private:
  // Construct a StringLiteral as a substring of an existing StringLiteral, no error checks!
  StringLiteral(const StringLiteral& other, int start, size_t length)
      : str{other.str + start}
      , strLen{length} {}

  const char* str;
  size_t strLen;
};

}  // namespace lagom

namespace std {
template <>
struct hash<lagom::StringLiteral> {
  typedef lagom::StringLiteral argument_type;
  typedef std::size_t result_type;

  result_type operator()(argument_type const& s) const { return s.hash(); }
};
}

std::ostream& operator<<(std::ostream& os, const lagom::StringLiteral& stringLiteral);

#endif /* STRINGLITERAL_H_ */
