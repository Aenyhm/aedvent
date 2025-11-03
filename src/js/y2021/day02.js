var utils = require("./utils");

class Command {
  constructor(direction, value) {
    this.direction = direction;
    this.value = value;
  }
}

function parse_line(line) {
  const [dir, val] = line.split(' ');
  return new Command(dir, Number(val));
}

function part1(lines) {
  let x = 0;
  let y = 0;

  for (const line of lines) {
    let command = parse_line(line);
    
    switch (command.direction) {
      case "forward": x += command.value; break;
      case "down":    y += command.value; break;
      case "up":      y -= command.value; break;
    }
  }

  return x*y;
}

function part2(lines) {
  let aim = 0;
  let x = 0;
  let y = 0;

  for (const line of lines) {
    let command = parse_line(line);
    
    switch (command.direction) {
      case "forward":
        x += command.value;
        y += aim*command.value;
        break;
      case "down":
        aim += command.value;
        break;
      case "up":      
        aim -= command.value;
        break;
    }
  }

  return x*y;
}

const example_lines = utils.file_to_lines("02/example.txt");
const puzzle_lines = utils.file_to_lines("02/input.txt");

utils.assert(part1(example_lines), 150);
utils.assert(utils.profile(part1)(puzzle_lines), 2039256);

utils.assert(part2(example_lines), 900);
utils.assert(utils.profile(part2)(puzzle_lines), 1856459736);
