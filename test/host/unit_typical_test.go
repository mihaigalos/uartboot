package main

import (
	"testing"

	myparser "github.com/mihaigalos/go-ihex/parser"
)

type FakeProgressHandler struct{}

func (p FakeProgressHandler) New(int, int) {
}

func (p FakeProgressHandler) Update(int) {
}

func (p FakeProgressHandler) Finish() {
}

func TestIsFileValid_whenTypical(t *testing.T) {
	expected := true

	file := linesInFile("test/host/demo.hex")

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

func TestMetadataYamlReadWorks_whenTypical(t *testing.T) {
	expected := Metadata{
		LastFreeBytePointer:  0x00,
		BootloaderName:       "uartboot",
		ApplicationName:      "demo......",
		ApplicationTimestamp: 0x600EEEF5,
		WritingTimestamp:     0x600EEEF5,
		Length:               2,
		Crc:                  0x1234,
	}

	m := Metadata{}
	actual := m.readMetadata("test/host/demo_metadata.yaml")

	if *actual != expected {
		t.Errorf("No Match: %d != %d", actual, expected)
	}
}

func TestMetadataYamlReadFails_whenNotWhatExpected(t *testing.T) {
	expected := Metadata{
		LastFreeBytePointer:  0xFF,
		BootloaderName:       "uartboot",
		ApplicationName:      "demo......",
		ApplicationTimestamp: 0x600EEEF5,
		WritingTimestamp:     0x600EEEF5,
		Length:               2,
		Crc:                  0x1234,
	}

	m := Metadata{}
	actual := m.readMetadata("test/host/demo_metadata.yaml")

	if *actual == expected {
		t.Errorf("No Match: %d != %d", actual, expected)
	}
}
