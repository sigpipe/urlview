#! /bin/bash

#   Copyright (c) 1998  Martin Schulze <joey@debian.org>
#   Slightly modified by Luis Francisco Gonzalez <luisgh@debian.org>

#   This program is free software; you can redistribute it and/or modify
#   it under the terms of the GNU General Public License as published by
#   the Free Software Foundation; either version 2 of the License, or
#   (at your option) any later version.

#   This program is distributed in the hope that it will be useful,
#   but WITHOUT ANY WARRANTY; without even the implied warranty of
#   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
#   GNU General Public License for more details.

#   You should have received a copy of the GNU General Public License
#   along with this program; if not, write to the Free Software
#   Foundation, Inc., 59 Temple Place - Suite 330, Boston, MA 02111-1307, USA.

###########################################################################
# Configurable section
###########################################################################
#
# Any entry in the lists of programs that urlview handler will try out will
# be made of /path/to/program + ':' + TAG where TAG is one of
# XW: XWindows program
# XT: Launch with an xterm if possible or as VT if not
# VT: Launch in the same terminal

# The lists of programs to be executed are
https_prgs="/usr/X11R6/bin/netscape:XW /usr/bin/lynx:XT"
http_prgs="/usr/bin/lynx:XT /usr/X11R6/bin/netscape:XW"
mailto_prgs="/usr/bin/mutt:VT /usr/bin/elm:VT /usr/bin/pine:VT /usr/bin/mail:VT"
gopher_prgs="/usr/bin/lynx:XT /usr/bin/gopher:XT"
ftp_prgs="/usr/bin/lynx:XT /usr/bin/ncftp:XT"

# Program used as an xterm (if it doesn't support -T you'll need to change
# the command line in getprg)
XTERM=/usr/X11R6/bin/xterm


###########################################################################
# Change bellow this at your own risk
###########################################################################
function getprg()
{
    local ele tag prog

    for ele in $*
    do
	tag=${ele##*:}
	prog=${ele%%:*}
	if [ -x $prog ]; then
	    case $tag in
	    XW)
		[ -n "$DISPLAY" ] && echo "X:$prog" && return 0
		;;
	    XT)
		[ -n "$DISPLAY" ] && [ -x "$XTERM" ] && \
		    echo "X:$XTERM -e $prog" && return 0
		echo "$prog" && return 0
		;;
	    VT)
		echo "$prog" && return 0
		;;
	    esac
	fi
    done
}

url="$1"; shift

type="${url%%:*}"

if [ "$url" = "$type" ]; then
    type="${url%%.*}"
    case "$type" in
    www|web)
	type=http
	;;
    esac
    url="$type://$url"
fi

case $type in
https)
    prg=`getprg $https_prgs`
    ;;
http)
    prg=`getprg $http_prgs`
    ;;
ftp)
    prg=`getprg $ftp_prgs`
    ;;
mailto)
    prg=`getprg $mailto_prgs`
    url="${url#mailto:}"
    ;;
gopher)
    prg=`getprg $gopher_prgs`
    ;;
*)
    echo "Unknown URL type.  Please report URL and viewer to:"
    echo "joey@debian.org."
    echo -n "Press enter to continue."; read x
    exit
    ;;
esac

if [ -n "$prg" ]; then
   if [ "${prg%:*}" = "X" ]; then
    ${prg#*:} "$url" 2>/dev/null &
   else
    $prg "$url"
   fi
fi




