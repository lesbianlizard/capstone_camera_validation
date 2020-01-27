
// CAMERA CODE
/*************************************************
 * Freeze image program
 * AARON WALDER
 * press 'f' to freeze and follow by pressing a number 
 * 1-9 in order to freeze for that amount of time
 * press 'p' for FPS
 * Press 'w' to white screen, and press 'w' again
 * to leave white screen
************************************************/

#include <stdio.h>
#include <opencv2/opencv.hpp>
#include <iostream>
#include <unistd.h> 
#include <stdio.h> 
#include "Distortion.hpp"
#include "Freeze.hpp"
#include "White.hpp"
#include "Translate.hpp"

void setup(cv::VideoCapture &vid, int &width, int &height);

enum distortion{FREEZE, WHITE, SHIFT};

int main()
{
    int width, height;
    cv::Mat frame; 
    std::string window = "window";
    std::vector<Distortion*> dis(3);
    enum distortion type = WHITE;
    cv::VideoCapture vid(0); 
    
    setup(vid, width, height);
    
    Freeze freeze(3000, &vid, &frame, window);
    White white(3000, &vid, &frame, window, 255, height, width); 
    Translate translate(3000, &vid, &frame, window, 50,50); 

    cv::namedWindow(window, cv::WINDOW_AUTOSIZE);
    
    dis[0] = &freeze;
    dis[1] = &white;
    dis[2] = &translate;

    while(1){
        dis[type]->run();
    }
        
    return 0; 
}


void setup(cv::VideoCapture &vid, int &width, int &height)
{
    int size_in;
    if(!vid.isOpened()){
        return;
    }

    while(1){
        std::cout << "Press 1: 640x480, 2: 1280x720, 3: 1920x1080" << std::endl;
        std::cin >> size_in;
        if(size_in == 1){
            height = 480;
            width = 640; 
            std::cout << "640x480" << std::endl;
            break;
        }
        else if(size_in == 2){
            height = 720;
            width = 1280;    
            std::cout << "1280x720" << std::endl;
            break;
        }
        else if(size_in == 3){
            height = 1080;
            width = 1920; 
            std::cout << "1920x1080" << std::endl;
            break; 
        }
    }

    vid.set(cv::CAP_PROP_FRAME_WIDTH, width);
    vid.set(cv::CAP_PROP_FRAME_HEIGHT, height); 
}