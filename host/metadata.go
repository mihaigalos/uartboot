package main

import (
	"io/ioutil"
	"log"

	"github.com/go-yaml/yaml"
)

const kMetadataSize int = 32

type Metadata struct {
	LastFreeBytePointer  uint16 `yaml:"lastFreeBytePointer"`
	BootloaderName       string `yaml:"bootloaderName"`
	ApplicationName      string `yaml:"applicationName"`
	ApplicationTimestamp uint32 `yaml:"applicationTimestamp"`
	WritingTimestamp     uint32 `yaml:"writingTimestamp"`
	Length               uint16 `yaml:"length"`
	Crc                  uint16 `yaml:"crc"`
}

func (m *Metadata) ReadMetadata(yamlIn string) *Metadata {
	yamlFile, err := ioutil.ReadFile(yamlIn)
	if err != nil {
		log.Printf("yamlFile.Get err   #%v ", err)
	}
	err = yaml.Unmarshal(yamlFile, m)
	if err != nil {
		log.Fatalf("Unmarshal: %v", err)
	}

	return m
}

func (m *Metadata) toByteArray() [32]byte {
	var result [kMetadataSize]byte
	pos := 0

	result[pos] = uint8(m.LastFreeBytePointer)
	pos++
	result[pos] = uint8(m.LastFreeBytePointer >> 8)
	pos++

	for i := 0; i < 8; i++ {
		result[pos] = m.BootloaderName[i]
		pos++
	}

	for i := 0; i < 10; i++ {
		result[pos] = m.ApplicationName[i]
		pos++
	}

	result[pos] = uint8(m.ApplicationTimestamp)
	pos++
	result[pos] = uint8(m.ApplicationTimestamp >> 8)
	pos++
	result[pos] = uint8(m.ApplicationTimestamp >> 16)
	pos++
	result[pos] = uint8(m.ApplicationTimestamp >> 24)
	pos++

	result[pos] = uint8(m.WritingTimestamp)
	pos++
	result[pos] = uint8(m.WritingTimestamp >> 8)
	pos++
	result[pos] = uint8(m.WritingTimestamp >> 16)
	pos++
	result[pos] = uint8(m.WritingTimestamp >> 24)
	pos++

	result[pos] = uint8(m.WritingTimestamp)
	pos++
	result[pos] = uint8(m.WritingTimestamp >> 8)
	pos++

	result[pos] = uint8(m.Crc)
	pos++
	result[pos] = uint8(m.Crc >> 8)
	pos++

	return result
}
