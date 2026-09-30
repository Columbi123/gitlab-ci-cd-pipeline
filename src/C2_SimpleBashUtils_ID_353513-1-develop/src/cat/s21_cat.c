#include "s21_cat.h"

int main(int argc, char *argv[]) {
  Options options = {0};
  par_options(argc, argv, &options);

  if (optind >= argc) {
    return 0;
  } else {
    for (int i = optind; i < argc; i++) {
      FILE *file = fopen(argv[i], "r");
      if (file == NULL) {
        perror(argv[i]);
        return 1;
      }
      process_file(file, options);
      fclose(file);
    }
  }

  return 0;
}

void par_options(int argc, char *argv[], Options *options) {
  int opt;

  while ((opt = getopt(argc, argv, "beEnstTv")) != -1) {
    switch (opt) {
      case 'b':
        options->b = 1;
        options->n = 0;  // -b отменяет -n
        break;
      case 'e':
        options->e = 1;
        options->v = 1;
        break;
      case 'E':
        options->e = 1;
        break;
      case 'n':
        options->n = 1;
        break;
      case 's':
        options->s = 1;
        break;
      case 't':
        options->t = 1;
        options->v = 1;
        break;
      case 'T':
        options->t = 1;
        break;
      case 'v':
        options->v = 1;
        break;
      default:
        break;
    }
  }
}

void process_file(FILE *file, Options options) {
  int line_number = 1;
  int empty_line_count = 0;
  int previous_char = '\n';
  int c;
  const int ASCII_CTRL_CHAR_OFFSET = 'A' - 1;

  while ((c = fgetc(file)) != EOF) {  // чтение файла посимвольно
    if (options.s && c == '\n' && previous_char == '\n') {
      empty_line_count++;
      if (empty_line_count > 1) continue;  // сжатиe пустых строк
    } else {
      empty_line_count = 0;
    }

    if (previous_char == '\n') {  // Нумерация строк (-n или -b)
      if ((options.b && c != '\n') || options.n) {
        printf("%6d\t", line_number++);
      }
    }

    if (options.t && c == '\t') {  // Отображение табов (-t/-T)
      printf("^I");
      previous_char = c;
      continue;
    }

    if (options.e && c == '\n') {  // Отображение концов строк (-e/-E)
      printf("$");
    }

    if (options.v) {
      if (c >= 0 && c < 32 && c != '\n' &&
          c != '\t') {  // Отображение непечатных символов (-v или -e/-t)
        printf("^%c", c + ASCII_CTRL_CHAR_OFFSET);
        previous_char = c;
        continue;
      } else if (c == 127) {
        printf("^?");
        previous_char = c;
        continue;
      }
    }

    if (c >= 0) {
      putchar(c);
    }
    previous_char = c;
  }
  if (ferror(file)) {
    perror("Error reading file");
  }
}
