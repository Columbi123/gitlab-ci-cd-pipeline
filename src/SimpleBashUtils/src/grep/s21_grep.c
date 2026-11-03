#include "s21_grep.h"

int main(int argc, char *argv[]) {
  Options options = {0};

  par_options(argc, argv, &options);
  process_file(&options);

  return 0;
}

void par_options(int argc, char *argv[], Options *options) {
  int opt;

  // Добавили все флаги: e, i, v, c, l, n, h, s, f, o
  while ((opt = getopt(argc, argv, "e:ivclnhsf:o")) != -1) {
    switch (opt) {
      case 'e':
        // Шаблон из аргумента -e
        if (options->pattern_count < MAX_PATTERNS) {
          strncpy(options->patterns[options->pattern_count++], optarg,
                  BUFSIZ - 1);
          options->patterns[options->pattern_count - 1][BUFSIZ - 1] = '\0';
        }
        options->e = 1;
        break;

      case 'i':
        options->i = 1;
        break;

      case 'v':
        options->v = 1;
        break;

      case 'c':
        options->c = 1;
        break;

      case 'l':
        options->l = 1;
        break;

      case 'n':
        options->n = 1;
        break;

      case 'h':
        options->h = 1;
        break;

      case 's':
        options->s = 1;
        break;

      case 'f':
        // Читаем шаблоны из файла
        read_patterns_from_file(optarg, options);
        options->f = 1;
        break;

      case 'o':
        options->o = 1;
        break;

      default:
        fprintf(stderr, "Usage: s21_grep [OPTION]... PATTERNS [FILE]...\n");
        exit(EXIT_FAILURE);
    }
  }

  // Если не было -e и -f, первый аргумент - это шаблон
  if (!options->e && !options->f && optind < argc) {
    if (options->pattern_count < MAX_PATTERNS) {
      strncpy(options->patterns[options->pattern_count++], argv[optind++],
              BUFSIZ - 1);
      options->patterns[options->pattern_count - 1][BUFSIZ - 1] = '\0';
    }
  }

  // Остальные аргументы - это файлы
  while (optind < argc && options->file_count < MAX_FILES) {
    strncpy(options->files[options->file_count++], argv[optind++], BUFSIZ - 1);
    options->files[options->file_count - 1][BUFSIZ - 1] = '\0';
  }

  // Проверка наличия шаблонов
  if (options->pattern_count == 0) {
    if (!options->s) {
      fprintf(stderr, "No patterns provided\n");
    }
    exit(EXIT_FAILURE);
  }
}

void read_patterns_from_file(const char *filename, Options *options) {
  FILE *file = fopen(filename, "r");
  if (!file) {
    if (!options->s) {
      fprintf(stderr, "s21_grep: %s: No such file or directory\n", filename);
    }
    return;
  }

  char line[BUFSIZ];
  while (fgets(line, sizeof(line), file) &&
         options->pattern_count < MAX_PATTERNS) {
    // Убираем символ новой строки
    size_t len = strlen(line);
    if (len > 0 && line[len - 1] == '\n') {
      line[len - 1] = '\0';
    }

    // Пропускаем пустые строки
    if (strlen(line) == 0) {
      continue;
    }

    strncpy(options->patterns[options->pattern_count++], line, BUFSIZ - 1);
    options->patterns[options->pattern_count - 1][BUFSIZ - 1] = '\0';
  }

  fclose(file);
}

void process_file(Options *options) {
  int show_filename = (options->file_count > 1) && !options->h;

  for (int i = 0; i < options->file_count; i++) {
    FILE *file = fopen(options->files[i], "r");
    if (!file) {
      if (!options->s) {
        fprintf(stderr, "s21_grep: %s: No such file or directory\n",
                options->files[i]);
      }
      continue;
    }

    char line[BUFSIZ];
    int line_num = 0;
    int match_count = 0;
    int cflags = REG_EXTENDED;

    if (options->i) {
      cflags |= REG_ICASE;
    }

    // Компилируем первый валидный regex
    regex_t regex;
    int regex_compiled = 0;

    for (int p = 0; p < options->pattern_count; p++) {
      if (regcomp(&regex, options->patterns[p], cflags) == 0) {
        regex_compiled = 1;
        break;
      }
    }

    if (!regex_compiled) {
      if (!options->s) {
        fprintf(stderr, "Invalid pattern\n");
      }
      fclose(file);
      continue;
    }

    // Основной цикл чтения файла
    while (fgets(line, sizeof(line), file)) {
      line_num++;

      // Проверяем совпадение
      int match = 0;
      for (int p = 0; p < options->pattern_count && !match; p++) {
        regex_t temp_regex;
        if (regcomp(&temp_regex, options->patterns[p], cflags) == 0) {
          if (regexec(&temp_regex, line, 0, NULL, 0) == 0) {
            match = 1;
          }
          regfree(&temp_regex);
        }
      }

      // Инверсия для -v
      if (options->v) {
        match = !match;
      }

      if (match) {
        match_count++;

        // Вывод результатов
        if (!options->c && !options->l) {
          if (options->o && !options->v) {
            // Для -o выводим только совпадения
            print_only_matches(&regex, line, options, options->files[i],
                               line_num);
          } else if (!options->o) {
            // Обычный вывод строки
            print_match(options->files[i], line_num, line, options);
          }
        }
      }
    }

    // Вывод статистики для -c и -l
    if (options->c) {
      if (show_filename) {
        printf("%s:", options->files[i]);
      }
      printf("%d\n", match_count);
    }

    if (options->l && match_count > 0) {
      printf("%s\n", options->files[i]);
    }

    regfree(&regex);
    fclose(file);
  }
}

void print_match(const char *filename, int line_num, const char *line,
                 Options *options) {
  int show_filename = (options->file_count > 1) && !options->h;

  if (show_filename) {
    printf("%s:", filename);
  }

  if (options->n) {
    printf("%d:", line_num);
  }

  printf("%s", line);

  // Если строка не заканчивается на \n, добавляем его
  if (line[strlen(line) - 1] != '\n') {
    printf("\n");
  }
}

void print_file_stat(const char *filename, int match_count, Options *options) {
  int show_filename = (options->file_count > 1) && !options->h;

  if (options->l) {
    if (match_count > 0) {
      printf("%s\n", filename);
    }
  } else if (options->c) {
    if (show_filename) {
      printf("%s:", filename);
    }
    printf("%d\n", match_count);
  }
}

void print_only_matches(regex_t *regex, const char *line, Options *options,
                        const char *filename, int line_num) {
  const char *p = line;
  regmatch_t match;
  int show_filename = (options->file_count > 1) && !options->h;

  // Ищем все совпадения в строке
  while (regexec(regex, p, 1, &match, 0) == 0) {
    if (match.rm_so == match.rm_eo) {
      // Пустое совпадение, пропускаем
      p++;
      if (*p == '\0') break;
      continue;
    }

    if (show_filename) {
      printf("%s:", filename);
    }

    if (options->n) {
      printf("%d:", line_num);
    }

    // Выводим только совпавшую часть
    printf("%.*s\n", (int)(match.rm_eo - match.rm_so), p + match.rm_so);

    // Продолжаем поиск со следующей позиции
    p += match.rm_eo;

    // Если достигли конца строки, выходим
    if (*p == '\0') break;
  }
}