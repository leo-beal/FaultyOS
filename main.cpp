#include <iostream>
#include <thread>
#include <chrono>
#include <sys/unistd.h>
#include <mutex>
#include <processthreadsapi.h>
#include <windows.h>
#include <fstream>

DWORD PID;
DWORD WPID;
//HANDLE ME;
HANDLE THEM;


void startup(LPCTSTR lpApplicationName, std::string options)
{
    // additional information
    STARTUPINFO si;
    PROCESS_INFORMATION pi;

    // set the size of the structures
    ZeroMemory( &si, sizeof(si) );
    si.cb = sizeof(si);
    ZeroMemory( &pi, sizeof(pi) );

    // start the program up
    CreateProcess( lpApplicationName,   // the path
                   (char*)options.c_str(),        // Command line
                   NULL,           // Process handle not inheritable
                   NULL,           // Thread handle not inheritable
                   FALSE,          // Set handle inheritance to FALSE
                   0,              // No creation flags
                   NULL,           // Use parent's environment block
                   NULL,           // Use parent's starting directory
                   &si,            // Pointer to STARTUPINFO structure
                   &pi             // Pointer to PROCESS_INFORMATION structure (removed extra parentheses)
    );
    // Close process and thread handles.
    //CloseHandle( pi.hProcess );
    CloseHandle( pi.hThread );
    THEM = pi.hProcess;
}

//type being watched, not type of watcher
void watchProc(std::string type){
    DWORD dwExitCode;
    while(true){
        if(GetExitCodeProcess(THEM, &dwExitCode)){
            if(dwExitCode==STILL_ACTIVE) {
                //still running
            }
            else {
                //not running anymore
                std::cout << "Process died. Restarting" << std::endl;
                CloseHandle(THEM);
                if(type == "count"){
                    startup("FaultyOS.exe", "-t count -p " + std::to_string(PID));
                }else{
                    startup("FaultyOS.exe", "-t watch -p " + std::to_string(PID));
                }
                //std::this_thread::sleep_for(std::chrono::milliseconds(500));
            }
        }
        else{
            //query failed, handle probably doesn't have the PROCESS_QUERY_INFORMATION access
        }
    }
}

void count(std::ofstream& log, int from, int64_t offset){

    for(int x = from; x <= 100; x++){
        std::this_thread::sleep_for(std::chrono::milliseconds(offset));
        log << "\n" << x << " " << std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::system_clock::now().time_since_epoch()).count() << "\r" << std::flush;
        std::cout << x << std::endl << std::flush;
        std::this_thread::sleep_for(std::chrono::seconds(1));
    }
}

int main(int argc, char* argv[]) {

    bool isCounter = true;

    PID = GetCurrentProcessId();

    std::ofstream log;
    std::ifstream iLog;
    log.open("checkpoint.txt", std::ofstream::out | std::ofstream::app);

    //log << "test";

    for(int x = 0; x < argc; x++){
        if(std::string(argv[x]) == "-t"){
            if(std::string(argv[x+1]) == "watch"){
                isCounter = false;
            }
        }
        if(std::string(argv[x]) == "-p"){
            WPID = std::stoi(std::string(argv[x+1]));
        }
    }

    if(isCounter && argc == 1){
        log.open("checkpoint.txt", std::ofstream::out | std::ofstream::app | std::ofstream::trunc);
        startup("FaultyOS.exe", "-t watch -p " + std::to_string(PID));
        std::thread tCount(count, std::ref(log), 0, 0);
        std::thread tWatch(watchProc, "watch");
        //count(log, 0, 0);
        tCount.join();
        tWatch.join();
    }else if(isCounter && argc > 1){
        log.open("checkpoint.txt", std::ofstream::out | std::ofstream::app);
        THEM = OpenProcess(PROCESS_ALL_ACCESS, false, WPID);
        //get file info
        iLog.open("checkpoint.txt");
        std::string num;
        std::string time;
        std::getline(iLog, num, ' ');
        std::getline(iLog, time, ' ');
        std::cout << num << " " << time << std::endl;
        int64_t offset = 1000 - std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::system_clock::now().time_since_epoch()).count() - std::stoi(time);
        if(offset < 0){
            offset = 0;
        }
        std::thread tCount(count, std::ref(log), std::stoi(num) + 1, offset);
        std::thread tWatch(watchProc, "watch");
        tCount.join();
        tWatch.join();
    }else{
        THEM = OpenProcess(PROCESS_ALL_ACCESS, false, WPID);
        watchProc("count");
    }



    return 0;
}