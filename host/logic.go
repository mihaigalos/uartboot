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

func sendOverUart(args []string) bool {

	fileName := args[0]
	lines := linesInFile(fileName)

	if !myparser.IsFileValid(lines) {
		log.Fatalf("Failed CRC check: %s", fileName)
		return false
	}

	page := newPage()
	posInPage := 0
	pageCount := 0

	var IEEETable = crc32.MakeTable(crc32.IEEE)

	for _, line := range lines {
		n := int(myparser.NumberOfBytes(line))
		payload := myparser.Payload(line)

		for i := 0; i < n; i++ {
			page[posInPage] = payload[i]
			posInPage++

			if posInPage == kPayloadInPageSize {
				appendDestination(&page, pageCount)
				appendCRC32(&page, IEEETable)
				serializePage(page)
				page = newPage()
				posInPage = 0
				pageCount++
			}
		}
	}

	if posInPage != 0 {
		appendDestination(&page, pageCount)
		appendCRC32(&page, IEEETable)
		serializePage(page)
	}
	fmt.Printf("\n\nDone. Wrote %d pages.\n", pageCount+1)
	return true
}
