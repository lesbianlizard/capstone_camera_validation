#include<opencv2/objdetect/objdetect.hpp>  
#include<opencv2/highgui/highgui.hpp>  
#include<opencv2/imgproc/imgproc.hpp>  
#include<opencv2/video/video.hpp>
#include <opencv2/videoio/videoio_c.h>
#include <iostream>
#include <opencv2/features2d/features2d.hpp>



using namespace cv;
using namespace std;

Mat translateImg(Mat& img, int offsetx, int offsety) {
	Mat trans_mat = (Mat_<double>(2, 3) << 1, 0, offsetx, 0, 1, offsety);
	warpAffine(img, img, trans_mat, img.size());
	return img;
}
bool ArrayCheck(int a) {
	return true;
}

int main()
{
	VideoCapture capture,capture1;
	capture.open("C:\\Users\\Laptop\\Desktop\\videos\\original_400.avi"); // 打开摄像头
	capture1.open("C:\\Users\\Laptop\\Desktop\\videos\\shift_400.avi");

	if (!capture.isOpened())
	{
		cout << "open camera failed. " << endl;
		return -1;
	}

	if (!capture1.isOpened()) {
		cout << "open camera failed " << endl;
		return -1;
	}


	Mat img,imgGray,img1,imgGray1,temp,temp1,temp2,dst,dst1;
	int i = 0;
	while (1)
	{

		capture >> img;// 读取图像至img
		capture1 >> img1;

		if (img.empty())
		{
			break;
		}
		if (img.channels() == 3)
		{
			cvtColor(img, imgGray, 7);
			cvtColor(img1, imgGray1, 7);
			
		}
		else
		{
			imgGray = img;
			imgGray1 = img1;
		}
		
		
		
		
		vector<KeyPoint> keypoints;
		vector<KeyPoint> keypoints1;

		Mat descriptors1, descriptors2;
		Ptr<ORB> orb = ORB::create();
		orb->detect(img, keypoints);
		orb->detect(img1, keypoints1);

		orb->compute(img, keypoints, descriptors1);
		orb->compute(img1, keypoints1, descriptors2);

		Ptr<DescriptorMatcher> matcher = DescriptorMatcher::create("BruteForce-Hamming");
		vector< DMatch> matches;
		vector<vector<DMatch>> knnmatches;
		
		matcher->knnMatch(descriptors1, descriptors2, knnmatches, 2);

		float min_dist = FLT_MAX;
		for (int r = 0; r < knnmatches.size(); ++r)
		{
			//find the good matched
			if (knnmatches[r][0].distance > 0.75 * knnmatches[r][1].distance)
				continue;

			float dist = knnmatches[r][0].distance;
			if (dist < min_dist) min_dist = dist;
		}

		matches.clear();
		for (size_t r = 0; r < knnmatches.size(); ++r)
		{
			if (knnmatches[r][0].distance > 0.6 * knnmatches[r][1].distance ||knnmatches[r][0].distance > 5 * max(min_dist, 10.0f))
				continue;
			matches.push_back(knnmatches[r][0]);
		}

		cout << "The matched pointed : " << matches.size() << endl;
		cout <<"the key points of imgGray: " <<  keypoints.size() << endl;
		cout <<"the key points of imgGray1: " << (keypoints.size() <= keypoints1.size() ? keypoints.size() : keypoints1.size()) << endl;
		drawMatches(img, keypoints, img1, keypoints1, matches, temp2);
		if (matches.size() > 10 && !(matches.size() == (keypoints.size() <= keypoints1.size() ? keypoints.size() : keypoints1.size()))) {
			Point2d phase_shift;
			imgGray.convertTo(dst, CV_32FC1);
			imgGray1.convertTo(dst1, CV_32FC1);
			phase_shift = phaseCorrelate(dst, dst1); //get the shift degree
			cout << endl << "warp :" << endl << "\tX shift : " << phase_shift.x << "\tY shift : " << phase_shift.y << endl;
			cout << "shifted" << endl;
		}
		else if (matches.size() == (keypoints.size() <= keypoints1.size() ? keypoints.size() : keypoints1.size())) {
			cout << "full matched" << endl;
		}
		else if(matches.size() <10) {
			cout << "Not matched!" << endl;
		}
		
		
	
		
		imshow("test", temp2);
		if (waitKey(1) == 27)		// delay ms 等待按键退出
		{
			break;
		}
		i++;
	}

	return 0;
}

