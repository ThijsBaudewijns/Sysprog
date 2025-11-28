#ifndef ITEXTPROCESSOR_H_
#define ITEXTPROCESSOR_H_

#include <string>

namespace sp {

    class ITextProcessor {
    public:
        virtual ~ITextProcessor() = default;

        // Returns the name of this text processor
        virtual std::string name() const = 0;

        // Returns a description of what this processor does
        virtual std::string description() const = 0;

        // Processes the input text and returns the modified text
        virtual std::string process(const std::string& input) const = 0;
    };

}

#endif // ITEXTPROCESSOR_H_