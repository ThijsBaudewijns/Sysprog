#include <iostream>
#include <vector>
#include <string>
#include <filesystem>
#include <memory>
#include <fstream>
#include <sstream>
#include <algorithm>
#include "PluginWrapper.hpp"
#include "ITextProcessor.hpp"

// Scan directory for plugin files
std::vector<std::string> scan_dir(const std::string& path, const std::string& ext) {
    std::vector<std::string> plugins;

    if (!std::filesystem::exists(path)) {
        std::cerr << "Warning: plugins directory '" << path << "' does not exist.\n";
        return plugins;
    }

    for (const auto& entry : std::filesystem::directory_iterator{ path }) {
        std::string filename{ entry.path().string() };
        if (filename.size() >= ext.size() &&
            filename.substr(filename.size() - ext.size()) == ext) {
            plugins.push_back(filename);
        }
    }
    return plugins;
}

// Read entire file content
std::string read_file(const std::string& filepath) {
    std::ifstream file(filepath);
    if (!file) {
        throw std::runtime_error("Could not open file: " + filepath);
    }

    std::ostringstream oss;
    oss << file.rdbuf();
    return oss.str();
}

// Write content to file
void write_file(const std::string& filepath, const std::string& content) {
    std::ofstream file(filepath);
    if (!file) {
        throw std::runtime_error("Could not write to file: " + filepath);
    }
    file << content;
}

int main(int argc, char* argv[]) {
    try {
        std::cout << "=== Text Processing Plugin System ===\n\n";

        // (1) Scan directory for plugins
        std::vector<std::string> files{ scan_dir("plugins", ".plug") };

        if (files.empty()) {
            std::cerr << "No plugins found in 'plugins' directory.\n";
            std::cerr << "Please ensure .plug files are in the plugins folder.\n";
            return 1;
        }

        // (2) Load plugins
        std::vector<sp::PluginWrapper> plugins;
        for (const std::string& file : files) {
            std::cout << "Loading: " << file << "\n";
            plugins.emplace_back(file);
        }

        // (3) Instantiate processor objects
        using factory = sp::ITextProcessor* (*)();
        std::vector<std::unique_ptr<sp::ITextProcessor>> processors;

        for (const sp::PluginWrapper& wrapper : plugins) {
            factory func = reinterpret_cast<factory>(wrapper.lookup("create_instance"));
            processors.emplace_back(func());
        }

        std::cout << "\nAvailable processors:\n";
        for (size_t i = 0; i < processors.size(); ++i) {
            std::cout << "  [" << (i + 1) << "] " << processors[i]->name()
                << " - " << processors[i]->description() << "\n";
        }

        // Get input file
        std::cout << "\nEnter input file path: ";
        std::string input_file;
        std::getline(std::cin, input_file);

        std::string text = read_file(input_file);
        std::cout << "\nOriginal text (" << text.length() << " characters):\n";
        std::cout << "---\n" << text << "\n---\n";

        // Select processors
        std::cout << "\nEnter processor numbers to apply (space-separated, e.g., '1 3 2'): ";
        std::string selection;
        std::getline(std::cin, selection);

        std::istringstream iss(selection);
        std::vector<int> choices;
        int choice;
        while (iss >> choice) {
            if (choice >= 1 && choice <= static_cast<int>(processors.size())) {
                choices.push_back(choice - 1);
            }
            else {
                std::cerr << "Warning: Ignoring invalid choice " << choice << "\n";
            }
        }

        if (choices.empty()) {
            std::cout << "No valid processors selected.\n";
            return 0;
        }

        // (4) Apply processors in order
        std::string processed_text = text;
        std::cout << "\nApplying processors:\n";
        for (int idx : choices) {
            std::cout << "  -> " << processors[idx]->name() << "\n";
            processed_text = processors[idx]->process(processed_text);
        }

        std::cout << "\nProcessed text:\n";
        std::cout << "---\n" << processed_text << "\n---\n";

        // Option to save output
        std::cout << "\nSave to file? (y/n): ";
        std::string save_choice;
        std::getline(std::cin, save_choice);

        if (!save_choice.empty() && (save_choice[0] == 'y' || save_choice[0] == 'Y')) {
            std::cout << "Enter output file path: ";
            std::string output_file;
            std::getline(std::cin, output_file);
            write_file(output_file, processed_text);
            std::cout << "Saved to: " << output_file << "\n";
        }

        std::cout << "\nDone!\n";

    }
    catch (const std::exception& ex) {
        std::cerr << "Error: " << ex.what() << std::endl;
        return 1;
    }

    return 0;
}