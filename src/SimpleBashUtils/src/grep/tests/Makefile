SHELL := /bin/bash
FLAG = -Wall -Wextra -Werror -std=c11
FILES = test1.txt test2.txt
CHECK = 2>&1 valgrind --tool=memcheck --leak-check=yes

all: tests

tests:
	# Базовые тесты
	-diff <(../s21_grep hello $(FILES)) <(grep hello $(FILES))
	-diff <(../s21_grep -e hello $(FILES)) <(grep -e hello $(FILES))
	-diff <(../s21_grep -i hello $(FILES)) <(grep -i hello $(FILES))
	-diff <(../s21_grep -v hello $(FILES)) <(grep -v hello $(FILES))
	-diff <(../s21_grep -c hello $(FILES)) <(grep -c hello $(FILES))
	-diff <(../s21_grep -l hello $(FILES)) <(grep -l hello $(FILES))
	-diff <(../s21_grep -n hello $(FILES)) <(grep -n hello $(FILES))

	# Дополнительные флаги
	-diff <(../s21_grep -h hello $(FILES)) <(grep -h hello $(FILES))
	-diff <(../s21_grep -s hello test1.txt nonexistent.txt 2>&1) <(grep -s hello test1.txt nonexistent.txt 2>&1)
	-diff <(../s21_grep -f patterns.txt test1.txt) <(grep -f patterns.txt test1.txt)
	-diff <(../s21_grep -o hello $(FILES)) <(grep -o hello $(FILES))

	# Комбинации флагов
	-diff <(../s21_grep -in hello $(FILES)) <(grep -in hello $(FILES))
	-diff <(../s21_grep -iv hello $(FILES)) <(grep -iv hello $(FILES))
	-diff <(../s21_grep -vc hello $(FILES)) <(grep -vc hello $(FILES))
	-diff <(../s21_grep -ho hello $(FILES)) <(grep -ho hello $(FILES))
	-diff <(../s21_grep -ln hello $(FILES)) <(grep -ln hello $(FILES))
	-diff <(../s21_grep -ic hello $(FILES)) <(grep -ic hello $(FILES))

	# Проверка памяти
	@$(CHECK) ../s21_grep hello test1.txt > /dev/null
	@$(CHECK) ../s21_grep -e hello test1.txt > /dev/null
	@$(CHECK) ../s21_grep -i hello test1.txt > /dev/null
	@$(CHECK) ../s21_grep -v hello test1.txt > /dev/null
	@$(CHECK) ../s21_grep -c hello test1.txt > /dev/null
	@$(CHECK) ../s21_grep -l hello test1.txt > /dev/null
	@$(CHECK) ../s21_grep -n hello test1.txt > /dev/null
	@$(CHECK) ../s21_grep -h hello test1.txt > /dev/null
	@$(CHECK) ../s21_grep -o hello test1.txt > /dev/null
	@$(CHECK) ../s21_grep -f patterns.txt test1.txt > /dev/null

clean:
	rm -f *.o *.out

.PHONY: all tests clean