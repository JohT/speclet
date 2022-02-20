#include "SpectralDataBuffer.h"
#include "../utilities/PerformanceManager.h"

SpectralDataBuffer::SpectralDataBuffer() {
	mWriteAccess	= false;
	mItemSize		= 0;
	sizeCheckCounter = 0;
	buffer = new std::list<ItemType>();
}

SpectralDataBuffer::~SpectralDataBuffer() {
	if (buffer) delete buffer;
	buffer = 0;
}

void SpectralDataBuffer::write(ItemType item) {
//	const ScopedLock myScopedLock (criticalSection);
	mWriteAccess = true;
	PerformanceManager::getSingletonInstance()->start(T("bufferWrite"));

	//if there are too many buffer entries,
	//delete the old ones to avoid memory problems
	if (sizeCheckCounter > SIZECHECKCOUNT) {
		sizeCheckCounter = 0;
		unsigned int size = buffer->size();
		if (size > CAPACITY) {
			if (buffer) buffer->clear();
			DBG(T("SpectralDataBuffer::write: buffer had to be cleared, size was " ) + juce::String(size));
		}
	}
	sizeCheckCounter++;

	if (buffer) buffer->push_back(item);
	PerformanceManager::getSingletonInstance()->stop(T("bufferWrite"));
	mWriteAccess = false;
}

void SpectralDataBuffer::read(ItemType* pItem) {
	PerformanceManager::getSingletonInstance()->start(T("bufferRead"));

	assert(pItem);
	if (mWriteAccess) return;
	if (!buffer)		return;
	
	pItem->assign(buffer->front().begin(), buffer->front().end());
	if (!buffer->empty()) buffer->pop_front();
	//TODO faster buffer? threadsafe?

	PerformanceManager::getSingletonInstance()->stop(T("bufferRead"));
}

SpectralDataBuffer::ItemSizeType SpectralDataBuffer::size(void) {
	if (!buffer) return 0;
	return buffer->size();
}

SpectralDataBuffer::ItemSizeType SpectralDataBuffer::unread(void) {
	if (!buffer) return 0;
	return buffer->size();
}

SpectralDataBuffer::ItemStatisticsType	SpectralDataBuffer::getStatistics(ItemType* pItem) {
	assert(pItem);
	ItemStatisticsType statistics;

	double value_sum = 0.0;

	for (unsigned int i = 0; i < pItem->size(); i++) {
		float value = pItem->at(i);

		if (i==0) {
			statistics.min = value;
			statistics.max = value;
		}
		if (value > statistics.max) statistics.max = value;
		if (value < statistics.min) statistics.min = value;

		value_sum = value_sum + value;
	}
	statistics.avg = value_sum / (float)pItem->size();

	return statistics;
}