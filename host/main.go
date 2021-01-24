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

type SendHandlerStdOutImpl struct{}

func (s SendHandlerStdOutImpl) send(page *Page, pageCount int, crcTable *crc32.Table) {
	send(page, pageCount, crcTable, "serializePageToStdout")
}

type SendHandlerUsbImpl struct{}

func (s SendHandlerUsbImpl) send(page *Page, pageCount int, crcTable *crc32.Table) {
	send(page, pageCount, crcTable, "serializePageToUsb")
}

func main() {
	argsWithoutProg := os.Args[1:]
	pageCount := run(SendHandlerStdOutImpl{}, ProgressHandlerImpl{}, argsWithoutProg)
	fmt.Printf("\n\nDone. Wrote %d pages.\n", pageCount+1)
}
