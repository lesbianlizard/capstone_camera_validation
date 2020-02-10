#ifndef _FREEZE_H_
#define _FREEZE_H_

#include "Distortion.hpp"

class Freeze: public Distortion 
{
    public: 
        Freeze(uint32_t dur);
        void run(cv::Mat *&frame); 
        void update(std::vector<std::string> cmd);
    
    private:
        cv::Mat m_prev;
};

#endif // _FREEZE_H_