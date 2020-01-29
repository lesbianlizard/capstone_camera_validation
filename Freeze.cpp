#include "Freeze.hpp"
 
 
Freeze::Freeze(uint32_t dur, cv::VideoCapture* vid, cv::Mat* frame, std::string window) : 
    Distortion(dur, vid, frame, window), m_on(false)
{}

void Freeze::distort()
{}

void Freeze::update()
{}

void Freeze::run()
{
    if(!m_on)
        readFrame(); 
    checkFrame();
    char cmd = cv::waitKey(1);
    if(cmd == '1')
    {
        startTimer();
        m_on = true; 
    }
    if(!checkTime())
        m_on = false;
    render();
}


