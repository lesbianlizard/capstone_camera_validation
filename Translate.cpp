#include "Translate.hpp"
 
Translate::Translate(uint32_t dur, int x, int y, int x_off, int y_off) : 
    Distortion(dur), m_width(x), m_height(y), m_x(x_off), m_y(y_off)
{}

// ['3', 't/f', 'dur(ms)', 'x_offset', 'y_offset']
void Translate::update(std::vector<std::string> cmd)
{

    if(stoi(cmd[1], nullptr))
        activate();
    setTime(stoi(cmd[2], nullptr));
    int x_off = stoi(cmd[3], nullptr);
    int y_off = stoi(cmd[4], nullptr);
    if(x_off <= m_width && x_off >= -m_width)
        setX(x_off);
    if(y_off <= m_height && y_off >= -m_height)
        setY(y_off);
}

void Translate::setX(int x)
{
    m_x = x;
}

void Translate::setY(int y)
{
    m_y = y;
}

void Translate::run(cv::Mat *&frame)
{
    if(m_on)
    {
        startTimer();
        m_on = false;
    }
    if(isActive())
    {
        cv::Mat trans_mat = (cv::Mat_<double>(2, 3) << 1, 0, m_x, 0, 1, m_y);  
        cv::warpAffine(*frame, *frame, trans_mat, frame->size());   
    }
}