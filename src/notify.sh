#!/bin/bash

set -euo pipefail

STATUS="${1:-unknown}"

if [ "$STATUS" = "success" ]; then
    MESSAGE="✅ DO6 CI/CD SUCCESS

CI stages successfully passed:
- Build: PASSED
- Codestyle: PASSED
- Integration tests: PASSED

CD stage successfully passed:
- Deploy: PASSED

Branch: ${CI_COMMIT_REF_NAME}
Pipeline: ${CI_PIPELINE_URL}"
else
    MESSAGE="❌ DO6 CI/CD FAILED

Pipeline execution failed.

Branch: ${CI_COMMIT_REF_NAME}
Pipeline: ${CI_PIPELINE_URL}"
fi

curl --fail --silent --show-error \
    -X POST \
    "https://api.telegram.org/bot${TELEGRAM_BOT_TOKEN}/sendMessage" \
    -d "chat_id=${TELEGRAM_CHAT_ID}" \
    --data-urlencode "text=${MESSAGE}"

echo
echo "=== Telegram notification sent ==="