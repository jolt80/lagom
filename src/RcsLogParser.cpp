/*
 * RcsLogParser.cpp
 *
 *  Created on: Nov 1, 2015
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
#include <string>
#include <Screen.h>
#include <State.h>
#include <Log.h>
#include <Logger.h>
#include <TokenMatcher.h>
#include <StringLiteral.h>

#include <cassert>
#include <thread>

using namespace std;
using namespace std::chrono;
using namespace re2;

Logger logger(".debug_log");

void log_line_scanner(Log& log, State& state)  {
	logger.registerClient("log_scanner");
	int lastTokenizedLine{0};
	while(!log.areLineNumbersParsed() && state.running)
	{
		log.scanForLines(INT_MAX,1000);
		usleep(100);
	}
	while(lastTokenizedLine < log.getNumLines() && state.running)
	{
		lastTokenizedLine = log.tokenizeLines(lastTokenizedLine,1000);
		usleep(100);
	}
}

int main2(int argc, char* argv[]) {
	if(argc != 2) {
		cout << "Usage: rcs_log_parser <logfile>" << endl;
		exit(1);
	}

	logger.registerClient("main");

	Settings settings;

	Log log(settings);

	if(!log.map(argv[1])) {
		return 1;
	}

	State state;
	// Spawn a thread that scans the whole file for log lines
    thread log_line_scanner_t(log_line_scanner,std::ref(log),std::ref(state));

    int logline{0};
    while(logline < log.getNumLines()) {
    	cout << logline << " | ";
    	std::string** tokens = log.getLogTokens(logline);
    	if( tokens != nullptr) {
			for(int i{0}; i < 9; ++i) {
				cout << *(tokens[i]);
				if(i != 8) cout << " | ";
			}
    	}
    	logline += 10;
    	usleep(1000);
    	cout << endl;
    }

	log_line_scanner_t.join();

	return 0;
}

//void guiLoop(Screen& screen,State& state)  {
//	logger.registerClient(" gui");
//	while(state.running)
//	{
//		if(!screen.log.areLineNumbersParsed()) {
//			screen.log.scanForLines(INT_MAX,10000);
//		}
////		else if(!screen.areLinesScannedForWidths()) {
////			screen.scanForWidths(25000);
////		}
//		else {
//			usleep(10000);
//		}
//		screen.drawLog();
//	}
//}

int main(int argc, char* argv[]) {
	if(argc != 2) {
		cout << "Usage: rcs_log_parser <logfile>" << endl;
		exit(1);
	}

	logger.registerClient("main");

	Settings settings;

	Log log(settings);

	if(!log.map(argv[1])) {
		return 1;
	}

	State currentState{settings};
	Screen screen(log,currentState,settings);
	screen.drawLog();

	// Spawn a thread that scans the whole file for log lines
    thread log_line_scanner_t(log_line_scanner,std::ref(log),std::ref(currentState));

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
				currentState.search = screen.getInputLine();
				int foundLine = log.searchForLineContaining(currentState.currLine,currentState.search);
				if(foundLine != log.getNumLines()) {
					currentState.currLine = foundLine;
				}
			}
			break;
			case 'n':
			case KEY_ENTER:
			{
				int foundLine = log.searchForLineContaining(currentState.currLine+1,currentState.search);
				if(foundLine != log.getNumLines()) {
					currentState.currLine = foundLine;
				}
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
				::addstr("line> ");
				currentState.currLine = screen.getInputInteger();
				break;
			case KEY_IC:
				currentState.filtered = !currentState.filtered;
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
