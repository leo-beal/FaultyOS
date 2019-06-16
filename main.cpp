#include <iostream>
#include <thread>
#include <chrono>
#include <sys/unistd.h>
#include <mutex>
//#include <w32api/processthreadsapi.h>
#include <processthreadsapi.h>

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

    bool dontSpawn = false;

    for(int x = 0; x < argc; x++){
        if(std::string(argv[x]) == "-N"){
            dontSpawn = true;
        }
    }
    if(!dontSpawn) {
        std::string run = "FaultyOS.exe -N";
        system(run.c_str());
        system(run.c_str());
        system(run.c_str());
        system(run.c_str());
    }
    count("", 100);
    //CreateProcess("FaultyOS.exe", NULL, NULL, NULL, false, false, NULL, NULL, NULL, NULL);

    return 0;
}