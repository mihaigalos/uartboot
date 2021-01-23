package main

import (
	"hash/crc32"
	"testing"
)

const kBufferSize = 100_000

var sendBuffer [kBufferSize]byte
var posInBuffer int

func sendMock(page *Page, pageCount int, crcTable *crc32.Table) {
	for i := 0; i < kPageSize; i++ {
		sendBuffer[posInBuffer] = page[i]
		posInBuffer++
	}
}

type FakeProgressHandler int

func (p FakeProgressHandler) New(int, int) {
}

func (p FakeProgressHandler) Update(int) {
}

func (p FakeProgressHandler) Finish() {
}
func TestSendWorks_whenTypical(t *testing.T) {
	expected := true
	args := []string{"demo.hex"}
	var progressHandler FakeProgressHandler

	actual := sendOverUart(sendMock, progressHandler, args)

	if actual != expected {
		t.Errorf("No Match: %b != %b", actual, expected)
	}
}

func TestSendCorrect_whenTypical(t *testing.T) {
	expected := []byte{
		0x0C, 0x94, 0x34, 0x00, 0x0C, 0x94, 0x4F, 0x00, 0x0C, 0x94, 0x4F, 0x00, 0x0C, 0x94, 0x4F, 0x00,
		0x0C, 0x94, 0x4F, 0x00, 0x0C, 0x94, 0x4F, 0x00, 0x0C, 0x94, 0x4F, 0x00, 0x0C, 0x94, 0x4F, 0x00,
		0x0C, 0x94, 0x4F, 0x00, 0x0C, 0x94, 0x4F, 0x00, 0x0C, 0x94, 0x4F, 0x00, 0x0C, 0x94, 0x4F, 0x00,
		0x0C, 0x94, 0x4F, 0x00, 0x0C, 0x94, 0x4F, 0x00, 0x0C, 0x94, 0x4F, 0x00, 0x0C, 0x94, 0x4F, 0x00,
		0x0C, 0x94, 0x4F, 0x00, 0x0C, 0x94, 0x4F, 0x00, 0x0C, 0x94, 0x4F, 0x00, 0x0C, 0x94, 0x4F, 0x00,
		0x0C, 0x94, 0x4F, 0x00, 0x0C, 0x94, 0x4F, 0x00, 0x0C, 0x94, 0x4F, 0x00, 0x0C, 0x94, 0x4F, 0x00,
		0x0C, 0x94, 0x4F, 0x00, 0x0C, 0x94, 0x4F, 0x00, 0x11, 0x24, 0x1F, 0xBE, 0xCF, 0xEF, 0xD4, 0xE0,
		0xDE, 0xBF, 0xCD, 0xBF, 0x11, 0xE0, 0xA0, 0xE0, 0xB1, 0xE0, 0xE8, 0xEF, 0xF0, 0xE0, 0x02, 0xC0,
		0x00, 0x00, 0x0A, 0x6E, 0x8A, 0xAF,

		0x05, 0x90, 0x0D, 0x92, 0xA0, 0x30, 0xB1, 0x07, 0xD9, 0xF7, 0x11, 0xE0, 0xA0, 0xE0, 0xB1, 0xE0,
		0x01, 0xC0, 0x1D, 0x92, 0xA0, 0x30, 0xB1, 0x07, 0xE1, 0xF7, 0x0C, 0x94, 0x67, 0x00, 0x0C, 0x94,
		0x00, 0x00, 0x8F, 0xEF, 0x84, 0xB9, 0x87, 0xB9, 0x8E, 0xEF, 0x8A, 0xB9, 0x08, 0x95, 0x01, 0xC0,
		0x01, 0x97, 0x00, 0x97, 0x59, 0xF0, 0x20, 0xE0, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
		0x00, 0x00, 0x00, 0x00, 0x2F, 0x5F, 0x2A, 0x35, 0x99, 0xF3, 0xF6, 0xCF, 0x08, 0x95, 0x8F, 0xEF,
		0x84, 0xB9, 0x87, 0xB9, 0x8E, 0xEF, 0x8A, 0xB9, 0x8F, 0xEF, 0x88, 0xB9, 0x85, 0xB9, 0x8B, 0xB9,
		0x84, 0xEF, 0x91, 0xE0, 0x0E, 0x94, 0x57, 0x00, 0x18, 0xB8, 0x15, 0xB8, 0x1B, 0xB8, 0x84, 0xEF,
		0x91, 0xE0, 0x0E, 0x94, 0x57, 0x00, 0xF0, 0xCF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF,
		0x00, 0x80, 0x35, 0xD8, 0xED, 0xF5}
	args := []string{"demo.hex"}
	var progressHandler FakeProgressHandler

	sendOverUart(sendMock, progressHandler , args)

	for i,e := range expected{
		actual := sendBuffer[i]
		if actual != e {
			t.Errorf("Mismatch at byte %d: 0x%X != 0x%X",i, actual, e)
		}
	}
}
