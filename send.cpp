#include <iostream>
#include <sys/socket.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <netinet/in.h>
#include <unistd.h>
#include <opencv2/opencv.hpp>
#include "opencv2/core.hpp"
#include "opencv2/highgui.hpp"
#include <vector>
#include <chrono>
#define PORT 8080
#define MB 2591 * 1944

using namespace std;
using namespace cv;

int socket_setup_server(){
	int opt = 1;

	int sockfd = socket(AF_INET,SOCK_STREAM,0);
	if(sockfd == -1){
		printf("error in socket()\n");
	}

	if(setsockopt(sockfd, SOL_SOCKET, SO_REUSEADDR | SO_REUSEPORT, &opt, sizeof(opt))){
		printf("error in setsockopt()\n");
	}

	struct sockaddr_in address;
	int addrlen = sizeof(address);
	address.sin_family = AF_INET;
	address.sin_addr.s_addr = INADDR_ANY;
	address.sin_port = htons(PORT);
	int bind1 = bind(sockfd, (struct sockaddr*)&address, addrlen);
	if(bind1 < 0){
		printf("error in bind\n");
	}

	int listen1 = listen(sockfd, 3);

	int new_socket = accept(sockfd, (struct sockaddr*)&address, (socklen_t*)&addrlen);
	return new_socket;
	
}

void split(Mat input_camera_frame, int socket, Mat& freeze_camera_frame, Mat& noise_camera_frame, Mat& shift_camera_frame){
	int buffer_for_size[1] = {0};
	vector <uchar> buffer;
	buffer.resize(MB);
	imencode(".jpg", input_camera_frame, buffer);
	int buffer_size = buffer.size();
	buffer_for_size[0] = buffer_size;
	send(socket, &buffer_for_size, sizeof(buffer_for_size), 0);

	freeze_camera_frame = input_camera_frame;
	noise_camera_frame = input_camera_frame;
	shift_camera_frame = input_camera_frame;

	size_t l = buffer_size;
	send(socket, buffer.data(), l, 0);

//	send(socket, &frame_id, sizeof(frame_id), 0);


}



int main(){

	Mat input_camera_frame;
	Mat corruption_frame;
	Mat freeze_camera_frame;
	Mat noise_camera_frame;
	Mat shift_camera_frame;
	
	

	float sum = 0;
	float times[60];
	int index = 0;
	double avg_fps = 30;
	
	int socket = socket_setup_server();

	VideoCapture cap("test_vid.mp4");
	namedWindow("Input", CV_WINDOW_AUTOSIZE);
	namedWindow("To Freeze Block", CV_WINDOW_AUTOSIZE);
	namedWindow("To Noise Block", CV_WINDOW_AUTOSIZE);
	namedWindow("To Shift Block", CV_WINDOW_AUTOSIZE);
	while(1){
		cout << "AVG FPS: " << avg_fps << endl;
		auto start = std::chrono::high_resolution_clock::now();
		cap >> input_camera_frame;
		if(input_camera_frame.empty()){
			break;
		}
		
		

		split(input_camera_frame, socket, freeze_camera_frame, noise_camera_frame, shift_camera_frame);
			
		imshow("Input", input_camera_frame);

		imshow("To Freeze Block", freeze_camera_frame);
		
		imshow("To Noise Block", noise_camera_frame);
		
		imshow("To Shift Block", shift_camera_frame);
		
		cout << "Input Size: " << input_camera_frame.cols << "x" << input_camera_frame.rows << endl;
		cout << "To Freeze Block Size: " << freeze_camera_frame.cols << "x" << freeze_camera_frame.rows << endl;
		cout << "To Noise Block Size: " << noise_camera_frame.cols << "x" << noise_camera_frame.rows << endl;
		cout << "To Shift Block Size: " << shift_camera_frame.cols << "x" << shift_camera_frame.rows << endl;


		char c = (char)waitKey(15);
		if(c==27){
			break;
		}

		//Calculated elapsed time:
                auto elapsed = std::chrono::high_resolution_clock::now() - start;
                //Convert to microseconds:
                long long microseconds = std::chrono::duration_cast<std::chrono::microseconds>(elapsed).count();
                //Convert to seconds:
                long double seconds = (long double)microseconds / 1e6;
                //Calculate FPS of the last frame
           	sum = 0;
                //cout << index << endl;
                times[index] = seconds;
                for (int j = 0; j <= index; j++) {
                        sum = sum + times[j];
                }
                avg_fps = 1/(sum / index);
                if (index < 59) {
                        index++;
                }
                else {
                        for (int j = 0; j < 59; j++) {
                                times[j] = times[j + 1];
                        }
                }



	}


	//close(sockfd);
	return 0;
}
