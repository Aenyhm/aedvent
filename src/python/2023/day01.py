import re
import time
from collections import OrderedDict
from collections.abc import Callable, Iterable

from utils import get_file_content


SPELLED_NUMBERS = (
    "one",
    "two",
    "three",
    "four",
    "five",
    "six",
    "seven",
    "eight",
    "nine",
)


def file_to_lines(file_name: str) -> list[str]:
    return get_file_content(file_name).split("\n")


def get_digits_indices(line: str) -> dict[int, int]:
    return {index: int(char) for index, char in enumerate(line) if char.isdigit()}


def get_spelled_indices(line: str) -> dict[int, int]:
    return {
        match.start(): SPELLED_NUMBERS.index(spelled_number) + 1
        for spelled_number in SPELLED_NUMBERS
        for match in re.finditer(pattern=spelled_number, string=line)
    }


def line_to_calibration(
    line: str, parse_functions: Iterable[Callable[[str], dict[int, int]]]
) -> int:
    number_indices = {}
    for parse_function in parse_functions:
        number_indices |= parse_function(line)

    numbers = list(OrderedDict(sorted(number_indices.items())).values())

    return int(f"{numbers[0]}{numbers[-1]}")


def part1(lines: list[str]) -> int:
    calibrations = (line_to_calibration(line, [get_digits_indices]) for line in lines)

    return sum(calibrations)


def part2(lines: list[str]) -> int:
    calibrations = (
        line_to_calibration(line, [get_digits_indices, get_spelled_indices])
        for line in lines
    )

    return sum(calibrations)


if __name__ == "__main__":
    example1_lines = file_to_lines("data/2023/01/example1.txt")
    example2_lines = file_to_lines("data/2023/01/example2.txt")
    puzzle_lines = file_to_lines("data/2023/01/input.txt")

    assert part1(example1_lines) == 142

    start = time.time()
    assert part1(puzzle_lines) == 55816
    end = time.time()
    print(f'{(end - start)*1000:.3} ms')

    assert part2(example2_lines) == 281

    start = time.time()
    assert part2(puzzle_lines) == 54980
    end = time.time()
    print(f'{(end - start)*1000:.3} ms')
