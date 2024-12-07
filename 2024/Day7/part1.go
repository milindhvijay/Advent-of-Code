package main

import (
	"bufio"
	"fmt"
	"os"
	"strconv"
	"strings"
)

type Equation struct {
	target int
	nums   []int
}

func evaluateExpression(nums []int, operators []byte) int {
	result := nums[0]
	for i, op := range operators {
		if op == '+' {
			result += nums[i+1]
		} else {
			result *= nums[i+1]
		}
	}
	return result
}

func isValidEquation(target int, nums []int) bool {
	ops := make([]byte, len(nums)-1)
	return findValidCombo(target, nums, ops, 0)
}

func findValidCombo(target int, nums []int, ops []byte, pos int) bool {
	if pos == len(ops) {
		return evaluateExpression(nums, ops) == target
	}

	ops[pos] = '+'
	if findValidCombo(target, nums, ops, pos+1) {
		return true
	}

	ops[pos] = '*'
	return findValidCombo(target, nums, ops, pos+1)
}

func parseInput(filename string) ([]Equation, error) {
	f, err := os.Open(filename)
	if err != nil {
		return nil, err
	}
	defer f.Close()

	var eqs []Equation
	scanner := bufio.NewScanner(f)
	buf := make([]byte, 64*1024)
	scanner.Buffer(buf, 64*1024)

	for scanner.Scan() {
		line := scanner.Text()
		idx := strings.IndexByte(line, ':')
		if idx == -1 {
			continue
		}

		target, err := strconv.Atoi(strings.TrimSpace(line[:idx]))
		if err != nil {
			continue
		}

		ns := strings.Fields(line[idx+1:])
		nums := make([]int, len(ns))
		for i, n := range ns {
			if nums[i], err = strconv.Atoi(n); err != nil {
				break
			}
		}
		if err != nil {
			continue
		}

		eqs = append(eqs, Equation{target: target, nums: nums})
	}

	if err := scanner.Err(); err != nil {
		return nil, err
	}

	return eqs, nil
}

func total(eqs []Equation) int {
	total := 0
	for _, eq := range eqs {
		if isValidEquation(eq.target, eq.nums) {
			total += eq.target
		}
	}
	return total
}

func main() {
	eqs, err := parseInput("input.txt")
	if err != nil {
		fmt.Fprintf(os.Stderr, "%v\n", err)
		os.Exit(1)
	}

	fmt.Println(total(eqs))
}

