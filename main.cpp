#include <iostream>
#include <thread>
#include <chrono>
#include <sys/unistd.h>
#include <mutex>
#include <processthreadsapi.h>
#include <windows.h>

std::mutex oneCount;



VOID startup(LPCTSTR lpApplicationName)
{
    std::string option = "-N";
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

void count(std::string path, int countTo){
    oneCount.lock();
    for(int x = 0; x <= countTo; x++){
        //startup("FaultyOS.exe"); Dont do this
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
        startup("FaultyOS.exe");
        startup("FaultyOS.exe");
        startup("FaultyOS.exe");
        startup("FaultyOS.exe");
    }

    count("", 100);

    return 0;
}