//============================================================================
// Name        : RcsLogParser.cpp
// Author      : Tomas Szabo
// Version     :
// Copyright   : Free stuff
// Description : Hello World in C++, Ansi-style
//============================================================================

#include <iostream>
#include <string>
#include <Screen.h>
#include <Log.h>

using namespace std;

int main(int argc, char* argv[]) {
	Log log;
	if(!log.map(argv[1])) {
		return 1;
	}

	for(size_t line = 57650; line < 57680; ++line) {
		cout << log.getLine(line);
	}

	return 0;
}

void print_stuff() {
	Screen screen;

	screen.println("Hello world");

	string input = screen.getInput();

	screen.println("this was the input: " + input);

	input = screen.getInput();
}
