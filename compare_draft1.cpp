
// CAMERA CORRPTION + COMPARE CODE DRAFT 1
// FOLLOW INSTRUCTIONS BELOW

/*************************************************
 * Freeze image program
 * AARON WALDER
 * press 'f' to freeze and follow by pressing a number 
	* 1-9 in order to freeze for that amount of time
 * press 'p' for FPS
 * press 'r' for red screen
 * press 'g' for green screen
 * press 'b' for blue screen
 * press 'h' for white screen
 * press 'w' to shift up
 * press 'a' to shift left
 * press 'd' to shift right
 * press 's' to shift down
 * press 'l' to test shift comparison
	*when program begins you can enter resolution
	* and you enter the shift
	* only enter either an x value or y value
	* otherwise it will not work
************************************************/

#include <stdio.h>
#include <opencv2/opencv.hpp>
#include <iostream>
#include <unistd.h> 
#include <stdio.h> 
#include <cmath>

using namespace cv;
using namespace std; 


Mat translateImg(Mat& img, int offsetx, int offsety) {
	Mat trans_mat = (Mat_<double>(2, 3) << 1, 0, offsetx, 0, 1, offsety);
	warpAffine(img, img, trans_mat, img.size());
	return img;
}

int main(){
    Mat frame;
    Mat prev_frame; 
    Mat white;
    Mat mask; 
    Mat white_image;//= imread("white.jpg", 1);
    Mat imgGray;
    int x = 0;
    int y = 0;
    
    
    VideoCapture vid(-1); // start video capture
    if(!vid.isOpened()){
        return 0; 
    }
    int width, height, size_in;
    while(1){
        cout << "Press 1: 640x480, 2: 1280x720, 3: 1920x1080" << endl;
        cin >> size_in;
        if(size_in == 1){
            height = 480;
            width = 640; 
            cout << "640x480" << endl;
            break;
        }
        else if(size_in == 2){
            height = 720;
            width = 1280;    
            cout << "1280x720" << endl;
            break;
        }
        else if(size_in == 3){
            height = 1080;
            width = 1920; 
            cout << "1920x1080" << endl;
            break; 
        }
        
    }
    vid.set(CAP_PROP_FRAME_WIDTH, width);
    vid.set(CAP_PROP_FRAME_HEIGHT, height);
   
   Mat white1(height, width, CV_8UC3, Scalar(255,255,255)); 
   Mat black1(height, width, CV_8UC3, Scalar(0,0,0)); 
   
   Mat white_test(height, width, CV_8UC3, Scalar(0,0,255)); 

   
   int shift_y = 0;
   int shift_x = 0; 
   cout << "Shift x: " << endl;
   cin >> shift_x;
   cout << "Shift y: " << endl;
   cin >> shift_y;
   
    vid>>frame; 
    //white = frame; 
    //imshow("froze", white_image); 
    //usleep(2000000);

    while(1){
        vid >> frame; // capture each frame
        prev_frame = frame; 
        if(frame.empty()){ // if there is no frame, break out
                break; 
        }
        char c=(char)waitKey(25);
        if(c==27){ // press 'esc' to quit
            break;
        }
	
	//************************FREEZE********************
        else if(c==102){ // press 'f' to freeze 
            // while loop to freeze indefinitely
            // press esc to end freeze
            cout << "Indicator 2 " << endl; 
            cout << "Freeze Entered: press a # 2-9" << endl;

            while(1){
                char c1 = (char)waitKey(25);
                int j = 0; 
                switch(c1){
                        case 50:
                            imshow("frozen", prev_frame);
                            for(int i =0; i < 2; i++){
                                    cout << "Freeze Time: " << i+1 << " sec" << endl;
                                    usleep(1000000); 
                            }
                            j=1;
                            break; 
                        case 51:
                            imshow("frozen", prev_frame);
                            for(int i =0; i < 3; i++){
                                    cout << "Freeze Time: " << i+1 << " sec" << endl;
                                    usleep(1000000); 
                            }
                            j=1;
                            break; 
                        case 52: 
                            imshow("frozen", prev_frame);
                            for(int i =0; i < 4; i++){
                                    cout << "Freeze Time: " << i+1 << " sec" << endl;
                                    usleep(1000000); 
                            }                            
                            j=1; 
                            break; 
                        case 53:
                            imshow("frozen", prev_frame);
                            for(int i =0; i < 5; i++){
                                    cout << "Freeze Time: " << i+1 << " sec" << endl;
                                    usleep(1000000); 
                            }                            j=1; 
                            break; 
                        case 54:
                            imshow("frozen", prev_frame);
                            for(int i =0; i < 6; i++){
                                    cout << "Freeze Time: " << i+1 << " sec" << endl;
                                    usleep(1000000); 
                            }                            
                            j=1; 
                            break; 
                        case 55:
                            imshow("frozen", prev_frame);
                            for(int i =0; i < 7; i++){
                                    cout << "Freeze Time: " << i+1 << " sec" << endl;
                                    usleep(1000000); 
                            }
                            j=1; 
                            break; 
                        case 56:
                            imshow("frozen", prev_frame);
                            for(int i =0; i < 8; i++){
                                    cout << "Freeze Time: " << i+1 << " sec" << endl;
                                    usleep(1000000); 
                            }
                            j=1; 
                            break; 
                        case 57: 
                            imshow("frozen", prev_frame);
                            for(int i =0; i < 9; i++){
                                    cout << "Freeze Time: " << i+1 << " sec" << endl;
                                    usleep(1000000); 
                            }
                            j=1; 
                            break; 
                        case 27: 
                            j=1; 
                            break;
                        default:
                            //vid>>frame; 
                            imshow("frozen",frame);
                            //usleep(1000000);
                }// switch 
                if(j==1){
                    cout << "Unfrozen" << endl;
                    break;
                }
            } // while 
        } // else if 
	
	//***********WHITE SCREEN*****************
        else if(c=='h'){
            
	    imshow("frozen", white1);
	    if (waitKey(0)){
	    
	    }
	    int cnt9 = 0;
	    for(int i =0; i < frame.cols; i++){
		for(int j = 0; j < frame.rows; j++){
			Vec3b white_p = white1.at<Vec3b>(i,j);
			Vec3b white_t = white1.at<Vec3b>(i,j);
			if(white_p == white_t){
				cnt9 = cnt9 + 1;
			}
		}
	   }
	   if(cnt9 >= 307000){
		cout << "WHITE SCREEN DETECTED" << endl;
	   }
		
	}
	
	
	//*************RED SCREEN*****************
	else if(c == 'r'){
	    Mat red1(height, width, CV_8UC3, Scalar(0,0,255)); 

	    imshow("frozen", green1);
	    if (waitKey(0)){
	    
	    }
	}
	//************GREEN SCREEN******************
	 else if(c=='g'){
	    Mat green1(height, width, CV_8UC3, Scalar(0,255,0)); 

	    imshow("frozen", green1);
	    if (waitKey(0)){
	    
	    }
	}
	
	//*******************BLUE SCREEN****************
	 else if(c=='b'){
	    Mat blue1(height, width, CV_8UC3, Scalar(255,0,0)); 

	    imshow("frozen", blue1);
	    if (waitKey(0)){
	    
	    }
	}
        
	
	//*****************SHIFT UP**********************
	else if(c == 'w'){
		//char c2 = (char)waitKey(25);
		//++ down right
		// -+ down left
		// -- up left
		// +- up right
		//int x = 0;
		//int y = -50;
		//y = y- 50; // ****
		int p = 0;
		while(1){
			p--;
			vid >> frame;
			cvtColor(frame, imgGray, 0);
			translateImg(imgGray, 0, -70);
			imshow("frozen", imgGray);
			if((char)waitKey(1) == 27){
				break; 
			}
			int cnt2 = 0;
			int cnt3 = 0;
			for(int i =0; i < frame.cols; i++){
				for(int j = 0; j < frame.rows; j++){
					Vec3b bgrPixel = imgGray.at<Vec3b>(i,j);
					Vec3b y = frame.at<Vec3b>(i,j);
					//cout << i << ": shift " << bgrPixel << endl;
					//cout << i << ": frame "  << y << endl;

					if(bgrPixel == imgGray.at<Vec3b>(480,640)){
						if(y != black1.at<Vec3b>(0,0)){
							cnt2++;
						}
					}

				}
			}
			cout << "Shifted up " << cnt2/480 << " pixels ";
			float percent1 = (cnt2/480.0)/480.0; 
			percent1 = percent1 * 100;
			cout << percent1 << " %" << endl;
		
			break;
			
		}
	}
	
	
	//****************SHIFT LEFT******************
	else if(c == 'a'){
		//char c2 = (char)waitKey(25);
		//++ down right
		// -+ down left
		// -- up left
		// +- up right
		//int x = -50;
		//int y = 0;
		int p = -10; 
		
		while(1){
			p--; 
			vid >> frame;
			cvtColor(frame, imgGray, 0);
			int h = -300;
			translateImg(imgGray, -100, 0);
			imshow("frozen", imgGray);
			
			int cnt2 = 0;
			int cnt3 = 0;
			int c = 0;

			for(int i =0; i < frame.rows; i++){
				for(int j = 0; j < frame.cols; j++){

					Vec3b bgrPixel = imgGray.at<Vec3b>(i,j+215); // j+215
					Vec3b y = frame.at<Vec3b>(i,j);
					//cout << "x " << j << "Y: " << i << " " << bgrPixel << endl;

					//cout << " " << bgrPixel;
					//cout << i << ": frame "  << y << endl;

					if(bgrPixel == black1.at<Vec3b>(0,0)){
						if(y != black1.at<Vec3b>(0,0)){
							cnt2++;
						}
					}

				}
			}
			cout << "P: " << -p << endl;
			cout << "Shifted left " << cnt2/640 << " pixels ";
			float percent1 = (cnt2/640.0)/640.0; 
			percent1 = percent1 * 100;
			cout << percent1 << " %" << endl;
			if((char)waitKey(1) == 27){
				break; 
			}
			break;
			
		}

	}
	
	//*************SHIFT RIGHT********************
	else if(c == 'd'){
		//char c2 = (char)waitKey(25);
		//++ down right
		// -+ down left
		// -- up left
		// +- up right
		//x = x + 50; // ****
		//int x = 50;
		//int y = 0;
		int p = 0;
		while(1){
			 p++;
			vid >> frame;
			cvtColor(frame, imgGray, 0);
			translateImg(imgGray, 100, 0);
			imshow("frozen", imgGray);
			if((char)waitKey(1) == 27){
				break; 
			}
			int cnt2 = 0;
			int cnt3 = 0;
			for(int i =0; i < frame.rows; i++){
				for(int j = 0; j < frame.cols; j++){
					Vec3b bgrPixel = imgGray.at<Vec3b>(i,j);
					Vec3b y = frame.at<Vec3b>(i,j);
					//cout << "x " << i << "Y: " << j << " " << bgrPixel << endl;
					//cout << i << ": frame "  << y << endl;

					if(bgrPixel == black1.at<Vec3b>(0,0)){
						if(y != black1.at<Vec3b>(0,0)){
							cnt2++;
							//cout << "I: " << i << " J: " << j << endl;
						}					}

				}
			}
			cout << "Shifted right " << cnt2/640 <<  " pixels ";
			float percent1 = (cnt2/640.0)/640.0; 
			percent1 = percent1 * 100;
			cout << percent1 << " %" << endl;
		
			break;
			
		}
	}
	
	//SHIFT DOWN***********************************
	else if(c == 's'){
		//char c2 = (char)waitKey(25);
		//++ down right
		// -+ down left
		// -- up left
		// +- up right
		//int x = 0;
		//int y = 50;
		//y = y  + 50; //***
		int p = 0; 
		while(1){
			 p++;
			vid >> frame;
			cvtColor(frame, imgGray, 0);
			translateImg(imgGray, 0, 90);
			imshow("frozen", imgGray);
			if((char)waitKey(1) == 27){
				break; 
			}
			
			int cnt2 = 0;
			int cnt3 = 0;
			for(int i =0; i < frame.cols; i++){
				for(int j = 0; j < frame.rows; j++){
					Vec3b bgrPixel = imgGray.at<Vec3b>(i,j);
					Vec3b y = frame.at<Vec3b>(i,j);
					//cout << j << ": shift " << bgrPixel << endl;
					//cout << i << ": frame "  << y << endl;

					if(bgrPixel == imgGray.at<Vec3b>(0,0)){
						if(y != black1.at<Vec3b>(0,0)){
							
							cnt2++;
						}
					}

				}
			}
			cout << "Shifted down " << cnt2/480 << " pixels ";
			float percent1 = (cnt2/480.0)/480.0; 
			percent1 = percent1 * 100;
			cout << percent1 << " %" << endl;
		
			break;
			
			
		}
	}
	
	//*********PRESS l to test the shift comparison ********************
	else if(c == 'l'){
		int p = 0; 
		int cnt2 = 0;
		int cnt3 = 0;
		float percent_r = 0;
		float percent_l = 0;
		float percent_u = 0;
		float percent_d = 0; 

		while(1){
			 //p++;
			vid >> frame;
			cvtColor(frame, imgGray, 0);
			translateImg(imgGray, shift_x, shift_y);
			Vec3b bgrPixel = imgGray.at<Vec3b>(0,0);
			Vec3b y = frame.at<Vec3b>(0,0);


			//-Y = up, +Y = down, -x = left, +x = right
			imshow("frozen", imgGray);
			if((char)waitKey(1) == 27){
				break; 
			}
			
			//check shift down ***********************************
			int cnt_u1 = 0;
			int cnt_u2 = 0;
			float pixel_u = 0;
			for(int i = 0; i < 120; i++){
				//cout << "cnt1 " << cnt_u1 << endl;
				if(cnt_u1 >= 640){
					cnt_u2 = cnt_u2 + 1; 
				}
				cnt_u1 = 0;
				// cout <<" shift " << bgrPixel << " orig " << y << endl;
 				for(int j = 0; j < frame.cols; j++){ // 480
					bgrPixel = imgGray.at<Vec3b>(i,j);
					y = frame.at<Vec3b>(i,j);
					//cout << " frame "  << y;

					if(bgrPixel == black1.at<Vec3b>(0,0)){
						if(y != black1.at<Vec3b>(0,0)){
							//cout << "j " << j << endl;
							cnt_u1++;
						}
					}
					else{
						//cnt3 = 1; 
					}

				}
				

			}
			pixel_u = cnt_u2;
			percent_u = (pixel_u/480.0)*100;
			percent_u = round(percent_u);
			if(percent_u > 1){
				cout << "SHIFTED DOWN " << pixel_u << " pixels ";
				cout << "and " << percent_u << "%" << endl;
			}
			
			//check shift up ***************************
			cnt2 = 0;
			cnt3 = 0;
			for(int i = 360; i < frame.rows; i++){ // 640
				//cout << cnt3 << endl;
				if(cnt3 >= 600){
					cnt2++; 
				}
				cnt3 = 0;
				//cout <<" shift " << bgrPixel << " orig " << y << endl;
				for(int j = 0; j < frame.cols; j++){ // 480
					bgrPixel = imgGray.at<Vec3b>(i,j);
					y = frame.at<Vec3b>(i,j);
					//cout << j << ": shift " << bgrPixel << endl;
					//cout << i << ": frame "  << y << endl;

					if(bgrPixel == black1.at<Vec3b>(0,0)){
						if(y != black1.at<Vec3b>(0,0)){
							//cout << "j " << j << endl;
							cnt3++;
							//cout << " Cnt3 " << cnt3;

						}
					}
					else{
						//cnt3 = 1; 
					}

				}
				

			}
			cnt2++;
			float pixel_d = cnt2;
			percent_d = (cnt2/480.0)*100;
			percent_d = round(percent_d);
			if(percent_d > 1){
				
				cout << "SHIFTED UP " << cnt2;
				cout << " and " << percent_d << "%" << endl;
			}
			
			
			// shift left check**************************************
			int cnt4 = 0;
			int cnt5 = 0;
			int cnt_l = 0;
			int check = 0;
			for(int i =0; i < frame.rows; i++){
				if(cnt5 == cnt_l){
					cnt4 = 0;
				}
				
				else{
					cnt4 = cnt_l;
				}
				cnt5 = cnt_l;
				
				if(cnt_l ==0){
					check++;
				}
				//cout << "COUNT L " << cnt_l << endl;
				for(int j = 480; j < frame.cols; j++){
					Vec3b bgrPixel = imgGray.at<Vec3b>(i,j+215);
					Vec3b y = frame.at<Vec3b>(i,j);
					//cout << " " << bgrPixel;
					//cout << i << ": frame "  << y << endl;

					if(bgrPixel == black1.at<Vec3b>(0,0)){
						if(y != black1.at<Vec3b>(0,0)){
							
							cnt_l++;
						}					}

				}
			}
			//cout << "CHECK" << check << endl;
			if(check > 10){
				cnt4 = 0;
			}
			float pixel_l = cnt4/640.0;
			percent_l = (pixel_l / 640.0)*100;
			percent_l = round(percent_l);
			if(percent_l > 1){
				cout << "SHIFTED LEFT " << pixel_l;
				cout << " and " << percent_l << "%" << endl;
			}
			
			
			//shift right check****************************
			int cnt6 = 0;
			int cnt7 = 0;
			int cnt_r = 0;
			for(int i =0; i < frame.rows; i++){
				if(cnt6 >= 160){
					cnt_r = cnt6 + cnt_r; 
				}
				cnt6 = 0; 
				for(int j = 0; j < 161; j++){
					Vec3b bgrPixel = imgGray.at<Vec3b>(j,i);
					Vec3b y = frame.at<Vec3b>(j,i);
					//cout << " " << bgrPixel;
					//cout << i << ": frame "  << y << endl;

					if(bgrPixel == black1.at<Vec3b>(0,0)){
						if(y != black1.at<Vec3b>(0,0)){
							
							cnt6++;
						}					}

				}
			}
			float pixel_r = cnt_r/640.0;
			pixel_r = pixel_r/(.3348925);
			percent_r = (pixel_r / 640.0)*100;
			percent_r = round(percent_r);
			if(percent_r > 1){
				cout << "SHIFTED RIGHT " << pixel_r;
				cout << " and " << percent_r << "%" << endl;
			}

			
			usleep(10000);
			break;

		}

	}
	
	//************************FPS****************************
        else if(c==112){ // press 'p' to show fps
            double fps = vid.get(CAP_PROP_FPS); 
            cout << "FPS " << fps << " frames/sec" << endl; 
        }
        imshow("frozen", frame); 
    }
    
    return 0; 
}

