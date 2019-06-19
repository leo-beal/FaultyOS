#include <iostream>
#include <thread>
#include <chrono>
#include <sys/unistd.h>
#include <mutex>
#include <processthreadsapi.h>
#include <windows.h>
#include <fstream>

std::mutex oneCount;



VOID startup(LPCTSTR lpApplicationName)
{
    std::string option = "-N -T";
    // additional information
    STARTUPINFO si;
    PROCESS_INFORMATION pi;

    // set the size of the structures
    ZeroMemory( &si, sizeof(si) );
    si.cb = sizeof(si);
    ZeroMemory( &pi, sizeof(pi) );

    // start the program up
    CreateProcess( lpApplicationName,   // the path
                   (char*)option.c_str(),        // Command line
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
    CloseHandle( pi.hProcess );
    CloseHandle( pi.hThread );
}

void count(const std::ofstream& log, int from, int offset){

    for(int x = from; x <= 100; x++){
        //checkpoint
        //increment
        //wait

        std::this_thread::sleep_for(std::chrono::milliseconds(offset));
        std::cout << x << std::endl << std::flush;
        std::this_thread::sleep_for(std::chrono::seconds(1));
    }
}

int main(int argc, char* argv[]) {

    bool dontSpawn = false;

    std::ofstream log;
    log.open("checkpoint.txt", std::ofstream::out | std::ofstream::app);

    for(int x = 0; x < argc; x++){
        if(std::string(argv[x]) == "-N"){
            dontSpawn = true;
        }
        if(std::string(argv[x]) == "-T"){
            std::cout << "I am a child!" << std::endl;
        }
    }
    if(!dontSpawn) {
        startup("FaultyOS.exe");
        startup("FaultyOS.exe");
        startup("FaultyOS.exe");
        startup("FaultyOS.exe");
    }

    count(log, 0, 0);

    return 0;
}