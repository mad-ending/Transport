// Natalia Piasta, 299487

#include "validation.hpp"

void error(string error) {
    cerr<<error<<endl;
    exit (EXIT_FAILURE);
}

void validate_ip(char* ip) {
    struct sockaddr_in sa;
    if(!inet_pton(AF_INET, ip, &sa.sin_addr))
        error("Invalid IP address!");
}

void validate_port(int port) {
    if(port<1 || port>65535)
        error("Invalid port!\nPort should be integer ϵ[1, 65535]!");
}

void validate_size(long long size) {
    if(size <= 0)
        error("Invalid size!\nSize should be > 0, otherwise there is nothing to do.");
}