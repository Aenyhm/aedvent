use std::cmp::Reverse;

fn sum_elf_calories(s: &str) -> u32 {
    s.split("\n")
        .filter(|&x| !x.is_empty())
        .map(|x| x.parse::<u32>().unwrap())
        .sum::<u32>()
}

pub fn day1(input: String) {
    let mut groups = input
        .split("\n\n")
        .map(sum_elf_calories)
        .collect::<Vec<u32>>();

    groups.sort_by_key(|&x| Reverse(x));

    let it = groups.into_iter();

    let result1 = it.clone().next().unwrap();
    let result2 = it.take(3).sum::<u32>();

    assert_eq!(result1, 75501);
    assert_eq!(result2, 215594);
}
