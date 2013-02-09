cc = clang
bin = ./ant
cflags = -Wall -ansi -pedantic -O2 -pipe
src = ant.c types.c utils.c
libs = -lcurses

$(bin): clean
	$(cc) -o $@ $(cflags) $(src) $(libs)

test: $(bin)
	$(bin)

clean:
	rm -f $(bin)

install:
	install -m 0755 $(bin) /usr/local/games/$(bin)

uninstall:
	rm /usr/local/games/$(bin)

.PHONY: test clean install uninstall
