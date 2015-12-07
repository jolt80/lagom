/*
 * Settings.cpp
 *
 *  Created on: Nov 17, 2015
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

#include <StringLiteral.h>
#include <iostream>
#include <fstream>
#include <Settings.h>
#include <cstdlib>
#include <cassert>

#include <ParsingException.h>

using namespace std;

Settings::Settings() {
	const char* timeStr = "\\[\\d{4}-\\d{2}-\\d{2}\\s*(\\d{1,2}:\\d{1,2}:\\d{1,2}\\.\\d{3}).*?\\]";
	const char* timeDiffStr = "\\((\\+\\d+\\.\\d+)\\)";
	const char* card = "(\\w+)";
	const char* lttngObjAndTraceLevel = "\\w+:(\\w+):";
	const char* cpuId = "{ cpu_id = (\\w+) },";
	const char* process = "\\{ processAndObjIf = \"(.*?)\\(.*?\\)\",";
	const char* ObjIf = "\\{ processAndObjIf = \".*?\\((.*?)\\)\",";
	const char* fileAndLine = "fileAndLine = \".*?(\\w+\\.\\w+):(\\d+)\",";
	const char* msg = "msg = \"\\s*(.*)\" }";
	const char* separator = "\\s*";

	std::string lttngPrefixPattern;
	lttngPrefixPattern += timeStr;
	lttngPrefixPattern += separator;
	lttngPrefixPattern += timeDiffStr;
	lttngPrefixPattern += separator;
	lttngPrefixPattern += card;
	lttngPrefixPattern += separator;
	lttngPrefixPattern += lttngObjAndTraceLevel;
	lttngPrefixPattern += separator;
	lttngPrefixPattern += cpuId;

	TokenMatcherSettings prefix{ "prefix", lttngPrefixPattern };
	std::vector<TokenMatcherSettings> tokenPatterns;
	tokenPatterns.push_back( { "fileAndLine", fileAndLine,true,":" } );
	tokenPatterns.push_back( { "process", process } );
	tokenPatterns.push_back( { "traceObj", ObjIf } );
	tokenPatterns.push_back( { "msg", msg } );

	LogLineTokenizer* lttngBase = new LogLineTokenizer{"LttngBase", prefix, tokenPatterns};
	tokenizers.push_back(lttngBase);

	tokens[0] = TokenDefinition{"Line", 5, Alignment::right, true};
	tokens[1] = TokenDefinition{"Time", 12, Alignment::left, true};
	tokens[2] = TokenDefinition{"TimeDiff", 12, Alignment::left, false};
	tokens[3] = TokenDefinition{"Card", 3, Alignment::left, false};
	tokens[4] = TokenDefinition{"TraceLevel", 10, Alignment::left, false};
	tokens[5] = TokenDefinition{"CpuId", 2, Alignment::left, false};
	tokens[6] = TokenDefinition{"FileAndLine", 30, Alignment::left, false};
	tokens[7] = TokenDefinition{"Process", 18, Alignment::left, Alignment::right, true};
	tokens[8] = TokenDefinition{"TraceObj", 20, Alignment::left, Alignment::right, true};
	tokens[9] = TokenDefinition{"Msg", 500, Alignment::left, Alignment::right, true};
}

Settings::Settings(std::string filePath) {
	ifstream inFile{filePath,ios::in};
	string inLine;

	if (!inFile.is_open()) {
		cerr << "Couldn't open settings file." << endl;
		::exit(EXIT_FAILURE);
	}
	int fileLine{0};

	while ( getline (inFile,inLine) )
	{
		fileLine++;
		StringLiteral line{inLine};
		line.trimWhitespaceFromStart();
		if(line.startsWith("#") || line.empty()) continue;
		if(line.startsWith("TokenDefinitions")) {
			int index{0};
			while ( getline (inFile,inLine)) {
				fileLine++;
				StringLiteral line{inLine};
				line.trimWhitespaceFromStart();
				if(line.startsWith("#") || line.empty()) continue;
				if(line.startsWith("/TokenDefinitions")) break;
				try {
					buildTokenDefinition(index++,line);
				}
				catch(const ParsingException& e) {
					if(e.hasStringPos()) {
						int linePos = e.getStringPos() - inLine.c_str();
						throw ParsingException{filePath,fileLine,linePos,e.what()};
					}
				}
			}
		}


		if(line.startsWith("LogLineTokenizer:")) {
			line.trimFromStart(strlen("LogLineTokenizer:"));
			std::string name = line.toString();
			TokenMatcherSettings prefix;
			std::vector<TokenMatcherSettings> tokenMatchers;
			while ( getline (inFile,inLine)) {
				fileLine++;
				StringLiteral line{inLine};
				line.trimWhitespaceFromStart();
				if(line.startsWith("#") || line.empty()) continue;
				if(line.startsWith("/LogLineTokenizer")) break;

				if(!prefix.isInitialized()) {
					if(!line.startsWith("prefix")) {
						int linePos = line.getStr() - inLine.c_str();
						throw ParsingException{filePath,fileLine,linePos,"Expected \"prefix\""};
					}

					try {
						buildTokenMatcherSettings(line,prefix);
					}
					catch(const ParsingException& e) {
						if(e.hasStringPos()) {
							int linePos = e.getStringPos() - inLine.c_str();
							throw ParsingException{filePath,fileLine,linePos,e.what()};
						}
					}
				}
				else {
					TokenMatcherSettings matcher;
					try {
						buildTokenMatcherSettings(line,matcher);
					}
					catch(const ParsingException& e) {
						if(e.hasStringPos()) {
							int linePos = e.getStringPos() - inLine.c_str();
							throw ParsingException{filePath,fileLine,linePos,e.what()};
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
	if(index > 9) return false;

	int keyNr;
	std::string name;
	int width;
	bool visibility;
	Alignment alignment;
	Alignment crop;

	// find key nr
	int commaPos = line.findFirstOf(':');
	if(commaPos == line.getLength()) throw ParsingException{"Expected \":\"",line.getStr()};
	keyNr = line.subString(0,commaPos).toInt();
	line.trimFromStart(commaPos+1);
	line.trimWhitespaceFromStart();
	assert(keyNr == (index + 1) % 10);

	// find name
	commaPos = line.findFirstOf(',');
	if(commaPos == line.getLength()) throw ParsingException{"Expected \",\"",line.getStr()};
	name = line.subString(0,commaPos).toString();
	line.trimFromStart(commaPos+1);
	line.trimWhitespaceFromStart();

	// find width
	commaPos = line.findFirstOf(',');
	if(commaPos == line.getLength()) throw ParsingException{"Expected \",\"",line.getStr()};
	width = line.subString(0,commaPos).toInt();
	line.trimFromStart(commaPos+1);
	line.trimWhitespaceFromStart();

	// find visibility
	commaPos = line.findFirstOf(',');
	if(commaPos == line.getLength()) throw ParsingException{"Expected \",\"",line.getStr()};
	visibility = static_cast<bool>(line.subString(0,commaPos).toInt());
	line.trimFromStart(commaPos+1);
	line.trimWhitespaceFromStart();

	// find alignment
	commaPos = line.findFirstOf(',');
	if(commaPos == line.getLength()) throw ParsingException{"Expected \",\"",line.getStr()};
	StringLiteral subStr = line.subString(0,commaPos);
	if(subStr == "left") {
		alignment = Alignment::left;
	}
	else if(subStr == "right") {
		alignment = Alignment::right;
	}
	else {
		throw ParsingException{"Expected \"left\" or \"right\"",line.getStr()};
		return false;
	}
	line.trimFromStart(commaPos+1);
	line.trimWhitespaceFromStart();

	// find crop
	subStr = line.subString(0,commaPos);
	if(subStr == "left") {
		crop = Alignment::left;
	}
	else if(subStr == "right") {
		crop = Alignment::right;
	}
	else {
		throw ParsingException{"Expected \"left\" or \"right\"",line.getStr()};
		return false;
	}

	tokens[index] = TokenDefinition{name,width,alignment,crop,visibility};
	return true;
}

bool Settings::buildTokenMatcherSettings(StringLiteral line, TokenMatcherSettings& matcherSettings) {
	std::string name{};
	//int numberOfMatches{};
	std::string pattern{};
	bool combine{false};
	std::string separator{};

	// find name
	int separatorPos = line.findFirstOf('<');
	if(separatorPos == line.getLength()) {
		throw ParsingException{"Expected \"<\"",line.getStr()};
		return false;
	}
	name = line.subString(0,separatorPos).toString();
	line.trimFromStart(separatorPos+1);
	line.trimWhitespaceFromStart();

	// tokenMatcher with multiple captures mapped to single token
	if(line.findFirstOf('>') > line.findFirstOf(':')) {
		combine = true;
		// find number of matches
		separatorPos = line.findFirstOf(',');
		if(separatorPos == line.getLength()) {
			throw ParsingException{"Expected \",\"",line.getStr()};
			return false;
		}
		//numberOfMatches = line.subString(0,separatorPos).toInt();
		line.trimFromStart(separatorPos+1);
		separatorPos = line.findFirstOf('>');
		if(separatorPos == line.getLength()) {
			throw ParsingException{"Expected \">\"",line.getStr()};
			return false;
		}
		separator = line.subString(0,separatorPos).toString();
		line.trimFromStart(separatorPos+1);
		line.trimWhitespaceFromStart();
	}
	else {
		// find number of matches
		separatorPos = line.findFirstOf('>');
		if(separatorPos == line.getLength()) {
			throw ParsingException{"Expected \">\"",line.getStr()};
			return false;
		}
		//numberOfMatches = line.subString(0,separatorPos).toInt();
		line.trimFromStart(separatorPos+1);
		line.trimWhitespaceFromStart();
	}

	// find number of matches
	line.trimWhitespaceFromStart();
	separatorPos = line.findFirstOf('=');
	if(separatorPos > 0) {
		throw ParsingException{"Expected \"=\"",line.getStr()};
		return false;
	}
	line.trimFromStart(separatorPos+1);
	pattern = line.toString();

	matcherSettings.name = name;
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
	for(auto tokenizer : tokenizers) {
		if(printSeparator) ss << ",";
		ss << index << ":" << *tokenizer;
		printSeparator = true;
	}
	ss << "),tokens(";
	index = 0;
	printSeparator = false;
	for(auto token : tokens) {
		if(printSeparator) ss << "\n";
		ss << index << ":" << token;
		printSeparator = true;
		++index;
	}
	ss << ")}";
	return ss.str();
}


bool Settings::operator==(const Settings& other) const {
	if(tokenizers.size() != other.tokenizers.size()) {
		std::cout << "tokenizers size not the same" << std::endl;
		return false;
	}
	for(unsigned int i{0}; i < tokenizers.size(); ++i) {
		if(*tokenizers[i] != *(other.tokenizers[i])) {
			std::cout << "tokenizers index " << i << " not the same" << std::endl;
			return false;
		}
	}
	if(tokens.size() != other.tokens.size()) {
		std::cout << "tokens size not the same" << std::endl;
		return false;
	}
	for(unsigned int i{0}; i < tokenizers.size(); ++i) {
		if(tokens[i] != other.tokens[i]) {
			std::cout << "tokens " << i << " not the same" << std::endl;
			return false;
		}
	}
	return true;
}

Settings::~Settings() {
	for(auto tokenizer : tokenizers) delete tokenizer;
}

const TokenDefinition& Settings::getTokenDefinition(int tokenIndex) const {
	return tokens[tokenIndex];
}

std::ostream& operator<<(std::ostream& stream, const Settings& settings)
{
	return stream << settings.toString();
}
