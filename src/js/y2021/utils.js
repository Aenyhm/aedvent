import { readFileSync } from "node:fs";

const DATA_FOLDER = "../../../data/2021";

export function file_to_lines(file_path) {
  const content = readFileSync(`${DATA_FOLDER}/${file_path}`, "utf8");

  return content.split('\n');
}

export function assert(result, expected) {
  console.assert(result === expected, `result was ${result} but we expected ${expected}`);
}

export function profile(fn, runs=10) {
  return function() {
    const times = []
    let result = null;
    for (let i = 0; i < runs; i++) {
      const start = performance.now();
      result = fn.apply(this, arguments);
      const end = performance.now();
      times.push(end - start);
    }
    console.log(`${fn.name}: ${Math.round(mean(times)*10)/10} ms`);
    return result;
  };
}

function sum(arr) {
  return arr.reduce((acc, value) => acc + value)
}

function mean(arr) {
  return sum(arr)/arr.length;
}
