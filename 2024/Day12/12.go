package main

import (
	"fmt"
	"os"
	"strings"
)

type pair struct {
	r, c int
}

func (p pair) neighbours() [4]pair {
	return [4]pair{
		{p.r - 1, p.c},
		{p.r, p.c + 1},
		{p.r + 1, p.c},
		{p.r, p.c - 1},
	}
}

func (p pair) diags() [4]pair {
	return [4]pair{
		{p.r - 1, p.c + 1},
		{p.r + 1, p.c + 1},
		{p.r + 1, p.c - 1},
		{p.r - 1, p.c - 1},
	}
}

type garden map[pair]rune

func main() {
	bytes, _ := os.ReadFile("input.txt")
	contents := string(bytes)
	lines := strings.Split(contents, "\n")

	board := make(garden)
	for r, line := range lines {
		if line == "" {
			continue
		}
		for c, char := range line {
			board[pair{r, c}] = char
		}
	}

	plots := make(map[pair]int)
	areas := make(map[int]int)
	perimeters := make(map[int]int)
	corners := make(map[int]int)
	plot := 1
	for loc, kind := range board {
		if plots[loc] != 0 {
			continue
		}
		queue := []pair{loc}
		for len(queue) > 0 {
			current := queue[0]
			queue = queue[1:]
			if plots[current] != 0 {
				continue
			}
			plots[current] = plot
			areas[plot]++
			var diff [4]bool
			for i, n := range current.neighbours() {
				if board[n] != kind {
					perimeters[plot]++
					diff[i] = true
				} else {
					queue = append(queue, n)
				}
			}
			diags := current.diags()
			for i := 0; i < 4; i++ {
				if diff[i] && diff[(i+1)%4] {
					corners[plot]++
				}
				if !diff[i] && !diff[(i+1)%4] && board[diags[i]] != kind {
					corners[plot]++
				}
			}
		}
		plot++
	}

	var sumA, sumB int
	for k, area := range areas {
		sumA += area * perimeters[k]
		sumB += area * corners[k]
	}
	fmt.Println(sumA)
	fmt.Println(sumB)
}

