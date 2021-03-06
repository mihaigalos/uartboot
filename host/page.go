package main

const kPayloadInPageSize int = 128
const kDestinationSize int = 2
const kCRC32Size int = 2
const kPageSize = kPayloadInPageSize + kDestinationSize + kCRC32Size

const kOffsetDestination = kPayloadInPageSize
const kOffsetCRC32 = kPayloadInPageSize + kDestinationSize

func newPage() Page {
	var page Page
	for i := 0; i < kPageSize; i++ {
		page[i] = 0xFF
	}
	return page
}

func appendDestination(page *Page, pageCount int) {
	totalBytesWritten := pageCount * kPayloadInPageSize
	page[kOffsetDestination+0] = uint8(totalBytesWritten)
	page[kOffsetDestination+1] = uint8(totalBytesWritten >> 8)
}

func pageToByteArray(page *Page) []byte {
	byteArray := make([]byte, kPageSize)
	for i := 0; i < kPageSize; i++ {
		byteArray[i] = byte(page[i])
	}
	return byteArray
}

func appendCRC32(page *Page) {
	computedCrc32 := uint16(0)
	for i := 0; i < kPayloadInPageSize+kDestinationSize; i++ {
		computedCrc32 += uint16(page[i])
	}

	page[kOffsetCRC32+0] = uint8(computedCrc32)
	page[kOffsetCRC32+1] = uint8(computedCrc32 >> 8)
}
