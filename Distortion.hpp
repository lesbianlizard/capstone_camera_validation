#ifndef _DISTORTION_H_
#define _DISTORTION_H_

#include <opencv2/opencv.hpp>
#include <string>

class Distortion 
{
    public: 
        Distortion(int dur, cv::VideoCapture* vid, cv::Mat* frame, std::string window);
        virtual void run() = 0; 
        void setTime(int dur); 

    protected:
        void checkFrame();
        void readFrame();
        void render();
        std::string m_window;
        int m_dur;     // in ms
        cv::VideoCapture* m_vid;
        cv::Mat* m_frame;

};

#endif // _DISTORTION_H_