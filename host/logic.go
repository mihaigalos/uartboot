package main

import (
	"fmt"
	"hash/crc32"

	"github.com/mihaigalos/go-bar/bar"
	myparser "github.com/mihaigalos/intel-hex-parser/parser"
)

type Page [kPageSize]byte

func sendOverUart(args []string) bool {

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
				send(&page, pageCount, crcTable)
				page = newPage()
				posInPage = 0
				pageCount++
				progressBar.Update(progressBar.GetCurrent() + kPayloadInPageSize)
			}
		}
	}

	if posInPage != 0 {
		send(&page, pageCount, crcTable)
	}
	progressBar.Finish()
	fmt.Printf("\n\nDone. Wrote %d pages.\n", pageCount+1)
	return true
}
