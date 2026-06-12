#include <iostream>
#include "CommandProcessor.h"

int main()
{
    std::vector<std::shared_ptr<User>> users;
    std::vector<Project> projects;
    std::shared_ptr<User> currentUser = nullptr;

    try {
        FileSystem::load(users, projects);
    }
    catch (const std::exception& error) {
        std::cout << "Could not load data: " << error.what() << std::endl;
    }

    CommandProcessor commandProcessor(users, projects, currentUser);
    std::string line;

    while (!commandProcessor.shouldClose()) {
        std::getline(std::cin, line);

        if (line.empty()) {
            continue;
        }

        try {
            commandProcessor.execute(line);
        }
        catch (const std::invalid_argument& error) {
            std::cout << "[Error] " << error.what() << std::endl;
        }
        catch (const std::exception& error) {
            std::cout << "[Error] " << error.what() << std::endl;
        }
    }
}