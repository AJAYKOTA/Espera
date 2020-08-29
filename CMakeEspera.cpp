// CMakeEspera.cpp : Defines the entry point for the application.
//

// Producer will produce and cousumer will cousume with synchronization of a buffer and buffer size defineas 100.
// Until  Producer thread produces any data consumer thread can't comsume.
// Threads will use condition_variable to notify each other.
// we need mutex if we use condition_variable beacaue CV waits on mutex.
// it is one of way to slove producer consumer problem .

// packages and packages dimensions , i have taken as one variable.

// if package dimension between 20 to 60 as value , Package is labeled, otherwise it is discarded.



#include "CMakeEspera.h"
#include <cstdio> 
#include <cstdlib>
#include <ctime>
#include <thread>
#include <mutex>
#include <queue>
#include <chrono>
#include <iostream>
#include <condition_variable>

using namespace std;

//define mutex
std::mutex mu;
// Variables that are shared between producer and consumer.
std::condition_variable cond;
//buffer defination.
deque<int> buffer;
const unsigned int maxBufferSize = 100;
bool running;

int generateNumber() {
    int result = rand() % 100 + 1;
    return result;
}

void ProducerThread(int packagesizedata) {
    while (packagesizedata) {
        // value and counter must both be updated atomically
        // using a mutex lock
        std::unique_lock<std::mutex> locker(mu);
       // check if buffer id emptyand if it is, then unlock the mutexand sleep, if not thean go aheadand consume.
        cond.wait(locker, []() {return buffer.size() < maxBufferSize; });
        buffer.push_back(packagesizedata);
        //pushing value into the buffer.
        cout << "Produced:" << packagesizedata << endl;
        packagesizedata--;
        //unlock mutex.
        locker.unlock();
        // Notify the consumer that a new value is ready.
        cond.notify_one();
        //Simulate exaggerated 10ms delay
        this_thread::sleep_for(chrono::milliseconds(1000));
    }
}

void ConsumerThread() {

    while (running) {
        //lock mutex, if success then go ahead otherwise wait for mutex to get free.
        std::unique_lock<mutex> locker(mu);
        //check if buffer is full and if it is, then unlock the mutex and sleep, if not thean go ahead and produce.
        cond.wait(locker, []() { return buffer.size() > 0;});
        //consume item from buffer.
        int packagesize = buffer.back();
        buffer.pop_back();
        // current date/time based on current system
        time_t now = time(0);
        // convert now to string form
        char* dt = ctime(&now);
        cout << "timepoint is: " << dt << endl;
        if (packagesize >= 20 && packagesize <= 60) {
            cout << "---------labeled--------------  Consumer packagesize : " << packagesize << endl;
        }
        else {
            cout << "---------discarded-------------- Consumer packagesize : " << packagesize << endl;
        }
        //unlock mutex.
        locker.unlock();
        //notify producer
        cond.notify_one();
        //Simulate exaggerated 10ms delay
        this_thread::sleep_for(chrono::milliseconds(1000));
    }

}

int main()
{
    running = true;
    //int value = generateNumber();
    int value = 70;
    //cout << "value : " << value << endl;
    std::thread producer(ProducerThread, value);
    std::thread consumer(ConsumerThread);

    producer.join();
    consumer.join();
    return 0;
}