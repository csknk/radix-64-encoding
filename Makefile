SOURCES:= $(wildcard *.c) $(wildcard *.h)
OBJECTS:= $(wildcard *.c)
OUT:= bin/main
main: $(SOURCES)
	# cc -g -W -Wall -o $(OUT) $(OBJECTS) `libgcrypt-config --cflags --libs`
	cc -fsanitize=address -g -o $(OUT) $(OBJECTS) `libgcrypt-config --cflags --libs`
