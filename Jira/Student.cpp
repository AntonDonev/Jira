#include "Student.h"

Student::Student(const std::string& username, const std::string& password) : User(username, password, "Student")
{
}

void Student::enterProject(Project& project, const std::shared_ptr<Student>& student)
{
	project.addUser(student);
}
