#include <iostream>
#include <thread>
#include <chrono>

void count(std::string path, int countTo){
    for(int x = 0; x <= countTo; x++){
        //checkpoint
        //increment
        //wait
        std::cout << x << std::endl << std::flush;
        std::this_thread::sleep_for(std::chrono::seconds(1));
    }
}

int main(int argc, char* argv[]) {

    count("", 100);

    return 0;
}