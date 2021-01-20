package main

import (
	"fmt"
	"hash/crc32"
	"log"

	"github.com/mihaigalos/go-serial/serial"
)

func serializePageToStdout(page *Page) {
	fmt.Println("\n")
	for i := 0; i < kPageSize; i++ {
		fmt.Printf("%02X", page[i])
		if (i+1)%16 == 0 && i != 0 {
			fmt.Println("")
		}
	}
}

func serializePageToUart(page *Page) {
	options := serial.OpenOptions{
		PortName:        "/dev/ttyUSB0",
		BaudRate:        38400,
		DataBits:        8,
		StopBits:        1,
		MinimumReadSize: 1,
	}

	port, err := serial.Open(options)
	if err != nil {
		log.Fatalf("serial.Open: %v", err)
	}

	_, err = port.Write(pageToByteArray(page))

	port.Close()
}

func send(page *Page, pageCount int, crcTable *crc32.Table) {
	appendDestination(page, pageCount)
	appendCRC32(page, crcTable)
	serializePageToUart(page)
}
