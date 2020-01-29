#ifndef _FREEZE_H_
#define _FREEZE_H_

#include <unistd.h> 
#include "Distortion.hpp"

class Freeze: public Distortion 
{
    public: 
        Freeze(uint32_t dur, cv::VideoCapture* vid, cv::Mat* frame, std::string window);
        void run(); 
        void update();

    private:
        void distort();
        bool m_on;
};

#endif // _FREEZE_H_