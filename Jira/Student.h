#pragma once
#include "User.h"
#include "Project.h"
class Student : public User {
public:
	Student(const std::string& username, const std::string& password);
	void enterProject(Project& project, const std::shared_ptr<Student>& student);
};
