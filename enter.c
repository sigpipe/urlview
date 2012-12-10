/*
 * Copyright (C) 1996,1997 Michael R. Elkins <me@cs.hmc.edu>
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
 */ 

#include "enter.h"

#include <termios.h>
#include <sys/types.h>
#include <fcntl.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <errno.h>
#include <ctype.h>

#ifndef URLVIEW

/* global vars used for the string-history routines */
static char **Hist = NULL;
static short HistCur = 0;
static short HistLast = 0;

void mutt_init_history (void)
{
  int i;
  static int OldSize = 0;
  
  if (Hist)
  {
    for (i = 0 ; i < OldSize ; i ++)
      safe_free ((void **) &Hist[i]);
    safe_free ((void **) &Hist);
  }
  
  if (HistSize)
    Hist = safe_calloc (HistSize, sizeof (char *));
  HistCur = 0;
  HistLast = 0;
  OldSize = HistSize;
}

static void sh_add (char *s)
{
  int prev;

  if (!HistSize)
    return; /* disabled */

  if (*s)
  {
    prev = HistLast - 1;
    if (prev < 0) prev = HistSize - 1;
    if (!Hist[prev] || strcmp (Hist[prev], s) != 0)
    {
      safe_free ((void **) &Hist[HistLast]);
      Hist[HistLast++] = safe_strdup (s);
      if (HistLast > HistSize - 1)
	HistLast = 0;
    }
  }
  HistCur = HistLast; /* reset to the last entry */
}

static char *sh_next (void)
{
  int next;

  if (!HistSize)
    return (""); /* disabled */

  next = HistCur + 1;
  if (next > HistLast - 1)
    next = 0;
  if (Hist[next])
    HistCur = next;
  return (Hist[HistCur] ? Hist[HistCur] : "");
}

static char *sh_prev (void)
{
  int prev;

  if (!HistSize)
    return (""); /* disabled */

  prev = HistCur - 1;
  if (prev < 0)
  {
    prev = HistLast - 1;
    if (prev < 0)
    {
      prev = HistSize - 1;
      while (prev > 0 && Hist[prev] == NULL)
	prev--;
    }
  }
  if (Hist[prev])
    HistCur = prev;
  return (Hist[HistCur] ? Hist[HistCur] : "");
}
#endif /* ! URLVIEW */

/* redraw flags for mutt_enter_string() */
enum
{
  M_REDRAW_INIT = 1,	/* recalculate lengths */
  M_REDRAW_LINE,	/* redraw entire line */
  M_REDRAW_EOL,		/* redraw from current position to eol */
  M_REDRAW_PREV_EOL	/* redraw from curpos-1 to eol */
};

/* Returns:
 *	1 need to redraw the screen and call me again
 *	0 if input was given
 * 	-1 if abort.
 *
 */
