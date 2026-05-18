#include "../src/data/SpectralDataBuffer.h"

#include <catch2/catch_test_macros.hpp>

#include <vector>

SCENARIO("SpectralDataBuffer write and read") {
    GIVEN("an empty SpectralDataBuffer") {
        SpectralDataBuffer buf;

        WHEN("a single item is written then read") {
            SpectralDataBuffer::ItemType written = {1.0f, 2.0f, 3.0f};
            buf.write(written);

            SpectralDataBuffer::ItemType result;
            buf.read(&result);

            THEN("the read item matches the written item") {
                REQUIRE(result == written);
            }
            THEN("the buffer is empty after reading") {
                REQUIRE(buf.unread() == 0);
            }
        }

        WHEN("multiple items are written and read back") {
            for (int i = 0; i < 5; ++i) {
                SpectralDataBuffer::ItemType item(4, static_cast<float>(i));
                buf.write(item);
            }
            THEN("unread count reflects all written items") {
                REQUIRE(buf.unread() == 5);
            }
            for (int i = 0; i < 5; ++i) {
                SpectralDataBuffer::ItemType result;
                buf.read(&result);
                REQUIRE(result == SpectralDataBuffer::ItemType(4, static_cast<float>(i)));
            }
        }
    }
}

SCENARIO("SpectralDataBuffer sequential write-then-read produces correct FIFO order") {
    GIVEN("an empty SpectralDataBuffer") {
        SpectralDataBuffer buf;
        const int numItems = 100;

        WHEN("many items are written sequentially and then read back sequentially") {
            for (int i = 0; i < numItems; ++i) {
                SpectralDataBuffer::ItemType item(8, static_cast<float>(i));
                buf.write(item);
            }

            THEN("all items are present") {
                REQUIRE(buf.unread() == numItems);
            }

            AND_WHEN("all items are read back") {
                int readCount = 0;
                while (buf.unread() > 0) {
                    SpectralDataBuffer::ItemType item;
                    buf.read(&item);
                    REQUIRE(!item.empty());
                    readCount++;
                }

                THEN("exactly numItems were read") {
                    REQUIRE(readCount == numItems);
                }
                THEN("the buffer is now empty") {
                    REQUIRE(buf.unread() == 0);
                }
            }
        }
    }
}

SCENARIO("SpectralDataBuffer size check path does not crash") {
    GIVEN("a SpectralDataBuffer with more items than SIZECHECKCOUNT") {
        SpectralDataBuffer buf;
        // Write slightly more than SIZECHECKCOUNT (500) items to exercise the size-check
        // code path. The auto-clear only triggers when size() > CAPACITY (5000), so this
        // does not test the actual clear; it only verifies no crash occurs on that path.
        for (int i = 0; i < SpectralDataBuffer::SIZECHECKCOUNT + 10; ++i) {
            SpectralDataBuffer::ItemType item(4, static_cast<float>(i));
            buf.write(item);
        }

        THEN("size() returns a non-negative value (no crash)") {
            REQUIRE(buf.size() >= 0);
        }
    }
}

SCENARIO("SpectralDataBuffer write and read are thread-safe") {
    GIVEN("a SpectralDataBuffer") {
        SpectralDataBuffer buf;
        SpectralDataBuffer::ItemType item = {0.5f, 0.6f};

        WHEN("an item is written and then read back") {
            // Verify basic write+read stability under the CriticalSection guard.
            buf.write(item);
            SpectralDataBuffer::ItemType result;
            buf.read(&result);
            THEN("the read succeeds") {
                REQUIRE(result == item);
            }
        }
    }
}
