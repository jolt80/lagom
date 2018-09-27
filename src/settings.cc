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

#include <cassert>
#include <cstdlib>
#include <fstream>
#include <iostream>

#include "file_operation_exception.h"
#include "parsing_exception.h"
#include "settings.h"
#include "string_literal.h"

using namespace std;

namespace lagom {

Settings::Settings() {
  const char* timeStr = "\\[.*?(\\d{1,2}:\\d{1,2}:\\d{1,2}\\.\\d{3}).*?\\]";
  const char* timeDiffStr = "\\((\\+[\\?\\d]+\\.[\\?\\d]+)\\)";
  const char* host = "(\\S+)";
  const char* providerAndTp = "(\\w+:\\w+):";
  const char* procInfo = ".*\\{ (proc\\w* = \".*?\",\\s*vpid\\s*=\\s*\\d+,\\s*vtid\\s*=\\s*\\d+)\\s*},";
  const char* fileAndLine = "\\{ file_line = \".*?(\\w+\\.\\w+):(\\d+)\",";
  const char* msg = "\\{ file_line = \".*?\\w+\\.\\w+:\\d+\",\\s*(.*)";
  const char* separator = "\\s*";

  std::string lttngPrefixPattern;
  lttngPrefixPattern += timeStr;
  lttngPrefixPattern += separator;
  lttngPrefixPattern += timeDiffStr;
  lttngPrefixPattern += separator;
  lttngPrefixPattern += host;
  lttngPrefixPattern += separator;
  lttngPrefixPattern += providerAndTp;
  lttngPrefixPattern += separator;
  lttngPrefixPattern += procInfo;

  TokenMatcherSettings prefix{"prefix", 5, lttngPrefixPattern};
  std::vector<TokenMatcherSettings> tokenPatterns;
  tokenPatterns.push_back({"fileAndLine", 1, fileAndLine, true, ":"});
  tokenPatterns.push_back({"msg", msg});

  LogLineTokenizer* lttngBase = new LogLineTokenizer{"LttngBase", prefix, tokenPatterns};
  tokenizers.push_back(lttngBase);

  tokens[0] = TokenDefinition{"Line", 5, Alignment::right, true};
  tokens[1] = TokenDefinition{"Time", 12, Alignment::left, true};
  tokens[2] = TokenDefinition{"TimeDiff", 12, Alignment::left, false};
  tokens[3] = TokenDefinition{"Host", 15, Alignment::left, false};
  tokens[4] = TokenDefinition{"TraceProviderAndTp", 35, Alignment::left, false};
  tokens[5] = TokenDefinition{"ProcInfo", 60, Alignment::left, Alignment::right, false};
  tokens[6] = TokenDefinition{"FileAndLine", 30, Alignment::left, true};
  tokens[7] = TokenDefinition{"Msg", 500, Alignment::left, Alignment::right, true};
}

Settings::Settings(std::string filePath) {
  ifstream inFile{filePath, ios::in};
  string inLine;

  if (!inFile.is_open())
    throw FileOperationException{FileOperationExceptionFailureCode::OPEN, "couldn't open settings file " + filePath};
  int fileLine{0};

  while (getline(inFile, inLine)) {
    fileLine++;
    StringLiteral line{inLine};
    line.trimWhitespaceFromStart();
    if (line.startsWith("#") || line.empty()) continue;
    if (line.startsWith("TokenDefinitions")) {
      int index{0};
      while (getline(inFile, inLine)) {
        fileLine++;
        StringLiteral line{inLine};
        line.trimWhitespaceFromStart();
        if (line.startsWith("#") || line.empty()) continue;
        if (line.startsWith("/TokenDefinitions")) break;
        try {
          buildTokenDefinition(index++, line);
        }
        catch (const ParsingException& e) {
          if (e.hasStringPos()) {
            int linePos = e.getStringPos() - inLine.c_str();
            throw ParsingException{filePath, fileLine, linePos, e.what()};
          }
        }
      }
    }

    if (line.startsWith("LogLineTokenizer:")) {
      line.trimFromStart(strlen("LogLineTokenizer:"));
      std::string name = line.toString();
      TokenMatcherSettings prefix;
      int numMatches{0};
      std::vector<TokenMatcherSettings> tokenMatchers;
      while (getline(inFile, inLine)) {
        fileLine++;
        StringLiteral line{inLine};
        line.trimWhitespaceFromStart();
        if (line.startsWith("#") || line.empty()) continue;
        if (line.startsWith("/LogLineTokenizer"))
          break;
        else if (numMatches >= 9)
          throw ParsingException{
              filePath, fileLine, 0, "number of tokens is >=9 for the tokenizer, expected \"/LogLineTokenizer\""};

        if (!prefix.isInitialized()) {
          if (!line.startsWith("prefix")) {
            int linePos = line.getStr() - inLine.c_str();
            throw ParsingException{filePath, fileLine, linePos, "Expected \"prefix\""};
          }

          try {
            buildTokenMatcherSettings(line, prefix);
            numMatches += prefix.numberOfTokens;
          }
          catch (const ParsingException& e) {
            if (e.hasStringPos()) {
              int linePos = e.getStringPos() - inLine.c_str();
              throw ParsingException{filePath, fileLine, linePos, e.what()};
            }
          }
        }
        else {
          TokenMatcherSettings matcher;
          try {
            buildTokenMatcherSettings(line, matcher);
            numMatches += matcher.numberOfTokens;
          }
          catch (const ParsingException& e) {
            if (e.hasStringPos()) {
              int linePos = e.getStringPos() - inLine.c_str();
              throw ParsingException{filePath, fileLine, linePos, e.what()};
            }
          }
          tokenMatchers.push_back(matcher);
        }
      }
      LogLineTokenizer* tokenizer = new LogLineTokenizer{name, prefix, tokenMatchers};
      tokenizers.push_back(tokenizer);
    }
  }

  inFile.close();
}

bool Settings::buildTokenDefinition(int index, StringLiteral line) {
  if (index > 9) return false;

  int keyNr;
  std::string name;
  int width;
  bool visibility;
  Alignment alignment;
  Alignment crop;

  // find key nr
  int commaPos = line.findFirstOf(':');
  if (commaPos == line.getLength()) throw ParsingException{"Expected \":\"", line.getStr()};
  keyNr = line.subString(0, commaPos).toInt();
  line.trimFromStart(commaPos + 1);
  line.trimWhitespaceFromStart();
  assert(keyNr == (index + 1) % 10);

  // find name
  commaPos = line.findFirstOf(',');
  if (commaPos == line.getLength()) throw ParsingException{"Expected \",\"", line.getStr()};
  name = line.subString(0, commaPos).toString();
  line.trimFromStart(commaPos + 1);
  line.trimWhitespaceFromStart();

  // find width
  commaPos = line.findFirstOf(',');
  if (commaPos == line.getLength()) throw ParsingException{"Expected \",\"", line.getStr()};
  width = line.subString(0, commaPos).toInt();
  line.trimFromStart(commaPos + 1);
  line.trimWhitespaceFromStart();

  // find visibility
  commaPos = line.findFirstOf(',');
  if (commaPos == line.getLength()) throw ParsingException{"Expected \",\"", line.getStr()};
  visibility = static_cast<bool>(line.subString(0, commaPos).toInt());
  line.trimFromStart(commaPos + 1);
  line.trimWhitespaceFromStart();

  // find alignment
  commaPos = line.findFirstOf(',');
  if (commaPos == line.getLength()) throw ParsingException{"Expected \",\"", line.getStr()};
  StringLiteral subStr = line.subString(0, commaPos);
  if (subStr == "left") {
    alignment = Alignment::left;
  }
  else if (subStr == "right") {
    alignment = Alignment::right;
  }
  else {
    throw ParsingException{"Expected \"left\" or \"right\"", line.getStr()};
    return false;
  }
  line.trimFromStart(commaPos + 1);
  line.trimWhitespaceFromStart();

  // find crop
  subStr = line.subString(0, commaPos);
  if (subStr == "left") {
    crop = Alignment::left;
  }
  else if (subStr == "right") {
    crop = Alignment::right;
  }
  else {
    throw ParsingException{"Expected \"left\" or \"right\"", line.getStr()};
    return false;
  }

  tokens[index] = TokenDefinition{name, width, alignment, crop, visibility};
  return true;
}

bool Settings::buildTokenMatcherSettings(StringLiteral line, TokenMatcherSettings& matcherSettings) {
  std::string name{};
  int numberOfMatches{};
  std::string pattern{};
  bool combine{false};
  std::string separator{};

  // find name
  int separatorPos = line.findFirstOf('<');
  if (separatorPos == line.getLength()) {
    throw ParsingException{"Expected \"<\"", line.getStr()};
    return false;
  }
  name = line.subString(0, separatorPos).toString();
  line.trimFromStart(separatorPos + 1);
  line.trimWhitespaceFromStart();

  // tokenMatcher with multiple captures mapped to single token
  if (line.findFirstOf('>') > line.findFirstOf(':')) {
    combine = true;
    // find number of matches
    separatorPos = line.findFirstOf(',');
    if (separatorPos == line.getLength()) {
      throw ParsingException{"Expected \",\"", line.getStr()};
      return false;
    }
    numberOfMatches = 1;
    line.trimFromStart(separatorPos + 1);
    separatorPos = line.findFirstOf('>');
    if (separatorPos == line.getLength()) {
      throw ParsingException{"Expected \">\"", line.getStr()};
      return false;
    }
    separator = line.subString(0, separatorPos).toString();
    line.trimFromStart(separatorPos + 1);
    line.trimWhitespaceFromStart();
  }
  else {
    // find number of matches
    separatorPos = line.findFirstOf('>');
    if (separatorPos == line.getLength()) {
      throw ParsingException{"Expected \">\"", line.getStr()};
      return false;
    }
    numberOfMatches = line.subString(0, separatorPos).toInt();
    line.trimFromStart(separatorPos + 1);
    line.trimWhitespaceFromStart();
  }

  // find number of matches
  line.trimWhitespaceFromStart();
  separatorPos = line.findFirstOf('=');
  if (separatorPos > 0) {
    throw ParsingException{"Expected \"=\"", line.getStr()};
    return false;
  }
  line.trimFromStart(separatorPos + 1);
  pattern = line.toString();

  matcherSettings.name = name;
  matcherSettings.numberOfTokens = numberOfMatches;
  matcherSettings.pattern = pattern;
  matcherSettings.combine = combine;
  matcherSettings.separator = separator;

  return true;
}

std::string Settings::toString() const {
  std::stringstream ss;

  ss << "Settings{tokenizers(";
  int index{0};
  bool printSeparator = false;
  for (auto tokenizer : tokenizers) {
    if (printSeparator) ss << ",";
    ss << index << ":" << *tokenizer;
    printSeparator = true;
  }
  ss << "),tokens(";
  index = 0;
  printSeparator = false;
  for (auto token : tokens) {
    if (printSeparator) ss << "\n";
    ss << index << ":" << token;
    printSeparator = true;
    ++index;
  }
  ss << ")}";
  return ss.str();
}

bool Settings::operator==(const Settings& other) const {
  if (tokenizers.size() != other.tokenizers.size()) {
    return false;
  }
  for (unsigned int i{0}; i < tokenizers.size(); ++i) {
    if (*tokenizers[i] != *(other.tokenizers[i])) {
      return false;
    }
  }
  if (tokens.size() != other.tokens.size()) {
    return false;
  }
  for (unsigned int i{0}; i < tokenizers.size(); ++i) {
    if (tokens[i] != other.tokens[i]) {
      return false;
    }
  }
  return true;
}

Settings::~Settings() {
  for (auto tokenizer : tokenizers) delete tokenizer;
}

const TokenDefinition& Settings::getTokenDefinition(int tokenIndex) const { return tokens[tokenIndex]; }

std::ostream& operator<<(std::ostream& stream, const Settings& settings) { return stream << settings.toString(); }

void Settings::writeDefaultSettingsFile(std::string filePath) {
  ofstream outFile{filePath, ios::out};

  if (!outFile.is_open())
    throw FileOperationException{FileOperationExceptionFailureCode::OPEN, "couldn't open settings file " + filePath};

  outFile << "hello world" << endl << flush;
}

}  // namespace lagom
