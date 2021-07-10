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

template <typename T>
int Queue<T>::Size() {
  return queueSize;
}

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
