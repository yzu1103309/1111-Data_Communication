#include <iostream>
#include <string>
#include <cstring>
#include <sys/types.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <netinet/in.h>
#include <unistd.h>

using namespace std;

int main()
{
    char buffer[50];
    memset(buffer, '\0', sizeof(buffer));
    struct sockaddr_in serverAddr = {0}, cli = {0};

    int endpoint = socket(AF_INET, SOCK_STREAM, 0);
    if(endpoint == -1)
    {
        perror("failed o create socket");
        exit(EXIT_FAILURE);
    }
    else
    {
        cout << endl << "\e[0;32m[Success]\e[1;37m Socket created on file descriptor " << endpoint << endl;
    }


    serverAddr.sin_family = AF_INET;
    serverAddr.sin_port = htons(6666);
    serverAddr.sin_addr.s_addr = INADDR_ANY;

    int status = bind(endpoint, (const struct sockaddr *)&serverAddr, sizeof(serverAddr));

    if(status == -1)
    {
        perror("failed to bind");
        close(endpoint);
        exit(EXIT_FAILURE);
    }
    else
    {
        cout << endl << "\e[0;32m[Success]\e[1;37m Socket binded with address " << endl;
    }

    status = listen(endpoint, 5);
    if(status != 0)
    {
        perror("listen failed");
        close(endpoint);
        exit(EXIT_FAILURE);
    }
    else
    {
        cout << endl << "\e[0;32m[Success]\e[1;37m listening on socket " << endl;
    }

    socklen_t length_cli = sizeof(cli);
    int client_endpoint = accept(endpoint, (struct sockaddr *)&cli, &length_cli);

    if(status == -1)
    {
        perror("failed to accept");
        close(endpoint);
        exit(EXIT_FAILURE);
    }
    else
    {
        cout << endl << "\e[0;32m[Success]\e[1;37m connection with client was set up on file descriptor " << client_endpoint << endl;
    }

    read(client_endpoint, &buffer, sizeof(buffer) );
    cout << endl << "\e[0;32m[Success]\e[1;37m Data received : " << buffer << endl;

    write(client_endpoint, buffer, sizeof(buffer) );
    cout << endl << "\e[0;32m[Finished]\e[1;37m sent back check message "<< endl;

    cout << endl << "\e[1;33m[close]\e[1;37m closing socket"<< endl << endl;
    close(endpoint);
}
