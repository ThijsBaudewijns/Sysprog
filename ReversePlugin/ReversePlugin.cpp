#include "ReversePlugin.hpp"
#include <algorithm>
#include <sstream>

namespace sp {

    class ReversePlugin : public ITextProcessor {
    public:
        std::string name() const override;
        std::string description() const override;
        std::string process(const std::string& input) const override;
    };

}

std::string sp::ReversePlugin::name() const {
    return "ReverseLine";
}

std::string sp::ReversePlugin::description() const {
    return "Reverses each line of text";
}

std::string sp::ReversePlugin::process(const std::string& input) const {
    std::istringstream iss(input);
    std::ostringstream oss;
    std::string line;
    bool first = true;

    while (std::getline(iss, line)) {
        if (!first) {
            oss << '\n';
        }
        first = false;

        std::reverse(line.begin(), line.end());
        oss << line;
    }

    return oss.str();
}

sp::ITextProcessor* create_instance() {
    return new sp::ReversePlugin;
}