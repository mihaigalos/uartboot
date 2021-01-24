package main

import (
	"hash/crc32"

	myparser "github.com/mihaigalos/go-ihex/parser"
)

type Page [kPageSize]byte

type SendHandler interface {
	send(page *Page, pageCount int, crcTable *crc32.Table)
}

type ProgressHandler interface {
	New(begin int, end int)
	Update(newProgress int)
	Finish()
}

func doSend(page *Page, pageCount int, crcTable *crc32.Table, sendHandler SendHandler, progressHandler ProgressHandler, newProgress int) {
	appendDestination(page, pageCount)
	appendCRC32(page, crcTable)
	sendHandler.send(page, pageCount, crcTable)

	progressHandler.Update(newProgress)
}

func run(sendHandler SendHandler, progressHandler ProgressHandler, args []string) int {

	hexFile := NewHexFile(args[0])
	progressHandler.New(0, myparser.TotalNumberOfBytes(hexFile))

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
				doSend(&page, pageCount, crcTable, sendHandler, progressHandler, newProgress)
				page = newPage()
				posInPage = 0
				pageCount++
			}
		}
	}

	if posInPage != 0 {
		newProgress := posInPage + pageCount*kPayloadInPageSize
		doSend(&page, pageCount, crcTable, sendHandler, progressHandler, newProgress)
		pageCount++
	}
	progressHandler.Finish()

	return pageCount
}
