package main

import (
	"fmt"
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

func (s SendHandlerStdOutImpl) send(page *Page, pageCount int) {
	send(page, pageCount, "serializePageToStdout")
}

func (s SendHandlerStdOutImpl) sendMetadata(metadata *Metadata) {
	sendMetadata(metadata, "serializeMetadataToStdout")
}

type SendHandlerUsbImpl struct{}

func (s SendHandlerUsbImpl) send(page *Page, pageCount int) {
	send(page, pageCount, "serializePageToUsb")
}

func (s SendHandlerUsbImpl) sendMetadata(metadata *Metadata) {
	sendMetadata(metadata, "serializeMetadataToUsb")
}

func main() {
	argsWithoutProg := os.Args[1:]
	pageCount := run(SendHandlerUsbImpl{}, ProgressHandlerImpl{}, argsWithoutProg)
	fmt.Printf("\n\nDone. Wrote %d pages.\n", pageCount+1)
}
