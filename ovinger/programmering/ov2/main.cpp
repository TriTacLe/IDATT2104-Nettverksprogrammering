#include <condition_variable>
#include <iostream>
#include <thread>
#include <mutex>
#include <list>
#include <vector>
#include <functional>
#include <chrono>
#include <unistd.h>

using namespace std;

// Thread pool implementation
class Workers {
private:
    int n_threads;
    vector<thread> threads;
    condition_variable cv;
    mutex mtx;
    list<function<void()>> tasks;
    bool post_flag;

public:
    Workers(int n){
      n_threads = n;
      post_flag = false;
    }

    // Start thread in tasks
    void start() {
      for (int i = 0; i < n_threads; i++) {
        // lager ny thread direkte i threads vector
        threads.emplace_back([this]() {
          // For every thread
          while (true) {
            function<void()> task;
              {
                unique_lock<mutex> lock(mtx);
                cv.wait(lock, [this]() { return !tasks.empty() || post_flag;
              });
              
                // Check if tasks er tom eller om stop er kalt
              if (tasks.empty() && post_flag) {
                return; 
              }

                task = tasks.front();
                tasks.pop_front();
              }
            task(); // outside of lock
            }
        });
      }
    }
    
    // ADding task to the task list (tasks) 
    void post(function<void()> task) {
      {
        unique_lock<mutex> lock(mtx);
        tasks.push_back(task);
      }
      cv.notify_one(); // En task tilgjengelig og vekker bare en 
    }

    void stop() {
      {
        unique_lock<mutex> lock(mtx);
        post_flag = true;
      }
      cv.notify_all();
   }

    void join() {
      for (auto& thread : threads) {
        thread.join();
      }
    }

    void post_timeout(function<void()> task, int ms) {
      thread([this,task,ms](){
        this_thread::sleep_for(chrono::milliseconds(ms));
        post(task);  
        }).detach();
    };
};

int main() {
    Workers worker_threads(4);
    Workers event_loop(1);

    worker_threads.start();
    event_loop.start();

    worker_threads.post([] {
        cout << "Task A" << endl;
    });

    worker_threads.post([] {
        cout << "Task B" << endl;
    });

    event_loop.post([] {
        cout << "Task C" << endl;
    });

    event_loop.post([] {
        cout << "Task D" << endl;
    });


    worker_threads.post_timeout([] {
      cout << "Task E with timeout" << endl;
    }, 1000);

    sleep(2);
    worker_threads.stop();
    event_loop.stop();

    worker_threads.join();
    event_loop.join();

    return 0;
}

