// Natalia Piasta
// 299487

#include <iostream>
#include <netinet/ip.h>
#include <arpa/inet.h>
#include <string>

using namespace std;

class Datagram {
public:
    int start = 0;
    int length = 0;
    double time = 0;
    bool received = false;
    string request; // GET start length
    string response; // DATA start length

    Datagram(int start, int length, double time);
};