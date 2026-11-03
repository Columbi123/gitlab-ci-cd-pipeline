# CI/CD для утилит на C

Учебный проект: автоматизация сборки, проверки форматирования и интеграционного тестирования двух консольных утилит `s21_cat` и `s21_grep`.

## Стек

C, GNU Make, Bash, GitLab CI/CD, GitHub Actions, SSH/SCP.

## Сборка и проверка локально (Linux/WSL)

```bash
make -C src/SimpleBashUtils/src/cat
make -C src/SimpleBashUtils/src/grep
mkdir -p src/artifacts
cp src/SimpleBashUtils/src/cat/s21_cat src/artifacts/
cp src/SimpleBashUtils/src/grep/s21_grep src/artifacts/
bash src/integration_tests.sh
```

- `.gitlab-ci.yml` — первоначальный pipeline в GitLab; этап деплоя требует отдельной настройки окружения и SSH.
- `.github/workflows/ci-separated.yml` — GitHub Actions: сборка, линтинг и интеграционные тесты; деплой намеренно выключен до настройки безопасного окружения.
- `src/deploy.sh` — скрипт ручного развёртывания; требует `PROD_USER` и `PROD_HOST`.
- `src/notify.sh` — уведомления Telegram, токен и ID чата берутся только из переменных окружения.
- `src/report.md` — учебный отчёт. Недоступные исходные скриншоты отмечены как отсутствующие.

**Безопасность:** не сохраняйте токены Telegram или SSH-ключи в Git; не включайте деплой до настройки хоста и ключей.
