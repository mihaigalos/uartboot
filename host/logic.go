package main

import (
	"fmt"
	"hash/crc32"

	"github.com/mihaigalos/go-bar/bar"
	myparser "github.com/mihaigalos/go-ihex/parser"
)

type Page [kPageSize]byte

type SendHandler func(page *Page, pageCount int, crcTable *crc32.Table)

func doSend(page *Page, pageCount int, crcTable *crc32.Table, sendHandler SendHandler, progressBar *bar.Bar, newProgress int) {
	appendDestination(page, pageCount)
	appendCRC32(page, crcTable)
	sendHandler(page, pageCount, crcTable)

	progressBar.Update(newProgress)
}

func sendOverUart(sendHandler SendHandler, args []string) bool {

	hexFile := NewHexFile(args[0])
	var progressBar bar.Bar
	progressBar.New(0, myparser.TotalNumberOfBytes(hexFile))

	page := newPage()
	posInPage := 0
	pageCount := 0

	var crcTable = crc32.MakeTable(crc32.IEEE)

	for _, line := range hexFile {
		n := int(myparser.NumberOfBytes(line))
		payload := myparser.Payload(line)

		for i := 0; i < n; i++ {
			page[posInPage] = payload[i]
			posInPage++

			if posInPage == kPayloadInPageSize {
				newProgress := posInPage + pageCount*kPayloadInPageSize
				doSend(&page, pageCount, crcTable, sendHandler, &progressBar, newProgress)
				page = newPage()
				posInPage = 0
				pageCount++
			}
		}
	}

	if posInPage != 0 {
		newProgress := posInPage + pageCount*kPayloadInPageSize
		doSend(&page, pageCount, crcTable, sendHandler, &progressBar, newProgress)
	}
	progressBar.Finish()
	fmt.Printf("\n\nDone. Wrote %d pages.\n", pageCount+1)
	return true
}
