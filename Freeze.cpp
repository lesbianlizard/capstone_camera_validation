#include "Freeze.hpp"
 
 
Freeze::Freeze(uint32_t dur, cv::VideoCapture* vid, cv::Mat* frame, std::string window) : 
    Distortion(dur, vid, frame, window)
{}

Freeze::~Freeze() {}

void Freeze::setPrev()
{
    m_prevFrame = m_frame;
}

void Freeze::run()
{
    setPrev();
    readFrame(); 
    checkFrame();
    char cmd = cv::waitKey(1);
    if(cmd == 'm'){
        cv::imshow(m_window, *m_prevFrame);
        usleep(m_dur*1000); 
    }
    else 
        render();
}

