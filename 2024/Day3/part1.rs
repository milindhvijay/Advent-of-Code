use std::fs;

fn sum(text: &str) -> i32 {
    let re = regex::Regex::new(r"mul\((\d+),(\d+)\)").unwrap();
    re.captures_iter(text)
        .map(|c| c[1].parse::<i32>().unwrap() * c[2].parse::<i32>().unwrap())
        .sum()
}

fn main() {
    let input = fs::read_to_string("input.txt").unwrap();
    println!("{}", sum(&input));
}
