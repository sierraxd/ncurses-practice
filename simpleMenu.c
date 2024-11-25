#include <curses.h>
#include <ncurses.h>
#include <stdlib.h>

enum {
	PAIR_HOVER = 1
};

void drawMenu(WINDOW *win, char **items, int cursor) {
	const int wh = getmaxy(win), ww = getmaxx(win);
	int x = 0, y = 1;

	wmove(win, 0, 0);
	wprintw(win, "Cursor: %d", cursor);

	for (size_t i = 0; items[i] != NULL; i++, y++) {
		wmove(win, y, x);
		if (cursor == i) {
			attron(COLOR_PAIR(PAIR_HOVER));
			waddstr(win, items[i]);
			attroff(COLOR_PAIR(PAIR_HOVER));
		} else {
			waddstr(win, items[i]);
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
	cbreak();
	keypad(stdscr, TRUE);
	curs_set(0);

	if (FALSE == has_colors()) {
		endwin();
		printf("Your terminal does not support color\n");
		return -2;
	}
	initColor();
	wtimeout(stdscr, 0);

	return 0;
}

int main(int argc, char **argv)
{
	if (setup() < 0) {
		return 1;
	}

	char* MENU[] = {
		"File", "Edit", "View", "Help", "Exit",
		NULL
	};
	const int menuSize = 5;

	int cursor = 0;

	int running = 0;
	while (running) {
		drawMenu(stdscr, MENU, cursor);
		switch (getch()) {
			case 'q':
			case 27:
				running = false;
				break;
			case KEY_DOWN:
				if (cursor < menuSize - 1) {
					cursor++;
				}
				break;
			case KEY_UP:
				if (cursor > 0) {
					cursor--;
				}
				break;
		}
	}

	endwin();
	return 0;
}
