CC = gcc
CFLAGS = -std=c11 -Wall -Wextra -Werror
EXECUTABLE = s21_cat
SOURCES = cat.c
HEADERS = cat.h

.PHONY: all clean
all: $(EXECUTABLE)

$(EXECUTABLE): $(SOURCES) $(HEADERS)
	$(CC) $(CFLAGS) $(SOURCES) -o $@

clean:
	rm -f $(EXECUTABLE)
