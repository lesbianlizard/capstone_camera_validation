
/*************************************************
* Corruption Unit OPENCV Program
* Contributors: Kamron Ebrahimi, Aaron Walder, Pu Huang
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
#include "Distortion.hpp"
#include "Freeze.hpp"
#include "White.hpp"
#include "Translate.hpp"
#include "PracticalSocket.h"   
#include "config.h"
#include "Compare.hpp"

// my IP, moose1 "192.168.1.122"

#define COMP_IP "192.168.1.124"
#define ETHERNET true

enum distortion{FREEZE, WHITE, SHIFT};
enum distortion type;
std::mutex mtx;
cv::Mat* dframe;  // of type CV_8UC3
std::vector<Distortion*> dis(3);

void *handleIPC(void *threadid);
void *handleEthernet(void *threadid);
void dispatch(std::vector<std::string> cmd);
std::string GetMatType(const cv::Mat* mat);
bool isWhite(cv::Mat* dframe);
bool isCorrupt(cv::Mat* dframe, cv::Mat *dup);
bool isCorrupt2(cv::Mat* dframe, cv::Mat dup);


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

    string servAddress = argv[1]; // First arg: server address
    unsigned short servPort = Socket::resolveService(argv[2], "udp");
	

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

        cv::Mat frame, dup, send;
        cv:: Mat diff;

        // Prepare required variables
        cv::Point point;
        cv::Mat correlation;
        double max_val;


        std::vector < uchar > encoded;
        cv::VideoCapture cap(0); // Grab the camera
        cv::namedWindow(window, cv::WINDOW_AUTOSIZE);
        if (!cap.isOpened()) {
            cerr << "OpenCV Failed to open camera";
            exit(1);
        }

        clock_t last_cycle = clock();
        while (1) {
            cap >> frame;
            dup = frame.clone();
            dframe = &frame; 
            mtx.lock();
            dis[type]->run(dframe);
            mtx.unlock();

            /*
            if(dframe->size().width==0)continue; // simple integrity check; skip erroneous data...
            resize(*dframe, send, cv::Size(FRAME_WIDTH, FRAME_HEIGHT), 0, 0, cv::INTER_LINEAR);
            std::vector < int > compression_params;
            compression_params.push_back(cv::IMWRITE_JPEG_QUALITY);
            compression_params.push_back(jpegqual);

            cv::imencode(".jpg", send, encoded, compression_params);
            cv::imshow(window, send);
            int total_pack = 1 + (encoded.size() - 1) / PACK_SIZE;

            int ibuf[1];
            ibuf[0] = total_pack;
            sock.sendTo(ibuf , sizeof(int), servAddress, servPort);

            for (int i = 0; i < total_pack; i++)
                sock.sendTo( & encoded[i * PACK_SIZE], PACK_SIZE, servAddress, servPort);
            */
            
            cv::imshow(window2, dup);
            cv::imshow(window, *dframe);
            cv::waitKey(FRAME_INTERVAL);
            comparator.run(&dup, dframe);

            /*
            clock_t next_cycle = clock();
            double duration = (next_cycle - last_cycle) / (double) CLOCKS_PER_SEC;
            cout << "\teffective FPS:" << (1 / duration) << " \tkbps:" << (PACK_SIZE * total_pack / duration / 1024 * 8) << endl;
            cout << next_cycle - last_cycle;
            last_cycle = next_cycle;
            */
        }

    } catch (SocketException & e) {
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
1.) Get comparison working and integrated
2.) write uncorrupted and corrupted frames to struct
3.) Serialize the struct
4.) Get socket working with multiple serialized frames
5.) second queue based thread for sendign over socket and serializing the data
*/

std::string GetMatType(const cv::Mat* mat)
{
    const int mtype = mat->type();

    switch (mtype)
    {
    case CV_8UC1:  return "CV_8UC1";
    case CV_8UC2:  return "CV_8UC2";
    case CV_8UC3:  return "CV_8UC3";
    case CV_8UC4:  return "CV_8UC4";

    case CV_8SC1:  return "CV_8SC1";
    case CV_8SC2:  return "CV_8SC2";
    case CV_8SC3:  return "CV_8SC3";
    case CV_8SC4:  return "CV_8SC4";

    case CV_16UC1: return "CV_16UC1";
    case CV_16UC2: return "CV_16UC2";
    case CV_16UC3: return "CV_16UC3";
    case CV_16UC4: return "CV_16UC4";

    case CV_16SC1: return "CV_16SC1";
    case CV_16SC2: return "CV_16SC2";
    case CV_16SC3: return "CV_16SC3";
    case CV_16SC4: return "CV_16SC4";

    case CV_32SC1: return "CV_32SC1";
    case CV_32SC2: return "CV_32SC2";
    case CV_32SC3: return "CV_32SC3";
    case CV_32SC4: return "CV_32SC4";

    case CV_32FC1: return "CV_32FC1";
    case CV_32FC2: return "CV_32FC2";
    case CV_32FC3: return "CV_32FC3";
    case CV_32FC4: return "CV_32FC4";

    case CV_64FC1: return "CV_64FC1";
    case CV_64FC2: return "CV_64FC2";
    case CV_64FC3: return "CV_64FC3";
    case CV_64FC4: return "CV_64FC4";

    default:
        return "Invalid type of matrix!";
    }
}
