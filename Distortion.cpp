#include "Distortion.hpp"

Distortion::Distortion(int dur, cv::VideoCapture* vid, cv::Mat* frame, std::string window) :
    m_dur(dur), m_vid(vid), m_frame(frame), m_window(window),
    m_start(std::chrono::high_resolution_clock::now()), m_end(std::chrono::milliseconds(0))
{}


void Distortion::setTime(int dur)
{
    m_dur = dur; 
}

void Distortion::startTimer()
{
    m_end = m_start + std::chrono::milliseconds(m_dur);
}

bool Distortion::checkTime()
{
    m_start = std::chrono::high_resolution_clock::now();
    if(std::chrono::duration_cast<std::chrono::milliseconds>(m_end - m_start).count() > 0)
        return true;
    return false;
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

