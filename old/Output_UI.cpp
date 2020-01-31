#include <opencv2/opencv.hpp>
#include <iostream>
#include "opencv2/core/core.hpp"
#include "opencv2/imgproc/imgproc.hpp"
#include <opencv2/highgui/highgui.hpp>
#include <iostream>
#include <string>
#include <time.h>



using namespace std;
using namespace cv;

struct resolution {
	int X;
	int Y;
};

void output_ui(Mat& img, bool shift_detect, bool noise_detect, bool freeze_detect, float fps, resolution res) {

/*
	if (img.empty() || &img == NULL) {
		cout << "Empty Image" << endl;
		return;
	}
*/
	Point warning = Point(100, 100);
	//Point pt = Point(100, 200);
	Scalar color = Scalar(0, 0, 255);
	int thickness = 5;
	int font_size = 2;
	int font = FONT_HERSHEY_COMPLEX_SMALL;
	int text_x = 100;
	int text_y = 200;

	//FPS1080:
	putText(img, "FPS:", Point(100, 980), font, font_size, Scalar(0, 0, 0), thickness, 8, false);
	putText(img, to_string(fps), Point(210, 980), font, font_size, Scalar(0, 0, 0), thickness, 8, false);

	//FPS720:
	//putText(img, "FPS:", Point(100, 620), font, font_size, Scalar(0, 0, 0), thickness, 8, false);
	//putText(img, to_string(fps), Point(210, 620), font, font_size, Scalar(0, 0, 0), thickness, 8, false);

	//Resolution:
	putText(img, to_string(res.Y), Point(100, 1030), font, font_size, Scalar(0, 0, 0), thickness, 8, false);
	putText(img, "x", Point(210, 1030), font, font_size, Scalar(0, 0, 0), thickness, 8, false);
	putText(img, to_string(res.X), Point(240, 1030), font, font_size, Scalar(0, 0, 0), thickness, 8, false);

	if (shift_detect || noise_detect || freeze_detect) {
		putText(img, "Warning:", warning, font, font_size, color, thickness, 8, false);
		//void rectangle(Mat& img, Point pt1, Point pt2, const Scalar& color, int thickness=1, int lineType=8, int shift=0)
		rectangle(img, Point(0, 0), Point(1920, 1080), color, 20);
	}
	else {
		return;
	}
	if (shift_detect) {
		putText(img, "Video Shift Detected", Point(text_x, text_y), font, font_size, color, thickness, 8, false);
		//void arrowedLine(Mat& img, Point pt1, Point pt2, const Scalar& color, int thickness=1, int line_type=8, int shift=0, double tipLength=0.1)
		//arrowedLine(img, Point(200, 500), Point(10, 500), color, thickness);
		text_y = text_y + 50;
	}
	if (noise_detect) {
		putText(img, "Noise Detected", Point(text_x, text_y), font, font_size, color, thickness, 8, false);
		text_y = text_y + 50;
	}
	if (freeze_detect) {
		putText(img, "Video Frozen", Point(text_x, text_y), font, font_size, color, thickness, 8, false);
		text_y = text_y + 50;
	}

}


int main(int argc, char* argv[]){
	bool shift = false;
	bool noise = false;
	bool freeze = false;
	Mat img;
	char c;
	double seconds;
	double ms;
	struct resolution res;
	double fps = 0;
	float avg_fps = 30;
	int diff = 1;
	float fps_min = 30;
	float fps_max = 30;
	int temp = 0;
	int n = 1;
	Mat empty;
	float times[60];
	int index = 0;
	float sum = 0;

	string arg = argv[1];

	res.X = 0;
	res.Y = 0;

	//manual mode

	if (argc != 2) {
		return -1;
	}
	int arg1 = argv[1][0] - 48;
	int arg2 = argv[1][1] - 48;
	int arg3 = argv[1][2] - 48;
	if (arg1 == 1 || arg1 == 0) {
		shift = arg1;
	}
	if (arg2 == 1 || arg2 == 0) {
		noise = arg2;
	}
	if (arg3 == 1 || arg3 == 0) {
		freeze = arg3;
	}




	//img = imread("test.jpg");
	namedWindow("image", WINDOW_NORMAL);
	resizeWindow("image", 1920, 1080);
	
	VideoCapture cap("test_1080.mp4");
	//VideoCapture cap(0);
	if (!cap.isOpened()) {
		return -1;
	}
	printf("Opened\n");
	
	//while ((cap.read(img)) != NULL) {
	while(1){
		cap >> img;
		//Start Clock:
		//auto start = std::chrono::high_resolution_clock::now(); //windows only

		/*--------------------------------------------
		Call Output UI Block:
		----------------------------------------------*/
		//&img = NULL;
		avg_fps = cap.get(CAP_PROP_FPS);

		//img = empty;
		output_ui(img, shift, noise, freeze, avg_fps, res);
		
		//Display Frame:
		if (!img.empty()) {
			imshow("image", img);
		}
		//Grab input from Keyboard:
		c=(char)waitKey(diff);
		if (c == 27) { //ESC key
			break;
		}
		if (arg == "manual") {
			if (c == 113) { //q key
				shift = !shift;
			}
			else if (c == 119) { //w key
				noise = !noise;
			}
			else if (c == 101) { //e key
				freeze = !freeze;
			}
		}
		//Calculated elapsed time:
		//auto elapsed = std::chrono::high_resolution_clock::now() - start;
		//Convert to microseconds:
		//long long microseconds = std::chrono::duration_cast<std::chrono::microseconds>(elapsed).count();
		//Convert to seconds:
		//long double seconds = (long double)microseconds / 1e6;
		//Convert to milliseconds:
		//double ms = seconds * 1000;
		//Calculate FPS of the last frame
		//fps = 1 / seconds;
		//Update resolution:
		res.X = img.rows;
		res.Y = img.cols;
		//Keep track of Min/Max FPS
		if (fps > fps_max) {
			fps_max = fps;
		}
		if (fps < fps_min) {
			fps_min = fps;
		}

		/*------------------------
		DELAY:
		-------------------------*/
		/*
		diff = 33 - ms;
		cout << diff << endl;
		if (diff <= 0) {
			diff = 1;
		}
		*/

		/*
		temp = 70 - ms;
		diff = (temp + n*diff) / (n+1);
		*/
		diff = 1;
		


		/*--------------------------------------
		MOVING AVERAGE:
		---------------------------------------*/
		//avg_fps = (fps + n*avg_fps) / (n+1);



		/*--------------------------------------
		AVERAGE OVER LAST 60 FRAMES:
		----------------------------------------*/
		sum = 0;
		//cout << index << endl;
		/*
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
		*/

		n++;
	}
	
	
	cout << fps_min << endl;
	cout << fps_max << endl;

	waitKey(0);

	cap.release();
	return 0;
}
