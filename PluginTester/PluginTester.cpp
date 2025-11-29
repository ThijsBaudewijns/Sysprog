#include <iostream>
#include <string>
#include <memory>
#include "PluginWrapper.hpp"
#include "ITextProcessor.hpp"

int main(int argc, char* argv[]) {
    // Debug: Show what we received
    std::cout << "PluginTester started\n";
    std::cout << "argc = " << argc << "\n";
    for (int i = 0; i < argc; ++i) {
        std::cout << "argv[" << i << "] = " << argv[i] << "\n";
    }
    std::cout << "\n";

    // Check if plugin path was provided
    if (argc < 2) {
        std::cerr << "Usage: " << argv[0] << " <plugin_path>\n";
        return 1;
    }

    std::string plugin_path = argv[1];

    try {
        std::cout << "Testing plugin: " << plugin_path << "\n";

        // Load the plugin
        sp::PluginWrapper wrapper(plugin_path);
        std::cout << "Plugin loaded successfully\n";

        // Look up the create_instance function
        using factory = sp::ITextProcessor* (*)();
        factory func = reinterpret_cast<factory>(wrapper.lookup("create_instance"));
        std::cout << "create_instance function found\n";

        // Create an instance
        std::unique_ptr<sp::ITextProcessor> processor(func());
        std::cout << "Instance created successfully\n";

        // Test the required functions
        std::string name = processor->name();
        std::cout << "Plugin name: " << name << "\n";

        std::string description = processor->description();
        std::cout << "Plugin description: " << description << "\n";

        // Test the process function with sample text
        std::string test_input = "Hello World";
        std::string result = processor->process(test_input);
        std::cout << "Process test: \"" << test_input << "\" -> \"" << result << "\"\n";

        // Verify result is not empty (basic validation)
        if (result.empty() && !test_input.empty()) {
            std::cerr << "Warning: Process function returned empty result\n";
        }

        std::cout << "\n=== Plugin test PASSED ===\n";
        return 0;

    }
    catch (const std::exception& ex) {
        std::cerr << "\n=== Plugin test FAILED ===\n";
        std::cerr << "Error: " << ex.what() << "\n";
        return 1;
    }
    catch (...) {
        std::cerr << "\n=== Plugin test FAILED ===\n";
        std::cerr << "Unknown error occurred\n";
        return 1;
    }
}