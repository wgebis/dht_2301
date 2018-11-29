AC_INIT([dht_am2301], [1.0], [wgebis@gmail.com])
AM_INIT_AUTOMAKE([-Wall -Werror foreign])
AC_PROG_CC
AC_CONFIG_HEADERS([config.h])
AC_CONFIG_FILES([
 Makefile
 src/Makefile
])
AC_CHECK_HEADERS([wiringPi.h])
AC_SEARCH_LIBS([wiringPiSetup], [wiringPi], [], [
  AC_MSG_ERROR([unable to find wiringPiSetup()])
]) 
AC_SEARCH_LIBS([pinMode], [wiringPiDev], [], [
  AC_MSG_ERROR([unable to find pinMode()])
])
AC_OUTPUT
