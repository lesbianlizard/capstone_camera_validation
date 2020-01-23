#include<opencv2/objdetect/objdetect.hpp>  
#include<opencv2/highgui/highgui.hpp>  
#include<opencv2/imgproc/imgproc.hpp>  
#include<opencv2/video/video.hpp>
#include <opencv2/videoio/videoio_c.h>
#include <iostream>

using namespace cv;
using namespace std;

Mat translateImg(Mat& img, int offsetx, int offsety) {
	Mat trans_mat = (Mat_<double>(2, 3) << 1, 0, offsetx, 0, 1, offsety);
	warpAffine(img, img, trans_mat, img.size());
	return img;
}

int main()
{
	VideoCapture capture;
	capture.open(0); // open the camera 
	if (!capture.isOpened())
	{
		cout << "open camera failed. " << endl;
		return -1;
	}

	Mat img, imgGray;
	while (1)
	{	
		
		capture >> img;// read the image to img
		if (img.empty())
		{
			continue;
		}
		if (img.channels() == 3)
		{
			cvtColor(img, imgGray, 7);
		}
		else
		{
			imgGray = img;
		}
		translateImg(img, 100, 50);
		imshow("Cameral shifting", img); // Display
		if (waitKey(1) > 0)		// delay ms 
		{
			break;
		}
	}

	return 0;
}

