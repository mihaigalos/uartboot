package main

import (
	"bufio"
	"log"
	"os"

	myparser "github.com/mihaigalos/go-ihex/parser"
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

func NewHexFile(fileName string) []string {
	var lines []string
	lines = linesInFile(fileName)

	if !myparser.IsFileValid(lines) {
		log.Fatalf("Failed CRC check: %s", fileName)
		return nil
	}
	return lines
}
