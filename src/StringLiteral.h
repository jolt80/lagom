/*
 * StringLiteral.h
 *
 *  Created on: Nov 2, 2015
 *      Author: qtomsza
 */

#ifndef STRINGLITERAL_H_
#define STRINGLITERAL_H_

#include <string>
#include <iostream>
#include <functional>
#include <string>
#include <cstring>
#include <re2/stringpiece.h>

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
 * used after the char buffer that's used as input in the constructor is deleted.
 *
 */
class StringLiteral {
  public:
    /**
     * Constructors.
     */
    constexpr StringLiteral() : str{nullptr}, strLen{0} {}
    constexpr StringLiteral(const char* str_, int strLen_) : str{str_}, strLen{strLen_} {}

    // Construct a StringLiteral from an std::string or a const char*
    StringLiteral(std::string string) : str{string.c_str()}, strLen{static_cast<int>(string.size())} {}
    StringLiteral(const char* str_) : str{str_}, strLen{static_cast<int>(strlen(str_))} {}
    StringLiteral(const char* startPtr, const char* endPtr) : str{startPtr}, strLen{(int)(endPtr-startPtr)} {}

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
     * incomparability: r(x, y) == false and r(y, x) == false and r(y, z) == false and r(z, y) == false implies r(x, z) == false and r(z, x) == false
     *
     *  - Ordering by string length.
     *  - For equally long strings the ordering that strcmp provides (it fulfills the strict weak ordering criteria).
     */
    bool operator<( const StringLiteral & rhs ) const;

    /**
     * Equality operators for some useful types.
     */
    bool operator==( const char * compareStr ) const;
    bool operator==( const std::string& compareStr ) const;
    bool operator==( const StringLiteral& compareStr ) const;
    bool operator!=( const char * compareStr ) const;
    bool operator!=( const std::string& compareStr ) const;
    bool operator!=( const StringLiteral& compareStr ) const;

    StringLiteral& operator=( const StringLiteral& other );
    StringLiteral& operator=( const char * other );

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
    StringLiteral subString( int start, int length) const;

    /**
     * Find first occurence of character in StringLiteral
     *
     *  Returns
     *   - offset from start of string
     *   - length of string if not found
     */
    int findFirstOf(char charToFind) const;
    int contains(std::string containsStr) const;

    /**
     * Returns true if the StringLiteral starts with a specific string.
     */
    bool startsWith(const StringLiteral& compareStr) const;

    bool empty() const;
    void clear();

    bool trimFromStart(int offset);
    bool trimFromEnd(int offset);

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
    StringLiteral(const StringLiteral& other, int start, int length) : str{other.str+start}, strLen{length} { }

    const char* str;
    int strLen;
};

namespace std
{
template<>
struct hash<StringLiteral>
{
    typedef StringLiteral argument_type;
    typedef std::size_t result_type;

    result_type operator()(argument_type const& s) const
    {
      return s.hash();
    }
};
}

std::ostream& operator<<(std::ostream& os, const StringLiteral& stringLiteral);

#endif /* STRINGLITERAL_H_ */
