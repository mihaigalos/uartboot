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

func (m *Metadata) readMetadata(yamlIn string) *Metadata {
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
