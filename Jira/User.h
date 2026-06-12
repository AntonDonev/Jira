#pragma once
#include <vector>
#include <memory>
#include <string>
#include "Validator.h"
#include <iostream>


class User {
	static unsigned userCount;
protected:
	std::string username;
	std::string password;
	std::string role;
	unsigned id;
public:
	User(const std::string& username, const std::string& password, const std::string& role);

	const std::string& getUsername() const;
	const std::string& getPassword() const;
	const std::string& getRole() const;
	unsigned getId() const;

	virtual ~User() = default;
};