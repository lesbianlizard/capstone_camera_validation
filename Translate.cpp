#include "Translate.hpp"
 
Translate::Translate(uint32_t dur, cv::VideoCapture* vid, cv::Mat* frame, std::string window, int x, int y) : 
    Distortion(dur, vid, frame, window), m_x(x), m_y(y)
{}

void Translate::setX(int x)
{
    m_x = x;
}


void Translate::setY(int y)
{
    m_y = y;
}

void Translate::distort()
{
	cv::Mat trans_mat = (cv::Mat_<double>(2, 3) << 1, 0, m_x, 0, 1, m_y);
	cv::warpAffine(*m_frame, *m_frame, trans_mat, m_frame->size());               
}


void Translate::run()
{
    readFrame(); 
    checkFrame();
    char cmd = cv::waitKey(1);
    if(cmd == '1')
        startTimer();
    if(checkTime())
        distort();
    render();
}