package main

import (
	"fmt"
	"hash/crc32"
)

func serializePageToStdout(page *Page) {
	fmt.Println("\n")
	for i := 0; i < kPageSize; i++ {
		fmt.Printf("%02X", page[i])
		if (i+1)%16 == 0 && i != 0 {
			fmt.Println("")
		}
	}
}

func serializePageToUart(page *Page) {

}

func send(page *Page, pageCount int, crcTable *crc32.Table) {
	appendDestination(page, pageCount)
	appendCRC32(page, crcTable)
	serializePageToStdout(page)
}
