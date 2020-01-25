#ifndef _WHITE_H_
#define _WHITE_H_

#include <unistd.h> 
#include "Distortion.hpp"

class White: public Distortion 
{
    public: 
        White(uint32_t dur, cv::VideoCapture* vid, cv::Mat* frame, std::string window, int shade, int height, int width);
        void run(); 
        void setShade(int shade);

    private:
        cv::Mat m_blank;
};

#endif // _WHITE_H_