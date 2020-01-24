#include "Distortion.hpp"

Distortion::Distortion(int dur, cv::VideoCapture* vid, cv::Mat* frame, std::string window) :
    m_dur(dur), m_vid(vid), m_frame(frame), m_window(window)
{}

Distortion::~Distortion()
{}
//Distortion.hpp Distortion.cpp Freeze.hpp Freeze.cpp corruption.cpp

void Distortion::setTime(int dur)
{
    m_dur = dur; 
}

void Distortion::readFrame()
{
    *m_vid >> *m_frame; 
}

void Distortion::checkFrame()
{
    if(m_frame->empty())
    {
        std::cout<<"EXIT";
        exit(EXIT_FAILURE);
    }
}

void Distortion::render()
{
    cv::imshow(m_window, *m_frame);
    cv::waitKey(1);
}
