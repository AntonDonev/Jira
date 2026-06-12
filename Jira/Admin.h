#pragma once
#include "User.h"
class Admin : public User {
	Admin(const std::string& username, const std::string& password);

public:
	static Admin& getInstance();

	Admin(const Admin& other) = delete;
	Admin& operator=(const Admin& other) = delete;
};