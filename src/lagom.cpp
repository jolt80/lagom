/*
 * lagom.cpp
 *
 *  Created on: Nov 1, 2015
 *      Author: Tomas Szabo
 *
 *  lagom is a log viewer with regex filtering/search capabilities, configurable
 *  tokenization.
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
#include <string>
#include <Screen.h>
#include <State.h>
#include <Log.h>
#include <Logger.h>
#include <TokenMatcher.h>
#include <StringLiteral.h>
#include <UnfilteredLogView.h>
#include <LogViewRepository.h>

#include <AutoMeasureDuration.h>
#include <cassert>
#include <thread>

#include <History.h>
#include <FileOperationException.h>

extern "C" {
	#include <sys/stat.h>
}

using namespace std;
using namespace std::chrono;
using namespace re2;

extern Logger logger;

void log_line_scanner(Log& log, State& state)  {
	logger.registerClient("log_scanner");
	int lastTokenizedLine{0};
	{
		AutoMeasureDuration meas(cout,"parse log numbers ");
		while(!log.areLineNumbersParsed() && state.running)
		{
			log.scanForLines(INT_MAX,1000);
			usleep(1);
		}
	}
	{
		AutoMeasureDuration meas(cout,"tokenization of log numbers ");
		while(lastTokenizedLine < log.getNumLines() && state.running)
		{
			lastTokenizedLine = log.tokenizeLines(lastTokenizedLine,1000);
			usleep(1);
		}
	}
}

void createDirIfDoesntExist(std::string path) {
	struct stat sb;
	if (stat(path.c_str(), &sb) == 0 && S_ISDIR(sb.st_mode)) {
		// directory exists
		return;
	}
	else {
		mkdir(path.c_str(),S_IRWXU | S_IRWXG);
	}
}

int main(int argc, char* argv[]) {
	if(argc != 2) {
		cout << "Usage: lagom <logfile>" << endl;
		exit(1);
	}

//	createDirIfDoesntExist("~/.lagom");
//
	string logPath = argv[1];
//	string settingsPath = "~/.lagom/settings";
//
	size_t tgfWebStringPos = logPath.find("tgfweb.lmera.ericsson.se/");
	if(tgfWebStringPos != string::npos) {
		logPath = "/proj/tgf_li/" + logPath.substr(tgfWebStringPos + 25);
	}

	logger.registerClient("main");

	Settings* settings;
//	try {
//		settings = new Settings{settingsPath};
//	}
//	catch(FileOperationException e) {
//		if(e.fault == FileOperationExceptionFailureCode::OPEN) {
//			Settings::writeDefaultSettingsFile(settingsPath);
//			settings = new Settings{settingsPath};
//		}
//	}

	settings = new Settings;
	History searchHistory;
	History filterHistory;

	Log log(*settings);
	LogViewRepository logViews(log);

	if(!log.map(logPath.c_str())) {
		return 1;
	}

	State currentState{*settings};

	// Spawn a thread that scans the whole file for log lines
    thread log_line_scanner_t(log_line_scanner,std::ref(log),std::ref(currentState));

	UnfilteredLogView unfilteredLogView{&log};
    LogView* currentLogView = &unfilteredLogView;
	Screen screen(currentLogView,currentState,*settings);
	screen.drawLog();

	int numSameInput = 0;
	int lastInput = 0;
	int input = 0;
	while((input = ::getch()) != 'q') {
		if(lastInput == input) ++numSameInput;
		else numSameInput = 0;

		switch(input) {
			case KEY_UP:
			{
				if(currentState.currLine > 0)
					currentState.currLine--;
				else
					currentState.currLine = 0;
			}
			break;
			case KEY_DOWN:
				currentState.currLine++;
			break;
//			case KEY_LEFT:
//				if(currentState.lineOffset >= 10) currentState.lineOffset -= 10;
//			break;
//			case KEY_RIGHT:
//				currentState.lineOffset += 10;
//			break;
			case KEY_PPAGE:
			{
				if(currentState.currLine >= screen.getRows())
					currentState.currLine -= screen.getRows();
				else
					currentState.currLine = 0;
			}
			break;
			case KEY_NPAGE:
				currentState.currLine += screen.getRows();
			break;
			case 's':
			case '/':
			{
				::addstr("search> ");
				currentState.search = screen.getInputLine(&searchHistory);
				currentState.currLine = currentLogView->searchForLineContaining(currentState.currLine,currentState.search);
			}
			break;
			case '?':
			{
				currentState.searchBackwards = !currentState.searchBackwards;
			}
			break;
			case 'f':
			{
				::addstr("filter> ");
				//screen.saveCursorPosition();
				std::string filterExp = screen.getInputLine(&filterHistory);
				LogView* filteredView;
				if(filterExp == "") {
					filteredView = &unfilteredLogView;
				}
				else {
					filteredView = logViews.getFilteredLogView(filterExp);
					if(nullptr == filteredView) {
						std::string errormsg{"     "};
						errormsg += logViews.getLastErrorMessage();
						screen.print(errormsg.c_str());
						screen.refresh();
						sleep(3);
						filteredView = currentLogView;
					}
				}
				int newCurrLine = filteredView->findCurrentLine(currentLogView->getLineNumber(currentState.currLine));
				currentLogView = filteredView;
				screen.setLogView(filteredView);
				currentState.currLine = newCurrLine;
				currentState.forceUpdate = true;

			}
			break;
			case 'n':
			{
				currentState.currLine = currentLogView->searchForLineContaining(currentState.currLine,currentState.search,currentState.searchBackwards);
			}
			break;
			case 'N':
			{
				currentState.currLine = currentLogView->searchForLineContaining(currentState.currLine,currentState.search,!currentState.searchBackwards);
			}
			break;
			case KEY_END:
			{
				currentState.currLine = INT_MAX;
			}
			break;
			case KEY_HOME:
			{
				currentState.currLine = 0;
			}
			break;
			case 'l':
			{
				::addstr("line> ");
				int line = screen.getInputInteger();
				if(line != 0) line--;
				currentState.currLine = line;
			}
			break;
			case KEY_IC:
				currentState.tokenized = !currentState.tokenized;
				break;
			case '1':
				currentState.tokenVisible[0] = !currentState.tokenVisible[0];
				break;
			case '2':
				currentState.tokenVisible[1] = !currentState.tokenVisible[1];
				break;
			case '3':
				currentState.tokenVisible[2] = !currentState.tokenVisible[2];
				break;
			case '4':
				currentState.tokenVisible[3] = !currentState.tokenVisible[3];
				break;
			case '5':
				currentState.tokenVisible[4] = !currentState.tokenVisible[4];
				break;
			case '6':
				currentState.tokenVisible[5] = !currentState.tokenVisible[5];
				break;
			case '7':
				currentState.tokenVisible[6] = !currentState.tokenVisible[6];
				break;
			case '8':
				currentState.tokenVisible[7] = !currentState.tokenVisible[7];
				break;
			case '9':
				currentState.tokenVisible[8] = !currentState.tokenVisible[8];
				break;
			case '0':
				currentState.tokenVisible[9] = !currentState.tokenVisible[9];
				break;
			case KEY_RESIZE:
			{
				logger.log("Triggering resize");
				screen.updateSize();
				currentState.forceUpdate = true;
			}
		}
		lastInput = input;

		screen.drawLog();
	}

	currentState.running = false;

	log_line_scanner_t.join();

	return 0;
}
