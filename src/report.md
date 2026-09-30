# DO6 — CI/CD

Краткий отчет по настройке CI/CD для проекта SimpleBashUtils. Все основные действия и результаты подтверждены скриншотами.

## Part 1. Setting up the gitlab-runner

- Поднята виртуальная машина Ubuntu Server 24.04 LTS для GitLab Runner.
- Установлен и запущен `gitlab-runner`; для совместимости с сервером GitLab использована ветка Runner 18.11.
- Runner зарегистрирован в проекте `DO6_CICD` с именем `do6-runner` и исполнителем `shell`.
- **Скриншот 1 — работа зарегистрированного GitLab Runner и использование Shell executor.**

  ![Part 1 — GitLab Runner работает](report_images/part1_01_runner_final.png)

- **Скриншот 2 — зарегистрированный runner `do6-runner`, URL GitLab и executor `shell`.**

  ![Part 1 — регистрация runner](report_images/part1_02_runner_registered.png)

## Part 2. Building

- В `.gitlab-ci.yml` добавлен этап `build`.
- На этапе сборки через Makefile собираются приложения `s21_cat` и `s21_grep`.
- Полученные исполняемые файлы копируются в `src/artifacts/`.
- Результаты сборки сохраняются как GitLab artifacts со сроком хранения 30 дней.
- При ошибке сборки job завершается с ненулевым кодом и pipeline получает статус `Failed`.
- **Скриншот 1 — сборка `s21_cat` и `s21_grep` и подготовка artifacts.**

  ![Part 2 — build log](report_images/part2_01_build_log.png)

- **Скриншот 2 — успешное завершение build pipeline со статусом `Passed`.**

  ![Part 2 — pipeline passed](report_images/part2_02_pipeline_passed.png)

## Part 3. Codestyle test

- В pipeline добавлен этап `codestyle`, выполняемый после успешного `build`.
- Для проверки исходного кода используется `clang-format`.
- Проверка выполняется для `.c` и `.h` файлов проектов `s21_cat` и `s21_grep` с параметрами `--dry-run --Werror --style=file`.
- Если форматирование не соответствует правилам, `clang-format` возвращает ошибку и pipeline завершается неуспешно.
- **Скриншот 1 — последовательность стадий `build` и `codestyle`, обе завершены успешно.**

  ![Part 3 — build and codestyle](report_images/part3_01_pipeline_build_codestyle.png)

- **Скриншот 2 — вывод `clang-format` для `s21_cat` и `s21_grep`, проверка завершена успешно.**

  ![Part 3 — codestyle log](report_images/part3_02_codestyle_log.png)

## Part 4. Integration tests

- В pipeline добавлен этап `integration_tests`.
- Этап запускается только после успешного прохождения `build` и `codestyle`.
- Для интеграционной проверки используется скрипт, который запускает собранные `s21_cat` и `s21_grep` на тестовых данных и сравнивает результат с системными утилитами `cat` и `grep`.
- Для каждого кейса в лог выводится результат `PASS` или `FAIL`; при наличии хотя бы одного `FAIL` скрипт возвращает `exit 1`.
- **Скриншот 1 — pipeline с тремя последовательными этапами: `build`, `codestyle`, `integration_tests`.**

  ![Part 4 — three stages](report_images/part4_01_pipeline_three_stages.png)

- **Скриншот 2 — результаты интеграционных тестов `s21_cat` и `s21_grep`; тесты завершены успешно.**

  ![Part 4 — integration tests log](report_images/part4_02_integration_log.png)

## Part 5. Deployment stage

- Поднята вторая виртуальная машина Ubuntu Server 24.04 LTS, выполняющая роль production-сервера.
- В `.gitlab-ci.yml` добавлен этап `deploy` с ручным запуском (`when: manual`).
- Deploy становится доступен только после успешного прохождения предыдущих CI-этапов.
- Для пользователя `gitlab-runner` настроен SSH-доступ к production-серверу по ключу.
- Скрипт `src/deploy.sh` получает artifacts после сборки, копирует `s21_cat` и `s21_grep` по `scp` во временный каталог production-сервера, после чего устанавливает их в `/usr/local/bin`.
- При ошибке SSH, SCP или установки скрипт завершает job с ошибкой.
- После выполнения задания сохранены snapshots обеих виртуальных машин; snapshots в Git не загружались.
- **Скриншот 1 — deploy ожидает ручного запуска после успешного прохождения CI.**

  ![Part 5 — manual deploy](report_images/part5_01_manual_deploy.png)

- **Скриншот 2 — успешное копирование и установка бинарных файлов на production-сервер.**

  ![Part 5 — deploy log](report_images/part5_02_deploy_log.png)

- **Скриншот 3 — проверка `s21_cat` и `s21_grep` непосредственно на production-сервере из `/usr/local/bin`.**

  ![Part 5 — production check](report_images/part5_03_prod_check.png)

## Part 6. Bonus. Notifications

- Создан Telegram-бот `malladok DO6 CI/CD` для уведомлений о состоянии pipeline.
- Токен бота и `chat_id` не хранятся в репозитории; они передаются job через окружение GitLab Runner.
- В pipeline добавлены jobs `notify_success` и `notify_failure`.
- Успешное уведомление содержит результаты как CI-этапов (`build`, `codestyle`, `integration_tests`), так и CD-этапа (`deploy`).
- **Скриншот 1 — успешное выполнение `notify_success` и отправка сообщения в Telegram.**

  ![Part 6 — notify success log](report_images/part6_01_notify_log.png)

- **Скриншот 2 — Telegram-уведомление об успешном прохождении CI и CD.**

  ![Part 6 — Telegram success notification](report_images/part6_02_telegram_success.png)
