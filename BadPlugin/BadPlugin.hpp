#ifndef BADPLUGIN_H_
#define BADPLUGIN_H_

#include "ITextProcessor.hpp"

extern "C" {
    __declspec(dllexport) sp::ITextProcessor* create_instance();
}

#endif