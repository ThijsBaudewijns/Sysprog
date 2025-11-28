#ifndef PLUGINWRAPPER_H_
#define PLUGINWRAPPER_H_

#include <string>

namespace sp {

    class PluginWrapper {
    public:
        PluginWrapper(const std::string& path);
        ~PluginWrapper();

        PluginWrapper(const PluginWrapper&) = delete;
        PluginWrapper& operator=(const PluginWrapper&) = delete;

        PluginWrapper(PluginWrapper&& other) noexcept;
        PluginWrapper& operator=(PluginWrapper&& other) noexcept;

        void* plugin() const { return _plugin; }
        void* lookup(const std::string& symbol) const;

    private:
        void* _plugin{ nullptr };
        void cleanup();
        void move(PluginWrapper&& other) noexcept;
    };

}

#endif // PLUGINWRAPPER_H_