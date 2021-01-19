package main

import (
	"bufio"
	"fmt"
	"hash/crc32"
	"log"
	"os"

	myparser "github.com/mihaigalos/intel-hex-parser/parser"
)

type Page [kPageSize]uint8
type HexFile struct {
	lines []string
}

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

func NewHexFile(fileName string) *HexFile {
	f := new(HexFile)
	f.lines = linesInFile(fileName)

	if !myparser.IsFileValid(f.lines) {
		log.Fatalf("Failed CRC check: %s", fileName)
		return nil
	}
	return f
}

func sendOverUart(args []string) bool {

	hexFile := NewHexFile(args[0])

	page := newPage()
	posInPage := 0
	pageCount := 0

	var crcTable = crc32.MakeTable(crc32.IEEE)

	for _, line := range hexFile.lines {
		n := int(myparser.NumberOfBytes(line))
		payload := myparser.Payload(line)

		for i := 0; i < n; i++ {
			page[posInPage] = payload[i]
			posInPage++

			if posInPage == kPayloadInPageSize {
				send(&page, pageCount, crcTable)
				page = newPage()
				posInPage = 0
				pageCount++
			}
		}
	}

	if posInPage != 0 {
		send(&page, pageCount, crcTable)
	}
	fmt.Printf("\n\nDone. Wrote %d pages.\n", pageCount+1)
	return true
}
