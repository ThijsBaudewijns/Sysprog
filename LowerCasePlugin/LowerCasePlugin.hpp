#ifndef LOWERCASEPLUGIN_H_
#define LOWERCASEPLUGIN_H_

#include "ITextProcessor.hpp"

extern "C" {
    __declspec(dllexport) sp::ITextProcessor* create_instance();
}

#endif // LOWERCASEPLUGIN_H_