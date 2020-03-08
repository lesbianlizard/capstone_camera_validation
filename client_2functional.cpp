#include "opencv2/opencv.hpp"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <chrono>
#include <iostream>
#include <fstream>
#include <vector> 
#include <pthread.h>  
#include <zmq.hpp> 

using namespace cv;
using namespace std;


#define IMG_WIDTH 640
#define IMG_HEIGHT 480
#define PACKAGE_NUM 2
#define PORT1 8079
#define PORT2 8081
#define MB (2591 * 1944) / 5
#define BUFFER_SIZE IMG_WIDTH*IMG_HEIGHT*3/PACKAGE_NUM

std::vector<std::string> cmd; // vector of strings from the GUI

struct sentbuf
{
    char buf[BUFFER_SIZE];
    int flag;
};

Mat translateImg(Mat& img, int offsetx, int offsety) { // function used to translate image
    Mat trans_mat = (Mat_<double>(2, 3) << 1, 0, offsetx, 0, 1, offsety);
    warpAffine(img, img, trans_mat, img.size());
    return img;
}


Mat shift_up(Mat img_corr, int percentage){ // call this function to shift up (user percentage not pixels)
    cout << endl <<  "Indicator 1: shift " << endl;
    cout << "Shift indicator: 12 - UP" << endl;
    cout << "Percentage shift indicator: " << percentage << endl;
    float up_percent = (percentage / 100.0)*480;
    up_percent = round(up_percent);
    cout << "Shifted UP by: " << up_percent << " = " << percentage << "%" << endl;
    //cvtColor(orig, img_corr, 0);
    translateImg(img_corr, 0, -up_percent); // calls translate function
    return img_corr;
} // shift up

Mat shift_left(Mat img_corr, int percentage){
    cout << endl << "Indicator 1: shift " << endl;
    cout << "Shift indicator: 11 - LEFT" << endl;
    cout << "Percentage shift indicator: " << percentage << endl;
    float left_percent = (percentage / 100.0)*640.0;
    left_percent = round(left_percent);
    cout << "Shifted LEFT by: " << left_percent << " = " << percentage << "%" << endl;
    //cvtColor(orig, img_corr, 0);
    translateImg(img_corr, -left_percent, 0);
    return img_corr;
} // shift left

Mat shift_right(Mat img_corr, int percentage){
    cout << endl << "Indicator 1: shift " << endl;
    cout << "Shift indicator: 10 - RIGHT" << endl;
    cout << "Percentage shift indicator: " << percentage << endl;
    float right_percent = (percentage / 100.0)*640.0;
    right_percent = round(right_percent);
    cout << "Shifted RIGHT by: " << right_percent << " = " << percentage << "%" << endl;
    //cvtColor(orig, img_corr, CV_8UC3);
    translateImg(img_corr, right_percent, 0);
    //img_corr.convertTo(img_corr, CvType.CV_8uC3);
    return img_corr;
} // shift right

Mat shift_down(Mat img_corr, int percentage){
    cout << endl << "Indicator 1: shift " << endl;
    cout << "Shift indicator: 13 - DOWN" << endl;
    cout << "Percentage shift indicator: " << percentage << endl;
    float down_percent = (percentage / 100.0)*480.0;
    down_percent = round(down_percent);
    cout << "Shifted DOWN by: " << down_percent << " = " << percentage << "%" << endl;
    //cvtColor(orig, img_corr, 0);
    translateImg(img_corr, 0, down_percent);
    return img_corr;
} //  shift down

Mat discolor_red(){
    cout << "Indicator 3: Discolor " << endl;
    Mat img_corr = Mat(480, 640, CV_8UC3, Scalar(0,0,255));
    return img_corr;
} // discolor red

Mat discolor_green(){
    cout << "Indicator 3: Discolor " << endl;
    Mat img_corr = Mat(480, 640, CV_8UC3, Scalar(0,255,0));
    return img_corr;
} // discolor green

