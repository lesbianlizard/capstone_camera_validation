#include "Freeze.hpp"
 
 
Freeze::Freeze(uint32_t dur) : 
    Distortion(dur)
{}

void Freeze::update()
{}

void Freeze::run(cv::Mat *&frame)
{
    if(m_on)
    {
        startTimer();
        m_prev = frame->clone();
        m_on = false; 
    }
    if(isActive())
        frame = &m_prev;
}


