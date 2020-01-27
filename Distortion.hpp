#ifndef _DISTORTION_H_
#define _DISTORTION_H_

#include <opencv2/opencv.hpp>
#include <string>
#include <chrono>

class Distortion 
{
    public: 
        Distortion(int dur, cv::VideoCapture* vid, cv::Mat* frame, std::string window);
        virtual void run() = 0; 
        void setTime(int dur); 

    protected:
        virtual void distort() = 0;
        void startTimer();
        bool checkTime();
        void checkFrame();
        void readFrame();
        void render();
        int m_dur;                  // in ms
        std::string m_window;
        cv::VideoCapture* m_vid;
        cv::Mat* m_frame;
        std::chrono::system_clock::time_point m_start; 
        std::chrono::system_clock::time_point m_end;
};

#endif // _DISTORTION_H_