Mat discolor_blue(){
    cout << "Indicator 3: Discolor " << endl;
    Mat img_corr = Mat(480, 640, CV_8UC3, Scalar(255,0,0));
    return img_corr;
} // discolor blue

Mat discolor_white(){
    cout << "Indicator 3: Discolor " << endl;
    Mat img_corr = Mat(480, 640, CV_8UC3, Scalar(255,255,255));
    return img_corr;
} // discolor white

struct split_args
{
  Mat camera_frame;
  int frame_id;
  int socket;  
};

// splits camera signal (original away from corruption) gorilla
//void split(Mat camera_frame, Mat corruption_frame, int frame_id, int socket){
void* split(void* args){
    struct split_args *args_in = (struct split_args*)args;
    Mat camera_frame = args_in->camera_frame;
    int frame_id = args_in->frame_id;
    int socket = args_in->socket;
    
	int buffer_for_size[1] = {0};
	vector <uchar> buffer;
	buffer.resize(MB);
	imencode(".jpg", camera_frame, buffer);
	int buffer_size = buffer.size();
	buffer_for_size[0] = buffer_size;
	send(socket, &buffer_for_size, sizeof(buffer_for_size), 0);

	size_t l = buffer_size;
	send(socket, buffer.data(), l, 0);
	send(socket, &frame_id, sizeof(frame_id), 0);
    
    return NULL;
}

int socket_setup(uint16_t portnum){
	int opt = 1;

	int sockfd = socket(PF_INET,SOCK_STREAM,IPPROTO_TCP);
	if(sockfd == -1){
		printf("error in socket(): %s\n", strerror(errno));
	}

	if(setsockopt(sockfd, SOL_SOCKET, SO_REUSEADDR | SO_REUSEPORT, &opt, sizeof(opt))){
		printf("error in setsockopt(): %s\n", strerror(errno));
	}

	struct sockaddr_in address;
	int addrlen = sizeof(address);
	address.sin_family = AF_INET;
	address.sin_addr.s_addr = INADDR_ANY;
	address.sin_port = htons(portnum);
	int bind1 = bind(sockfd, (struct sockaddr*)&address, addrlen);
	if(bind1 < 0){
		printf("error in bind: %s\n", strerror(errno));
	}

	int listen1 = listen(sockfd, 3);
    if(listen1 < 0){
		printf("error in listen1: %s\n", strerror(errno));
	}

	int new_socket = accept(sockfd, (struct sockaddr*)&address, (socklen_t*)&addrlen);
	return new_socket;
	
}

//IPC handler
/**
 * Kamron Ebrahimi - 2020
 * **/
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
        //std::cout<< cmd_str <<std::endl;
        std::stringstream ss(cmd_str);
        //std::vector<std::string> cmd;
        while(ss.good())
        {   
            std::string substr;
            getline( ss, substr, ',' );
            cmd.push_back( substr );
        }
    
        for(unsigned int i=0; i < cmd.size(); i++)
            std::cout << "i: " << i << " " << cmd.at(i) << ' ';

        // lock the mutex and update the filter
     
        //  Send reply back to client
        zmq::message_t reply (3);
        memcpy (reply.data (), "ACK", 3);
        socket.send (reply);
    }
}

