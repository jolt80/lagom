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

void acceleratedInc(size_t& val, int num, size_t scaling = 1);
void acceleratedDec(size_t& val, int num, size_t scaling = 1);

int main(int argc, char* argv[]) {
	Log log;
	if(!log.map(argv[1])) {
		return 1;
	}
//
//	cout << log.getLine(57672) << endl;
//
//	cout << log.toString() << endl;

	Screen screen(log);

	size_t currLine = 0;
	size_t lineOffset = 0;

	screen.drawLog(currLine);
	screen.refresh();

	int numSameInput = 0;
	int lastInput = 0;
	int input = 0;
	while((input = ::getch()) != 'q') {
		if(lastInput == input) ++numSameInput;
		else numSameInput = 0;

		switch(input) {
			case KEY_UP:
				acceleratedDec(currLine,numSameInput);
			break;
			case KEY_DOWN:
				acceleratedInc(currLine,numSameInput);
			break;
			case KEY_LEFT:
				if(lineOffset >= 10) lineOffset -= 10;
			break;
			case KEY_RIGHT:
				lineOffset += 10;
			break;
			case KEY_PPAGE:
			{
				acceleratedDec(currLine,numSameInput,screen.getRows());
			}
			break;
			case KEY_NPAGE:
				acceleratedInc(currLine,numSameInput,screen.getRows());
		}

		if(currLine + screen.getRows() > log.numLines()) currLine = log.numLines();
		screen.drawLog(currLine,lineOffset);
		screen.refresh();
		lastInput = input;
	}

	return 0;
}

void acceleratedInc(size_t& val, int num, size_t scaling) {
	size_t toInc;

	if ( num > 100 ) {
		toInc = scaling * 10;
	} else if ( num > 50 ) {
		toInc = scaling * 5;
	} else {
		toInc = scaling;
	}

	val += toInc;
}

void acceleratedDec(size_t& val, int num, size_t scaling) {
	size_t toDec;

	if ( num > 100 ) {
		toDec = scaling * 10;
	} else if ( num > 50 ) {
		toDec = scaling * 5;
	} else {
		toDec = scaling;
	}

	if(toDec > val) val = 0;
	else val -= toDec;
}


//
//#include <ncurses.h>
//#include <stdlib.h>
//
//#define MAXLEN 128
//
//typedef struct Line {
//    char contents[MAXLEN];
//    struct Line *prev, *next;
//} Line;
//
//Line *head, *curr;
//
//int nr_lines;
//int curr_line;
//
//int term_rows, term_cols;
//
//void load(const char *filename);
//void draw(Line *l);
//
//int main(int argc, char **argv)
//{
//    if (argc == 1)
//    {
//        fputs("less: No file to open\n", stderr);
//        return 1;
//    }
//
//    initscr();
//    noecho();
//    keypad(stdscr, TRUE);   // for KEY_UP, KEY_DOWN
//    getmaxyx(stdscr, term_rows, term_cols);
//    addstr("Reading text...\n");
//
//    load(argv[1]);
//
//    curr = head;
//    curr_line = 0;
//    draw(curr);
//
//    int ch;
//    while ((ch = getch()) != EOF && ch != 'q')
//    {
//        if (ch == KEY_UP && curr->prev != NULL)
//        {
//            curr_line--;
//            curr = curr->prev;
//        }
//        else if (ch == KEY_DOWN && curr->next != NULL
//            && curr_line + term_rows < nr_lines)
//        {
//            curr_line++;
//            curr = curr->next;
//        }
//        draw(curr);
//    }
//
//    endwin();
//    return 0;
//}
//
//void load(const char *filename)
//{
//    FILE *fp = fopen(filename, "r");
//    Line *lp;
//
//    head = (Line*)malloc(sizeof(Line));
//    head->prev = head->next = NULL;
//    curr = head;
//
//    while (fgets(curr->contents, MAXLEN, fp))
//    {
//        lp = (Line*)malloc(sizeof(Line));
//        lp->prev = curr;
//        curr->next = lp;
//        curr = lp;
//        nr_lines++;
//    }
//    curr->next = NULL;
//}
//
//void draw(Line *l)
//{
//    int i;
//    clear();
//    for (i = 0; i < term_rows && l != NULL; i++, l = l->next)
//        addstr(l->contents);
//}
