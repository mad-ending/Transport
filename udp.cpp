// Natalia Piasta
// 299487

#include "udp.hpp"

UDP::UDP(char* ip, int port, char* file_name, long long size) {
    this->ip = ip;
    this->port = port;
    this->file.open(file_name);
    this->requested_bytes = size;
    this->number_of_datagrams_to_send = (this->requested_bytes+DATAGRAM_SIZE)/DATAGRAM_SIZE;

    // Socket initialization
    this->sockfd = this->Socket(AF_INET, SOCK_DGRAM, 0);

    bzero(&this->server_address, sizeof(this->server_address));
    this->server_address.sin_family = AF_INET;
    this->server_address.sin_port = htons(this->port);
    inet_pton(AF_INET, this->ip, &this->server_address.sin_addr);
}

void UDP::run() {
    this->handle_datagrams();

    while(!completed()) {
        this->handle_window();
        this->send_datagrams();
        this->receive_datagrams();
        this->save_data();
    }
    cout<<setprecision(3)<<fixed<<((double)(this->number_of_datagrams_sent)/this->number_of_datagrams_to_send) * 100<<"\% done"<<endl;

    this->file.close();
    this->Close(sockfd);
}

void UDP::handle_datagrams() {
    for(int n = 0; n < this->number_of_datagrams_to_send; n++) {
        int bytes_to_save = n * DATAGRAM_SIZE;
        int request_bytes = min(this->requested_bytes - bytes_to_save, DATAGRAM_SIZE);

        auto datagram = Datagram(n * DATAGRAM_SIZE, request_bytes, clock());
        this->datagrams.push_back(datagram);
    }
}

void UDP::handle_window() {
    list<Datagram>::iterator it = this->datagrams.begin();
    advance(it, this->number_of_datagrams_sent);
    for(; it != this->datagrams.end() and this->window.size()<WINDOW_SIZE; it++) {
        window.push_back(*it);
    }
}

void UDP::send_datagrams() {
    while(!this->window.empty()) {
        auto obj = this->window.front();
        this->window.pop_front();

        string request = obj.request;
        if(!obj.received) {}
            this->Sendto(this->sockfd, request.c_str(), request.length(), 0, (struct sockaddr *) &this->server_address, sizeof(this->server_address));
    }
}

void UDP::receive_datagrams() {
    struct timeval tv;
    tv.tv_sec = 0;
    tv.tv_usec = 100000;
    fd_set descriptors;

    FD_ZERO(&descriptors);
    FD_SET(this->sockfd, &descriptors);

    while(true) {
        int ready = this->Select(this->sockfd + 1, &descriptors, NULL, NULL, &tv);
        if(ready == 0)
            break;

        char buffer[BUFFER_SIZE];
        char from_ip[20];

        Recvfrom(this->sockfd, buffer, BUFFER_SIZE, 0, (struct sockaddr *)&this->server_address, sizeof(this->server_address));
        if(inet_ntop(AF_INET,&(this->server_address.sin_addr), from_ip, sizeof(from_ip)) == NULL)
            error("inet_ntop error!");

        bool valid_ip = (string)from_ip == (string)this->ip;
        bool valid_port = ntohs(this->server_address.sin_port) == this->port;
        if(valid_ip and valid_port) {
            unsigned int start = 0;
            unsigned int received_len = 0;
            int items_filled = sscanf((char *)buffer, "DATA %u %u\n", &start, &received_len);
            if(items_filled != 2)
                continue;
            
            int index = start/DATAGRAM_SIZE;
            list<Datagram>::iterator it = this->datagrams.begin();
            advance(it, index);
            if(!it->received) {
                it->response = "DATA " + to_string(start) + " " + to_string(received_len) + "\n";
                it->received = true;
            }
        }         
    }
}

void UDP::save_data() {
    list<Datagram>::iterator it = this->datagrams.begin();
    advance(it, this->number_of_datagrams_sent);
    for(; it != this->datagrams.end(); it++) {
        if(!it->received)
            break;
        this->file << it->response;
        cout<<setprecision(3)<<fixed<<((double)(this->number_of_datagrams_sent)/this->number_of_datagrams_to_send) * 100<<"\% done"<<endl;

        this->number_of_datagrams_sent++; 
    }
    window.clear();
}

// Checking can transport be completed
bool UDP::completed() {
    if(this->number_of_datagrams_sent * DATAGRAM_SIZE >= this->requested_bytes)
        return true;
    return false;
}

int UDP::Socket(int domain, int type, int protocol) {
    int sockfd = socket(domain, type, protocol); 
    if(sockfd < 0)
        error("Socket error!");
    return sockfd;
}

int UDP::Select(int numfds, fd_set *readfds, fd_set *writefds, fd_set *exceptfds, struct timeval *timeout) {
    int x = select(numfds, readfds, writefds, exceptfds, timeout);
    if(x < 0)
        error("Select error");
    return x;
}

void UDP::Close(int sockfd) {
    if(close(sockfd) < 0)
        error("Close error!");
}

void UDP::Sendto(int sockfd, const void *buf, size_t len, int flags, const struct sockaddr *addr, socklen_t addrlen) {
    int x = sendto(sockfd, buf, len, flags, (struct sockaddr*)addr, addrlen);
	if(x < 0){	
		error("Sendto error");	
	}
}

ssize_t UDP::Recvfrom(int sockfd, void *buf, size_t len, int flags, const struct sockaddr *addr, socklen_t addrlen) {
    int x = recvfrom(sockfd, buf, len, flags, (struct sockaddr*)addr, &addrlen);
    if(x < 0) {
        cout<<strerror(errno)<<endl;
        error("Recvfrom error!");
    }
    return x;
}