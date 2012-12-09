/*
 * Copyright (C) 1997 Michael R. Elkins <me@cs.hmc.edu>
 * 
 *     This program is free software; you can redistribute it and/or modify
 *     it under the terms of the GNU General Public License as published by
 *     the Free Software Foundation; either version 2 of the License, or
 *     (at your option) any later version.
 * 
 *     This program is distributed in the hope that it will be useful,
 *     but WITHOUT ANY WARRANTY; without even the implied warranty of
 *     MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *     GNU General Public License for more details.
 * 
 *     You should have received a copy of the GNU General Public License
 *     along with this program; if not, write to the Free Software
 *     Foundation, Inc., 675 Mass Ave, Cambridge, MA 02139, USA.
 *
 */ 

#ifdef USE_SLANG
#include <slcurses.h>
#define KEY_DC SL_KEY_DELETE
#else
#ifdef HAVE_NCURSES_H
#include <ncurses.h>
#else
#include <curses.h>
#endif
#endif /* USE_SLANG */

#define mutt_flushinp flushinp
#define BEEP beep
#define M_PASS 1
#define FOREVER for(;;)
#define MENU_EDITOR 0
#define ISSPACE isspace
#define CI_is_return(x) (x == '\n' || x == '\r')
#define IsPrint isprint

enum
{
  OP_EDITOR_BACKSPACE = 1,
  OP_EDITOR_BOL,
  OP_EDITOR_EOL,
  OP_EDITOR_KILL_LINE,
  OP_EDITOR_KILL_EOL,
  OP_EDITOR_BACKWARD_CHAR,
  OP_EDITOR_FORWARD_CHAR,
  OP_EDITOR_DELETE_CHAR,
  OP_EDITOR_KILL_WORD,
};

extern int LastKey;
extern int km_dokey (int);

#include <limits.h>
