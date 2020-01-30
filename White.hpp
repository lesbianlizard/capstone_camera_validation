#ifndef _WHITE_H_
#define _WHITE_H_

#include "Distortion.hpp"

class White: public Distortion 
{
    public: 
        White(uint32_t dur, int shade, int height, int width);
        void run(cv::Mat *&frame); 
        void update();

    private:
        void setShade(int shade);
        cv::Mat m_blank;
};

#endif // _WHITE_H_