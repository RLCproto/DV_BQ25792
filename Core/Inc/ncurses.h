/*
 * ncurses.h
 *
 *  Created on: 16 maj 2023
 *      Author: Maciej
 */

#ifndef NCURSES_H_
#define NCURSES_H_

#define NCURSES_RESET		"\033[m"

#define NCURSES_BOLD		"\033[1m"
#define NCURSES_UNDERLINE	"\033[4m"
#define NCURSES_BLINK		"\033[5m"
#define NCURSES_INVERSE		"\033[7m"

#define NCURSES_FG_BLACK	"\033[0;30m"
#define NCURSES_FG_RED		"\033[0;31m"
#define NCURSES_FG_GREEN	"\033[0;32m"
#define NCURSES_FG_YELLOW	"\033[0;33m"
#define NCURSES_FG_BLUE		"\033[0;34m"
#define NCURSES_FG_MAGENTA	"\033[0;35m"
#define NCURSES_FG_CYAN		"\033[0;36m"
#define NCURSES_FG_WHIT		"\033[0;37m"
#define NCURSES_FG_DEFAULT	"\033[0;39m"

#define NCURSES_BG_BLACK	"\033[0;40m"
#define NCURSES_BG_RED		"\033[0;41m"
#define NCURSES_BG_GREEN	"\033[0;42m"
#define NCURSES_BG_YELLOW	"\033[0;43m"
#define NCURSES_BG_BLUE		"\033[0;44m"
#define NCURSES_BG_MAGENTA	"\033[0;45m"
#define NCURSES_BG_CYAN		"\033[0;46m"
#define NCURSES_BG_WHITE	"\033[0;47m"
#define NCURSES_BG_DEFAULT	"\033[0;49m"

#define NCURSES_GOTOXY(_x,_y)	"\x1B[" #_y ";" #_x "H"

#endif /* NCURSES_H_ */
