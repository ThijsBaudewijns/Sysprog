#include <iostream>
#include <vector>
#include <string>
#include <filesystem>
#include <windows.h>

// Scan directory for plugin files
std::vector<std::string> scan_plugins(const std::string& path) {
    std::vector<std::string> plugins;

    if (!std::filesystem::exists(path)) {
        std::cerr << "Warning: plugins directory '" << path << "' does not exist.\n";
        return plugins;
    }

    for (const auto& entry : std::filesystem::directory_iterator{ path }) {
        std::string filename{ entry.path().string() };
        if (filename.size() >= 5 && filename.substr(filename.size() - 5) == ".plug") {
            plugins.push_back(filename);
        }
    }
    return plugins;
}

// Test a plugin by running PluginTester in a separate process
bool test_plugin(const std::string& tester_path, const std::string& plugin_path) {
    STARTUPINFOA si;
    PROCESS_INFORMATION pi;

    ZeroMemory(&si, sizeof(si));
    si.cb = sizeof(si);
    ZeroMemory(&pi, sizeof(pi));

    // Build command line: "PluginTester.exe <plugin_path>"
    // CreateProcess requires a modifiable string
    // Quote paths in case they contain spaces
    std::string cmdline_str = "\"" + tester_path + "\" \"" + plugin_path + "\"";
    std::vector<char> cmdline(cmdline_str.begin(), cmdline_str.end());
    cmdline.push_back('\0'); // Null terminator

    std::cout << "\n========================================\n";
    std::cout << "Testing: " << plugin_path << "\n";
    std::cout << "Command: " << cmdline_str << "\n";
    std::cout << "========================================\n";

    // Start the child process
    if (!CreateProcessA(
        nullptr,                // No module name (use command line)
        cmdline.data(),         // Command line
        nullptr,                // Process handle not inheritable
        nullptr,                // Thread handle not inheritable
        FALSE,                  // Set handle inheritance to FALSE
        0,                      // No creation flags
        nullptr,                // Use parent's environment block
        nullptr,                // Use parent's starting directory
        &si,                    // Pointer to STARTUPINFO structure
        &pi))                   // Pointer to PROCESS_INFORMATION structure
    {
        std::cerr << "CreateProcess failed (" << GetLastError() << ")\n";
        return false;
    }

    // Wait until child process exits
    WaitForSingleObject(pi.hProcess, INFINITE);

    // Get exit code
    DWORD exit_code;
    bool success = false;

    if (GetExitCodeProcess(pi.hProcess, &exit_code)) {
        if (exit_code == 0) {
            std::cout << "\nPlugin is TRUSTED - exited normally\n";
            success = true;
        }
        else {
            std::cout << "\nPlugin is UNTRUSTED - exited with code " << exit_code << "\n";
            success = false;
        }
    }
    else {
        std::cerr << "\nPlugin is UNTRUSTED - could not get exit code, error "
            << GetLastError() << "\n";
        success = false;
    }

    // Close process and thread handles
    CloseHandle(pi.hProcess);
    CloseHandle(pi.hThread);

    return success;
}

int main(int argc, char* argv[]) {
    std::cout << "=== Plugin Sandbox Tester ===\n";
    std::cout << "This program tests plugins in a separate process for safety.\n\n";

    // Path to the PluginTester executable
    std::string tester_path = "PluginTester.exe";

    // Check if PluginTester exists
    if (!std::filesystem::exists(tester_path)) {
        std::cerr << "Error: PluginTester.exe not found in current directory.\n";
        std::cerr << "Please ensure PluginTester.exe is built and in the same directory.\n";
        return 1;
    }

    // Get all plugins
    std::vector<std::string> plugins = scan_plugins("plugins");

    if (plugins.empty()) {
        std::cerr << "No plugins found in 'plugins' directory.\n";
        return 1;
    }

    std::cout << "Found " << plugins.size() << " plugin(s) to test.\n";

    // Test each plugin
    int trusted_count = 0;
    int untrusted_count = 0;

    for (const std::string& plugin : plugins) {
        if (test_plugin(tester_path, plugin)) {
            trusted_count++;
        }
        else {
            untrusted_count++;
        }
    }

    // Summary
    std::cout << "\n========================================\n";
    std::cout << "Testing Summary:\n";
    std::cout << "  Trusted plugins:   " << trusted_count << "\n";
    std::cout << "  Untrusted plugins: " << untrusted_count << "\n";
    std::cout << "========================================\n";

    return 0;
}