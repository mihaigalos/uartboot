package main

import (
	"bufio"
	"fmt"
	"log"
	"os"
)

func linesInFile(fileName string) []string {
	var lines []string
	file, err := os.Open(fileName)

	if err != nil {
		log.Fatalf("Failed to open file: %s", fileName)
	}

	scanner := bufio.NewScanner(file)
	scanner.Split(bufio.ScanLines)

	for scanner.Scan() {
		lines = append(lines, scanner.Text())
	}
	file.Close()
	return lines
}

func parseFile(args []string) {

	fileName := args[0]

	for _, line := range linesInFile(fileName) {
		fmt.Println(line)
	}
}
