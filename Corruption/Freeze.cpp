#include "Freeze.hpp"
 
 
Freeze::Freeze(uint32_t dur) : 
    Distortion(dur)
{}

// ['1', 't/f', 'dur(ms)']
void Freeze::update(std::vector<std::string> cmd)
{
    if(stoi(cmd[1], nullptr))
        activate();
    setTime(stoi(cmd[2], nullptr));
}

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


