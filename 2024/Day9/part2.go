package main

import (
	"bufio"
	"fmt"
	"os"
	"sort"
)

type File struct {
	ID     int
	Start  int
	Length int
}

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

	files := map[int]*File{}
	for i, v := range disk {
		if v == -1 {
			continue
		}
		if files[v] == nil {
			files[v] = &File{ID: v, Start: i, Length: 1}
		} else {
			files[v].Length++
		}
	}

	fileList := make([]*File, 0, len(files))
	for _, file := range files {
		fileList = append(fileList, file)
	}
	sort.Slice(fileList, func(i, j int) bool {
		return fileList[i].ID > fileList[j].ID
	})

	for _, file := range fileList {
		for i := 0; i <= file.Start-file.Length; i++ {
			isFree := true
			for j := 0; j < file.Length; j++ {
				if disk[i+j] != -1 {
					isFree = false
					break
				}
			}
			if isFree {
				for j := 0; j < file.Length; j++ {
					disk[i+j] = file.ID
					disk[file.Start+j] = -1
				}
				file.Start = i
				break
			}
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

