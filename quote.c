/*
 * Copyright (C) 2000 Thomas Roessler <roessler@does-not-exist.org>
 * 
 *     This program is free software; you can redistribute it
 *     and/or modify it under the terms of the GNU General Public
 *     License as published by the Free Software Foundation; either
 *     version 2 of the License, or (at your option) any later
 *     version.
 * 
 *     This program is distributed in the hope that it will be
 *     useful, but WITHOUT ANY WARRANTY; without even the implied
 *     warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR
 *     PURPOSE.  See the GNU General Public License for more
 *     details.
 * 
 *     You should have received a copy of the GNU General Public
 *     License along with this program; if not, write to the Free
 *     Software Foundation, Inc., 675 Mass Ave, Cambridge, MA
 *     02139, USA.
 * 
 */


#include <stdio.h>

#include "quote.h"


/* 
 * prepare a file name to survive the shell's quoting rules.
 * From the Unix programming FAQ by way of Liviu.
 */

char *quote (char *d, size_t l, const char *f)
{
  size_t i, j = 0;

  if(!f) 
  {
    *d = '\0';
    return 0;
  }

  /* leave some space for the trailing characters. */
  l -= 6;
  
  d[j++] = '\'';
  
  for(i = 0; j < l && f[i]; i++)
  {
    if(f[i] == '\'' || f[i] == '`')
    {
      d[j++] = '\'';
      d[j++] = '\\';
      d[j++] = f[i];
      d[j++] = '\'';
    }
    else
      d[j++] = f[i];
  }
  
  d[j++] = '\'';
  d[j]   = '\0';
  
  return d;
}
