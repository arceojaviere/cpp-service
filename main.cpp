#include <iostream>
#include <string>
#include "SampleService.cpp"
static int EXITCODE_OK = 0;
int main(int arg_count, char ** args){
    std::cout << "Initializing....\n";
    myservice::MyTCPService service = myservice::MyTCPService();
    service.startup((short)8080);
    std::cout << "Exiting....\n";
    return EXITCODE_OK;
}
