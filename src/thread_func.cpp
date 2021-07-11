#include "queue.h"
#include "thread_func.h"
#include <iostream>
#include <thread>
#include <mutex>
#include <chrono>
#include <atomic>
#include <condition_variable> 
#include <unistd.h>
#include <cstdlib>
using namespace std;


std::condition_variable_any read_cv;
std::condition_variable_any write_cv;

std::atomic<bool> rStatus (true);
std::atomic<bool> wStatus (true);

/**
 * This thread notifies the other threads about the status of the Queue (if full or empty)
 **/
void notifyStatus(Queue<int> *queue) {
  while(rStatus || wStatus) {
    int count = queue->Count();
    if(count > 0) {
      read_cv.notify_all();
    }
    if(count < queue->Size()) {
      write_cv.notify_all();
    }
  }
}

/**
 * This thread reads elements from the Queue.
 * If the quue is empty it waits for a new element to be added or times out
 **/
void reading_thread(Queue<int> *queue) {
  int i_read = 0;
  std::mutex readMtx;
  cout << "Reading thread\n";
  while (1)
  {
    std::unique_lock<std::mutex> lck(readMtx);
    
    if (read_cv.wait_for(lck,std::chrono::seconds(WAIT_TIMEOUT))==std::cv_status::timeout) {
      cout << "Reading timeout\n";
      rStatus = false;
      return;
    }
    cout << "Read Count " << queue->Count() << endl;
    if (!queue->Pop(&i_read)){
      rStatus = false;
      return;
    }
    write_cv.notify_all();
    cout << " ==> The popped element is " << i_read << endl;

    usleep(READ_THREAD_SLEEP);
  }
}

/**
 * This thread writes elements to the Queue.
 * If the quue is full it waits for a new element to be read/deleted or times out
 **/
void writing_thread(Queue<int> *queue) {
  int i_write = MIN_QUEUE_VALUE;
  std::mutex writeMtx;
  cout << "writing thread\n";
  while (1)
  {
    std::unique_lock<std::mutex> lck(writeMtx);
    
    if (write_cv.wait_for(lck,std::chrono::seconds(WAIT_TIMEOUT))==std::cv_status::timeout) {
      cout << "Writing timeout\n";
      wStatus = false;
      return;
    }
    cout << "Write Count " << queue->Count() << endl;
      
    if (!queue->Push(i_write)){
      wStatus = false;
      return;
    }
    read_cv.notify_all();
    cout << " ==> The pushed element is "  << i_write << endl;
    i_write = (i_write==MAX_QUEUE_VALUE)?0:i_write+1; //keep boundaries for the values we push

    usleep(WRITE_THREAD_SLEEP);
  }
}
