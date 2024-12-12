mod day1;
mod day2;
mod day3;

use crate::get_day_input;
use crate::y2022::day1::day1;
use crate::y2022::day2::day2;
use crate::y2022::day3::day3;
use std::fs;

fn get_day_input(year: u16, n: u8) -> String {
    fs::read_to_string(format!("./data/{year}/{:02n}/input.txt")).unwrap()
}

fn main() {
    day1(get_day_input(2022, 1));
    day2(get_day_input(2022, 2));
    day3(get_day_input(2022, 3));
}
