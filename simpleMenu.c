#include <curses.h>
#include <ncurses.h>
#include <stdlib.h>

#define UPDATE_TIME 100

enum {
	PAIR_HOVER = 1
};

typedef void (*TCallback)();
typedef struct MenuItem TMenuItem;

struct MenuItem {
	char *caption;
	TCallback action;
};

void onExitAction() {
	endwin();
	exit(0);
}

void drawMenu(WINDOW *win, const TMenuItem* menu, int cursor) {
	const int wh = getmaxy(win), ww = getmaxx(win);
	int x = 0, y = 1;

	wmove(win, 0, 0);
	wprintw(win, "Cursor: %d", cursor);

	for (size_t i = 0; menu[i].caption != NULL; i++, y++) {
		wmove(win, y, x);
		if (cursor == i) {
			attron(COLOR_PAIR(PAIR_HOVER));
			waddstr(win, menu[i].caption);
			attroff(COLOR_PAIR(PAIR_HOVER));
		} else {
			waddstr(win, menu[i].caption);
		}
	}
	wrefresh(win);
}

void initColor() {
	start_color();
	init_pair(PAIR_HOVER, COLOR_BLACK, COLOR_MAGENTA);
}

int setup() {
	if (NULL == initscr()) {
		perror("[-] Ebat ty loh, bratan");
		return -1;
	}
	raw();
	noecho();
	nonl();
	cbreak();
	keypad(stdscr, TRUE);
	curs_set(0);

	if (FALSE == has_colors()) {
		endwin();
		printf("Your terminal does not support color\n");
		return -2;
	}
	initColor();

	wtimeout(stdscr, UPDATE_TIME);
	/* nodelay(stdscr, TRUE); */

	return 0;
}

int main(int argc, char **argv)
{
	if (setup() < 0) {
		return 1;
	}

	const TMenuItem MENU[] = {
		{ "File", NULL },
		{ "Edit", NULL },
		{ "View", NULL },
		{ "Help", NULL },
		{ "Exit", onExitAction },
		{ NULL, NULL }
	};
	const int menuSize = 5;

	int cursor = 0;

	drawMenu(stdscr, MENU, cursor);
	bool running = true;
	while (running) {
		int ch = getch();
		if (ch == ERR) {
			continue;
		}

		switch (ch) {
			case 'q':
			case 27:
				running = false;
				break;
			case 'j':
			case KEY_DOWN:
				if (cursor < menuSize - 1) {
					cursor++;
				} else {
					cursor = 0;
				}
				break;
			case 'k':
			case KEY_UP:
				if (cursor > 0) {
					cursor--;
				} else {
					cursor = menuSize - 1;
				}
				break;
			case '\r':
				if (MENU[cursor].action) {
					MENU[cursor].action();
				}
				break;
		}
		drawMenu(stdscr, MENU, cursor);
	}

	endwin();
	return 0;
}
