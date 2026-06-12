#pragma once
#include "Student.h"
#include "TeachingAssistant.h"
#include "Lecturer.h"
class ObjectFactory {
public:
	static std::shared_ptr<User> createUser(const std::string& username, const std::string& password, const std::string& role);
	static Project createProject(const std::string& name, const std::string& description);
	static Stage createStage(const std::string& name, const std::string& startingDate, const std::string& endingDate);
	static Task createTask(const std::string& title, const std::string& description, const std::string& type,
		const std::string& priority, const std::string& startingDate, const std::shared_ptr<User>& creator,
		const std::string& endingDate);
};