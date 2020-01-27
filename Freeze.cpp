#include "Freeze.hpp"
 
 
Freeze::Freeze(uint32_t dur, cv::VideoCapture* vid, cv::Mat* frame, std::string window) : 
    Distortion(dur, vid, frame, window)
{}

// Send the data out of this function!!
void Freeze::distort()
{
    cv::imshow(m_window, m_prevFrame);
    cv::waitKey(1);
}

void Freeze::setPrev()
{
    m_prevFrame = m_frame->clone();
}

void Freeze::run()
{
    readFrame(); 
    checkFrame();
    char cmd = cv::waitKey(1);
    if(cmd == '1')
    {
        startTimer();
        setPrev();
    }
    if(checkTime())
        distort();
    else
        render();
}


