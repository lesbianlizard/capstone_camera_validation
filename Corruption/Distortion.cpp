#include "Distortion.hpp"

Distortion::Distortion(int dur) : 
    m_dur(dur), m_start(std::chrono::high_resolution_clock::now()), m_end(std::chrono::milliseconds(0)), m_on(false)
{}

void Distortion::setTime(int dur)
{
    m_dur = dur; 
}

void Distortion::startTimer()
{
    takeTime();
    m_end = m_start; 
}

void Distortion::activate()
{
    m_on = true;
}

// migrate adding the duratino to m_end in the checkTime function so dynamically
// increasing the duration through the gui effects the bevahior if a timer is alread set
bool Distortion::isActive()
{
    takeTime();
    if(std::chrono::duration_cast<std::chrono::milliseconds>((m_end + std::chrono::milliseconds(m_dur)) - m_start).count() > 0)
        return true;
    return false;
}

void Distortion::takeTime()
{
    m_start = std::chrono::high_resolution_clock::now();
}

