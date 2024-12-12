set -eo pipefail

path=$(dirname $0)
name=$1
dist=$3

case "$(uname -s)" in
  Linux*)  platform="linux";;
  Darwin*) platform="mac";;
esac

exe_path="${path}/dist/${name}-${platform}-${dist}"

if [[ ${dist} = "debug" ]]; then
  debug_flags="-fno-omit-frame-pointer -fstack-protector-strong -fno-common -Wall -Wextra -Wshadow -Wstrict-aliasing=2 -Wformat -O1"

  if [ ${platform} = "linux" ]; then
    debug_flags="${debug_flags} -g3 -fsanitize=undefined,bounds,null -fanalyzer -Wanalyzer-null-dereference"
  else
    debug_flags="${debug_flags} -g2"
  fi

  flags="${flags} ${debug_flags}"
else
  flags="${flags} -O3"
fi

compiler="cc"
libs="-lm"

files=$(find "./src/c/2024" -type f -name "day*.c" | sort)
if [[ $2 = "last" ]]; then
  files=$(echo "${files}" | tail -n 1)
fi

for file in ${files}; do
  file_name=$(basename ${file})
  echo "Running ${file_name}"
  echo "---------------"
  full_exe="${exe_path}-${file_name}"
  full_exe=${full_exe::-2}
  ${compiler} ${flags} -o ${full_exe} ${file} ${libs}
  ASAN_OPTIONS=detect_leaks=0 ./${full_exe}
  echo ""
  echo "***************"
done
