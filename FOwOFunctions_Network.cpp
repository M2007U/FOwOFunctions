#pragma once

#include <iostream>
#include <iomanip>

#include <stdlib.h>
#include <cstdlib>

#include <functional>

//for file writing and reading
#include <fstream>
#include <sstream>

#include <string>
#include <vector>
#include <cstring>
#include <cctype>

#include <cmath>
#include <math.h>
#include <random>

#include <typeinfo>
#include <sys/types.h>
#include <unistd.h>

//for networks
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <fcntl.h>

//for directories
#include <sys/stat.h>
#include <bits/stdc++.h>
#include <dirent.h>

#include "FOwOFunctions.cpp"

using namespace std;

class FOwO_network_action
{
    public :

    FOwO_cout fowo_cout;

    int talk_send(string Given, int TargetSocket )
    {
        //attempts to send a string across the connection
        //if success, this will return the number of sent bytes
        //if fail, return -1


        int ReturnResult = 0;

        if(TargetSocket >= 0)
        {
            const char* GivenPass1 = Given.data();
            ssize_t SentBytes = send(TargetSocket, &GivenPass1, Given.length(), 0); //ssize_t send(int sockfd, const void *buf, size_t len, int flags);
            if(SentBytes == -1)
            {
                //something is wrong
                ReturnResult = -1;
                cout << fowo_cout.ConsoleQuick("error","unable to send message") << endl;
            }
            else
            {
                ReturnResult = SentBytes;
            }
        }
        else
        {
            cout << fowo_cout.ConsoleQuick("error", "invalid socket") << endl;
            ReturnResult = -1;
        }

        return ReturnResult;
    }

    string talk_recv(int InTargetSocket, int InBufferLength)
    {
        //attempt to receive a message on the network
        //if success, will return the received string
        //if fail, will return empty string

        string ResultString = "";

        if(InTargetSocket >= 0)
        {
            char buffer[InBufferLength];
            int BytesReceived = recv(InTargetSocket, buffer, InBufferLength - 1,0);

            if(BytesReceived > 0)
            {
                buffer[BytesReceived] = '\0'; // null terminate the received data
                string Result(buffer);
                ResultString = Result;
            }
            else if (BytesReceived == 0)
            {
                cout << fowo_cout.ConsoleQuick("error","connection closed by remote host") << endl;
                ResultString = "";
            }
        }
        else
        {
            cout << fowo_cout.ConsoleQuick("error","invalid socket") << endl;
            ResultString = "";
        }

        return ResultString;
    }

};

class FOwO_network_machine_server
{
    public :

    FOwO_string_shorthand fowo_string_shorthand;
    FOwO_cout fowo_cout;

    int ServerSocket;
    int ClientSocket;
    struct sockaddr_in ServerAddr;
    struct sockaddr_in ClientAddr;
    socklen_t AddrSize;
    string IPAddr;
    int PortNumber;

    stringstream STR_Buffer;
    string STR_Send = "";
    string STR_Recv = "";

