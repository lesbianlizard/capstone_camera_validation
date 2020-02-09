#ifndef _COMPARE_H_
#define _COMPARE_H_

#include <opencv2/opencv.hpp>

class Compare 
{
    public: 
        Compare();
        void run(cv::Mat *frame, cv::Mat *dframe);  
        void printStatus();     
        
    private:
        cv::Mat m_prev;
        cv::Vec3b m_shade; 
        bool m_frozen; 
        int m_bs;
        int m_ts;
        int m_ls;
        int m_rs;

        bool _identical(cv::Mat* a, cv::Mat* b);
        bool _isCorrupt(cv::Mat* dframe, cv::Mat *frame);
        bool _isTranslated(cv::Mat* dframe); 
        bool _isFrozen(cv::Mat* dframe);
        bool _isWhite(cv::Mat* dframe);
        bool _colEmpty(cv::Mat* a, int i);
        bool _rowEmpty(cv::Mat* a, int i);
};

#endif // _COMPARE_H_