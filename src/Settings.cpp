/*
 * Settings.cpp
 *
 *  Created on: Nov 17, 2015
 *      Author: jolt
 */

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
	tokens[4] = TokenDefinition{"TraceLevel", 6, Alignment::LEFT, false};
	tokens[5] = TokenDefinition{"CpuId", 2, Alignment::LEFT, false};
	tokens[6] = TokenDefinition{"FileAndLine", 20, Alignment::LEFT, false};
	tokens[7] = TokenDefinition{"Process", 15, Alignment::LEFT, Alignment::RIGHT, true};
	tokens[8] = TokenDefinition{"TraceObj", 15, Alignment::LEFT, Alignment::RIGHT, true};
	tokens[9] = TokenDefinition{"Msg", 500, Alignment::LEFT, Alignment::RIGHT, true};
}

Settings::~Settings() {
	for(auto tokenizer : tokenizers) delete tokenizer;
}

const TokenDefinition& Settings::getTokenDefinition(int tokenIndex) const {
	return tokens[tokenIndex];
}
