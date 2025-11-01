set -eo pipefail

files=$(find "y2023" -type f -name "day*.py" | sort)
for file in ${files}; do
  file_name=$(basename ${file})
  echo "Running ${file_name}"
  echo "----------------"
  python3 ${file}
  echo ""
  echo "****************"
done
