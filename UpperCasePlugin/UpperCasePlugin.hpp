#ifndef UPPERCASEPLUGIN_H_
#define UPPERCASEPLUGIN_H_

#include "ITextProcessor.hpp"

extern "C" {
    __declspec(dllexport) sp::ITextProcessor* create_instance();
}

#endif // UPPERCASEPLUGIN_H_