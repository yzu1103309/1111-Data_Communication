#include <iostream>
#include <string>
#include <sys/types.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <netinet/in.h>
#include <unistd.h>

using namespace std;

int main()
{
    char buffer[50] = {0};
    struct sockaddr_in serverAddr = {0};

    int endpoint = socket(AF_INET, SOCK_DGRAM, 0);
    if(endpoint == -1)
    {
        perror("failed to create socket");
        exit(EXIT_FAILURE);
    }
    else
    {
        cout << endl << "\e[0;32m[Success]\e[1;37m Socket created on file descriptor " << endpoint << endl;
    }

    serverAddr.sin_family = AF_INET;
    serverAddr.sin_port = htons(6666);
    serverAddr.sin_addr.s_addr = INADDR_ANY;

    int rc = bind(endpoint, (const struct sockaddr *)&serverAddr, sizeof(serverAddr));

    if(rc == -1)
    {
        perror("failed to bind");
        close(endpoint);
        exit(EXIT_FAILURE);
    }
    else
    {
        cout << endl << "\e[0;32m[Success]\e[1;37m Socket binded with address " << endl;
    }

    socklen_t len = 0;
    int mLength = recvfrom(endpoint, (char *)buffer, 50, MSG_WAITALL, 0, &len);

    buffer[mLength] = '\n';
    cout << endl << "\e[0;32m[Success]\e[1;37m Data received : " << buffer;

    cout << endl << "\e[1;33m[close]\e[1;37m closing socket"<< endl;
    close(endpoint);
}
