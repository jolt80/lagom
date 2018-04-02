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

/*
 *  lagom is a log viewer with regex filtering/search capabilities, configurable
 *  tokenization.
 */

#include <iostream>
#include <string>
#include "log.h"
#include "logger.h"
#include "screen.h"
#include "state.h"
#include "string_literal.h"
#include "token_matcher.h"
#include "unfiltered_log_view.h"

#include <cassert>
#include <thread>
#include "auto_measure_duration.h"

#include "file_operation_exception.h"
#include "filtered_log_view_factory.h"
#include "history.h"

#include <glog/logging.h>

extern "C" {
#include <sys/stat.h>
}

using namespace std;
using namespace std::chrono;
using namespace re2;

using namespace lagom;

namespace lagom {
extern Logger logger;
}

void log_line_scanner(Log& log, State& state) {
  logger.registerClient("log_scanner");
  int lastTokenizedLine{0};
  {
    AutoMeasureDuration meas(cout, "parse log numbers ");
    while (!log.areLineNumbersParsed() && state.running) {
      log.scanForLines(INT_MAX, 1000);
      usleep(1);
    }
  }
  {
    AutoMeasureDuration meas(cout, "tokenization of log numbers ");
    while (lastTokenizedLine < log.getNumLines() && state.running) {
      lastTokenizedLine = log.tokenizeLines(lastTokenizedLine, 1000);
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
    if (mkdir(path.c_str(), S_IRWXU | S_IRWXG | S_IROTH | S_IXOTH) != 0) {
      cerr << "mkdir() failed with errno " << errno << endl;
      exit(1);
    }
  }
}

int main(int argc, char* argv[]) {
  if (argc != 2) {
    cout << "Usage: lagom <logfile>" << endl;
    exit(1);
  }

  google::InitGoogleLogging(argv[0]);

  LOG(ERROR) << "This should work";

  string logPath = argv[1];

  size_t tgfWebStringPos = logPath.find("tgfweb.lmera.ericsson.se/");
  if (tgfWebStringPos != string::npos) {
    logPath = "/proj/tgf_li/" + logPath.substr(tgfWebStringPos + 25);
  }

  string homeDirPath = getenv("HOME");
  createDirIfDoesntExist(homeDirPath + "/.lagom");

  string settingsPath = homeDirPath + "/.lagom/settings";
  Settings* settings = nullptr;
  //	try {
  //		settings = new Settings{settingsPath};
  //	}
  //	catch(FileOperationException e) {
  //		if(e.fault == FileOperationExceptionFailureCode::OPEN) {
  //			//Settings::writeDefaultSettingsFile(settingsPath);
  //			settings = new Settings{settingsPath};
  //		}
  //	}
  settings = new Settings{};

  Log log(*settings);
  FilteredLogViewFactory logViews(log);

  if (!log.map(logPath.c_str())) {
    return 1;
  }

  string searchHistoryPath = homeDirPath + "/.lagom/search_history";
  string filterHistoryPath = homeDirPath + "/.lagom/filter_history";
  History searchHistory{searchHistoryPath};
  History filterHistory{filterHistoryPath};
  State currentState{*settings};

  // Spawn a thread that scans the whole file for log lines
  thread log_line_scanner_t(log_line_scanner, std::ref(log), std::ref(currentState));

  auto unfilteredLogView = std::make_shared<UnfilteredLogView>(log);
  std::shared_ptr<LogView> currentLogView = unfilteredLogView;
  Screen screen(currentLogView, currentState, *settings);
  screen.drawLog();

  int numSameInput = 0;
  int lastInput = 0;
  int input = 0;
  while ((input = ::getch()) != 'q') {
    if (lastInput == input)
      ++numSameInput;
    else
      numSameInput = 0;

    switch (input) {
      case KEY_UP: {
        if (currentState.currLine > 0)
          currentState.currLine--;
        else
          currentState.currLine = 0;
      } break;
      case KEY_DOWN:
        currentState.currLine++;
        break;
      //			case KEY_LEFT:
      //				if(currentState.lineOffset >= 10) currentState.lineOffset -= 10;
      //			break;
      //			case KEY_RIGHT:
      //				currentState.lineOffset += 10;
      //			break;
      case KEY_PPAGE: {
        if (currentState.currLine >= screen.getRows())
          currentState.currLine -= screen.getRows();
        else
          currentState.currLine = 0;
      } break;
      case KEY_NPAGE:
        currentState.currLine += screen.getRows();
        break;
      case 's':
      case '/': {
        ::addstr("search> ");
        currentState.search = screen.getInputLine(&searchHistory);
        currentState.currLine = currentLogView->searchForLineContaining(currentState.currLine, currentState.search);
      } break;
      case '?': {
        currentState.searchBackwards = !currentState.searchBackwards;
      } break;
      case 'f': {
        ::addstr("filter> ");
        // screen.saveCursorPosition();
        std::string filterExp = screen.getInputLine(&filterHistory);
        std::shared_ptr<LogView> filteredView;
        if (filterExp == "") {
          filteredView = unfilteredLogView;
        }
        else {
          filteredView = logViews.getFilteredLogView(filterExp);
          if (!filteredView) {
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
        screen.switchLogView(filteredView);
        currentState.currLine = newCurrLine;
        currentState.forceUpdate = true;

      } break;
      case 'n': {
        currentState.currLine = currentLogView->searchForLineContaining(
            currentState.currLine, currentState.search, currentState.searchBackwards);
      } break;
      case 'N': {
        currentState.currLine = currentLogView->searchForLineContaining(
            currentState.currLine, currentState.search, !currentState.searchBackwards);
      } break;
      case KEY_END: {
        currentState.currLine = INT_MAX;
      } break;
      case KEY_HOME: {
        currentState.currLine = 0;
      } break;
      case 'l': {
        ::addstr("line> ");
        int line = screen.getInputInteger();
        if (line != 0) line--;
        currentState.currLine = line;
      } break;
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
      case KEY_RESIZE: {
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
