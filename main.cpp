// Natalia Piasta
// 299487

#include <iostream>
#include "udp.hpp"
#include "validation.hpp"

using namespace std;

int main(int argc, char* argv[]) {
    if(argc != 5)
        throw invalid_argument("Invalid number of arguments!\nUsage:\n./transport <ip_address> <port> <file_name> <size>");

    char* ip = argv[1];
    int port = atoi(argv[2]);
    char* file_name = argv[3];
    long long size = atoi(argv[4]);

    validate_ip(ip);
    validate_port(port);
    validate_size(size);

    // Initialization
    UDP protocol = UDP(ip, port, file_name, size);
    // Run the download
    protocol.run();
    
    return EXIT_SUCCESS;
}