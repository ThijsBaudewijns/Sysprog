#ifndef REVERSEPLUGIN_H_
#define REVERSEPLUGIN_H_

#include "ITextProcessor.hpp"

extern "C" {
    __declspec(dllexport) sp::ITextProcessor* create_instance();
}

#endif // REVERSEPLUGIN_H_