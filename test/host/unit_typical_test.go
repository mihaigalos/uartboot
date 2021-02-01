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
		LastFreeBytePointer:  0x1234,
		BootloaderName:       "uartboot",
		ApplicationName:      "demo......",
		ApplicationTimestamp: 0x600EEEF5,
		WritingTimestamp:     0x600EEEF5,
		Length:               2,
		Crc:                  0x1234,
	}
	m := Metadata{}

	actual := m.ReadMetadata("test/host/demo_metadata.yaml")

	if *actual != expected {
		t.Errorf("No Match: %d != %d", actual, expected)
	}
}

func TestMetadataYamlReadFails_whenNotWhatExpected(t *testing.T) {
	expected := Metadata{
		LastFreeBytePointer:  0xFFFF,
		BootloaderName:       "uartboot",
		ApplicationName:      "demo......",
		ApplicationTimestamp: 0x600EEEF5,
		WritingTimestamp:     0x600EEEF5,
		Length:               2,
		Crc:                  0x1234,
	}
	m := Metadata{}

	actual := m.ReadMetadata("test/host/demo_metadata.yaml")

	if *actual == expected {
		t.Errorf("No Match: %d != %d", actual, expected)
	}
}

func TestMetadataSerializeWorks_whenTypical(t *testing.T) {
	input := Metadata{
		LastFreeBytePointer:  0x1234,
		BootloaderName:       "uartboot",
		ApplicationName:      "demo......",
		ApplicationTimestamp: 0x600EEEF5,
		WritingTimestamp:     0x600EEEF5,
		Length:               2,
		Crc:                  0x7890,
	}
	expected := [32]byte{0x34, 0x12,
		0x75, 0x61, 0x72, 0x74, 0x62, 0x6F, 0x6F, 0x74,
		0x64, 0x65, 0x6D, 0x6F, 0x2E, 0x2E, 0x2E, 0x2E, 0x2E, 0x2E,
		0xF5, 0xEE, 0x0E, 0x60,
		0xF5, 0xEE, 0x0E, 0x60,
		0xF5, 0xEE,
		0x90, 0x78}

	actual := input.toByteArray()

	for i, e := range actual {
		if e != expected[i] {
			t.Errorf("No Match: %X != %X", e, expected[i])
		}
	}

}

func TestMetadataSerializeFails_whenCRCIncorrectlyDecoded(t *testing.T) {
	input := Metadata{
		LastFreeBytePointer:  0x1234,
		BootloaderName:       "uartboot",
		ApplicationName:      "demo......",
		ApplicationTimestamp: 0x600EEEF5,
		WritingTimestamp:     0x600EEEF5,
		Length:               2,
		Crc:                  0xFFFF,
	}
	expected := [32]byte{0x12, 0x34,
		0x75, 0x61, 0x72, 0x74, 0x62, 0x6F, 0x6F, 0x74,
		0x64, 0x65, 0x6D, 0x6F, 0x2E, 0x2E, 0x2E, 0x2E, 0x2E, 0x2E,
		0x60, 0x0E, 0xEE, 0xF5,
		0x60, 0x0E, 0xEE, 0xF5,
		0xEE, 0xF5,
		0x78, 0x90}

	actual := input.toByteArray()

	for i := kMetadataSize - 2; i < kMetadataSize; i++ {
		if actual[i] == expected[i] {
			t.Errorf("No Match: %X != %X", actual[i], expected[i])
		}
	}

}
