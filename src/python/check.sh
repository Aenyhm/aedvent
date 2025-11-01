set -eo pipefail

echo "Typing:"
uvx mypy .

echo "Lint:"
uvx ruff check

echo "Format:"
uvx ruff format
