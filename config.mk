# solisp version
VERSION = 2.0

# install paths
PREFIX = /usr/local
MANPREFIX = ${PREFIX}/share/man

# flags
CFLAGS = -Wall -Werror -std=c++17 -DVERSION=\"${VERSION}\"
LDFLAGS =

# compiler
CC = g++
