use std::collections::HashSet;
use std::fs;

struct Solver {
    cache: Vec<Option<bool>>,
}

impl Solver {
    fn new(design_len: usize) -> Self {
        Self {
            cache: vec![None; design_len + 1],
        }
    }

    fn is_possible<'a>(
        &mut self,
        design: &'a str,
        pos: usize,
        patterns: &HashSet<&'a str>,
    ) -> bool {
        if pos == design.len() {
            return true;
        }

        if let Some(result) = self.cache[pos] {
            return result;
        }

        let remaining = &design[pos..];
        let result = patterns.iter().any(|&pattern| {
            pattern.len() <= remaining.len()
                && remaining.starts_with(pattern)
                && self.is_possible(design, pos + pattern.len(), patterns)
        });

        self.cache[pos] = Some(result);
        result
    }
}

fn main() {
    let input = fs::read_to_string("input.txt").unwrap();
    let mut parts = input.trim().split("\n\n");
    let patterns: HashSet<&str> = parts.next().unwrap().split(", ").collect();
    let possible_count = parts
        .next()
        .unwrap()
        .lines()
        .filter(|&design| {
            let mut solver = Solver::new(design.len());
            solver.is_possible(design, 0, &patterns)
        })
        .count();
    println!("{}", possible_count);
}

