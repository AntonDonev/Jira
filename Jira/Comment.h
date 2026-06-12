#pragma once
#include "User.h"

class Comment {
	std::weak_ptr<User> author;
	std::string description;
	std::string date;
public:
	Comment(const std::shared_ptr<User>& author, const std::string& description, const std::string& date);
	const std::string& getDescription() const;
	const std::string& getDate() const;
	std::string getAuthorName() const;
};
