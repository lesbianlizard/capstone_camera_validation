#ifndef _FREEZE_H_
#define _FREEZE_H_

#include <unistd.h> 
#include "Distortion.hpp"

class Freeze: public Distortion 
{
    public: 
        Freeze(uint32_t dur, cv::VideoCapture* vid, cv::Mat* frame, std::string window);
        void run(); 

    private:
        void setPrev();
        cv::Mat* m_prevFrame;
};

#endif // _FREEZE_H_