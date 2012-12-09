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

#include "enter.h"

int LastKey;

int km_dokey (int n)
{
  switch (LastKey = getch ())
  {
    case KEY_LEFT:
    case '\002':
      return OP_EDITOR_BACKWARD_CHAR;
    case KEY_RIGHT:
    case '\006':
      return OP_EDITOR_FORWARD_CHAR;
    case '\010':
    case KEY_BACKSPACE:
      return OP_EDITOR_BACKSPACE;
    case '\004':
    case KEY_DC:
    case 127:
      return OP_EDITOR_DELETE_CHAR;
    case '\001':
      return OP_EDITOR_BOL;
    case '\005':
      return OP_EDITOR_EOL;
    case '\025':
      return OP_EDITOR_KILL_LINE;
    case '\013':
      return OP_EDITOR_KILL_EOL;
    case '\027':
      return OP_EDITOR_KILL_WORD;
    case '\007':
    case -1:
      return (-1);
  }
  return (0);
}
