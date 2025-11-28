#include "LowerCasePlugin.hpp"
#include <algorithm>
#include <cctype>

namespace sp {

    class LowerCasePlugin : public ITextProcessor {
    public:
        std::string name() const override;
        std::string description() const override;
        std::string process(const std::string& input) const override;
    };

}

std::string sp::LowerCasePlugin::name() const {
    return "LowerCase";
}

std::string sp::LowerCasePlugin::description() const {
    return "Converts all text to lower case";
}

std::string sp::LowerCasePlugin::process(const std::string& input) const {
    std::string result = input;
    std::transform(result.begin(), result.end(), result.begin(),
        [](unsigned char c) { return std::tolower(c); });
    return result;
}

sp::ITextProcessor* create_instance() {
    return new sp::LowerCasePlugin;
}