#include <iostream>
#include <sys/socket.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <netinet/in.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <opencv2/opencv.hpp>
#include "opencv2/core.hpp"
#include "opencv2/highgui.hpp"
#include <vector>
#include <chrono>
#define PORT 8080

using namespace std;
using namespace cv;

int socket_setup_client(){
	int sock = socket(AF_INET, SOCK_STREAM, 0);
	if(sock < 0){
		cout << "error in socket()" << endl;
	}

	struct sockaddr_in serv_addr;
	serv_addr.sin_family = AF_INET;
	serv_addr.sin_port = htons(PORT);
	int addrlen = sizeof(serv_addr);
	
	if(inet_pton(AF_INET, "127.0.0.1", &serv_addr.sin_addr) <= 0){
		cout << "error in inet_pton" << endl;
	}

	if(connect(sock, (struct sockaddr *)&serv_addr, addrlen) < 0){
		cout << "error in connect" << endl;
	}
	return sock;
}

void receive_frame(int socket,  Mat &original_image){

	int buffer1[4] = {0};
	int buffer_size = 0;
	vector	<uchar> buffer;
	recv(socket, &buffer1, 4, MSG_WAITALL);
	buffer_size = buffer1[0];
	buffer.resize(buffer_size);	
	recv(socket, buffer.data(), buffer_size, MSG_WAITALL);

	original_image = imdecode(buffer, 1);
//	recv(socket, &id, sizeof(id), MSG_WAITALL);


}

int main(){

	Mat original_image;
	int img_buffer_size = 10;
	//Mat img_buffer[img_buffer_size];
	//int id_buffer[img_buffer_size];
       	
	
	float sum = 0;
        float times[60];
        int index = 0;
        double avg_fps = 30;


	int socket = socket_setup_client();

	namedWindow("To Comparison Block", CV_WINDOW_AUTOSIZE);
	while(1){

		cout << "AVG FPS: " << avg_fps << endl;
		auto start = std::chrono::high_resolution_clock::now();
	

		receive_frame(socket, original_image);
		imshow("To Comparison Block", original_image);
			
		cout << "To Comparison Block Size: " << original_image.cols << "x" << original_image.rows << endl;

		char c = (char)waitKey(15);
		if(c == 27){
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


	destroyWindow("img");
	return 0;
}
