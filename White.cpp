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

// ['2', 't/f', 'dur(ms)', 'shade(0-255)']
void White::update(std::vector<std::string> cmd)
{
    if(stoi(cmd[1], nullptr))
        activate();
    setTime(stoi(cmd[2], nullptr));
    int shade = stoi(cmd[3], nullptr);
    if(shade <= 255 && shade >= 0)
        setShade(shade);
}

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

