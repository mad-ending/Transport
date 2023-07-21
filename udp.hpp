// Natalia Piasta
// 299487
// Natalia Piasta, 299487
#include <bits/stdc++.h>
#include <netinet/ip.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <unistd.h>
#include <errno.h>
#include "datagram.hpp"
#include "validation.hpp"

#define WINDOW_SIZE 500
#define DATAGRAM_SIZE 1000
#define BUFFER_SIZE 2048

using namespace std;

class UDP {
private:
    char* ip;
    int port;
    ofstream file;
    int sockfd;
    struct sockaddr_in server_address;

    int requested_bytes;
    int number_of_datagrams_to_send;
    int number_of_datagrams_sent = 0;
    // int number_of_saved_datagrams = 0;
    int id_datagram = 0;

    list<Datagram> datagrams;
    list<Datagram> window;

    void handle_datagrams();
    void handle_window();
    bool completed();
    void send_datagrams();
    void receive_datagrams();
    void save_data();

    int Socket(int domain, int type, int protocol);
    int Select(int numfds, fd_set *readfds, fd_set *writefds, fd_set *exceptfds, struct timeval *timeout);
    void Close(int sockfd);
    void Sendto(int sockfd, const void *buf, size_t len, int flags, const struct sockaddr *addr, socklen_t addrlen);
    ssize_t Recvfrom(int sockfd, void *buf, size_t len, int flags, const struct sockaddr *addr, socklen_t addrlen);

public:
    UDP(char* ip, int port, char* file_name, long long size);
    void run();
};