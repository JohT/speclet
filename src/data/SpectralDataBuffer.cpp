#include "SpectralDataBuffer.h"
#include "../utilities/PerformanceManager.h"

SpectralDataBuffer::SpectralDataBuffer()
    : buffer(new std::list<ItemType>()), mItemSize(0), mWriteAccess(false), sizeCheckCounter(0) {
}

SpectralDataBuffer::~SpectralDataBuffer() {
    delete buffer;
    buffer = nullptr;
}

void SpectralDataBuffer::write(const ItemType &item) {
    //	const ScopedLock myScopedLock (criticalSection);
    mWriteAccess = true;
    PerformanceManager::getSingletonInstance().start("bufferWrite");

    //if there are too many buffer entries,
    //delete the old ones to avoid memory problems
    if (sizeCheckCounter > SIZECHECKCOUNT) {
        sizeCheckCounter = 0;
        auto size = buffer->size();
        if (size > CAPACITY) {
            if (buffer != nullptr) {
                buffer->clear();
            }
            DBG("SpectralDataBuffer::write: buffer had to be cleared, size was " + juce::String(size));
        }
    }
    sizeCheckCounter++;

    if (buffer != nullptr) {
        buffer->push_back(item);
    }
    PerformanceManager::getSingletonInstance().stop("bufferWrite");
    mWriteAccess = false;
}

void SpectralDataBuffer::read(ItemType *pItem) {
    PerformanceManager::getSingletonInstance().start("bufferRead");

    assert(pItem);
    if (mWriteAccess) {
        return;
    }
    if (buffer == nullptr) {
        return;
    }

    pItem->assign(buffer->front().begin(), buffer->front().end());
    if (!buffer->empty()) {
        buffer->pop_front();
    }
    //TODO faster buffer? threadsafe?

    PerformanceManager::getSingletonInstance().stop("bufferRead");
}

auto SpectralDataBuffer::size() -> SpectralDataBuffer::ItemSizeType {
    if (buffer == nullptr) {
        return 0;
    }
    return buffer->size();
}

auto SpectralDataBuffer::unread() -> SpectralDataBuffer::ItemSizeType {
    if (buffer == nullptr) {
        return 0;
    }
    return buffer->size();
}

auto SpectralDataBuffer::getStatistics(ItemType *pItem) -> SpectralDataBuffer::ItemStatisticsType {
    assert(pItem != nullptr);
    ItemStatisticsType statistics;

    double valueSum = 0.0;

    for (unsigned int i = 0; i < pItem->size(); i++) {
        float value = pItem->at(i);

        if (i == 0) {
            statistics.min = value;
            statistics.max = value;
        }
        if (value > statistics.max) {
            statistics.max = value;
        }
        if (value < statistics.min) {
            statistics.min = value;
        }

        valueSum = valueSum + value;
    }
    statistics.avg = static_cast<float>(valueSum / static_cast<double>(pItem->size()));

    return statistics;
}