SHELL := /bin/bash
FILES = test1.txt test2.txt test3.txt
CHECK = 2>&1 valgrind --tool=memcheck --leak-check=yes

all: tests

tests:
	@for f in $(FILES); do \
		diff <(../s21_cat $$f) <(cat $$f) || true; \
		diff <(../s21_cat -b $$f) <(cat -b $$f) || true; \
		diff <(../s21_cat -e $$f) <(cat -e $$f) || true; \
		diff <(../s21_cat -n $$f) <(cat -n $$f) || true; \
		diff <(../s21_cat -s $$f) <(cat -s $$f) || true; \
		diff <(../s21_cat -t $$f) <(cat -t $$f) || true; \
		diff <(../s21_cat -T $$f) <(cat -T $$f) || true; \
		diff <(../s21_cat -etsnb $$f) <(cat -etsnb $$f) || true; \
	done
	@$(CHECK) ../s21_cat $(FILES) > /dev/null
	@$(CHECK) ../s21_cat -b $(FILES) > /dev/null
	@$(CHECK) ../s21_cat -n $(FILES) > /dev/null
	@$(CHECK) ../s21_cat -s $(FILES) > /dev/null
	@$(CHECK) ../s21_cat -e $(FILES) > /dev/null
	@$(CHECK) ../s21_cat -t $(FILES) > /dev/null

clean:
	rm -f *.o *.out

.PHONY: all tests clean
