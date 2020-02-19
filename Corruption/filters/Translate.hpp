#ifndef _TRANSLATE_H_
#define _TRANSLATE_H_

#include "Distortion.hpp"

class Translate: public Distortion 
{
    public: 
        Translate(uint32_t dur, int x, int y, int x_off, int y_off);
        void run(cv::Mat *&frame); 
        void update(std::vector<std::string> cmd);

    private:
        void setX(int x);
        void setY(int y);
        int m_width;
        int m_height;
        int m_x;
        int m_y;
};

#endif // _TRANSLATE_H_