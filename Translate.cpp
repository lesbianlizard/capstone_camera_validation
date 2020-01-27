#include "Translate.hpp"
 
Translate::Translate(uint32_t dur, cv::VideoCapture* vid, cv::Mat* frame, std::string window, int x, int y) : 
    Distortion(dur, vid, frame, window), m_x(x), m_y(y), 
    m_start(std::chrono::high_resolution_clock::now()), m_end(std::chrono::milliseconds(0))
{}

void Translate::setX(int x)
{
    m_x = x;
}


void Translate::setY(int y)
{
    m_y = y;
}

void Translate::translateFrame()
{
	cv::Mat trans_mat = (cv::Mat_<double>(2, 3) << 1, 0, m_x, 0, 1, m_y);
	cv::warpAffine(*m_frame, *m_frame, trans_mat, m_frame->size());               
}


void Translate::run()
{
    m_start = std::chrono::high_resolution_clock::now();
    readFrame(); 
    checkFrame();
    char cmd = cv::waitKey(1);
    if(cmd == '1')
        m_end = m_start + std::chrono::milliseconds(m_dur);
    if(std::chrono::duration_cast<std::chrono::milliseconds>(m_end - m_start).count() > 0)
        translateFrame();
    render();
}



   /* if(cmd == '2'){
        int val;
        std::cout<<"Enter a number corresponding to the shade (0-255): ";
        while(1)
        {
            std::cin >> val;
            std::cin.ignore();
            if(val >= 0 && val <= 255)
                break;
            else
                std::cout<<"Enter a value between 0 and 255: ";
        }
        setShade(val); */