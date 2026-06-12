#include "User.h"

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

unsigned User::getId() const {
	return id;
}
