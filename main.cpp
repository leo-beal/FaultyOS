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

void count(std::ofstream& log, int from, int offset){

    for(int x = from; x <= 100; x++){
        std::this_thread::sleep_for(std::chrono::milliseconds(offset));
        log.clear();
        log.seekp(0, std::fstream::beg);
        log  << x << " " << std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::system_clock::now().time_since_epoch()).count() << "\r" << std::flush;
        std::cout << x << std::endl << std::flush;
        std::this_thread::sleep_for(std::chrono::seconds(1));
    }
}

int main(int argc, char* argv[]) {

    bool dontSpawn = false;

    std::ofstream log;
    log.open("checkpoint.txt", std::ofstream::out);

    //log << "test";

    for(int x = 0; x < argc; x++){
        if(std::string(argv[x]) == "-N"){
            dontSpawn = true;
        }
        if(std::string(argv[x]) == "-T"){
            std::cout << "I am a child!" << std::endl;
        }
    }
    if(!dontSpawn) {
        //startup("FaultyOS.exe");
        //startup("FaultyOS.exe");
        //startup("FaultyOS.exe");
        //startup("FaultyOS.exe");
    }

    count(log, 0, 0);

    return 0;
}