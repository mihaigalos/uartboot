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
