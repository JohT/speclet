#include "SpectralDataBuffer.h"
#include "../utilities/PerformanceLogger.h"
#include "juce_core/juce_core.h"
#include <assert.h>

SpectralDataBuffer::SpectralDataBuffer()
    : buffer(new std::list<ItemType>()), mWriteAccess(false), sizeCheckCounter(0) {
}

SpectralDataBuffer::~SpectralDataBuffer() {
    delete buffer;
    buffer = nullptr;
}

void SpectralDataBuffer::write(const ItemType &item) {
    //	const ScopedLock myScopedLock (criticalSection);
    LOG_PERFORMANCE_OF_SCOPE("SpectralDataBuffer write");
    mWriteAccess = true;

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
    mWriteAccess = false;
}

void SpectralDataBuffer::read(ItemType *pItem) {
    LOG_PERFORMANCE_OF_SCOPE("SpectralDataBuffer read");

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
    //TODO(JohT) faster buffer? threadsafe? use juce framework solution?
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

SpectralDataBuffer::ItemStatisticsType::ItemStatisticsType(ItemType &item) {
    if (item.empty()) {
        return;
    }
    min = item.front();
    max = item.front();
    double sum = 0.0;

    for (ValueType &value : item) {
        if (value < min) {
            min = value;
        }
        if (value > max) {
            max = value;
        }
        sum = sum + value;
    }
    avg = static_cast<ValueType>(sum / static_cast<double>(item.size()));
}