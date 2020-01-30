#include "White.hpp"
 
White::White(uint32_t dur, int shade, int height, int width) : 
    Distortion(dur)
{
    m_blank = cv::Mat(height, width, CV_8UC3, cv::Scalar(shade,shade,shade)); 
}

void White::setShade(int shade)
{
    m_blank.setTo(cv::Scalar(shade, shade, shade));
    std::cout << "Shade set to " << shade << std::endl; 
}

void White::update()
{}

void White::run(cv::Mat *&frame)
{
    if(m_on)
    {
        startTimer();
        m_on = false;
    }
    if(isActive())
        frame = &m_blank;   
}

