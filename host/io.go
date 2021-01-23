package main

import (
	"fmt"
	"hash/crc32"
	"log"

	"github.com/mihaigalos/go-serial/serial"
)

const kMaxTriesWithCommunicationFailure = 3

type TECommunicationResult int

const (
	Invalid TECommunicationResult = iota
	Ok
	CRCMismatch
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

	buf := make([]byte, 1)
	for i := 0; i < kMaxTriesWithCommunicationFailure; i++ {
		_, err = port.Write(pageToByteArray(page))
		_, err = port.Read(buf)
		if TECommunicationResult(buf[0]) == Ok {
			fmt.Printf("Acknowledged.")
			break
		} else {
			fmt.Printf("Error: received 0x%X.", TECommunicationResult(buf[0]))
		}
	}

	port.Close()
}

func send(page *Page, pageCount int, crcTable *crc32.Table, serializer string) {

	if "serializePageToUart" == serializer {
		serializePageToUart(page)
	} else if "serializePageToStdout" == serializer {
		serializePageToStdout(page)
	}

	fmt.Println("")
}
