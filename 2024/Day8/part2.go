package main

import (
	"bufio"
	"fmt"
	"os"
)

type pos struct { r, c int }
type line struct { a, b, c int }

func gcd(x, y int) int {
    x, y = abs(x), abs(y)
    for y != 0 {
        x, y = y, x%y
    }
    return x
}

func abs(x int) int {
    if x < 0 {
        return -x
    }
    return x
}

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

    points := make(map[pos]bool)
    rows, cols := len(grid), len(grid[0])

    for _, p := range ant {
        if len(p) < 2 {
            continue
        }

        seen := make(map[line]bool)
        for i := 0; i < len(p)-1; i++ {
            for j := i + 1; j < len(p); j++ {
                p1, p2 := p[i], p[j]

                dy := p2.r - p1.r
                dx := p2.c - p1.c
                if dx == 0 && dy == 0 {
                    continue
                }

                a, b := dy, -dx
                c := dx*p1.r - dy*p1.c

                d := c
                if d == 0 {
                    d = gcd(abs(a), abs(b))
                } else {
                    d = gcd(gcd(abs(a), abs(b)), abs(c))
                }

                if d != 0 {
                    a, b, c = a/d, b/d, c/d
                }

                if a < 0 || (a == 0 && b < 0) {
                    a, b, c = -a, -b, -c
                }

                seen[line{a,b,c}] = true
            }
        }

        for l := range seen {
            if l.b != 0 {
                for x := 0; x < cols; x++ {
                    n := -l.a*x - l.c
                    if n%l.b == 0 {
                        y := n/l.b
                        if y >= 0 && y < rows {
                            points[pos{y,x}] = true
                        }
                    }
                }
            } else if l.a != 0 && -l.c%l.a == 0 {
                x := -l.c/l.a
                if x >= 0 && x < cols {
                    for y := 0; y < rows; y++ {
                        points[pos{y,x}] = true
                    }
                }
            }
        }
    }

    fmt.Println(len(points))
}

