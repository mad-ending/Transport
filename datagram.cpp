// Natalia Piasta
// 299487

#include "datagram.hpp"

Datagram::Datagram(int start, int length, double time) {
    this->start = start;
    this->length = length;
    this->time = time;
    this->request = "GET " + to_string(this->start) + " " + to_string(this->length) + "\n";
}