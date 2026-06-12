#include "ObjectFactory.h"

std::shared_ptr<User> ObjectFactory::createUser(const std::string& username, const std::string& password, const std::string& role)
{
    const std::string validatedRole = Validator::validateRole(role);

    if (validatedRole == "Lecturer") return std::make_shared<Lecturer>(username, password, validatedRole);
    if(validatedRole == "TeachingAssistant") return std::make_shared<TeachingAsisstant>(username, password, validatedRole);
    if (validatedRole == "Student") return std::make_shared<Student>(username, password, validatedRole);

    throw std::invalid_argument("Admin is initialized by the panel upon starting of the program");
}

Project ObjectFactory::createProject(const std::string& name, const std::string& description)
{
    return Project(name, description);
}

Stage ObjectFactory::createStage(const std::string& name, const std::string& startingDate, const std::string& endingDate)
{
    return Stage(name, startingDate, endingDate, "Planned");
}

Task ObjectFactory::createTask(const std::string& title, const std::string& description, const std::string& type, const std::string& priority, const std::string& startingDate, const std::shared_ptr<User>& creator, const std::string& endingDate)
{
    return Task(title, description, type, priority, "ToDo", creator, startingDate, endingDate);
}
