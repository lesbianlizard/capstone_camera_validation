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
#include "filters/Distortion.hpp"
#include "filters/Freeze.hpp"
#include "filters/White.hpp"
#include "filters/Translate.hpp"
#include "socket/PracticalSocket.h"   
#include "socket/config.h"
#include "Compare.hpp"
#include "proto/packet.pb.h"

using packet::FramePacket;


// my IP, moose1 "192.168.1.122"
// build prtocol buffer libraries from template .proto file 
// protoc -I=./ --cpp_out=./proto ./packet.proto
// ./corrupt 192.168.1.124 6666

enum distortion{FREEZE, WHITE, SHIFT};
enum distortion type;
std::mutex mtx;
cv::Mat* dframe;  // of type CV_8UC3
std::vector<Distortion*> dis(3);

void *handleIPC(void *threadid);
void *handleEthernet(void *threadid);
void dispatch(std::vector<std::string> cmd);

// uint8_t im_flag = 0;
// SocketMatTransmissionClient socketMat;
// std::mutex mtxEth;
// std::condition_variable flag;
// std::queue<cv::Mat*> myqueue;



int main(int argc, char * argv[]) {
    if ((argc < 3) || (argc > 3)) { // Test for correct number of arguments
        cerr << "Usage: " << argv[0] << " <Server> <Server Port>\n";
        exit(1);
    }



    std::string servAddress = argv[1]; // First arg: server address
    unsigned short servPort = Socket::resolveService(argv[2], "tcp");
    std::vector <uchar> compImg;         


    type = SHIFT;
    std::string window = "corrupt";
    std::string window2 = "normal";
        
    Freeze freeze(100);
    White white(50, 255, FRAME_HEIGHT, FRAME_WIDTH); 
    Translate translate(50, FRAME_HEIGHT, FRAME_WIDTH, 15, 15); 
    Compare comparator;

    cv::namedWindow(window, cv::WINDOW_AUTOSIZE);
    
    dis[0] = &freeze;
    dis[1] = &white;
    dis[2] = &translate;

    // spawn the IPC socket thread
    pthread_t ipcHandler;
    pthread_t ethernetHandler;
    pthread_create(&ipcHandler, NULL, handleIPC, (void *)1); 
    // pthread_create(&ethernetHandler, NULL, handleEthernet, (void *)2); 

    try {
        UDPSocket sock;
        int jpegqual =  ENCODE_QUALITY; // Compression Parameter

        cv::Mat frame, dup;
        cv::Mat cp1, cp2;
        std::vector <uchar> encode; 
        cv::VideoCapture cap(0); // Grab the camera
        cv::namedWindow(window, cv::WINDOW_AUTOSIZE);
        if (!cap.isOpened()) {
            cerr << "OpenCV Failed to open camera";
            exit(1);
        }

        //clock_t last_cycle = clock();
        while (1) {
            cap >> frame;
            dup = frame.clone();
            dframe = &frame; 
            mtx.lock();
            dis[type]->run(dframe);
            mtx.unlock();

            //if(dframe->size().width==0)continue; // simple integrity check; skip errosneous data...
            //resize(*dframe, cp1, cv::Size(FRAME_WIDTH, FRAME_HEIGHT), 0, 0, cv::INTER_LINEAR);
            //resize(dup, cp2, cv::Size(FRAME_WIDTH, FRAME_HEIGHT), 0, 0, cv::INTER_LINEAR);




            std::vector <int> compression_params;
            compression_params.push_back(cv::IMWRITE_JPEG_QUALITY);
            compression_params.push_back(jpegqual);

            //cv::imencode(".jpg", dup, encode, compression_params);
            cv::imencode(".jpg", *dframe, encode, compression_params);

std::string dat; 
cv::Mat raw; 
FramePacket packet;

// set the trivial fields
packet.set_rows(dframe->rows);
packet.set_cols(dframe->cols);
packet.set_elt_type(dframe->type());
packet.set_elt_size(encode.size());

// set the matrix's raw data
packet.set_mat_dataa(encode.data(), encode.size());

  if (packet.SerializeToString(&dat)) {
      if(packet.ParseFromString(dat)){
          

        //allocate the matrix
        raw.create(packet.rows(), packet.cols(), packet.elt_type());


        //set the matrix's data
        std::copy(reinterpret_cast<unsigned char *>(
            const_cast<char *>(packet.mat_dataa().data())),
            reinterpret_cast<unsigned char *>(
            const_cast<char *>(packet.mat_dataa().data()) + packet.elt_size()),
            raw.data);

        // decompress the image
        cv::Mat frame = imdecode(raw, cv::IMREAD_COLOR);
        cv::imshow(window, frame);
      }
  }






            int total_pack = 1 + (encode.size() - 1) / PACK_SIZE;
            int ibuf[1];
            ibuf[0] = total_pack;
            sock.sendTo(ibuf , sizeof(int), servAddress, servPort);
            for (int i = 0; i < total_pack; i++)
                sock.sendTo( & encode[i * PACK_SIZE], PACK_SIZE, servAddress, servPort);

        cv::waitKey(FRAME_INTERVAL);
            /*
            cv::imshow(window2, dup);
            cv::imshow(window, *dframe);
            
            comparator.run(&dup, dframe);
            */
        }

    }catch (SocketException & e) {
        cerr << e.what() << endl;
        exit(1);
    }

    return 0; 
}

