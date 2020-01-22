//SHIFT
#include<opencv2/highgui/highgui.hpp>  
#include<opencv2/imgproc/imgproc.hpp>  
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
	capture.open(1);
	if (!capture.isOpened())
	{
		cout << "open camera failed. " << endl;
		return -1;	}

	Mat img, imgGray;	
	while (1)
	{
			
		capture >> img;
		if(!capture.isOpened()){
			cout << "camera Failed" << endl;
			return -1;
		} 
		//translateImg(img,50,50);
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
		translateImg(imgGray,50,50);
		imshow("Cameral shifting", imgGray); // 显示
		if ((char)waitKey(1) == 27)		// delay ms 等待按键退出
		{
			break;
		}
	}

	return 0;
}
