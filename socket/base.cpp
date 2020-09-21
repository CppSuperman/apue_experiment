#include <stdio.h>
#include <arpa/inet.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <unistd.h>
#include <fcntl.h>
#include <netdb.h>
#include <signal.h>
#include <string>
#include <iostream>

using namespace std;

int ls;
int client_socket;

static void quit(int signo) 
{
    sigset_t mask, prev_mask;
    sigfillset(&mask);
    sigprocmask(SIG_BLOCK, &mask, &prev_mask);
    // close the listening socket 
    close(ls);
    // close socket
    close(client_socket);
    sigprocmask(SIG_SETMASK, &prev_mask, NULL);
    cout << "\nquit server" << endl;
    exit(0);

}

int main(int argc, char** argv) 
{
    if (argc != 2) 
    {
        cerr << "not assign port";
        return -1;
    }
    // register singal handler 
    signal(SIGINT, quit);

    // create a socket 
    ls = socket(AF_INET, SOCK_STREAM, 0);
    if (ls == -1) 
    {
        cerr << "Can't create a socket!";
        return -2;
    }
    // bind the socket to IP:port
    sockaddr_in hint;
    hint.sin_family = AF_INET;
    hint.sin_port = htons(atoi(argv[1]));
    inet_pton(AF_INET, "0.0.0.0", &hint.sin_addr);
    
    if (bind(ls, (sockaddr*)&hint, sizeof(hint)) == -1) 
    {
        cerr << "Can't bind to ip";
        return -3;
    }

    // mark the socket for listening in
    if (listen(ls, SOMAXCONN) == -1) 
    {
        cerr << "Can't listen!";
        return -4;
    }
    // accept a call
    sockaddr_in client;
    socklen_t client_size = sizeof(client);
    char host[NI_MAXHOST];
    char srv[NI_MAXSERV];
    client_socket = accept(ls, (sockaddr*)&client, &client_size);
    if (client_socket == -1) 
    {
        cerr << "client connecting error";
        return -5;
    }
    
    memset(host, 0, NI_MAXHOST);
    memset(srv, 0, NI_MAXSERV);

    int res = getnameinfo((sockaddr*)&client, client_size, host, NI_MAXHOST, srv, NI_MAXSERV, 0);
    if (res) 
        cout << host << " connected " << endl;
    else
    {
        inet_ntop(AF_INET, &client.sin_addr, host, NI_MAXHOST);
        cout << host << " connect on " << ntohs(client.sin_port) << endl;
    }
    // while receiving display message, echo message
    char buf[4096];
    while (true)
    {
        // clear the buf
        memset(buf, 0, 4096);
        // wait for message
        int bytes_recv = recv(client_socket, buf, 4096, 0);
        if (bytes_recv == -1) 
        {
            cerr << "there was a connection issue" << endl;
            break;
        }
        if (bytes_recv == 0) 
        {
            cout << " the client disconnected" << endl;
            break;
        }
        // display message
        cout << "Received: " << string(buf, 0, bytes_recv);
        // resend message
        send(client_socket, buf, bytes_recv + 1, 0);
    }
    
    return 0;
}