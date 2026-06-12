#include "User.h"
#include "Task.h"

unsigned User::userCount = 0;
User::User(const std::string& username, const std::string& password, const std::string& role) :
	id(userCount++), username(Validator::validateUsername(username)), password(Validator::validatePassword(password)), role(Validator::validateRole(role))
{
}

const std::string& User::getUsername() const
{
	return username;
}

const std::string& User::getPassword() const
{
	return password;
}

const std::string& User::getRole() const
{
	return role;
}

bool User::checkPassword(const std::string& passwordToCheck) const {
	return password == passwordToCheck;
}

bool User::hasRole(const std::string& roleToCheck) const {
	return role == roleToCheck;
}

unsigned User::getId() const {
	return id;
}
