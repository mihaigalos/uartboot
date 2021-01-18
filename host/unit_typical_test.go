package main

import (
	"testing"

	myparser "github.com/mihaigalos/intel-hex-parser/parser"
)

func TestIsFileValid_whenTypical(t *testing.T) {
	expected := true

	file := linesInFile("demo.hex")

	actual := myparser.IsFileValid(file)

	if actual != expected {
		t.Errorf("No Match: %d != %d", actual, expected)
	}

}

func TestNewPageWorks_whenTypical(t *testing.T) {
	expected := 0xFF

	page := newPage()
	for i := 0; i < 128; i++ {
		actual := page[i]
		if actual != uint8(expected) {
			t.Errorf("No Match: %d != %d", actual, expected)
		}
	}
}
