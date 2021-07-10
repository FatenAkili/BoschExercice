#include "queue.h"
#include <iostream>
#include <thread>
#include <mutex>
#include <chrono>
#include <atomic>
#include <condition_variable> 
#include <unistd.h>
#include <cstdlib>
using namespace std;

#define WAIT_TIMEOUT 10

std::condition_variable_any read_cv;
std::condition_variable_any write_cv;

std::atomic<bool> rStatus (true);
std::atomic<bool> wStatus (true);

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

    usleep(1200000);
  }
}

void writing_thread(Queue<int> *queue) {
  int i_write = 1;
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
    i_write = (i_write==100)?0:i_write+1; //keep boundaries for the values we push

    usleep(1000000);
  }
}

int main() {
  Queue<int> queue(2);
  
  std::thread th1 (writing_thread,&queue);
  std::thread th2 (reading_thread,&queue);
  std::thread th3 (notifyStatus, &queue);


  th1.join();
  th2.join();
  th3.join();

  if (!rStatus || !wStatus){
    cout << "exit main with error\n";
    return -1;
  }

  return 0;
}
