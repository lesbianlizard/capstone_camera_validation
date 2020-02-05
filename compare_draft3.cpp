


// CAMERA CODE

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
#include <fstream> 
#include <time.h>
#include <chrono>

using namespace cv;
using namespace std; 


Mat translateImg(Mat& img, int offsetx, int offsety) {
	Mat trans_mat = (Mat_<double>(2, 3) << 1, 0, offsetx, 0, 1, offsety);
	warpAffine(img, img, trans_mat, img.size());
	return img;
}


int comparison(Mat orig_img, Mat corr_img, int freeze_cnt){
	ofstream myfile;
	myfile.open("comp_log.txt");
	int comp_var = 0;
	int iter = 0;
	int height = 480;
	int width = 640;
	while(iter <= 4){
		iter++;
		//while(iter == 1){
		if(iter == 1){
			Mat red(height, width, CV_8UC3, Scalar(0,0,255)); 
			Mat blue(height, width, CV_8UC3, Scalar(255,0,0)); 
			Mat green(height, width, CV_8UC3, Scalar(0,255,0)); 
			Mat white(height, width, CV_8UC3, Scalar(255,255,255)); 

			int cntw = 0;
			int cntb = 0;
			int cntg = 0;
			int cntr = 0;
			int n = 0;
			Vec3b white_o = white.at<Vec3b>(0,0);
			Vec3b red_o = red.at<Vec3b>(0,0);
			Vec3b blue_o = blue.at<Vec3b>(0,0);
			Vec3b green_o = green.at<Vec3b>(0,0);
			Vec3b corr_o = corr_img.at<Vec3b>(0,0);
			//cout << " white " << white_o;
			//cout << " red " << red_o;
			//cout << " blue " << blue_o;
			//cout << " green " << green_o;
			
			for(int i =0; i < 630; i++){
				for(int j = 0; j < 470; j++){
					corr_o = corr_img.at<Vec3b>(j,i);
					//cout << "error after " << i<< " " << j <<" ";
					if(white_o == corr_o){
						cntw++;
					}
					else if(red_o == corr_o){
						cntr++;
					}
					else if(blue_o == corr_o){
						cntb++;
					}
					else if(green_o == corr_o){
						cntg++;
					}
					else{
						n = 0;
					}
					
				}
			}	
			//cout << "check 2" << endl;
			if(cntw >= 296000){
				cout << "WHITE SCREEN COMPARED" << endl;
				comp_var = 0;
				break;
			}
			else if(cntb >= 296000){
				cout << "BLUE SCREEN COMPARED" << endl;
				comp_var = 0;
				break;
			}
			else if(cntg >= 296000){
				cout << "GREEN SCREEN COMPARED" << endl;
				comp_var = 0;
				break;
			}
			else if(cntr >= 296000){
				cout << "RED SCREEN COMPARED" << endl;
				comp_var = 0;
				break;
			}
	   
		} // if 0
		else if(iter == 2){
			int p = 0; 
			int cnt2 = 0;
			int cnt3 = 0;
			float percent_r = 0;
			float percent_l = 0;
			float percent_u = 0;
			float percent_d = 0; 
			Mat black1(height, width, CV_8UC3, Scalar(0,0,0)); 
			//cvtColor(frame, imgGray, 0);
			//translateImg(imgGray, shift_x, shift_y);
			Vec3b bgrPixel = corr_img.at<Vec3b>(0,0);
			Vec3b y = orig_img.at<Vec3b>(0,0);

			//-Y = up, +Y = down, -x = left, +x = right
			//imshow("frozen", imgGray);
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
 				for(int j = 0; j < orig_img.cols; j++){ // 480
					bgrPixel = corr_img.at<Vec3b>(i,j);
					y = orig_img.at<Vec3b>(i,j);
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
			if(percent_u >= 1){
				cout << "COMP: SHIFTED DOWN " << pixel_u << " pixels ";
				cout << "and " << percent_u << "%" << endl;
				myfile << "SHIFTED DOWN " << pixel_u << " =" << percent_u << "%" << endl;
				comp_var = 0;
				break;
			}

			//check shift up ***************************
			cnt2 = 0;
			cnt3 = 0;
			for(int i = 360; i < orig_img.rows; i++){ // 640
				//cout << cnt3 << endl;
				if(cnt3 >= 600){
					cnt2++; 
				}
				cnt3 = 0;
				//cout <<" shift " << bgrPixel << " orig " << y << endl;
				for(int j = 0; j < orig_img.cols; j++){ // 480
					bgrPixel = corr_img.at<Vec3b>(i,j);
					y = orig_img.at<Vec3b>(i,j);
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
			if(percent_d >= 1){
				
				cout << "COMP: SHIFTED UP " << cnt2;
				cout << " and " << percent_d << "%" << endl;
				myfile << "SHIFTED up " << cnt2 << " =" << percent_d << "%" << endl;
				comp_var = 0;
				break;
			}

			// shift left check**************************************
			int cnt4 = 0;
			int cnt5 = 0;
			int cnt_l = 0;
			int check = 0;
			for(int i =0; i < orig_img.rows; i++){
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
				for(int j = 480; j < orig_img.cols; j++){
					bgrPixel = corr_img.at<Vec3b>(i,j+215);
					y = orig_img.at<Vec3b>(i,j);
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
			pixel_l = round(pixel_l);
			percent_l = (pixel_l / 640.0)*100;
			percent_l = round(percent_l);
			if(percent_l >= 4){
				cout << "COMP: SHIFTED LEFT " << pixel_l;
				cout << " and " << percent_l << "%" << endl;
				myfile << "SHIFTED LEFT " << pixel_l << " =" << percent_l << "%" << endl;
				comp_var = 0;
				break;
			}
			

			//shift right check****************************
			int cnt6 = 0;
			int cnt7 = 0;
			int cnt_r = 0;
			for(int i =0; i < orig_img.rows; i++){
				if(cnt6 >= 160){
					cnt_r = cnt6 + cnt_r; 
				}
				cnt6 = 0; 
				for(int j = 0; j < 161; j++){
					bgrPixel = corr_img.at<Vec3b>(j,i);
					y = orig_img.at<Vec3b>(j,i);
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
			pixel_r = round(pixel_r);
			percent_r = (pixel_r / 640.0)*100;
			percent_r = round(percent_r);
			if(percent_r >= 1){
				cout << "COMP: SHIFTED RIGHT " << pixel_r;
				cout << " and " << percent_r << "%" << endl;
				myfile << "SHIFTED RIGHT " << pixel_r << " =" << percent_r << "%" << endl;
				comp_var = 0;
				break;
			}


		} // else iter = 2
		
	} // while loop
	myfile.close();
	if(comp_var == 0){
		return 0;
	}
	else{
		return comp_var;
	}
} // compare function


int freeze_comp(Mat orig, Mat corr, int freeze_cnt){
	int frz_cnt = 0;
	int frz_not = 0;
	int frz_check = 0;
	//auto start = chrono::steady_clock::now();
	while(1){
		for(int i =0; i < orig.rows; i++){
			for(int j = 0; j < orig.cols; j++){ // 480
				Vec3b bgrPixel = corr.at<Vec3b>(i,j);
				Vec3b y = orig.at<Vec3b>(i,j);
				//cout << " frame "  << y;
				if(bgrPixel == y){
					frz_cnt++;
					//cout << frz_cnt << " ";
				}
				else{
					frz_not++;
					if(freeze_cnt > 0){
						float counter = freeze_cnt/57.0;
						int counter2 = round(counter); 
						cout << "COMP: frozen for: " << counter << " secs" << endl;
					}
					freeze_cnt = 0; 

					break;
				}

			} // inner for loop
		} // outer for loop
		if(frz_cnt >= 307000){
			freeze_cnt++;
			//cout << "frozen" << endl;
			//cout << "inc " << endl;
			//comp_var = freeze_cnt;
		}
		break;

	}// while 
	//auto end = chrono::steady_clock::now();
	//auto time1 = 
	//cout << "time freeze: " << chrono::duration_cast<chrono::microseconds>(end-start).count() << endl;
	return freeze_cnt; 	
}

int main(){
    Mat frame;
    Mat prev_frame; 
    Mat white;
    Mat mask; 
    Mat white_image;//= imread("white.jpg", 1);
    Mat imgGray;
    //Mat img_corr; 
    int x = 0;
    int y = 0;
    int frame_num = 0;
    
    ofstream myfile;
    myfile.open("corr_log.txt");
    
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
    vid.set(CAP_PROP_FPS,70);
    int fps2 = vid.get(CAP_PROP_FPS);
    cout << "FPS is " << fps2 << endl;
   
   Mat white1(height, width, CV_8UC3, Scalar(255,255,255)); 
   Mat starter(height, width, CV_8UC3, Scalar(255,0,100)); 

   Mat img_corr = starter; 

   Mat black1(height, width, CV_8UC3, Scalar(0,0,0)); 
   
   Mat white_test(height, width, CV_8UC3, Scalar(0,0,255)); 

   
   int shift_y = 0;
   int shift_x = 0; 
   int percentage= 0;
   //cout << "Shift x: " << endl;
   //cin >> shift_x;
   //cout << "Shift y: " << endl;
   //cin >> shift_y;
   int check_per = 0;
   while(check_per == 0){
	cout << "Enter shift percentage 1-25%: " << endl;
	cin >> percentage;
	if((int)percentage < 26 && (int)percentage > 0){
		check_per = 1;
	}
	else{
		cout << "Invalid percentage" << endl;
	}
   }
   
   //time_t time_now,end;
   //auto time1 = std::chrono::high_resolution_clock::now();

   //for(int i =0; i < 30; i++){
   int j = 0; 
   //time(&time_now);
   auto start = chrono::steady_clock::now();
   while(j < 30){
	vid>>frame; 
	j++;
	//if((char)waitKey(1) == 27){
             //   break; 
        //}
   }
  // time(&end);
   //double secs = difftime(end, time_now);
   auto end = chrono::steady_clock::now();
   auto time1 = chrono::duration_cast<chrono::milliseconds>(end-start).count();
   cout << "Time "  << time1 << endl;
   float time2 = time1/1000.0;
   cout << "Frame rate: " << 30/time2 << endl;
   
   //auto time2 = std::chrono::high_resolution_clock::now();
   //float duration = std::chrono::duration_cast<std::chrono::microseconds>(time2-time1).count();
   //cout << duration/100000.0 << endl;
   
    vid>>frame;
    //img_corr = frame; 
    //usleep(2000000);
    //white = frame; 
    //imshow("froze", white_image); 
    //usleep(2000000);
    int comp_num = 0;
    while(1){
        vid >> frame; // capture each frame
	imshow("frozen", frame); 

        //img_corr = frame; 
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
		int k = 0;
		int l = 0;
                switch(c1){
                        case 50:
                            imshow("frozen", prev_frame);
			    myfile << "Corrupted - Frozen for: 2 seconds" <<  endl;
                            for(int i =0; i < 114; i++){
				
				vid >> frame;
				//imshow("frozen", prev_frame);
				if((i%57) == 0){
					k++;
					cout << k << " seconds" << endl;
				}
				//comp_num = comparison(prev_frame, prev_frame,comp_num);
				comp_num = freeze_comp(prev_frame, prev_frame, comp_num);
                                    //cout << "Freeze Time: " << i+1 << " sec" << endl;
                                    //usleep(1000000); 
                            }
                            j=1;
                            break; 
                        case 51:
                            imshow("frozen", prev_frame);
			    myfile << "Corrupted - Frozen for: 3 seconds" <<  endl;			    
                            for(int i =0; i < 171; i++){
				
				vid >> frame;
				//imshow("frozen", prev_frame);
				if((i%57) == 0){
					k++;
					cout << k << " seconds" << endl;
				}
				comp_num = freeze_comp(prev_frame, prev_frame, comp_num);

                                    //cout << "Freeze Time: " << i+1 << " sec" << endl;
                                    //usleep(1000000); 
                            }
                            j=1;
                            break; 
                        case 52: 
                            imshow("frozen", prev_frame);
			    myfile << "Corrupted - Frozen for: 4 seconds" <<  endl;
                            for(int i =0; i < 228; i++){
				
				vid >> frame;
				//imshow("frozen", prev_frame);
				if((i%57) == 0){
					k++;
					cout << k << " seconds" << endl;
				}
				//comp_num = freeze_comp(prev_frame, prev_frame, comp_num);

                                    //cout << "Freeze Time: " << i+1 << " sec" << endl;
                                   //usleep(1000000); 
                            }                            
                            j=1; 
                            break; 
                        case 53:
                           //imshow("frozen", prev_frame);
			    myfile << "Corrupted - Frozen for: 5 seconds" <<  endl;
                            //for(int i =0; i < 5; i++){
			    for(int i = 0; i <285; i++){
				
				vid >> frame;
				//imshow("frozen", prev_frame);
				if((i%57) == 0){
					k++;
					cout << k << " seconds" << endl;
				}
				comp_num = freeze_comp(prev_frame, prev_frame, comp_num);

                                    //cout << "Freeze Time: " << i+1 << " sec" << endl;
                                    //usleep(1000000); 
                            }            
			    j=1; 
                            break; 
                        case 54:
                            imshow("frozen", prev_frame);
			    myfile << "Corrupted - Frozen for: 6 seconds" <<  endl;
                            for(int i =0; i < 342; i++){
				
				vid >> frame;
				//imshow("frozen", prev_frame);
				if((i%57) == 0){
					k++;
					cout << k << " seconds" << endl;
				}
				comp_num = freeze_comp(prev_frame, prev_frame, comp_num);

                                    //cout << "Freeze Time: " << i+1 << " sec" << endl;
                                    //usleep(1000000); 
                            }                            
                            j=1; 
                            break; 
                        case 55:
                            imshow("frozen", prev_frame);
			    myfile << "Corrupted - Frozen for: 7 seconds" <<  endl;
                            for(int i =0; i < 399; i++){
				
				vid >> frame;
				//imshow("frozen", prev_frame);
				if((i%57) == 0){
					k++;
					cout << k << " seconds" << endl;
				}
				comp_num = freeze_comp(prev_frame, prev_frame, comp_num);

                                    //cout << "Freeze Time: " << i+1 << " sec" << endl;
                                    //usleep(1000000); 
                            }
                            j=1; 
                            break; 
                        case 56:
                            imshow("frozen", prev_frame);
			    myfile << "Corrupted - Frozen for: 8 seconds" <<  endl;
                            for(int i =0; i < 456; i++){
				
				vid >> frame;
				//imshow("frozen", prev_frame);
				if((i%57) == 0){
					k++;
					cout << k << " seconds" << endl;
				}
				comp_num = freeze_comp(prev_frame, prev_frame, comp_num);

                                   // cout << "Freeze Time: " << i+1 << " sec" << endl;
                                    //usleep(1000000); 
                            }
                            j=1; 
                            break; 
                        case 57: 
                            imshow("frozen", prev_frame);
			    myfile << "Corrupted - Frozen for: 9 seconds" <<  endl;
                            for(int i =0; i < 513; i++){
				vid >> frame;
				//imshow("frozen", prev_frame);
				if((i%57) == 0){
					k++;
					cout << k << " seconds" << endl;
				}
				comp_num = freeze_comp(prev_frame, prev_frame, comp_num);

                                    //cout << "Freeze Time: " << i+1 << " sec" << endl;
                                    //usleep(1000000); 
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
		    vid >> frame; 
		    comp_num = freeze_comp(frame, img_corr, comp_num);

                    break;
                }
            } // while 
        } // else if 
	
	//***********WHITE SCREEN*****************
        else if(c=='h'){
	    myfile << "Corrupted - color: White" << endl;
	    img_corr = Mat(height, width, CV_8UC3, Scalar(255,255,255)); 
	    imshow("frozen", white1);
	    if (waitKey(0)){
	    
	    }
	    comparison(frame, img_corr, 0);
	    /*
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
	   */
		
	}
	
	
	//*************RED SCREEN*****************
	else if(c == 'r'){
	    img_corr = Mat(height, width, CV_8UC3, Scalar(0,0,255)); 
	    myfile << "Corrupted - color: Red" << endl;
	    imshow("frozen", img_corr);
	    if (waitKey(0)){
	    
	    }
	    comparison(frame, img_corr, 0);

	}
	//************GREEN SCREEN******************
	 else if(c=='g'){
	    img_corr = Mat(height, width, CV_8UC3, Scalar(0,255,0)); 
	    myfile << "Corrupted - color: green" << endl;
	    imshow("frozen", img_corr);
	    if (waitKey(0)){
	    
	    }
	    comparison(frame, img_corr, 0);

	}
	
	//*******************BLUE SCREEN****************
	 else if(c=='b'){
	    img_corr = Mat(height, width, CV_8UC3, Scalar(255,0,0)); 
	    myfile << "Corrupted - color: blue" << endl;
	    imshow("frozen", img_corr);
	    if (waitKey(0)){
	    
	    }
	    comparison(frame, img_corr, 0);
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
		float up_percent = ((int)percentage / 100.0)*480;
		up_percent = round(up_percent);
		cout << "Shifted UP by: " << up_percent << " = " << (int)percentage << "%" << endl;
		myfile << "corrupted UP by: " << up_percent << " = " << (int)percentage << "%" << endl;
		while(1){
			vid >> frame;
			cvtColor(frame, img_corr, 0);
			translateImg(img_corr, 0, -up_percent);
			imshow("frozen", img_corr);
			if((char)waitKey(1) == 27){
				break; 
			}
			/*
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
			*/
			
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
		float left_percent = ((int)percentage / 100.0)*640.0;
		left_percent = round(left_percent);
		cout << "Shifted LEFT by: " << left_percent << " = " << (int)percentage << "%" << endl;
		myfile << "corrupted LEFT by: " << left_percent << " = " << (int)percentage << "%" << endl;

		while(1){
			vid >> frame;
			cvtColor(frame, img_corr, 0);
			translateImg(img_corr, -left_percent, 0);
			imshow("frozen", img_corr);
			if((char)waitKey(1) == 27){
				break; 
			}
			/*
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
			*/
			
			
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
		float right_percent = ((int)percentage / 100.0)*640.0;
		right_percent = round(right_percent);
		cout << "Shifted RIGHT by: " << right_percent << " = " << (int)percentage << "%" << endl;
		myfile << "corrupted RIGHT by: " << right_percent << " = " << (int)percentage << "%" << endl;
		while(1){
			vid >> frame;
			cvtColor(frame, img_corr, 0);
			translateImg(img_corr, right_percent, 0);
			imshow("frozen", img_corr);
			if((char)waitKey(1) == 27){
				break; 
			}
			/*
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
			*/
			
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
		float down_percent = ((int)percentage / 100.0)*480.0;
		down_percent = round(down_percent);
		cout << "Shifted DOWN by: " << down_percent << " = " << (int)percentage << "%" << endl;
		myfile << "corrupted DOWN by: " << down_percent << " = " << (int)percentage << "%" << endl;

		while(1){
			vid >> frame;
			cvtColor(frame, img_corr, 0);
			translateImg(img_corr, 0, down_percent);
			imshow("frozen", img_corr);
			if((char)waitKey(1) == 27){
				break; 
			}
			/*
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
			*/
			
			
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
			if(percent_u >= 1){
				cout << "SHIFTED DOWN " << pixel_u << " pixels ";
				cout << "and " << percent_u << "%" << endl;
				myfile << "SHIFTED DOWN " << pixel_u << " =" << percent_u << "%" << endl;

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
			if(percent_d >= 1){
				
				cout << "SHIFTED UP " << cnt2;
				cout << " and " << percent_d << "%" << endl;
				myfile << "SHIFTED up " << cnt2 << " =" << percent_d << "%" << endl;

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
			pixel_l = round(pixel_l);
			percent_l = (pixel_l / 640.0)*100;
			percent_l = round(percent_l);
			if(percent_l >= 1){
				cout << "SHIFTED LEFT " << pixel_l;
				cout << " and " << percent_l << "%" << endl;
				myfile << "SHIFTED LEFT " << pixel_l << " =" << percent_l << "%" << endl;

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
			pixel_r = round(pixel_r);
			percent_r = (pixel_r / 640.0)*100;
			percent_r = round(percent_r);
			if(percent_r >= 1){
				cout << "SHIFTED RIGHT " << pixel_r;
				cout << " and " << percent_r << "%" << endl;
				myfile << "SHIFTED RIGHT " << pixel_r << " =" << percent_r << "%" << endl;
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
	else if(c=='u'){
		comparison(frame, img_corr, 0);
	}
	/*
	int it = 0;
	int total = 0;
	while(total<1000){
		auto begin = chrono::high_resolution_clock::now();
		vid >> frame;
		imshow("frozen", frame);
		auto end = chrono::high_resolution_clock::now();
		auto dur = end-begin;
		auto ms = std::chrono::duration_cast<std::chrono::milliseconds>(dur).count();
		total = ms + total;
		it++;
		cout << "mili: " << total << endl;
	}
	cout << "IT: " << it << endl;
	break;
	*/
	
	//time_t time_now,end;
	//time(&time_now);
	//comparison(frame, img_corr);
	//usleep(2000000);
	//time(&end);
	//double secs = difftime(end, time_now);
	//cout << "Time " << secs << endl;
	

	comp_num = comparison(frame, img_corr, comp_num);
	comp_num = freeze_comp(frame, img_corr, comp_num);
	//cout << "Comp num: " << comp_num << endl;
	frame_num++;
	myfile << "Frame: " << frame_num << " - original" << endl;
    } // while 1
    myfile.close();
    vid.release();
    return 0; 
}

// end of MY CODE FOR CORRUPTION/COMPARISON

//**********************************************************
 
