#pragma once
#include "FileSystem.h"

class CommandProcessor {
	std::vector<std::shared_ptr<User>>& users;
	std::vector<Project>& projects;
	std::shared_ptr<User>& currentUser;
	bool closeRequested = false;

	std::vector<std::string> getArguments(const std::string& line) const;
	void requireLoggedUser() const;
	void requireRole(const std::string& role, const std::string& secondRole = "") const;
	std::shared_ptr<User> findUser(const std::string& username) const;
	Project& findProject(const std::string& projectName) const;
	Project& findProjectByTaskId(const std::string& taskId) const;
	void listAllTasks() const;
	void listTasksByEndDate(const std::string& date, bool afterDate) const;
	void printHelp() const;

public:
	CommandProcessor(std::vector<std::shared_ptr<User>>& users, std::vector<Project>& projects, std::shared_ptr<User>& currentUser);
	void execute(const std::string& line);
	bool shouldClose() const;
};