int main()
{
    // Create sockets for image transmission
    int socket1 = socket_setup(PORT1);
    if (errno != 0)
    {
        printf("socket1 connection error: %s\n", strerror(errno));
    }
    printf("Done setting up socket 1\n");
    usleep(1000000);
    printf("Beginning setup socket 2\n");
    int socket2 = socket_setup(PORT2);
    if (errno != 0)
    {
        printf("socket2 connection error: %s\n", strerror(errno));
    }
    
    printf("Done setting up socket 2\n");

    pthread_t ipcHandler;
    pthread_t thread_send_orig;
    pthread_t thread_send_corrupt;
    pthread_create(&ipcHandler, NULL, handleIPC, (void *)1); 
    
    struct split_args args_send_orig;
    struct split_args args_send_corrupt;

    int height = 480; // defines height of image
    int width = 640; // defines width of image
    int frame_num = 0;
    int frame_id = 0;
    //int frame_id2 = 0;
    ofstream myfile;
    myfile.open("corr_log.txt"); // log of corruption done
    
    cv::VideoCapture capture; // video capture object
    capture.open(0);
    //VideoCapture *cap2 = &capture;
    capture.set(CAP_PROP_FRAME_WIDTH, width); // set width
    capture.set(CAP_PROP_FRAME_HEIGHT, height); // set height
    capture.set(CAP_PROP_FPS,60); // set FPS of corruption system
    
    cv::Mat image; // original frame
    cv::Mat prev_frame; // previous frame for freeze
    cv::Mat img_corr; // corrupted frame
    
    capture >> image;
    //int percentage = 5;
     //int frz_time = 5; 
    //cout << "frame num: " << frame_num << endl;
    frame_num = 0;
    //global_str = "5";
    cmd.push_back("5"); // set an original value for the function
    cmd.push_back("4");
    int corr_type = 0;
    int enable = 0;
    int ui_corr_type = 0;
    int ui_corr_type_prev = 0;
    //int freeze_counter = 0;
    int freeze_target = 0;
    int freeze_check = 0;
    
    while (1)
    {
        frame_num++; // incrememnet frame num each loop
        frame_id++; // increment frame_id each loop
        myfile << frame_num << ": ";
        if (!capture.isOpened()) // if video capture object fails, leave program
            return 0;
            
        prev_frame = image;// set previous frame to current frame
        capture >> image; // capture frame 
        
        // Show frame number on screen
        putText(image, to_string(frame_num), Point(550, 50), FONT_HERSHEY_COMPLEX_SMALL, 1, Scalar(0,0,0), 4, 8, false); 
        
        args_send_orig.camera_frame = image;
        args_send_orig.frame_id = frame_id;
        args_send_orig.socket = socket1;
        
        pthread_create(&thread_send_orig, NULL, split, &args_send_orig);
        
        // receiving data from web GUI
        ui_corr_type_prev = ui_corr_type;
        ui_corr_type = stoi(cmd[0], nullptr);
        
        if (ui_corr_type != ui_corr_type_prev)
        {
            corr_type = ui_corr_type;
        }
        
        enable = stoi(cmd[1], nullptr);

        //cout << "CMD[0] " << val << endl;
        // if freeze indicator
        if(corr_type == 1)
        {  
            if(enable == 1)
            {
                if (freeze_check == 0)
                {
                    freeze_target = frame_num + stoi(cmd[2], nullptr);
                    freeze_check++;
                    img_corr = image;
                }
                
                if (frame_num == freeze_target)
                {
                    corr_type = 0;
                    freeze_check = 0;
                }
                
                myfile << frame_num << ": freeze" << endl;
                //freeze(prev_frame, val, socket1, socket2,  capture, frame_num, frame_id, myfile);
              
            }
            else{
                img_corr = image; // set corrupted image as original if no corruption

                //putText(image, to_string(frame_num), Point(550, 50), FONT_HERSHEY_COMPLEX_SMALL, 1, Scalar(0,0,0), 4, 8, false); 
                //imshow("Corr: Original", image);
                //split(image, img_corr, frame_id, socket);
                    //socketMat.transmit(image);
                //imshow("corruption2", image);
                //socketMat.transmit(img_corr);
            }
        }
        
        // if discoloration indicator 
        else if(corr_type == 2){
            
            if(enable == 1){
                myfile << frame_num << ": white" << endl;
                img_corr = discolor_white();
                //putText(image, to_string(frame_num), Point(550, 50), FONT_HERSHEY_COMPLEX_SMALL, 1, Scalar(0,0,0), 4, 8, false); 
                //imshow("Corr: Original", image);
                //split(image, img_corr, frame_id, socket);
                ////imshow("corruption2", img_corr);
                //socketMat.transmit(img_corr);
            }
            else{
                  img_corr = image; // set corrupted image as original if no corruption

                //putText(image, to_string(frame_num), Point(550, 50), FONT_HERSHEY_COMPLEX_SMALL, 1, Scalar(0,0,0), 4, 8, false); 
                //imshow("Corr: Original", image);
                //split(image, img_corr, frame_id, socket);
                    ////socketMat.transmit(image);
                ////imshow("corruption2", image);
                //socketMat.transmit(img_corr);
            }
        }
        
        // if shift indicator 
        else if(corr_type == 3){
            
            if(enable == 1){
                int offx = stoi(cmd[3], nullptr);
                int offy = stoi(cmd[4], nullptr);
                //split(image, image, frame_id, socket);
                //putText(image, to_string(frame_num), Point(550, 50), FONT_HERSHEY_COMPLEX_SMALL, 1, Scalar(0,0,0), 4, 8, false); 
                //imshow("Corr: Original", image);
                if(offx > 0){ // shift right
                    float percentagex = (offx/640.0)*100.0;
                    myfile << frame_num << ": shift right: " << percentagex << "%"<< endl;
                    Mat image2 = image; 
                    img_corr = shift_right(image, percentagex);
              
                }
                else if(offx < 0){ // shift left
                    float percentagex = (-offx/640.0)*100.0;
                    myfile << frame_num << ": shift left: " << percentagex << "%"<< endl;
                    Mat image2 = image;
                    img_corr = shift_left(image, percentagex);
                  
                }
                if(offy > 0){ // shift down
                    float percentagey = (offy/480.0)*100.0;
                    myfile << frame_num << ": shift down: " << percentagey << "%"<< endl;
                    Mat image2 = image;
                    img_corr = shift_down(image, percentagey);
                   
                }
                else if(offy < 0){ // shift up
                    float percentagey = (-offy/480.0)*100.0;
                    myfile << frame_num << ": shift up: " << percentagey << "%"<< endl;
                    Mat image2 = image; 
                    //imshow("Corr: Original", image2);
                    //split(image2, image2, frame_id, socket);
                    img_corr = shift_up(image, percentagey);

                }
                //imshow("corruption2", img_corr);
                //socketMat.transmit(img_corr);
               
            }
            else{
                //putText(image, to_string(frame_num), Point(550, 50), FONT_HERSHEY_COMPLEX_SMALL, 1, Scalar(0,0,0), 4, 8, false); 
                img_corr = image; // set corrupted image as original if no corruption

                //imshow("Corr: Original", image);
                //split(image, img_corr, frame_id, socket);
                    //socketMat.transmit(image);
                //imshow("corruption2", image);
                //socketMat.transmit(img_corr);
            }
        }
        else {
            myfile << frame_num << ": original" << endl;
            img_corr = image; // set corrupted image as original if no corruption
            //putText(image, to_string(frame_num), Point(550, 50), FONT_HERSHEY_COMPLEX_SMALL, 1, Scalar(0,0,0), 4, 8, false); 
            //imshow("Corr: Original", image);
                                                                                                                  ////cout << "here" << endl;
            //split(image, img_corr, frame_id, socket);
            ////socketMat.transmit(image);
            //imshow("corruption2", img_corr);
            //socketMat.transmit(img_corr);
            //pthread_create(&thread_send_socketmat, NULL, socketMat.transmit, &img_corr); 
            //cout << "data size" << image.total()*image.elemSize() << endl;
        }
        
        imshow("Original Image", image);
        imshow("Corrupted Image", img_corr);
        cv::waitKey(1);
        
        args_send_corrupt.camera_frame = img_corr;
        args_send_corrupt.frame_id = frame_id;
        args_send_corrupt.socket = socket2;
        
        pthread_create(&thread_send_corrupt, NULL, split, &args_send_corrupt);
        pthread_join(thread_send_corrupt, NULL);
        pthread_join(thread_send_orig, NULL);
    
        cmd.clear(); // clear string vector from GUI
        
    } // while
    
    myfile.close(); // close file
    //socketMat.socketDisconnect(); // disconnect socket
    close(socket1);
    close(socket2);
    return 0;
} // main
