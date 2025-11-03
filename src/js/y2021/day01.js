var utils = require("./utils");

function count_depth_increase(values) {
  let result = 0;

  for (let i = 0; i < values.length - 1; i++) {
    if (values[i + 1] > values[i]) result++;
  }

  return result;
}

function part1(lines) {
  const values = lines.map(Number);

  return count_depth_increase(values);
}

function part2(lines) {
  const MEASUREMENTS_COUNT = 3;

  const values = lines.map(Number);

  const measurements_sums = [];

  for (let i = 0; i <= values.length - MEASUREMENTS_COUNT; i++) {
    let sum = 0;
    for (let j = 0; j < MEASUREMENTS_COUNT; j++) {
      sum += values[i + j];
    }
    measurements_sums.push(sum);
  }

  return count_depth_increase(measurements_sums);
}

const example_lines = utils.file_to_lines("01/example.txt");
const puzzle_lines = utils.file_to_lines("01/input.txt");

utils.assert(part1(example_lines), 7);
utils.assert(utils.profile(part1)(puzzle_lines), 1676);

utils.assert(part2(example_lines), 5);
utils.assert(utils.profile(part2)(puzzle_lines), 1706);
