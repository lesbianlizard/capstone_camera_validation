#ifndef _DISTORTION_H_
#define _DISTORTION_H_

#include <opencv2/opencv.hpp>
#include <mutex>  
#include <chrono>

class Distortion 
{
    public: 
        Distortion(int dur);
        virtual void run(cv::Mat *&frame) = 0; 
        void setTime(int dur); 
        void activate();

    protected:
        virtual void update() = 0;
        void startTimer();
        bool isActive();
        void takeTime();
        int m_dur;                  // in ms
        bool m_on;
        std::chrono::system_clock::time_point m_start; 
        std::chrono::system_clock::time_point m_end;
};

#endif // _DISTORTION_H_