    bool Connection_Setup (int InPortNumber, int InPortListenBacklog)
    {
        //tries to setup a connection

        //STEP 1 : create a socket
        ServerSocket = socket(AF_INET,SOCK_STREAM,0);
        if(ServerSocket == 0)
        {
            cout << fowo_cout.ConsoleQuick("error","socket creation error");
            return false;
        }
        else
        {
            cout << fowo_cout.ConsoleQuick("success","socket created");
        }

        //STEP 2 : setup Port Number
        ServerAddr.sin_family = AF_INET;
        ServerAddr.sin_addr.s_addr = INADDR_ANY;
        ServerAddr.sin_port = htons(InPortNumber);

        //STEP 3 : bind Socket and Port
        if( ::bind(ServerSocket, (struct sockaddr*) &ServerAddr, sizeof(ServerAddr)) < 0)
        {
            cout << fowo_cout.ConsoleQuick("error","Socket bind Failed") << endl;
            return false;
        }
        else
        {
            cout << fowo_cout.ConsoleQuick("success","Socket bind Success") << endl;
            cout << fowo_cout.ConsoleQuick("report","now attempt to listen") << endl;
        }

        //STEP 4 : Listen
        if (listen(ServerSocket, InPortListenBacklog) < 0) //refer to notes at the class FOwO_network
        {
            cout << fowo_cout.ConsoleQuick("error","listened to nothing ...") << endl;
            return false;
        }
        else
        {
            cout << fowo_cout.ConsoleQuick("success","listened success") << endl;
        }

        //STEP 5 : Connection Setup, accept connection from client
        AddrSize = sizeof(ClientAddr);
        ClientSocket = accept(ServerSocket, (struct sockaddr*) &ClientAddr, &AddrSize);
        if(ClientSocket < 0)
        {
            cout << fowo_cout.ConsoleQuick("error","accept connection failed") << endl;
            return false;
        }

        //STEP X : Success Connection
        cout << fowo_cout.ConsoleQuick("success","Connected, yay") << endl;
        cout << fowo_cout.ConsoleQuick("success","ServerSocket : " + to_string(ServerSocket)) << endl;
        cout << fowo_cout.ConsoleQuick("success","ClientSocket : " + to_string(ClientSocket)) << endl;
        cout << fowo_cout.ConsoleQuick("report","PortNumber : " + to_string(InPortNumber)) << endl;
        return true;
    }

    bool Connection_Setup_Force (int InPortNumber, int InPortListenBacklog)
    {
        //set up a connection
        //if the set up fails, try again
        //until it success to setup a connection

        bool JobDone = false;

        while(!JobDone)
        {
            JobDone = Connection_Setup(InPortNumber, InPortListenBacklog);
        }

        return true;
    }

    int MessageSEND (string InMessage)
    {
        //send a message across the network
        const char* MessageAsCString = InMessage.c_str();
        return (int) send(ClientSocket, MessageAsCString, strlen(MessageAsCString), 0);
    }

    string MessageRECV (int InBufferLength, bool InIfFailForceExit)
    {
        
        //receive a message from a port or a socket
        //if it fails to receive a message, then force the program to quit

        char MessageOwO[InBufferLength];
        memset(MessageOwO, 0, InBufferLength); //clear the buffer
        
        //NOTE :
        //if next time, some debugging says that the line is crashing the thing
        //its the main.cpp fault, need to investigate
        int BytesReceived = recv(ClientSocket, MessageOwO, InBufferLength - 1, 0);
        

        if (BytesReceived < 0)
        {
            cout << fowo_cout.ConsoleQuick("error","received failed") << endl;
            if(InIfFailForceExit)
            {
                exit(1);
            }
            return "";
        }
        else if (BytesReceived == 0)
        {
            cout << fowo_cout.ConsoleQuick("error","connection closed by the client") << endl;
            if(InIfFailForceExit)
            {
                exit(1);
            }
            return "";
        }
        else
        {
            MessageOwO[BytesReceived] = '\0';
            return string (MessageOwO);
        }

    }

    void Connection_Close()
    {
        if(ServerSocket > 0){close(ServerSocket) ; }
        if(ClientSocket > 0){close(ClientSocket) ; }
        cout << fowo_cout.ConsoleQuick("y","Server Connection Closed");
    }

};

class FOwO_network_machine_client
{
    public : 

    FOwO_string_shorthand fowo_string_shorthand;
    FOwO_cout fowo_cout;

    int ServerSocket;
    int ClientSocket;
    struct sockaddr_in ServerAddr;
    struct sockaddr_in ClientAddr;
    socklen_t AddrSize;
    string IPAddr;
    int PortNumber;

    stringstream STR_Buffer;
    string STR_Send = "";
    string STR_Recv = "";

