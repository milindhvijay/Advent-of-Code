use std::collections::HashSet;
use std::fs;

struct Solver {
    cache: Vec<Option<u64>>,
}

impl Solver {
    fn new(design_len: usize) -> Self {
        Self {
            cache: vec![None; design_len + 1],
        }
    }

    fn count_combinations<'a>(
        &mut self,
        design: &'a str,
        pos: usize,
        patterns: &HashSet<&'a str>,
    ) -> u64 {
        if pos == design.len() {
            return 1;
        }
        if let Some(result) = self.cache[pos] {
            return result;
        }
        let mut total = 0;
        let remaining = &design[pos..];
        for &pattern in patterns {
            if pattern.len() <= remaining.len() && remaining.starts_with(pattern) {
                total += self.count_combinations(design, pos + pattern.len(), patterns);
            }
        }
        self.cache[pos] = Some(total);
        total
    }
}

fn main() {
    let input = fs::read_to_string("input.txt").unwrap();
    let mut parts = input.trim().split("\n\n");
    let patterns: HashSet<&str> = parts.next().unwrap().split(", ").collect();
    let total: u64 = parts
        .next()
        .unwrap()
        .lines()
        .map(|design| {
            let mut solver = Solver::new(design.len());
            solver.count_combinations(design, 0, &patterns)
        })
        .sum();
    println!("{}", total);
}