int mutt_enter_string (unsigned char *buf, size_t buflen, int y, int x,
		       int flags)
{
  int curpos = 0;		/* the location of the cursor */
  int lastchar = 0; 		/* offset of the last char in the string */
  int begin = 0;		/* first character displayed on the line */
  int ch;			/* last typed character */
  int width = COLS - x - 1;	/* width of field */
  int redraw = M_REDRAW_INIT;	/* when/what to redraw */
  int pass = (flags == M_PASS);
  int j;

  FOREVER
  {
    if (redraw)
    {
      if (redraw == M_REDRAW_INIT)
      {
	/* full redraw */
	lastchar = curpos = strlen ((char *) buf);
	begin = lastchar - width;
      }
      if (begin < 0)
	begin = 0;
      switch (redraw)
      {
	case M_REDRAW_PREV_EOL:
	  j = curpos - 1;
	  break;
	case M_REDRAW_EOL:
	  j = curpos;
	  break;
	default:
	  j = begin;
      }
      move (y, x + j - begin);
      for (; j < lastchar && j < begin + width; j++)
	addch (buf[j]);
      clrtoeol ();
      if (redraw != M_REDRAW_INIT)
	move (y, x + curpos - begin);
      redraw = 0;
    }
    refresh ();

    /* first look to see if a keypress is an editor operation.  km_dokey()
     * returns 0 if there is no entry in the keymap, so restore the last
     * keypress and continue normally.
     */
    if ((ch = km_dokey (MENU_EDITOR)) == -1)
    {
      buf[curpos] = 0;
      return (-1);
    }

    if (ch != 0)
    {
      switch (ch)
      {
	case OP_EDITOR_BACKSPACE:
	  if (curpos == 0)
	  {
	    BEEP ();
	    break;
	  }
	  for (j = curpos ; j < lastchar ; j++)
	    buf[j - 1] = buf[j];
	  curpos--;
	  lastchar--;
	  if (!pass)
	  {
	    if (curpos > begin)
	    {
	      if (lastchar == curpos)
	      {
		move (y, x + curpos - begin);
		delch ();
	      }
	      else
		redraw = M_REDRAW_EOL;
	    }
	    else
	    {
	      begin -= width / 2;
	      redraw = M_REDRAW_LINE;
	    }
	  }
	  break;
	case OP_EDITOR_BOL:
	  /* reposition the cursor at the begininning of the line */
	  curpos = 0;
	  if (!pass)
	  {
	    if (begin)
	    {
	      /* the first char is not displayed, so readjust */
	      begin = 0;
	      redraw = M_REDRAW_LINE;
	    }
	    else
	      move (y, x);
	  }
	  break;
	case OP_EDITOR_EOL:
	  curpos = lastchar;
	  if (!pass)
	  {
	    if (lastchar < begin + width)
	      move (y, x + lastchar - begin);
	    else
	    {
	      begin = lastchar - width / 2;
	      redraw = M_REDRAW_LINE;
	    }
	  }
	  break;
	case OP_EDITOR_KILL_LINE:
	  lastchar = curpos = 0;
	  if (!pass)
	  {
	    begin = 0;
	    redraw = M_REDRAW_LINE;
	  }
	  break;
	case OP_EDITOR_KILL_EOL:
	  lastchar = curpos;
	  if (!pass)
	    clrtoeol ();
	  break;
	case OP_EDITOR_BACKWARD_CHAR:
	  if (curpos == 0)
	  {
	    BEEP ();
	  }
	  else
	  {
	    curpos--;
	    if (!pass)
	    {
	      if (curpos < begin)
	      {
		begin -= width / 2;
		redraw = M_REDRAW_LINE;
	      }
	      else
		move (y, x + curpos - begin);
	    }
	  }
	  break;
	case OP_EDITOR_FORWARD_CHAR:
	  if (curpos == lastchar)
	  {
	    BEEP ();
	  }
	  else
	  {
	    curpos++;
	    if (!pass)
	    {
	      if (curpos >= begin + width)
	      {
		begin = curpos - width / 2;
		redraw = M_REDRAW_LINE;
	      }
	      else
		move (y, x + curpos - begin);
	    }
	  }
	  break;
	case OP_EDITOR_DELETE_CHAR:
	  if (curpos != lastchar)
	  {
	    for (j = curpos; j < lastchar; j++)
	      buf[j] = buf[j + 1];
	    lastchar--;
	    if (!pass)
	      redraw = M_REDRAW_EOL;
	  }
	  else
	    BEEP ();
	  break;
	case OP_EDITOR_KILL_WORD:
	  /* delete to begining of word */
	  if (curpos != 0)
	  {
	    j = curpos;
	    while (j > 0 && ISSPACE (buf[j - 1]))
	      j--;
	    if (j > 0)
	    {
	      if (isalnum (buf[j - 1]))
	      {
		for (j--; j > 0 && isalnum (buf[j - 1]); j--)
		  ;
	      }
	      else
		j--;
	    }
	    ch = j; /* save current position */
	    while (curpos < lastchar)
	      buf[j++] = buf[curpos++];
	    lastchar = j;
	    curpos = ch; /* restore current position */
	    /* update screen */
	    if (!pass)
	    {
	      if (curpos < begin)
	      {
		begin = curpos - width / 2;
		redraw = M_REDRAW_LINE;
	      }
	      else
		redraw = M_REDRAW_EOL;
	    }
	  }
	  break;
	default:
	  BEEP ();
      }
    }
    else
    {
      /* use the raw keypress */
      ch = LastKey;

      if (CI_is_return (ch))
      {
	buf[lastchar] = 0;
	return (0);
      }
      else if (IsPrint (ch) && ((size_t)(lastchar + 1) < buflen))
      {
	for (j = lastchar; j > curpos; j--)
	  buf[j] = buf[j - 1];
	buf[curpos++] = ch;
	lastchar++;

	if (!pass)
	{
	  if (curpos >= begin + width)
	  {
	    begin = curpos - width / 2;
	    redraw = M_REDRAW_LINE;
	  }
	  else if (curpos == lastchar)
	    addch (ch);
	  else
	    redraw = M_REDRAW_PREV_EOL;
	}
      }
      else
      {
	mutt_flushinp ();
	BEEP ();
      }
    }
  }
  /* not reached */
}
