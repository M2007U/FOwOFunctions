//WARNING : when compiling, use the following command because this program uses threads :
//g++ -std=c++11 -o Jon CSC487-Network-Flex.cpp -pthread

#include <iostream>
#include <iomanip>

#include <stdlib.h>
#include <cstdlib>

#include <functional>

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
#include <thread>

#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>

#include "FOwOFunctions_2024.cpp"

using namespace std;

FOwO fowo;
string USER_ServerOrClient;
string BoxSEND = "";
string BoxRECV = "";

void threadFunction_send( thread &In_ThreadFunction_Recv ) //refer to int main void
{
    while(BoxSEND != "/exit") //here we assume that if the user enters "/exit", the program will end
    {
        BoxSEND = "";
        cout << "send? : ";
        getline(cin,BoxSEND);

        //this is where we decide how the machine will react with the user
        if (BoxSEND == "something")
        {
            //do something
        }
        else if ( BoxSEND == "something else")
        {
            //do another thing
        }
        else
        {
            //just send the message
            if(BoxSEND.length() > 0)
            {
                if (BoxSEND.at(0) == '/') //here we assume that if the first charracter of the user input is a /, then it is a command
                {
                    cout << fowo.strOwOng.shOwOrthand.ColorText("y") << "you can try \"/help\" to have the entire list of commands" << fowo.strOwOng.shOwOrthand.ColorText("d") << endl;
                }
                else
                {
                    
                    //encryption mechanisms goes here
                    //here we can have something like outMessage = SomeEncryption(BoxSEND)
                    //for now lets go raw
                    string outMessage = "";
                    outMessage = BoxSEND;
                    
                    fowo.netOwOrk.FLEX_MessageSEND(outMessage);
                    cout << fowo.strOwOng.shOwOrthand.ColorText("g") << endl;
                    cout << fowo.strOwOng.shOwOrthand.ColorText("y") << "sent : " << outMessage << fowo.strOwOng.shOwOrthand.ColorText("d") << endl;
                    cout << fowo.strOwOng.shOwOrthand.ColorText("d") << endl;
                }
 
            }
        }
        
    }
}

void threadFunction_recv() //refer to int main void
{
    while(true) //or we can make a user command to act as a break switch to break the while loop, refer to threadFunction_send
    {
        BoxRECV = fowo.netOwOrk.FLEX_MessageRECV(32768,true); //waits for a message to arrive

        //decryption mechanism goes here
        //here we can have something like inMessage = SomeDecryption(BoxRECV)
        //for now lets go raw
        string inMessage = "";
        inMessage = BoxRECV;

        //at this point, BoxRECV is decrypted
        cout << endl << fowo.strOwOng.shOwOrthand.ColorText("y") << "recv  : " << BoxRECV << fowo.strOwOng.shOwOrthand.ColorText("d") << endl;

        //do we print it out ?
        //or it can be interpreted as a command ?
        if (BoxRECV == "something")
        {
            //do something
        }
        else if ( BoxRECV == "something else")
        {
            //do another thing
        }
        else
        {
            //just send the message
            if(BoxRECV.length() > 0)
            {
                if (BoxRECV.at(0) == '/') //here we assume that if the first charracter of the user input is a /, then it is a command
                {
                    cout << fowo.strOwOng.shOwOrthand.ColorText("y") << "you can try \"/help\" to have the entire list of commands" << fowo.strOwOng.shOwOrthand.ColorText("d") << endl;
                }
                else
                {   
                    fowo.netOwOrk.FLEX_MessageSEND(inMessage);
                    cout << fowo.strOwOng.shOwOrthand.ColorText("g") << endl;
                    cout << fowo.strOwOng.shOwOrthand.ColorText("y") << "sent : " << inMessage << fowo.strOwOng.shOwOrthand.ColorText("d") << endl;
                    cout << fowo.strOwOng.shOwOrthand.ColorText("d") << endl;
                }

            }
        }
    }
    
}

int main ()
{
    //ask the user for the role of this machine
    cout << fowo.cOwOut.ConsoleQuick("prompt","Run as Server (s) or Client (c) ?") << endl;
    getline(cin, USER_ServerOrClient);

    if(USER_ServerOrClient.length() == 0)
    {
        cout << fowo.cOwOut.ConsoleQuick("error","choice needs to be either \'s\' for server or \'c\' for client, will not run, will terminate") << endl;
        exit(1);
    }

    if(USER_ServerOrClient.at(0) == 's')
    {
        //Attempt to setup as a server
        cout << fowo.cOwOut.ConsoleQuick("report","attempt to setup as server") << endl;
        fowo.netOwOrk.sOwOrver.Connection_Setup_Force(8000,16);
        cout << fowo.cOwOut.ConsoleQuick("success","setup successfull") << endl;
        fowo.netOwOrk.MachineType = "server";
    }
    else if (USER_ServerOrClient.at(0) == 'c')
    {
        //Attempt to setup as a client
        cout << fowo.cOwOut.ConsoleQuick("prompt","Target Server IP Address ?") << endl;
        string temp = "";
        getline(cin,temp);
        cout << fowo.cOwOut.ConsoleQuick("report","attempt to setup as client") << endl;
        fowo.netOwOrk.clOwOnt.Connection_Setup_Force(temp,8000);
        cout << fowo.cOwOut.ConsoleQuick("success","setup successfull") << endl;
        fowo.netOwOrk.MachineType = "client";

        //in the lab, the IP address are usually the following
        //169.254.243.102
        //169.254.80.249
    }
    else
    {
        cout << fowo.cOwOut.ConsoleQuick("error","choice needs to be either \'s\' for server or \'c\' for client, will not run, will terminate") << endl;
        exit(1);
    }


    //now split into different threads, one will listen, one will send
    thread thread_recv (threadFunction_recv); //when the machine receives something, it can do something or print it out
    thread thread_send (threadFunction_send, ref(thread_recv)); //when the user enters a string, it can treat it as a command for send it out as a message

    thread_send.join();

    return 0;
}