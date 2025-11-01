import re
import typing

import utils

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
SPELLED_MAP = {name: str(i + 1) for i, name in enumerate(SPELLED_NUMBERS)}

NUMBERS_PATTERN1 = re.compile(r"\d")
NUMBERS_PATTERN2 = re.compile(rf"(?=(\d|{'|'.join(SPELLED_NUMBERS)}))")

ParseLineFn = typing.Callable[[str], list[str]]


def parse_part1(line: str) -> list[str]:
    return NUMBERS_PATTERN1.findall(line)


def parse_part2(line: str) -> list[str]:
    raw_digits = NUMBERS_PATTERN2.findall(line)

    return [x if x.isdigit() else SPELLED_MAP[x] for x in raw_digits]


def iter_lines_to_calibrations(
    lines: list[str], parse_line_fn: ParseLineFn
) -> typing.Iterator[int]:
    for line in lines:
        digits = parse_line_fn(line)
        yield int(digits[0]) * 10 + int(digits[-1])


def program(lines: list[str], parse_line_fn: ParseLineFn) -> int:
    return sum(iter_lines_to_calibrations(lines, parse_line_fn))


def part1(lines: list[str]) -> int:
    return program(lines, parse_part1)


def part2(lines: list[str]) -> int:
    return program(lines, parse_part2)


if __name__ == "__main__":
    example1_lines = utils.file_to_lines("01/example1.txt")
    example2_lines = utils.file_to_lines("01/example2.txt")
    puzzle_lines = utils.file_to_lines("01/input.txt")

    assert part1(example1_lines) == 142
    assert utils.profile(part1)(puzzle_lines) == 55816

    assert part2(example2_lines) == 281
    assert utils.profile(part2)(puzzle_lines) == 54980
