package main

import (
	"fmt"
	"hash/crc32"
	"os"

	"github.com/mihaigalos/go-bar/bar"
)

var progressBar bar.Bar

type ProgressHandlerImpl struct{}

func (p ProgressHandlerImpl) New(begin int, end int) {
	progressBar.New(begin, end)
}

func (p ProgressHandlerImpl) Update(newProgress int) {
	progressBar.Update(newProgress)
}

func (p ProgressHandlerImpl) Finish() {
	progressBar.Finish()
}

type SendHandlerImpl struct{}

func (s SendHandlerImpl) send(page *Page, pageCount int, crcTable *crc32.Table) {
	send(page, pageCount, crcTable, "serializePageToStdout")
}

func main() {
	var progressHandler ProgressHandlerImpl
	var sendHandler SendHandlerImpl

	argsWithoutProg := os.Args[1:]
	pageCount := sendOverUart(sendHandler, progressHandler, argsWithoutProg)
	fmt.Printf("\n\nDone. Wrote %d pages.\n", pageCount+1)
}
