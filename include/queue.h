#ifndef QUEUE_H_
#define QUEUE_H_

#include <mutex>
#include <cstring>

template <typename T>
class Queue {
 public:
  Queue(int size);
  ~Queue();

  bool Push(T element);
  bool Pop(T* element) ;
  int Count();
  int Size();

private:
  T *buffer;
  int queueSize;
  int popIdx;
  int pushIdx;
  bool full;
  std::mutex mtx;
};

template <typename T>
Queue<T>::Queue(int size) {
    buffer = new T[size];
    queueSize = size;
    popIdx = 0;
    pushIdx = 0;
    full = false;
}

template <typename T>
Queue<T>::~Queue() {
  delete []buffer;
}

/**
 * The Push method adds a new element to the Queue (if not full)
 **/
template <typename T>
bool Queue<T>::Push(T element) {
  mtx.lock();
  bool b_ret = false;
  if (!full) {
    buffer[pushIdx] = element;
    pushIdx = (pushIdx + 1) % queueSize;
    full = (pushIdx == popIdx);
    b_ret = true;
  }
  mtx.unlock();
  return b_ret;
}

/**
 *  The Pop method gets/removes an element from the Queue (if not empty)
 **/
template <typename T>
bool Queue<T>::Pop(T* element) {
  mtx.lock();
  bool b_ret = false;

  if (Count() > 0) {
    *element = buffer[popIdx];
    popIdx = (popIdx + 1) % queueSize;
    full = false;
    b_ret = true;
  }

  mtx.unlock();

  return b_ret;
}

/**
 * Size method returns the total size of the Queue
 **/
template <typename T>
int Queue<T>::Size() {
  return queueSize;
}

/**
 * Count method returns the number of elements in the Queue
 **/
template <typename T>
int Queue<T>::Count() {
  int count = queueSize;
  if(pushIdx == popIdx && !full)
    count = 0;
  else if (pushIdx > popIdx)
    count = pushIdx - popIdx;
  else if (pushIdx < popIdx)
    count = queueSize + pushIdx - popIdx;
  return count;
}

#endif /* QUEUE_H_ */
