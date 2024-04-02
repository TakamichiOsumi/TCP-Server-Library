# TCP-Server-Library

Implement TCP-Server library with below features

* I/O Multiplexing
  A server launched by this library internally utilizes `select` system call to handle multiple clients.

* TCP message demarcation
  TCP is a byte-oriented protocol and does not know where the client's message starts and ends. This may lead to unpredictable message division or message concatenation caused by TCP layer system call like `recvfrom`. This library decides the client's message format and is intelligent enough to recognize the boundary of each message.
