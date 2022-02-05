# include <condition_variable>
# include <iostream>
# include <thread>
# include <list>
# include <functional>
# include <mutex>
# include <vector>
# include <Windows.h>
# include <string>
# include <condition_variable>


using namespace std;

class Workers {
    public: 
        list<function<void()>> tasks;
        mutex tasks_mutex;
        mutex finished_mutex; 
        int myNum;
        vector<thread> workerThreads;
        bool finished;
        condition_variable cv;

        Workers(int numWorkers){
            tasks;
            finished = false;
            tasks_mutex;
            finished_mutex; 
            myNum = numWorkers;
            cv;
            
        }


        void start(){
            for (int i = 0; i < myNum; i++)
            {
                workerThreads.emplace_back([&] {
                    while (true) {    
                        function<void()> task;
                        {unique_lock<mutex> lock(tasks_mutex);
                            {
                                unique_lock<mutex> lock(finished_mutex);
                                if (finished)
                                    {
                                        return;
                                    }
                            }
                            if (!tasks.empty()) {
                                
                                task = *tasks.begin(); 
                                tasks.pop_front();
                            } else{
                                cv.wait(lock);
                            }
                        }
                        if (task){
                            task(); 
                        }
                    }
                });
                
                
            }
        }
        void post(function<void()> task){
            unique_lock<mutex> lock(tasks_mutex);
            tasks.emplace_back(task);
            cv.notify_one();
        }

        void post_timeout(function<void()> task, int miliseconds){
            unique_lock<mutex> lock(tasks_mutex);
            tasks.emplace_back([&]{
                Sleep(miliseconds);
                task();
            });
            cv.notify_one();
        }

        void join(){
            
            {
                unique_lock<mutex> lock(tasks_mutex);
                tasks.emplace_back([&] {
                    unique_lock<mutex> lock(tasks_mutex);
                    unique_lock<mutex> lock2(finished_mutex);
                    finished = true;
                    cv.notify_all();
                });
            }
            cv.notify_one();
            for(auto &t: workerThreads){
                t.join();
            }

        }

        void stop(){
            {
                unique_lock<mutex> lock(finished_mutex);
                finished = true;

                cv.notify_all();
            }
            for(auto &t: workerThreads){
                t.join();
            }
        }

        

};

int main() {
    Workers worker_threads(4);
    Workers event_loop(1);
    
    worker_threads.start();
    event_loop.start();

    worker_threads.post_timeout([]{
        cout << "1. task of workerThread, with 5 second wait " << endl;
    }, 5000);

    worker_threads.post([]{
        cout << "2. task of workerThread " << endl;
    });

    worker_threads.post([]{
        cout << "3. task of workerThread " << endl;
    });

    event_loop.post_timeout([]{
        cout << "1. task of event_loop, with 5 second wait " << endl;
    }, 5000);

    event_loop.post([]{
        cout << "2. task of event_loop " << endl;
    });

    event_loop.post([]{
        cout << "3. task of event_loop " << endl;
    });

    worker_threads.join();
    event_loop.join(); 

    cout << "All tasks finished and run" << endl;

}