    bool Connection_Setup (string InIPAddress, int InPortNumber)
    {
        //tries to setup a connection

        //STEP 1 : Create Socket
        ClientSocket = socket(AF_INET,SOCK_STREAM,0);
        if (ClientSocket == 0)
        {
            cout << fowo_cout.ConsoleQuick("error","socket creation error") << endl;
            return false;
        }
        else
        {
            cout << fowo_cout.ConsoleQuick("success","socket created") << endl;
        }

        //STEP 2 : Accept and configure IPAddress and PortNumber
        IPAddr = InIPAddress;
        PortNumber = InPortNumber;
        ServerAddr.sin_family = AF_INET;
        ServerAddr.sin_port = htons(PortNumber);

        //STEP 3 : convert IPv4 or IPv6 from string form to binary form
        if( inet_pton(AF_INET, IPAddr.c_str(), &ServerAddr.sin_addr) <= 0)
        {
            cout << fowo_cout.ConsoleQuick("error","invalid address / address not supported") << endl;
            return false;
        }

        //STEP 4 : Connect to the server
        if( connect(ClientSocket, (struct sockaddr*) &ServerAddr, sizeof(ServerAddr)) < 0)
        {
            cout << fowo_cout.ConsoleQuick("error","connection failed") << endl;
            return false;
        }

        //STEP X : Connection success
        cout << fowo_cout.ConsoleQuick("success","connected, yay OwO") << endl;
        return true;
    }

    bool Connection_Setup_Force (string InIPAddress, int InPortNumber)
    {
        //set up a connection
        //if the set up fails, try again
        //until it success to setup a connection

        bool JobDone = false;

        while(!JobDone)
        {
            JobDone = Connection_Setup(InIPAddress, InPortNumber);
        }

        return true;
    }

    int MessageSEND (string InMessage)
    {
        //send a message across the network

        const char* MessageAsCString = InMessage.c_str();
        return (int) send(ClientSocket, MessageAsCString, strlen(MessageAsCString), 0);
    }

    string MessageRECV (int InBufferLength, bool InIfFailForceExit)
    {
        //receive a message from a port or a socket
        //if it fails to receive a message, then force the program to quit

        char MessageOwO[InBufferLength];
        memset(MessageOwO, 0, InBufferLength); //clear the buffer

        int BytesReceived = recv(ClientSocket, MessageOwO, InBufferLength - 1, 0);
        if (BytesReceived < 0)
        {
            cout << fowo_cout.ConsoleQuick("error","received failed") << endl;
            if(InIfFailForceExit)
            {
                exit(1);
            }
            return "";
        }
        else if (BytesReceived == 0)
        {
            cout << fowo_cout.ConsoleQuick("error","connection closed by the client") << endl;
            if(InIfFailForceExit)
            {
                exit(1);
            }
            return "";
        }
        else
        {
            MessageOwO[BytesReceived] = '\0';
            return string (MessageOwO);
        }

        
    }

    void Connection_Close()
    {
        if(ServerSocket > 0){close(ServerSocket) ; }
        if(ClientSocket > 0){close(ClientSocket) ; }
        cout << fowo_cout.ConsoleQuick("y","Client Connection Closed");
    }

};

class FOwO_network
{
    public :

    string MachineType;

    FOwO_network_action actiOwOns;
    FOwO_network_machine_server sOwOrver;
    FOwO_network_machine_client clOwOnt;

    FOwO_cout fowo_cout;
    FOwO_string fowo_string;

