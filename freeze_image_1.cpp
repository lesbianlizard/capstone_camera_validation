/*************************************************
 * Freeze image program
 * AARON WALDER
 * press 'f' to freeze and follow by pressing a number 
 * 1-9 in order to freeze for that amount of time
 * press 'p' for FPS
************************************************/

#include <stdio.h>
#include <opencv2/opencv.hpp>
#include <iostream>
#include <unistd.h> 
#include <stdio.h> 

using namespace cv;
using namespace std; 



int main(){
    Mat frame;
    Mat prev_frame; 
    Mat white;
    Mat mask; 
    
    VideoCapture vid(-1); // start video capture
    if(!vid.isOpened()){
        return 0; 
    }
    vid.set(CAP_PROP_FRAME_WIDTH, 640);
    vid.set(CAP_PROP_FRAME_HEIGHT, 480);
    cout << "Width: 640" << endl; 
    cout << "Height: 480" << endl; 
    vid>>frame; 
    //white = frame; 
    

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
        else if(c==102){ // press 'f' to freeze 
            // while loop to freeze indefinitely
            // press esc to end freeze
            while(1){
                char c1 = (char)waitKey(25);
                int j = 0; 
                cout << "Freeze Entered: press a # 1-9" << endl;
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
                            vid>>frame; 
                            imshow("frozen",frame);
                            usleep(1000000);
                }// switch 
                if(j==1){
                    cout << "Unfrozen" << endl;
                    break;
                }
            } // while 
        } // else if 
        else if(c==119){
            Mat white_image = imread("white.jpg");
            
            imshow("frozen", white_image);
            usleep(2000000);
        }
        
        else if(c==112){ // press 'p' to show fps
            double fps = vid.get(CAP_PROP_FPS); 
            cout << "FPS " << fps << " frames/sec" << endl; 
        }
        imshow("frozen", frame); 
    }
    
    return 0; 
}
