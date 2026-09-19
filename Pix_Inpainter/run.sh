#!/usr/bin/env bash
set -euo pipefail

xhost +local:docker >/dev/null 2>&1 || true

docker compose -f docker/docker-compose.yml up --build "$@"
