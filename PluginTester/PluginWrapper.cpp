#include "PluginWrapper.hpp"
#include <stdexcept>
#include <sstream>
#include <windows.h>

using namespace sp;

PluginWrapper::PluginWrapper(const std::string& path) {
    _plugin = LoadLibraryA(path.c_str());
    if (!_plugin) {
        std::ostringstream oss;
        oss << "Plugin could not be opened: " << path << ", error " << GetLastError();
        throw std::runtime_error(oss.str());
    }
}

PluginWrapper::~PluginWrapper() {
    cleanup();
}

PluginWrapper::PluginWrapper(PluginWrapper&& other) noexcept {
    move(std::move(other));
}

PluginWrapper& PluginWrapper::operator=(PluginWrapper&& other) noexcept {
    if (&other != this) {
        cleanup();
        move(std::move(other));
    }
    return *this;
}

void PluginWrapper::cleanup() {
    if (_plugin) {
        FreeLibrary(reinterpret_cast<HMODULE>(_plugin));
        _plugin = nullptr;
    }
}

void PluginWrapper::move(PluginWrapper&& other) noexcept {
    _plugin = other._plugin;
    other._plugin = nullptr;
}

void* PluginWrapper::lookup(const std::string& symbol) const {
    void* result = GetProcAddress(reinterpret_cast<HMODULE>(_plugin), symbol.c_str());
    if (!result) {
        std::ostringstream oss;
        oss << "Symbol '" << symbol << "' could not be found, error " << GetLastError();
        throw std::runtime_error(oss.str());
    }
    return result;
}