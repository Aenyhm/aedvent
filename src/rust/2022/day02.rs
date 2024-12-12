// Strat 1
// -------
// Rock > Scissors  : 1 > 3  -2 ;  2
// Paper > Rock     : 2 > 1   1 ; -1
// Scissors > Paper : 3 > 2   1 ; -1

// Strat 2
// -------
// Rock  > Win  -> Paper    : 1 > 2 (+1) - 1+6=7
// Rock  > Draw -> Rock     : 1 > 1 ( 0) - 1+3=4
// Rock  > Lose -> Scissors : 1 > 3 (-1) - 1+0=1
// -------
// Paper > Win  -> Scissors : 2 > 3 (+1) - 2+6=8
// Paper > Draw -> Paper    : 2 > 2 ( 0)
// Paper > Lose -> Rock     : 2 > 1 (-1)

enum Shape { Rock = 1, Paper = 2, Scissors = 3 }
enum Battle { Win = 6, Draw = 3, Lose = 0 }

fn get_op_value(c: char) -> u8 {
    (match c {
        'A' => Shape::Rock,
        'B' => Shape::Paper,
        'C' => Shape::Scissors,
        _ => panic!("Wrong letter {}", c)
    } as u8)
}

fn strat1(op_char: char, my_char: char) -> u8 {
    let op = get_op_value(op_char);
    let me = (match my_char {
        'X' => Shape::Rock,
        'Y' => Shape::Paper,
        'Z' => Shape::Scissors,
        _ => panic!("Wrong letter {}", my_char)
    } as u8);

    let diff = me as i8 - op as i8;

    me + match diff {
        0 => Battle::Draw,
        -2 | 1 => Battle::Win,
        2 | -1 => Battle::Lose,
        _ => panic!("Wrong score diff {}", diff)
    } as u8
}

fn strat2(op_char: char, my_char: char) -> u8 {
    let op = get_op_value(op_char);

    fn cycle_value(v: u8) -> u8 {
        match v { 0 => 3, 4 => 1, _ => v }
    }

    match my_char {
        'X' => Battle::Lose as u8 + cycle_value(op - 1),
        'Y' => Battle::Draw as u8 + op,
        'Z' => Battle::Win  as u8 + cycle_value(op + 1),
        _ => panic!("Wrong letter {}", op_char)
    }
}

fn line_to_score(line: &str, strat_fn: fn(char, char) -> u8) -> u8 {
    let mut values = line.split_whitespace()
        .flat_map(|word| word.chars());

    let op_char = values.next().unwrap();
    let my_char = values.next().unwrap();

    strat_fn(op_char, my_char)
}

pub fn day2(input: String) {
    let char_split = input.split('\n')
        .filter(|&x| !x.is_empty());

    let result1 = char_split.clone()
        .map(|x| line_to_score(x, strat1) as u16)
        .sum::<u16>();

    let result2 = char_split
        .map(|x| line_to_score(x, strat2) as u16)
        .sum::<u16>();

    assert_eq!(result1, 9759);
    assert_eq!(result2, 12429);
}

