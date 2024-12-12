fn find_common_char(strings: &[&str]) -> char {
    strings
        .iter()
        .fold(strings[0].chars().collect::<Vec<char>>(), |common_chars, s| {
            common_chars
                .iter()
                .filter(|&&c| s.contains(c))
                .cloned()
                .collect::<Vec<char>>()
        })
        .pop()
        .unwrap()
}

fn string_to_priority(strings: &[&str]) -> u16 {
    let value = find_common_char(strings) as u16;

    // ASCII : A = 65, a = 97
    // -> A : 27-65 = -38
    // -> a :  1-97 = -96
    value - if value < 97 { 38 } else { 96 }
}

fn line_to_priority(line: &str) -> u16 {
    let (left_str, right_str) = line.split_at(line.len() / 2);

    string_to_priority(&[left_str, right_str])
}

pub fn day3(input: String) {
    let result1 = input.split("\n")
        .filter(|&x| !x.is_empty())
        .map(line_to_priority)
        .sum::<u16>();

    assert_eq!(result1, 7742);

    let result2 = input.lines()
        .collect::<Vec<&str>>()
        .chunks(3)
        .map(string_to_priority)
        .sum::<u16>();

    assert_eq!(result2, 2276);
}
