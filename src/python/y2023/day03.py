import re
from dataclasses import dataclass

import utils


@dataclass(slots=True)
class NumberMatch:
    start_pos: int
    end_pos: int
    value: int


def get_line_numbers_matches(line: str) -> list[NumberMatch]:
    return [
        NumberMatch(match.start(), match.end(), int(match[0]))
        for match in re.finditer(r"\d+", string=line)
    ]


def has_relative_line_symbol(match: NumberMatch, relative_line: str) -> bool:
    chars = relative_line[
        max(match.start_pos - 1, 0) : min(match.end_pos + 1, len(relative_line))
    ]

    return bool(chars.replace(".", ""))


def find_adjacent_symbols(match: NumberMatch, index: int, lines: list[str]) -> bool:
    current_line = lines[index]

    return (
        # Previous character
        (match.start_pos > 0 and current_line[match.start_pos - 1] != ".")
        or
        # Next character
        (match.end_pos < len(current_line) and current_line[match.end_pos] != ".")
        or
        # Previous line
        (index > 0 and has_relative_line_symbol(match, lines[index - 1]))
        or
        # Next line
        (index < len(lines) - 1 and has_relative_line_symbol(match, lines[index + 1]))
    )


def part1(lines: list[str]) -> int:
    number_matches = map(get_line_numbers_matches, lines)

    return sum(
        match.value
        for index, line_number_matches in enumerate(number_matches)
        for match in line_number_matches
        if find_adjacent_symbols(match, index, lines)
    )


if __name__ == "__main__":
    example_content = utils.file_to_lines("03/example.txt")
    input_content = utils.file_to_lines("03/input.txt")

    assert part1(example_content) == 4361
    assert utils.profile(part1)(input_content) == 544433
