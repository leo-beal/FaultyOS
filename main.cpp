#include <iostream>
#include <thread>
#include <chrono>
#include <sys/unistd.h>
#include <mutex>

std::mutex oneCount;

void count(std::string path, int countTo){
    oneCount.lock();
    for(int x = 0; x <= countTo; x++){
        //checkpoint
        //increment
        //wait
        std::cout << x << std::endl << std::flush;
        std::this_thread::sleep_for(std::chrono::seconds(1));
    }
    oneCount.unlock();
}

int main(int argc, char* argv[]) {

    fork();
    count("", 100);

    return 0;
}