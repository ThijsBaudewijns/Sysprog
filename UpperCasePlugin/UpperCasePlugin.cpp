#include "UpperCasePlugin.hpp"
#include <algorithm>
#include <cctype>

namespace sp {

    class UpperCasePlugin : public ITextProcessor {
    public:
        std::string name() const override;
        std::string description() const override;
        std::string process(const std::string& input) const override;
    };

}

std::string sp::UpperCasePlugin::name() const {
    return "UpperCase";
}

std::string sp::UpperCasePlugin::description() const {
    return "Converts all text to UPPER CASE";
}

std::string sp::UpperCasePlugin::process(const std::string& input) const {
    std::string result = input;
    std::transform(result.begin(), result.end(), result.begin(),
        [](unsigned char c) { return std::toupper(c); });
    return result;
}

sp::ITextProcessor* create_instance() {
    return new sp::UpperCasePlugin;
}