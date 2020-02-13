/*************************************************
* Corruption Unit OPENCV Program
* Contributors: Kamron Ebrahimi
************************************************/

#include <stdio.h>
#include <opencv2/opencv.hpp>
#include <iostream>
#include <unistd.h> 
#include <stdio.h> 
#include <pthread.h>  
#include <cstdlib>  
#include <mutex>     
#include <sched.h>
#include <zmq.hpp>    
#include <queue>  
#include <condition_variable>
#include "socket/PracticalSocket.h"  
#include "socket/config.h"
#include "Compare.hpp"
#include "proto/packet.pb.h"

#define BUF_LEN 65540 // Larger than maximum UDP packet size

using packet::FramePacket;

// my IP, moose1 "192.168.1.122"
// build prtocol buffer libraries from template .proto file 
// protoc -I=./ --cpp_out=./proto ./packet.proto
// ./corrupt 192.168.1.124 6666

void *handleEthernet(void *threadid);
void *handleComparison(void *threadid);
void ReadXBytes(int socket, unsigned int x, void* buffer);

std::mutex mtx, mtx2;
std::queue<std::string> q;
std::queue<FramePacket> q2;
Compare comparator;

int main(int argc, char * argv[]) {

    if (argc < 2) { // Test for correct number of parameters
        cerr << "Usage: " << argv[0] << " <Server Port>" << endl;
        exit(1);
    }
    
    TCPServerSocket serverSock(atoi(argv[1]));
    TCPSocket* sock = serverSock.accept();
    sock->getForeignAddress();
    sock->getForeignPort(); 

    pthread_t ethernetHandler;
    pthread_t compareHandler;
    pthread_create(&compareHandler, NULL, handleComparison, (void *)1);
    pthread_create(&ethernetHandler, NULL, handleEthernet, (void *)2);


    try {
        int recvMsgSize;
        char buf[BUF_LEN];
        string dat; 
        while (1) {
            // Block until receive message from a client
            do {
                recvMsgSize = sock->recv(buf, BUF_LEN);
            } while (recvMsgSize > sizeof(int));
            int length = ((int * ) buf)[0];
            char* buffer = 0;
            buffer = new char[length];
            ReadXBytes(sock->sockDesc, length, (void*)buffer); 

            // copy binary data to a string
            for(int i = 0; i < length; i++)
                dat += buffer[i];   

            // push string into queue
            mtx.lock();
            q.push(dat);
            mtx.unlock();
            delete [] buffer;
            dat.clear();
    } // while

    } catch (SocketException & e) {
        cerr << e.what() << endl;
        exit(1);
    }

    return 0;
}


void ReadXBytes(int socket, unsigned int x, void* buffer)
{
    int bytesRead = 0;
    int result;
    while (bytesRead < x)
    {
        result = read(socket, buffer + bytesRead, x - bytesRead);
        if (result < 1 )
        {
            cout <<"error " <<endl;
            while(1){}
        }

        bytesRead += result;
    }
}


void *handleEthernet(void *threadid)
{

while(1){
        
    string ser_data;
    bool dat = false; 
    mtx.lock();
    if (!q.empty())
    {
        ser_data = q.front();
        q.pop(); 
        dat = true; 
    }
    mtx.unlock();
    
    if(dat){
        FramePacket packet;
        if(packet.ParseFromString(ser_data)){  
            mtx2.lock();
            q2.push(packet);
            mtx2.unlock();
        }
        else
            cout << "failed to deserialize data" << endl; 

        ser_data.clear();
    }
} // while

}


void *handleComparison(void *threadid)
{

    while(1){
        FramePacket packet; 
        bool dat = false; 
        mtx2.lock();
        if (!q2.empty())
        {
            packet = q2.front();
            q2.pop();
            dat = true;
        }
        mtx2.unlock();

            if(dat){
                cv::Mat rawA, rawB;    

                //allocate the matrix
                rawA.create(packet.rows(), packet.cols(), packet.elt_type());
                rawB.create(packet.rowsb(), packet.colsb(), packet.elt_type());

                //set the matrix's data
                std::copy(reinterpret_cast<unsigned char *>(
                    const_cast<char *>(packet.mat_dataa().data())),
                    reinterpret_cast<unsigned char *>(
                    const_cast<char *>(packet.mat_dataa().data()) + packet.elt_sizea()),
                    rawA.data);

                std::copy(reinterpret_cast<unsigned char *>(
                    const_cast<char *>(packet.mat_datab().data())),
                    reinterpret_cast<unsigned char *>(
                    const_cast<char *>(packet.mat_datab().data()) + packet.elt_sizeb()),
                    rawB.data);

                // decompress the image
                try{
                cv::Mat frameA = imdecode(rawA, cv::IMREAD_COLOR);
                cv::Mat frameB = imdecode(rawB, cv::IMREAD_COLOR);

                cv::imshow("Normal", frameA);
                cv::imshow("Corrupt", frameB);
                cv::waitKey(1);
                comparator.run(&frameA, &frameB);
                }
                catch(cv::Exception & e){
                    cout << "error decompressing the image" << endl;
                }
            }
        }   
}

