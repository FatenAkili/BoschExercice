#include <gtest/gtest.h>
#include "queue.h"

TEST(queueSize, getQueueSize) {
    Queue<int> queue(4);
    ASSERT_EQ(4, queue.Size());
}
 
TEST(queueCount, getQueueCount) {
    Queue<int> queue(4);
    queue.Push(3);
    queue.Push(1);
    ASSERT_EQ(2, queue.Count());
}
 
TEST(queuePop, getQueuePopEmpty) {
    Queue<int> queue(4);
    int iRead;
    ASSERT_EQ(false, queue.Pop(&iRead));
}
 
TEST(queuePop, getQueuePop) {
    Queue<int> queue(4);
    int iRead;
    int i = 3;
    queue.Push(i);
    ASSERT_EQ(true, queue.Pop(&iRead));
    ASSERT_EQ(i, iRead);
}
 
TEST(queuePush, getQueuePush) {
    Queue<int> queue(4);
    int i = 3;
    ASSERT_EQ(true, queue.Push(i));
}
 
TEST(queuePush, getQueuePushFull) {
    Queue<int> queue(2);
    int i = 3;
    ASSERT_EQ(true, queue.Push(i));
    ASSERT_EQ(true, queue.Push(i));
    ASSERT_EQ(false, queue.Push(i));
}
 
int main(int argc, char **argv) {
    testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}