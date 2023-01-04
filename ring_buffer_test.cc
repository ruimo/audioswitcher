#include <gtest/gtest.h>
#include "firmware/ringbuffer.h"

TEST(TestRingBuffer, init_buffer) {
  RingBuffer buf;
  init_buffer(&buf);
  EXPECT_EQ(buf.start_idx,  0);
  EXPECT_EQ(buf.len,  0);
}

TEST(TestRingBuffer, add_buffer) {
  RingBuffer buf;
  init_buffer(&buf);

  add_to_buffer(&buf, 1);
  EXPECT_EQ(buf.start_idx,  0);
  EXPECT_EQ(buf.len,  1);
}

TEST(TestRingBuffer, add_buffer_full) {
  RingBuffer buf;
  init_buffer(&buf);

  for (int i = 0; i < RING_BUFFER_SIZE; ++i) {
    add_to_buffer(&buf, i);
  }
    
  EXPECT_EQ(add_to_buffer(&buf, 1), false);
  EXPECT_EQ(buf.start_idx,  0);
  EXPECT_EQ(buf.len,  RING_BUFFER_SIZE);
}

TEST(TestRingBuffer, add_buffer_rotate) {
  RingBuffer buf;
  init_buffer(&buf);

  for (int i = 0; i < RING_BUFFER_SIZE; ++i) {
    EXPECT_EQ(add_to_buffer(&buf, i), true);
  }
    
  uint8_t received = 99;

  EXPECT_EQ(remove_from_buffer(&buf, &received), true);
  EXPECT_EQ(buf.start_idx,  1);
  EXPECT_EQ(buf.len,  RING_BUFFER_SIZE - 1);
  EXPECT_EQ(received,  0);

  EXPECT_EQ(remove_from_buffer(&buf, &received), true);
  EXPECT_EQ(buf.start_idx,  2);
  EXPECT_EQ(buf.len,  RING_BUFFER_SIZE - 2);
  EXPECT_EQ(received,  1);

  EXPECT_EQ(add_to_buffer(&buf, 99), true);
  EXPECT_EQ(add_to_buffer(&buf, 100), true);

  EXPECT_EQ(add_to_buffer(&buf, 101), false);
  EXPECT_EQ(buf.start_idx,  2);
  EXPECT_EQ(buf.len,  RING_BUFFER_SIZE);
}

TEST(TestRingBuffer, remove_buffer_rotate) {
  RingBuffer buf;
  init_buffer(&buf);

  for (int i = 0; i < RING_BUFFER_SIZE; ++i) {
    EXPECT_EQ(add_to_buffer(&buf, i), true);
  }

  uint8_t received;
  for (int i = 0; i < RING_BUFFER_SIZE - 1; ++i) {
    EXPECT_EQ(remove_from_buffer(&buf, &received), true);
  }
    
  EXPECT_EQ(buf.start_idx,  RING_BUFFER_SIZE - 1);
  EXPECT_EQ(buf.len,  1);

  EXPECT_EQ(add_to_buffer(&buf, 99), true);
  EXPECT_EQ(add_to_buffer(&buf, 100), true);

  EXPECT_EQ(buf.start_idx,  RING_BUFFER_SIZE - 1);
  EXPECT_EQ(buf.len,  3);

  EXPECT_EQ(remove_from_buffer(&buf, &received), true);
  EXPECT_EQ(received, RING_BUFFER_SIZE - 1);
  EXPECT_EQ(buf.start_idx,  0);
  EXPECT_EQ(buf.len,  2);

  EXPECT_EQ(remove_from_buffer(&buf, &received), true);
  EXPECT_EQ(received, 99);
  EXPECT_EQ(buf.start_idx,  1);
  EXPECT_EQ(buf.len,  1);

  EXPECT_EQ(remove_from_buffer(&buf, &received), true);
  EXPECT_EQ(received, 100);
  EXPECT_EQ(buf.start_idx,  2);
  EXPECT_EQ(buf.len,  0);

  EXPECT_EQ(remove_from_buffer(&buf, &received), false);
  EXPECT_EQ(buf.start_idx,  2);
  EXPECT_EQ(buf.len,  0);
}
