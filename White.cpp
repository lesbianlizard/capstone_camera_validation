#include "White.hpp"
 
White::White(uint32_t dur, cv::VideoCapture* vid, cv::Mat* frame, std::string window, int shade, int height, int width) : 
    Distortion(dur, vid, frame, window)
{
    m_blank = cv::Mat(height, width, CV_8UC3, cv::Scalar(shade,shade,shade)); 
}

void White::setShade(int shade)
{
    m_blank.setTo(cv::Scalar(shade, shade, shade));
    std::cout << "Shade set to " << shade << std::endl; 
}

void White::distort()
{
    cv::imshow(m_window, m_blank);
    cv::waitKey(1);
}

void White::run()
{
    readFrame(); 
    checkFrame();
    char cmd = cv::waitKey(1);
    if(cmd == '1')
        startTimer();
    if(cmd == '2'){
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
        setShade(val);
    }
    if(checkTime())
        distort();
    else
        render();
}

