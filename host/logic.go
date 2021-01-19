package main

import (
	"bufio"
	"fmt"
	"hash/crc32"
	"log"
	"os"

	myparser "github.com/mihaigalos/intel-hex-parser/parser"
)

const kPayloadInPageSize int = 128
const kDestinationSize int = 2
const kCRC32Size int = 4
const kPageSize = kPayloadInPageSize + kDestinationSize + kCRC32Size

const kOffsetDestination = kPayloadInPageSize
const kOffsetCRC32 = kPayloadInPageSize + kDestinationSize

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
func newPage() Page {
	var page Page
	for i := 0; i < kPageSize; i++ {
		page[i] = 0xFF
	}
	return page
}

func appendDestination(page *Page, pageCount int) {
	totalBytesWritten := pageCount * kPayloadInPageSize
	page[kOffsetDestination+0] = uint8(totalBytesWritten >> 8)
	page[kOffsetDestination+1] = uint8(totalBytesWritten)
}

func appendCRC32(page *Page, crcTable *crc32.Table) {

	byteArray := make([]byte, kPayloadInPageSize)
	for i := 0; i < kPayloadInPageSize; i++ {
		byteArray[i] = byte(page[i])
	}
	computedCrc32 := crc32.Checksum(byteArray, crcTable)

	page[kOffsetCRC32+3] = uint8(computedCrc32 >> 24)
	page[kOffsetCRC32+2] = uint8(computedCrc32 >> 16)
	page[kOffsetCRC32+1] = uint8(computedCrc32 >> 8)
	page[kOffsetCRC32+0] = uint8(computedCrc32)
}

func serializePage(page Page) {
	fmt.Println("\n")
	for i := 0; i < kPageSize; i++ {
		fmt.Printf("%02X", page[i])
		if (i+1)%16 == 0 && i != 0 {
			fmt.Println("")
		}
	}
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
