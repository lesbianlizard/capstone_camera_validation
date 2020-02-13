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
void ReadXBytes(int socket, unsigned int x, void* buffer);

// uint8_t im_flag = 0;
// SocketMatTransmissionClient socketMat;
// std::mutex mtxEth;
// std::condition_variable flag;
// std::queue<cv::Mat*> myqueue;

int main(int argc, char * argv[]) {

    if (argc < 2) { // Test for correct number of parameters
        cerr << "Usage: " << argv[0] << " <Server Port>" << endl;
        exit(1);
    }

/*
    std::string servAddress = argv[1]; // First arg: server address
    SocketMatTransmissionClient sock;
	if (socketMat.socketConnect(servAddress, atoi(argv[2])) < 0)
	{
		return 0;
	}
    */

    TCPServerSocket serverSock(atoi(argv[1]));
    TCPSocket* sock = serverSock.accept();
    sock->getForeignAddress();
    sock->getForeignPort(); 

    cv::namedWindow("normal", cv::WINDOW_AUTOSIZE);
    cv::namedWindow("corrupted", cv::WINDOW_AUTOSIZE);

    try {
        //char buffer[BUF_LEN];       // Buffer for echo string
        int recvMsgSize;            // Size of received message
        string sourceAddress;       // Address of datagram source
        unsigned short sourcePort;  // Port of datagram source
        clock_t last_cycle = clock();
        std::string dat;

        /*
        while (1) {
            // Block until receive message from a client
            do {
                //recvMsgSize = sock.recvFrom(buffer, BUF_LEN, sourceAddress, sourcePort);
                recvMsgSize = sock->recv(buffer, BUF_LEN);
            } while (recvMsgSize > sizeof(int));
            int total_pack = ((int * ) buffer)[0];

            cout << "expecting length of packs:" << total_pack << endl;
            std::string dat 
            char * longbuf = new char[PACK_SIZE * total_pack];
            for (int i = 0; i < total_pack; i++) {
                //recvMsgSize = sock.recvFrom(buffer, BUF_LEN, sourceAddress, sourcePort);
                recvMsgSize = sock->recv(buffer, BUF_LEN);
                if (recvMsgSize != PACK_SIZE) {
                    cerr << "Received unexpected size pack:" << recvMsgSize << endl;
                    continue;
                }
                //memcpy( & longbuf[i * PACK_SIZE], buffer, PACK_SIZE);
            }*/
            
    //    while (1) {
    //         // Block until receive message from a client
    //         do {
    //             //recvMsgSize = sock.recvFrom(buffer, BUF_LEN, sourceAddress, sourcePort);
    //             recvMsgSize = sock->recv(buffer, BUF_LEN);
    //         } while (recvMsgSize > sizeof(int));

    //         int dataSize = ((int * ) buffer)[0];
    //         cout << "expecting length of packs:" << dataSize << endl;
    //         std::string dat;
    //         vector<char> data; 
    //         uint32_t bytesRead;
    //         uint32_t total = 0; 
    //         while(total < dataSize){
    //             bytesRead = sock->recv(buffer, BUF_LEN);
    //             total += bytesRead; 
    //             for(int i = 0; i < bytesRead ; i++){
    //                 dat += buffer[i];
    //             }
    //             cout << "bytes read: " << bytesRead << endl;
    //             cout << "size of data: " << dat.size() <<endl; 
    //         }

    //         cout << "size of data: " << dat.size() <<endl;
    //         dat.clear(); 
    //    }


char buf[BUF_LEN]; 
while (1) {
            // Block until receive message from a client
        do {
            recvMsgSize = sock->recv(buf, BUF_LEN);
        } while (recvMsgSize > sizeof(int));
        int length = ((int * ) buf)[0];
        char* buffer = 0;
        buffer = new char[length];
        ReadXBytes(sock->sockDesc, length, (void*)buffer); 
        string datum;

        for(int i = 0; i < length; i++)
            datum += buffer[i];
        

        cout << "size of data: " <<  datum.size() <<endl; 
        


        cv::Mat rawA, rawB;     
        FramePacket packet;


      if(packet.ParseFromString(datum)){  
        cout << "deserialized data" << endl;


        //allocate the matrix
        rawA.create(packet.rows(), packet.cols(), packet.elt_type());
        rawB.create(packet.rows(), packet.cols(), packet.elt_type());

        //set the matrix's data
        std::copy(reinterpret_cast<unsigned char *>(
            const_cast<char *>(packet.mat_dataa().data())),
            reinterpret_cast<unsigned char *>(
            const_cast<char *>(packet.mat_dataa().data()) + packet.elt_size()),
            rawA.data);

        std::copy(reinterpret_cast<unsigned char *>(
            const_cast<char *>(packet.mat_datab().data())),
            reinterpret_cast<unsigned char *>(
            const_cast<char *>(packet.mat_datab().data()) + packet.elt_size()),
            rawB.data);

        // decompress the image
        cv::Mat frameA = imdecode(rawA, cv::IMREAD_COLOR);
        cv::Mat frameB = imdecode(rawB, cv::IMREAD_COLOR);

        cv::imshow("normal", frameA);
        cv::imshow("corrupted", frameB);
      }
      else{
          cout<<"failed to deserialize data"<<endl;
      }
    
        cv::waitKey(1);
      //empty the string
      datum.clear();







            /*cout << "Received packet from " << sourceAddress << ":" << sourcePort << endl;

            cv::Mat rawData = cv::Mat(1, length, CV_8UC1, buffer);
            cv::Mat frame = cv::imdecode(rawData, cv::IMREAD_COLOR);
            cv::imshow("normal", frame);
        

            cv::waitKey(1);
            clock_t next_cycle = clock();
            double duration = (next_cycle - last_cycle) / (double) CLOCKS_PER_SEC;
            //cout << "\teffective FPS:" << (1 / duration) << " \tkbps:" << (PACK_SIZE * total_pack / duration / 1024 * 8) << endl;

            last_cycle = next_cycle;
            */
        delete [] buffer;
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
        }

        bytesRead += result;
    }
}
