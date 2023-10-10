#include <iostream>
#include <cstring>
#include <sys/types.h>
#include <unistd.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <netinet/in.h>
using namespace std;

#define ACK 0
#define NORMAL 1
#define CLOSE 2

typedef struct packet{
    char data[256];
}Packet;

typedef struct frame{
    int type; //ACK:0, NORMAL:1
    int id;
    Packet packet;
}Frame;

//keep count of the frames for checking ack ID
int FrameID = 0;
//create the server address description object
struct sockaddr_in serverAddr = {0};

void start_process(Frame &output, Frame &input, int endpoint);

int main()
{
    //create input and output frames
    //each frame includes the packet (cstring message)
    Frame output;
    Frame input;

    //create a socket (endpoint) for the client
    //UDP uses "SOCK_DGRAM"
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

    //set the receiving server address
    serverAddr.sin_family = AF_INET; //IPV4
    serverAddr.sin_port = htons(6666);
    serverAddr.sin_addr.s_addr = INADDR_ANY;

    int status = bind(endpoint, (struct sockaddr *)&serverAddr, sizeof(serverAddr) );
    if(status == -1)
    {
        perror("Failed to bind socket with address");
        close(endpoint);
        exit(EXIT_FAILURE);
    }
    else
    {
        cout << endl << "\e[0;32m[Success]\e[1;37m Socket binded with address " << endl;
    }

    //start the sending process
    start_process(output, input, endpoint);

    close(endpoint);
}

void start_process(Frame &output, Frame &input, int endpoint)
{
    while(true)
    {
        sockaddr_in replyAddr;
        socklen_t replyAddrSize;
        recvfrom(endpoint, &input, sizeof(Frame), 0, (struct sockaddr *)&replyAddr, &replyAddrSize);

        if(input.type == CLOSE)
        {
            cout << endl << "\e[1;33m[close]\e[1;37m close signal received, exiting" << endl << endl;
            break;
        }

        if(input.type == NORMAL && input.id == FrameID)
        {
            cout << endl << "\e[0;32m[Success]\e[1;37m Data received : " << input.packet.data << endl;

            output.type = ACK;
            output.id = ++FrameID;

            sendto(endpoint, &output, sizeof(output), 0, (struct sockaddr *)&replyAddr, replyAddrSize);
            cout << endl << "\e[0;32m[Finished]\e[1;37m ACK sent" << endl;
        }

    }
}
