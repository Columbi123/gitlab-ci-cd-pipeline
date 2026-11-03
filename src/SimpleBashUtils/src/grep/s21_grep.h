#ifndef S21_GREP_H
#define S21_GREP_H

#include <getopt.h>
#include <regex.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#define MAX_PATTERNS 100
#define MAX_FILES 100

typedef struct {
  int e;  // флаг -e (шаблон)
  int i;  // флаг -i (игнорировать регистр)
  int v;  // флаг -v (инверсия)
  int c;  // флаг -c (количество строк)
  int l;  // флаг -l (только имена файлов)
  int n;  // флаг -n (номер строки)
  int h;  // флаг -h (не выводить имя файла)
  int s;  // флаг -s (подавлять ошибки)
  int f;  // флаг -f (читать шаблоны из файла)
  int o;  // флаг -o (только совпадения)

  char patterns[MAX_PATTERNS][BUFSIZ];
  int pattern_count;
  char files[MAX_FILES][BUFSIZ];
  int file_count;
} Options;

void par_options(int argc, char *argv[], Options *options);
void process_file(Options *options);
void print_match(const char *filename, int line_num, const char *line,
                 Options *options);
void print_file_stat(const char *filename, int match_count, Options *options);
void read_patterns_from_file(const char *filename, Options *options);
void print_only_matches(regex_t *regex, const char *line, Options *options,
                        const char *filename, int line_num);

#endif