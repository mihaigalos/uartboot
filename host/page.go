package main

import (
	"fmt"
	"hash/crc32"
)

const kPayloadInPageSize int = 128
const kDestinationSize int = 2
const kCRC32Size int = 4
const kPageSize = kPayloadInPageSize + kDestinationSize + kCRC32Size

const kOffsetDestination = kPayloadInPageSize
const kOffsetCRC32 = kPayloadInPageSize + kDestinationSize

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

func serializePage(page *Page) {
	fmt.Println("\n")
	for i := 0; i < kPageSize; i++ {
		fmt.Printf("%02X", page[i])
		if (i+1)%16 == 0 && i != 0 {
			fmt.Println("")
		}
	}
}

func send(page *Page, pageCount int, crcTable *crc32.Table) {
	appendDestination(page, pageCount)
	appendCRC32(page, crcTable)
	serializePage(page)
}
