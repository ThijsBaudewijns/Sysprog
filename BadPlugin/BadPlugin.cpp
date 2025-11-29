#include "BadPlugin.hpp"

namespace sp {

    class BadPlugin : public ITextProcessor {
    public:
        std::string name() const override;
        std::string description() const override;
        std::string process(const std::string& input) const override;
    };

}

std::string sp::BadPlugin::name() const {
    return "BadPlugin";
}

std::string sp::BadPlugin::description() const {
    return "This plugin intentionally crashes!";
}

std::string sp::BadPlugin::process(const std::string& input) const {
    // Intentionally cause a crash by dereferencing a null pointer
    int* bad_ptr = nullptr;
    *bad_ptr = 42;  // This will crash!

    return input;  // Never reached
}

sp::ITextProcessor* create_instance() {
    return new sp::BadPlugin;
}