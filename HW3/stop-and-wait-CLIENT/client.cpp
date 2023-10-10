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
//set the state to ready, in order to start sending new frames
bool ready = true;
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

    string ipv4 = "140.138.242.143";
    //set the receiving server address
    serverAddr.sin_family = AF_INET; //IPV4
    serverAddr.sin_port = htons(6666);
    serverAddr.sin_addr.s_addr = inet_addr(ipv4.c_str());

    cout << endl << "\e[0;32m[Success]\e[1;37m Set server address to " << ipv4 << endl;

    //start the sending process
    start_process(output, input, endpoint);

    close(endpoint);
}

void start_process(Frame &output, Frame &input, int endpoint)
{
    while(true)
    {
        if(ready == true)
        {
            output.id = FrameID;
            output.type = NORMAL;
            memset(output.packet.data, '\0', sizeof(output.packet.data) );

            //wait for user input
            cout << endl << "\e[1;33mPlease input data (input \"close\" to terminate): \e[1;37m";
            cin.getline(output.packet.data, 256);

            if(strcmp(output.packet.data, "close") == 0)
            {
                output.type = CLOSE;
                sendto(endpoint, &output, sizeof(output),
                       0, (const struct sockaddr*)&serverAddr, sizeof(serverAddr));
                cout << endl << "\e[1;33m[close]\e[1;37m Sent close signal, exiting " << endl << endl;
                break;
            }

            //send to the server
            sendto(endpoint, &output, sizeof(output),
                   0, (const struct sockaddr*)&serverAddr, sizeof(serverAddr));
            cout << endl << "\e[0;32m[Finished]\e[1;37m Frame sent, waiting for ACK reply" <<endl;
        }
        else if(ready == false)
        {
            //no ack received, resend the frame
            sendto(endpoint, &output, sizeof(output),
                   0, (const struct sockaddr*)&serverAddr, sizeof(serverAddr));
        }

        //start waiting for ack
        socklen_t address_size = sizeof(serverAddr);
        recvfrom(endpoint, &input, sizeof(input),
                 0, (struct sockaddr *)&serverAddr, &address_size);

        if(input.type == ACK && input.id == FrameID + 1)
        {
            cout << endl << "\e[0;32m[Success]\e[1;37m ACK Received, go on to the next frame" << endl;
            ready = true;
        }
        else
        {
            cout << endl << "\e[0;31m[Error]\e[1;37m ACK Lost, resending" << endl;
            ready = false;
        }

        ++FrameID;
    }
}
