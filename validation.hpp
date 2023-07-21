// Natalia Piasta, 299487

#include <iostream>
#include <arpa/inet.h>
#include <netinet/ip.h>
#include <sys/stat.h>

using namespace std;

void error(string error);
void validate_ip(char* ip);
void validate_port(int port);
void validate_size(long long size);
