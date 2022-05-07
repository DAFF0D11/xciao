# xciao - X Class name In And Out

PREFIX = /usr/local

CC = gcc
TARGET = xciao
SRC = xciao.c
LIBS = -lX11

all:
	$(CC) $(SRC) $(LIBS) -o $(TARGET)

clean:
	rm $(TARGET)

install:
	mkdir -p $(DESTDIR)$(PREFIX)/bin
	cp -f xciao $(DESTDIR)$(PREFIX)/bin
	chmod 755 $(DESTDIR)$(PREFIX)/bin/xciao

uninstall:
	rm -f $(DESTDIR)$(PREFIX)/bin/xciao\
