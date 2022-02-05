# include <condition_variable>
# include <iostream>
# include <thread>
# include <list>
# include <functional>
# include <mutex>
# include <vector>
# include <Windows.h>
# include <string>


using namespace std;

class Workers {
    public: 
        list<function<void()>> tasks;
        mutex tasks_mutex; 
        mutex tasks_mutex2; 
        mutex finished_mutex; 
        int myNum;
        vector<thread> workerThreads;
        bool finished;
        Workers(int numWorkers){
            tasks;
            finished = false;
            tasks_mutex; 
            tasks_mutex2;
            finished_mutex; 
            for (int i = 0; i < numWorkers; i++)
            {
                workerThreads.emplace_back([&]{
                    cout << "numTasks while in thread is now: " + to_string(tasks.size()) << endl;
                    while (true) {
                        {
                            unique_lock<mutex> lock(finished_mutex);
                            if (finished)
                                {
                                    return;
                                }
                        }
                        
                        function<void()> task;
                        {
                            //unique_lock<mutex> lock(tasks_mutex2);
                            if (!tasks.empty()) {
                        cout << "Inside loop" << endl;
                                task = *tasks.begin(); 
                                try
                                {
                                    tasks.pop_front(); 
                                }
                                catch(const std::exception& e)
                                {
                                    std::cerr << e.what() << '\n';
                                }
                                
                                
                            } else{
                                cout << "tasks are empty" << endl;
                                return;
                            }
                        }
                        if (task){
                            try
                            {
                                /* code */
                                cout << "trying task" << endl;
                                task(); 
                            }
                            catch(const std::exception& e)
                            {
                                cout << "Found error" << endl;
                                std::cerr << e.what() << '\n';
                            }
                            
                        }
                    }
                });
                cout << &tasks << endl;
            }
        }

        void removeThreadsWhenFinished(){
            cout << "All are added" << endl;
            bool finished = false;
            mutex finished_mutex; 
            unique_lock<mutex> lock(tasks_mutex2);
            workerThreads.emplace_back([&finished, &finished_mutex] {
                unique_lock<mutex> lock(finished_mutex);
                finished = true;
            });
            

        }
        
        

};

int main() {
    int numWorkers = 1;
    cout << "test123" << endl;
    Workers myWorkers(numWorkers);
    cout << "finished" << endl;

   

}