    /*
    when sending and receiving messages between servers and clients
    each machine has ServerSocket and ClientSocket

    on the server side, when setting up the connection, we have :

    ```
    ServerSocket = socket(AF_INET, SOCK_STREAM, 0); // Server creates a socket for listening
    ClientSocket = accept(ServerSocket, (struct sockaddr*)&ClientAddr, &AddrSize); // After binding and listening, it accepts a client connection
    ```

    so now both the server machine and client machine have both the ServerSocket and ClientSocket;
    but when sending or receiving messages, which socket to use ?
    
    On the ServerSide
    {
        ** ServerSocket **
        is used to listen for incoming connections from Clients.
        It does not send or receive messages directly with CLients

        ** ClientSocket **
        is created by the server when it accepts a connection.
        This is used to send and receive messages to/from the specific client.

        so on the server side, both send and recv should be
        ```
        send(ClientSocket, message, strlen(message), 0);
        recv(ClientSocket, message, sizeof(message) - 1, 0);
        ```
    }

    On the ClientSide
    {
        ** ClientSocket **
        This socket is used to send and receive messages to/from the server after the client connects

        ** ServerSocket **
        Clients don't quite have a ServerSocket,
        Clients only have a ClientSocket to communicate with the server

        so on the client side, both send and recv should be
        ```
        send(ClientSocket, message, strlen(message), 0);
        recv(CLientSocket, message, sizeof(message) - 1, 0);
        ```
    }

    
    */

    void FLEX_SetupForce(int IfServer_Port, int IfServer_BackLog, string IfClient_IPAddress, int IfClient_Port)
    {
        //setup a connection based on the MachineType

        if (MachineType == "server")
        {
            cout << fowo_cout.ConsoleQuick("report","attempt to setup as server") << endl;
            sOwOrver.Connection_Setup_Force(IfServer_Port, IfServer_BackLog);
            cout << fowo_cout.ConsoleQuick("success","setup successful") << endl;
        }
        else if (MachineType == "client")
        {
            cout << fowo_cout.ConsoleQuick("report","attempt to setup as server") << endl;
            clOwOnt.Connection_Setup_Force(IfClient_IPAddress, IfClient_Port);
            cout << fowo_cout.ConsoleQuick("success","setup successful") << endl;
        }
        else
        {
            cout << fowo_cout.ConsoleQuick("error","MachineType unclear") << endl;
        }
    }

    string FLEX_MessageSEND(string InMessage)
    {
        //sends a message, based on the machine type

        string OutString = "";

        if(MachineType == "server")
        {
            cout << fowo_string.shOwOrthand.ColorTextSegment("server send","y") << endl;
            OutString = sOwOrver.MessageSEND(InMessage);
        }
        else if (MachineType == "client")
        {
            cout << fowo_string.shOwOrthand.ColorTextSegment("client send","y") << endl;
            OutString = clOwOnt.MessageSEND(InMessage);
        }
        else
        {
            cout << fowo_cout.ConsoleQuick("error","MachineType is unclear, MachineType is : " + MachineType) << endl;
            OutString = "";
        }

        return OutString;
    }

    string FLEX_MessageRECV(int InBufferLength, bool IfFailForceExit)
    {
        //receives a message, based on the machine type

        string OutString = "";


        if(MachineType == "server")
        {
            cout << fowo_string.shOwOrthand.ColorTextSegment("server recv","y") << endl;
            OutString = sOwOrver.MessageRECV(InBufferLength, IfFailForceExit);
        }
        else if (MachineType == "client")
        {
            cout << fowo_string.shOwOrthand.ColorTextSegment("client recv","y") << endl;
            OutString = clOwOnt.MessageRECV(InBufferLength, IfFailForceExit);
        }
        else
        {
            cout << fowo_cout.ConsoleQuick("error","MachineType is unclear, MachineType is : " + MachineType) << endl;
            OutString = "";
        }

        return OutString;
    }

    void FLEX_Close()
    {
        //closes a connection, based on the MachineType

        if(MachineType == "server")
        {
            sOwOrver.Connection_Close();
        }
        else if (MachineType == "client")
        {
            clOwOnt.Connection_Close();
        }
        else
        {
            cout << fowo_cout.ConsoleQuick("error","MachineType is unclear, MachineType is : " + MachineType) << endl;
        }
    }

};

