#pragma once
#include <fstream>
#include "ObjectFactory.h"
#include "Admin.h"

class FileSystem {
	static std::vector<std::string> split(const std::string& text, char delimiter);
	static std::vector<std::string> readList(const std::string& text);
	static std::string writeList(const std::vector<std::string>& values);
	static std::shared_ptr<User> adminUser();
	static std::shared_ptr<User> findUser(const std::vector<std::shared_ptr<User>>& users, const std::string& username);
	static Project* findProject(std::vector<Project>& projects, const std::string& projectName);
	static void saveUsers(const std::vector<std::shared_ptr<User>>& users);
	static void saveProjects(const std::vector<Project>& projects);
	static void saveTasks(const std::vector<Project>& projects);
	static void loadUsers(std::vector<std::shared_ptr<User>>& users);
	static void loadProjects(std::vector<std::shared_ptr<User>>& users, std::vector<Project>& projects);
	static void loadTasks(std::vector<std::shared_ptr<User>>& users, std::vector<Project>& projects);
	static void loadStages(std::vector<Project>& projects);

public:
	static void save(const std::vector<std::shared_ptr<User>>& users, const std::vector<Project>& projects);
	static void load(std::vector<std::shared_ptr<User>>& users, std::vector<Project>& projects);
};
