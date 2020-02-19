#include "Compare.hpp"


Compare::Compare(){}

void Compare::run(cv::Mat *frame, cv::Mat *dframe){
    if(!_isCorrupt(frame, dframe))
        return;
        
    std::cout << "\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n" <<std::endl;
    if(_isWhite(dframe)){
        std::cout << "All white detected" <<std::endl;
        std::cout << "image colors: "<< m_shade <<std::endl;
        return;
    }
    if(_isFrozen(dframe)){
        std::cout << "Frozen frames detected" <<std::endl;
        return;
    }
    if(_isTranslated(dframe)){
        std::cout <<"TX detected: "<<std::endl;
        std::cout <<"right shift: "<< m_rs << std::endl;
        std::cout <<"left shift: "<< m_ls << std::endl;
        std::cout <<"top shift: "<< m_ts << std::endl;
        std::cout <<"bot shift: "<< m_bs << std::endl;
    }
}       

void Compare::printStatus(){}

bool Compare::_identical(cv::Mat* a, cv::Mat* b)
{
    if(a->rows != b->rows || a->cols != b->cols)
        return false;
    cv::Vec3b *d_row, *row;
    for (int j = 0; j < a->rows; ++j){  
        d_row = a->ptr<cv::Vec3b>(j);
        row = b->ptr<cv::Vec3b>(j);
        for (int i = 0; i < a->cols; ++i){
            if(d_row[i][0] != row[i][0] || d_row[i][1] != row[i][1] || d_row[i][2] != row[i][2])
                return false;
        }
    }
    return true;
}

bool Compare::_isCorrupt(cv::Mat* dframe, cv::Mat *frame)
{
    if(!_identical(dframe, frame))
        return true;
    m_prev = frame->clone(); 
    return false;
}

bool Compare::_isWhite(cv::Mat* dframe)   
{
    cv::Vec3b rgb = dframe->at<cv::Vec3b>(0, 0);
    cv::Size sz = dframe->size();
    cv::Mat test(sz.height, sz.width, CV_8UC3, rgb);
    if(!_identical(dframe, &test))
        return false;
    m_shade = rgb; 
    return true;
}

bool Compare::_isFrozen(cv::Mat* dframe) 
{
    if(!_identical(dframe, &m_prev))
        return false;
    return true;
}

// if we hit isTranslated then we have already tested for blank frames
bool Compare::_isTranslated(cv::Mat* dframe)
{
    m_ls = 0; m_rs = dframe->cols-1; m_ts = 0; m_bs = dframe->rows-1;
    while(_colEmpty(dframe, m_ls))
        m_ls++; 
    while(_colEmpty(dframe, m_rs)){
        m_rs--;
    }
    m_rs -= dframe->cols-1;
    while(_rowEmpty(dframe, m_ts))
        m_ts++;
    while(_rowEmpty(dframe, m_bs)){
        m_bs--;
    }
    m_bs -= dframe->rows-1;
}

bool Compare::_colEmpty(cv::Mat* a, int i){
    cv::Vec3b *row;   
    cv::Vec3b *rgb = a->ptr<cv::Vec3b>(0);
    for(int j=0; j < a->rows; ++j){
        row = a->ptr<cv::Vec3b>(j);
        if(row[i][0] != rgb[i][0] || row[i][1] != rgb[i][1] || row[i][2] != rgb[i][2])
            return false;
    }
    return true;
}

bool Compare::_rowEmpty(cv::Mat* a, int i)
{
    cv::Vec3b *rgb = a->ptr<cv::Vec3b>(i);
    cv::Vec3b *row = a->ptr<cv::Vec3b>(i);
    for(int j=0; j < a->cols; ++j){
        if(row[j][0] != rgb[0][0] || row[j][1] != rgb[0][1] || row[j][2] != rgb[0][2])
            return false;
    }
    return true; 
}


