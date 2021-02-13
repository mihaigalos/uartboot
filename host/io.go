package main

import (
	"fmt"
	"io"
	"log"
	"time"

	"github.com/mihaigalos/go-serial/serial"
)

const kMaxTriesWithCommunicationFailure = 3
const kWaitAfterWritePageMs = 10

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

func getUsbPort() io.ReadWriteCloser {
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

	return port
}

func serializePageToUsb(page *Page) {

	port := getUsbPort()

	buf := make([]byte, 1)
	byteArray := pageToByteArray(page)
	for i := 0; i < kMaxTriesWithCommunicationFailure; i++ {
		_, err := port.Write(byteArray)
		time.Sleep(kWaitAfterWritePageMs * time.Millisecond)
		_, err = port.Read(buf)
		if TECommunicationResult(buf[0]) == Ok {
			fmt.Printf("Acknowledged.")
			break
		} else {
			fmt.Printf("\n[%d] Error: received 0x%02X, expected 0x%02X (TECommunicationResult::Ok).", i, TECommunicationResult(buf[0]), Ok)
		}
	}

	port.Close()
}

func send(page *Page, pageCount int, serializer string) {

	if "serializePageToUsb" == serializer {
		serializePageToUsb(page)
	} else if "serializePageToStdout" == serializer {
		serializePageToStdout(page)
	}

	fmt.Println("")
}

func serializeMetadataToStdout(metadata *Metadata) {
	fmt.Println("\n")
	byteArray := metadata.toByteArray()
	for i := 0; i < kMetadataSize; i++ {
		fmt.Printf("%02X", byteArray[i])
		if (i+1)%16 == 0 && i != 0 {
			fmt.Println("")
		}
	}
}

func serializeMetadataToUsb(metadata *Metadata) {
	port := getUsbPort()
	byteArray := metadata.toByteArray()
	for i := 0; i < kMaxTriesWithCommunicationFailure; i++ {
		_, err := port.Write(byteArray)
		time.Sleep(kWaitAfterWritePageMs * time.Millisecond)
		_, err = port.Read(buf)
		if TECommunicationResult(buf[0]) == Ok {
			fmt.Printf("Acknowledged.")
			break
		} else {
			fmt.Printf("\n[%d] Error: received 0x%02X, expected 0x%02X (TECommunicationResult::Ok).", i, TECommunicationResult(buf[0]), Ok)
		}
	}
}

func sendMetadata(metadata *Metadata, serializer string) {
	if "serializeMetadataToStdout" == serializer {
		serializeMetadataToStdout(metadata)
	} else if "serializeMetadataToUsb" == serialized {
		serializeMetadataToUsb(metadata)
	}

	fmt.Println("")
}
