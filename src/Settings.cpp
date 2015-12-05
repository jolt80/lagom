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

#include <iostream>
#include <Settings.h>

Settings::Settings() {
	const char* timeStr = "\\[\\d{4}-\\d{2}-\\d{2}\\s*(\\d{1,2}:\\d{1,2}:\\d{1,2}\\.\\d{3}).*?\\]";
	const char* timeDiffStr = "\\((\\+\\d+\\.\\d+)\\)";
	const char* card = "(\\w+)";
	const char* triTraceLevel = "com_ericsson_triobjif:(.*?):";
	const char* lttngObjAndTraceLevel = "\\w+:(\\w+):";
	const char* cpuId = "{ cpu_id = (\\w+) },";
	const char* process = "\\{ processAndObjIf = \"(.*?)\\(.*?\\)\",";
	const char* ObjIf = "\\{ processAndObjIf = \".*?\\((.*?)\\)\",";
	const char* fileAndLine = "fileAndLine = \".*?(\\w+\\.\\w+):(\\d+)\",";
	const char* msg = "msg = \"\\s*(.*)\" }";
	const char* lttngMsg = "\\{ (.*) \\}";
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

	TokenMatcherSettings prefix{ lttngPrefixPattern };
	std::vector<TokenMatcherSettings> tokenPatterns;
	tokenPatterns.push_back( { fileAndLine,true,":" } );
	tokenPatterns.push_back( { process } );
	tokenPatterns.push_back( { ObjIf } );
	tokenPatterns.push_back( { msg } );

	LogLineTokenizer* lttngBase = new LogLineTokenizer{"LttngBase", prefix, tokenPatterns};
	tokenizers.push_back(lttngBase);

	tokens[0] = TokenDefinition{"Line", 5, Alignment::RIGHT, true};
	tokens[1] = TokenDefinition{"Time", 12, Alignment::LEFT, true};
	tokens[2] = TokenDefinition{"TimeDiff", 12, Alignment::LEFT, false};
	tokens[3] = TokenDefinition{"Card", 3, Alignment::LEFT, false};
	tokens[4] = TokenDefinition{"TraceLevel", 10, Alignment::LEFT, false};
	tokens[5] = TokenDefinition{"CpuId", 2, Alignment::LEFT, false};
	tokens[6] = TokenDefinition{"FileAndLine", 30, Alignment::LEFT, false};
	tokens[7] = TokenDefinition{"Process", 18, Alignment::LEFT, Alignment::RIGHT, true};
	tokens[8] = TokenDefinition{"TraceObj", 20, Alignment::LEFT, Alignment::RIGHT, true};
	tokens[9] = TokenDefinition{"Msg", 500, Alignment::LEFT, Alignment::RIGHT, true};
}

Settings::Settings(std::string filePath) : Settings{} {

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
		if(printSeparator) ss << ",";
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
		return false;
	}
	for(unsigned int i{0}; i < tokenizers.size(); ++i) {
		if(tokens[i] != other.tokens[i]) {
			std::cout << "tokens size not the same" << std::endl;
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
