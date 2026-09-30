CC = gcc
CFLAGS = -std=c11 -Wall -Wextra -Werror
EXECUTABLE = s21_cat
SOURCES = s21_cat.c
HEADERS = s21_cat.h

.PHONY: s21_cat clean

s21_cat: $(EXECUTABLE)

$(EXECUTABLE): $(SOURCES) $(HEADERS)
	$(CC) $(CFLAGS) $(SOURCES) -o $@

clean:
	rm -rf $(EXECUTABLE)
