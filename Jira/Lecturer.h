#pragma once
#include "User.h"
class Lecturer : public User {
public:
	Lecturer(const std::string& username, const std::string& password);
};
