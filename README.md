# Transport

Implementation of a simple downloader using the UDP protocol and sliding window algorithm to download a given number of bytes of the file from a UDP server.

Sends UDP datagrams containing the following message:
```
GET start length\n
```

And handles responses from server (datagram) that starts with:
```
DATA start length\n
```

## Usage:
```bash
$ ./transport <ip_address> <port> <file_name> <size>
```
