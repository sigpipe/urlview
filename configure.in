AC_INIT(urlview.c)
AM_INIT_AUTOMAKE(urlview, 0.9)
dnl AM_CONFIG_HEADER(config.h)
AC_PROG_CC_STDC
AC_PROG_CC_C99
CFLAGS="$CFLAGS -DURLVIEW -Dunix"
AC_ARG_WITH(slang, [  --with-slang[=DIR]      use S-Lang instead of curses],
	[AC_DEFINE(USE_SLANG)
	if test $withval = yes -a -d ../slang; then
		withval=../slang
	fi
	if test $withval != yes; then
		if test $withval/include; then
			CPPFLAGS="$CPPFLAGS -I$withval/include"
			LIBS="$LIBS -L$withval/lib -lslang -lm"
		else
			CPPFLAGS="$CPPFLAGS -I$withval/src"
			LIBS="$LIBS $withval/src/objs/libslang.a -lm"
		fi
	else
		if test -d /usr/include/slang; then
			CPPFLAGS="$CPPFLAGS -I/usr/include/slang"
		fi
		LIBS="$LIBS -lslang -lm"
	fi],

	[AC_CHECK_LIB(ncursesw, initscr,
	 [LIBS="$LIBS -lncursesw"],
	 [AC_CHECK_LIB(ncurses, initscr, [LIBS="$LIBS -lncurses"], [LIBS="$LIBS -lcurses"])])
	AC_CHECK_FUNCS(curs_set)
	AC_CHECK_HEADERS(ncurses.h)
	AC_CHECK_HEADER(ncursesw/curses.h, [CPPFLAGS="$CPPFLAGS -I/usr/include/ncursesw"],
		[AC_CHECK_HEADER(ncurses/curses.h, [CPPFLAGS="$CPPFLAGS -I/usr/include/ncurses"])])])

AC_REPLACE_FUNCS(snprintf)

AC_ARG_WITH(rx, [  --with-rx=DIR           where to find librx],
[if test $withval != yes; then
	CPPFLAGS="$CPPFLAGS -I$withval"
	if test -d $withval/include; then
		CPPFLAGS=$CPPFLAGS/include
	fi

	if test -d $withval/lib; then
		LIBS="$LIBS $withval/lib/librx.a"
	else
		LIBS="$LIBS $withval/librx.a"
	fi
fi],
[AC_CHECK_FUNC(regcomp, [], [AC_CHECK_LIB(rx, regcomp, [LIBS="$LIBS -lrx"],
	[AC_MSG_ERROR(you must install GNU librx)])])
AC_CHECK_HEADERS(regex.h)
])

AC_CHECK_HEADERS(stdarg.h)
AC_CHECK_HEADERS(varargs.h)

AC_ARG_ENABLE(warnings, [  --enable-warnings       turn on compiler warnings],
[if test $enableval = yes; then
	CFLAGS="$CFLAGS -pedantic -ansi -Wall"
fi])

AC_OUTPUT(Makefile)
