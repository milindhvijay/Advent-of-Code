package main

import (
	"bufio"
	"fmt"
	"os"
)

type pos struct { r, c int }

func main() {
    f, _ := os.Open("input.txt")
    defer f.Close()

    var grid []string
    scan := bufio.NewScanner(f)
    for scan.Scan() {
        grid = append(grid, scan.Text())
    }

    ant := make(map[rune][]pos)
    for i, row := range grid {
        for j, c := range row {
            if c != '.' {
                ant[c] = append(ant[c], pos{i, j})
            }
        }
    }

    nodes := make(map[pos]bool)
    rows, cols := len(grid), len(grid[0])

    for _, p := range ant {
        if len(p) < 2 {
            continue
        }
        for i := 0; i < len(p)-1; i++ {
            for j := i + 1; j < len(p); j++ {
                p1, p2 := p[i], p[j]
                n1 := pos{2*p1.r - p2.r, 2*p1.c - p2.c}
                n2 := pos{2*p2.r - p1.r, 2*p2.c - p1.c}

                if n1.r >= 0 && n1.r < rows && n1.c >= 0 && n1.c < cols {
                    nodes[n1] = true
                }
                if n2.r >= 0 && n2.r < rows && n2.c >= 0 && n2.c < cols {
                    nodes[n2] = true
                }
            }
        }
    }

    fmt.Println(len(nodes))
}

