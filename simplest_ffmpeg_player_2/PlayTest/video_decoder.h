#pragma once

#include <string>
#include <stdint.h>

class video_decoder
{
public:

    video_decoder();


    virtual ~video_decoder();
    typedef void(*error)(const std::string&);
    typedef int(*output)(const uint8_t* const* data, int w, int h, const int* linesize);
    //************************************
    // Method:    init
    // FullName:  video_decoder::init
    // Access:    public 
    // Returns:   bool
    // Qualifier:
    //************************************
    bool init(const std::string & name, output outfunc, error func);
    bool run();

private:
    std::string name_;
    output output_;
    bool run_;
};

