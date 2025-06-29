#include "TstClass.h"




//#include <cstdlib>
//
//int main() {
//    // Replace "script.py" with the path to your Python script
//    system( "python3 script.py" );
//    return 0;
//}
//
//
//#include <cstdio>
//#include <iostream>
//
//int main1() {
//    FILE* pipe = popen( "python3 script.py", "r" );
//    if ( !pipe ) {
//        std::cerr << "Failed to run script" << std::endl;
//        return 1;
//    }
//
//    char buffer[128];
//    while ( fgets( buffer, sizeof( buffer ), pipe ) != nullptr ) {
//        std::cout << buffer;
//    }
//
//    pclose( pipe );
//    return 0;
//}


//#include <cstdio>
//#include <iostream>
//#include <sys/types.h>
//#include <signal.h>
////#include <unistd.h>
//
//int main2() {
//    // Open a pipe to the Python script
//    FILE* pipe = _popen( "python3 script.py", "r" );
//    if ( !pipe ) {
//        std::cerr << "Failed to run script" << std::endl;
//        return 1;
//    }
//
//    // Get the PID of the subprocess
//    pid_t pid = fileno( pipe );
//    std::cout << "Subprocess PID: " << pid << std::endl;
//
//    // Do some work in the main program
//    // ...
//
//    // Close the subprocess when needed
//    if ( kill( pid, SIGTERM ) == -1 ) {
//        std::cerr << "Failed to terminate subprocess" << std::endl;
//    }
//
//    _pclose( pipe );
//    return 0;
//}


//#include <cstdio>
//#include <iostream>
//
//int main() {
//    // Open a pipe to the Python script
//    FILE* pipe = popen( "python3 script.py", "r" );
//    if ( !pipe ) {
//        std::cerr << "Failed to run script" << std::endl;
//        return 1;
//    }
//
//    // Read the output from the Python script line by line
//    char buffer[128];
//    while ( fgets( buffer, sizeof( buffer ), pipe ) != nullptr ) {
//        std::cout << buffer;
//    }
//
//    // Close the pipe
//    pclose( pipe );
//    return 0;
//}
