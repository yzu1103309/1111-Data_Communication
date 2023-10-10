#include <iostream>
#include <string>
#include <cstring>
#include <sys/types.h>
#include <unistd.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <netinet/in.h>
using namespace std;

int main()
{
    //string message = "Hello World";

    char message[50] = "Hello world";
    //create the server address description object
    struct sockaddr_in serverAddr = {0};

    //create a socket (endpoint) for the client
    int endpoint = socket(AF_INET, SOCK_STREAM, 0);

    if(endpoint == -1)
    {
        perror("Failed to create socket");
        exit(EXIT_FAILURE);
    }

    string ipv4 = "140.138.242.143";
    //set the receiving server address
    serverAddr.sin_family = AF_INET; //IPV4
    serverAddr.sin_port = htons(6666);
    serverAddr.sin_addr.s_addr = inet_addr(ipv4.c_str());

    cout << endl << "\e[0;32m[Success]\e[1;37m Set server address to " << ipv4 << endl;
    //set up connection before sending message
    int status = connect(endpoint, (const struct sockaddr *)&serverAddr, sizeof(serverAddr));

    if(status != 0)
    {
        perror("failed to connect server before ");
        exit(EXIT_FAILURE);
    }
    else
    {
        cout << endl << "\e[0;32m[Success]\e[1;37m Socket created on file descriptor " << endpoint << endl;
    }

    write(endpoint, message, sizeof(message) );
    cout << endl << "\e[0;32m[Finished]\e[1;37m Message sent, waiting for check reply" << endl;
    char check[50];
    memset(check, '\0' ,sizeof(check));
    read(endpoint, check, sizeof(check) );

    //The server will send back the exact message
    status = strcmp(check, message);

    if( status == 0 )
        cout << endl << "\e[0;32m[Success]\e[1;37m Message was sent properly and perfectly" << endl;
    else
        cout << endl << "\e[0;31m[Error]\e[1;37m Reply doesn't match with the original message" << endl;


    cout << endl << "\e[1;33m[close]\e[1;37m Closing socket" << endl << endl;
    close(endpoint);
}
