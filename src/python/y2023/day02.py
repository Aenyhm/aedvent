from collections.abc import Iterator
from dataclasses import dataclass
from functools import reduce
from typing import Self

import utils


@dataclass
class Turn:
    red: int = 0
    green: int = 0
    blue: int = 0

    def __iter__(self: Self) -> Iterator[tuple[str, int]]:
        yield from self.__dict__.items()

    def __getitem__(self: Self, color: str) -> int:
        return getattr(self, color)


@dataclass(slots=True)
class Game:
    gid: int
    turns: list[Turn]


MAX_COLORS_TURN = Turn(12, 13, 14)


def iter_turn_info(turn_info: str) -> Iterator[tuple[str, int]]:
    for cube_info in turn_info.split(", "):
        number, color = cube_info.split(" ")
        yield color, int(number)


def parse_turn(turn_info: str) -> Turn:
    return Turn(**dict(iter_turn_info(turn_info)))


def parse_game(game_info: str) -> Game:
    id_part, info_part = game_info.split(": ")

    return Game(
        int(id_part[5:]),
        list(map(parse_turn, info_part.split("; "))),
    )


def file_to_games(file_name: str) -> list[Game]:
    lines = utils.file_to_lines(file_name)

    return list(map(parse_game, lines))


def get_max_game_id(game: Game) -> int:
    success = all(
        MAX_COLORS_TURN[color] >= number
        for turn in game.turns
        for color, number in turn
    )

    return game.gid if success else 0


def get_min_turn(t1: Turn, t2: Turn) -> Turn:
    return Turn(**{color: max(n1, t2[color]) for color, n1 in t1})


def get_cubes_power(game: Game) -> int:
    min_turn = reduce(get_min_turn, game.turns, Turn())

    return min_turn.red * min_turn.green * min_turn.blue


def part1(games: list[Game]) -> int:
    return sum(map(get_max_game_id, games))


def part2(games: list[Game]) -> int:
    return sum(map(get_cubes_power, games))


if __name__ == "__main__":
    example_games = file_to_games("02/example.txt")
    games = file_to_games("02/input.txt")

    assert part1(example_games) == 8
    assert utils.profile(part1)(games) == 2285

    assert part2(example_games) == 2286
    assert utils.profile(part2)(games) == 77021
