package main

import (
	"bufio"
	"fmt"
	"os"
)

func readFirstLine(filename string) string {
	file, err := os.Open(filename)
	if err != nil {
		panic(err)
	}
	defer file.Close()

	scanner := bufio.NewScanner(file)
	if !scanner.Scan() {
		panic("empty input file")
	}

	return scanner.Text()
}

func main() {
	input := readFirstLine("input.txt")
	disk := make([]int, 0, len(input)*2)
	fileID := 0

	for i, r := range input {
		n := int(r - '0')
		if n < 0 || n > 9 {
			panic(fmt.Sprintf("invalid digit at position %d", i))
		}

		val := -1
		if i%2 == 0 {
			val = fileID
			fileID++
		}

		for j := 0; j < n; j++ {
			disk = append(disk, val)
		}
	}

	for i := 0; i < len(disk); i++ {
		if disk[i] != -1 {
			continue
		}

		found := false
		for j := len(disk) - 1; j > i; j-- {
			if disk[j] >= 0 {
				disk[i] = disk[j]
				disk[j] = -1
				found = true
				break
			}
		}

		if !found {
			break
		}
	}

	sum := 0
	for i, v := range disk {
		if v >= 0 {
			sum += i * v
		}
	}

	fmt.Println(sum)
}

