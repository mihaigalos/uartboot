package main

import (
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
	sendOverUart(send, progressHandler, argsWithoutProg)
}
