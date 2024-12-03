use std::fs;

fn sum(text: &str) -> i32 {
    let re = regex::Regex::new(r"mul\((\d+),(\d+)\)|do\(\)|don't\(\)").unwrap();
    let mut enabled = true;
    let mut total = 0;

    for m in re.find_iter(text) {
        match m.as_str() {
            "do()" => enabled = true,
            "don't()" => enabled = false,
            mul if enabled => {
                let nums: Vec<i32> = regex::Regex::new(r"\d+")
                    .unwrap()
                    .find_iter(mul)
                    .map(|n| n.as_str().parse().unwrap())
                    .collect();
                total += nums[0] * nums[1];
            }
            _ => (),
        }
    }
    total
}

fn main() {
    let memory = fs::read_to_string("input.txt").unwrap();
    println!("{}", sum(&memory));
}
