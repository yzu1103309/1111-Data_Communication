#include <iostream>
#include <string>
#include <sys/types.h>
#include <unistd.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <netinet/in.h>
using namespace std;

int main()
{
    string message = "Hello World";

    //create the server address description object
    struct sockaddr_in serverAddr = {0};

    //create a socket (endpoint) for the client
    int endpoint = socket(AF_INET, SOCK_DGRAM, 0);

    if(endpoint == -1)
    {
        perror("Failed to create socket");
        exit(EXIT_FAILURE);
    }
    else
    {
        cout << endl << "\e[0;32m[Success]\e[1;37m Socket created on file descriptor " << endpoint << endl;
    }

    string ipv4 = "140.138.242.143";
    //set the receiving server address
    serverAddr.sin_family = AF_INET; //IPV4
    serverAddr.sin_port = htons(6666);
    serverAddr.sin_addr.s_addr = inet_addr(ipv4.c_str());

    cout << endl << "\e[0;32m[Success]\e[1;37m Set server address to " << ipv4 << endl;

    int len = sendto(endpoint, (const char *)message.c_str(), message.size(),
                     0, (const struct sockaddr *)&serverAddr, sizeof(serverAddr) );

    if(len == -1)
    {
        perror("Fail to send message");
    }
    else
    {
        cout << endl << "\e[0;32m[Finished]\e[1;37m Message sent" << endl;
    }

    cout << endl << "\e[1;33m[close]\e[1;37m closing socket"<< endl << endl;
    close(endpoint);
}
