#include "queue.h"
#include "thread_func.h"
#include <iostream>
#include <thread>


int main() {
  Queue<int> queue(2);
  
  std::thread th1 (writing_thread,&queue);
  std::thread th2 (reading_thread,&queue);
  std::thread th3 (notifyStatus, &queue);


  th1.join();
  th2.join();
  th3.join();

  return 0;
}
