package main

import (
	"fmt"
	"os"

	"github.com/mihaigalos/go-bar/bar"
)

var progressBar bar.Bar

type ProgressHandlerImpl int

func (p ProgressHandlerImpl) New(begin int, end int) {
	progressBar.New(begin, end)
}

func (p ProgressHandlerImpl) Update(newProgress int) {
	progressBar.Update(newProgress)
}

func (p ProgressHandlerImpl) Finish() {
	progressBar.Finish()
}

func main() {
	var progressHandler ProgressHandlerImpl
	argsWithoutProg := os.Args[1:]
	pageCount := sendOverUart(send, progressHandler, argsWithoutProg)
	fmt.Printf("\n\nDone. Wrote %d pages.\n", pageCount+1)
}
