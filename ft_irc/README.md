# ft_irc

A simple IRC (Internet Relay Chat) server developed in **C++** as part of the 42 School curriculum.

## About the Project

The goal of ft_irc is to build an IRC server that allows multiple clients to communicate in real time using the IRC protocol.

The project focuses on **network programming, socket communication, and client-server architecture**.

## Features

- Multiple simultaneous client connections
- User authentication and nickname management
- Public and private messaging
- Channel creation and management
- Channel operator permissions
- IRC commands such as `JOIN`, `PRIVMSG`, `KICK`, `INVITE`, `TOPIC`, and `MODE`

## Technologies

- **Language:** C++
- **Networking:** TCP/IP, sockets
- **Concepts:** Client-server architecture, I/O multiplexing, IRC protocol

## Compilation

```bash
make
```

## Usage

Start the server:

```bash
./ircserv <port> <password>
```

Connect using a compatible IRC client.

## What I Learned

- Managing multiple network connections
- Understanding TCP/IP communication
- Implementing a communication protocol
- Handling client requests and server responses
- Building a network application in C++