void *handleIPC(void *threadid)
{
    //  Prepare our context and socket
    zmq::context_t context (1);
    zmq::socket_t socket (context, ZMQ_REP);
    socket.bind ("tcp://*:5555");

    while (true) {
        zmq::message_t request;
        //  Wait for next request from client
        socket.recv (&request);

        // process the data input
        std::string cmd_str = std::string(static_cast<char*>(request.data()), request.size());
        std::cout<< cmd_str <<std::endl;
        std::stringstream ss(cmd_str);
        std::vector<std::string> cmd;
        while(ss.good())
        {   
            std::string substr;
            getline( ss, substr, ',' );
            cmd.push_back( substr );
        }
        for(int i=0; i < cmd.size(); i++)
            std::cout << cmd.at(i) << ' ';


        // lock the mutex and update the filter
        mtx.lock();
        dispatch(cmd);
        mtx.unlock();

        //  Send reply back to client
        zmq::message_t reply (3);
        memcpy (reply.data (), "ACK", 3);
        socket.send (reply);
    }
}

// Format (use .join(',') method on a dynamically constructed list object) to produce these commands
// Freeze:   ['1', '1/0', 'dur(ms)']
// White:    ['2', '1/0', 'dur(ms)', 'shade(0-255)']
// Translate ['3', '1/0', 'dur(ms)', 'x_offset', 'y_offset']
// Introduce better error handling here!
void dispatch(std::vector<std::string> cmd)
{
    switch(stoi(cmd[0],nullptr))
    {
        case 1:
            type = FREEZE; 
            dis[type]->update(cmd);
            break;
        case 2:
            type = WHITE; 
            dis[type]->update(cmd);
            break;
        case 3:
            type = SHIFT; 
            dis[type]->update(cmd);
            break;
    }
}


/* Migrate h264 codec compression to here
void *handleEthernet(void *threadid)
{
    while(1)
    {
        mtxEth.lock();
        if(im_flag == 1){
            socketMat.transmit(*dframe);
            im_flag &= 0;
        }
        mtxEth.unlock();
    }
}
*/




            /*
            clock_t next_cycle = clock();
            double duration = (next_cycle - last_cycle) / (double) CLOCKS_PER_SEC;
            cout << "\teffective FPS:" << (1 / duration) << " \tkbps:" << (PACK_SIZE * total_pack / duration / 1024 * 8) << endl;
            cout << next_cycle - last_cycle;
            last_cycle = next_cycle;
            */


            /*      
  std::string dat; 
  cv::Mat newFrame; 
  FramePacket packet;
// set the trivial fields
packet.set_rows(dframe->rows);
packet.set_cols(dframe->cols);
packet.set_elt_type(dframe->type());
packet.set_elt_size((int)dframe->elemSize());

// set the matrix's raw data
size_t dataSize = dframe->rows * dframe->cols * dframe->elemSize();
packet.set_mat_dataa(dframe->data, dataSize);

  if (packet.SerializeToString(&dat)) {
      if(packet.ParseFromString(dat)){
          

        //allocate the matrix
        newFrame.create(packet.rows(),
        packet.cols(),
        packet.elt_type());

        //set the matrix's data
        size_t dataSize = packet.rows() *  packet.cols() * packet.elt_size();

        std::copy(reinterpret_cast<unsigned char *>(
            const_cast<char *>(packet.mat_dataa().data())),
            reinterpret_cast<unsigned char *>(
            const_cast<char *>(packet.mat_dataa().data()) + dataSize),
            newFrame.data);

        cv::imshow(window, newFrame);

      }
  }
*/
