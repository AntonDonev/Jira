#pragma once
#include <stdexcept>
#include "User.h"

class Validator {
public:
	static 	const std::string& validatePassword(const std::string& password);
	static const std::string& validateUsername(const std::string& username);
	static const std::string& validateRole(const std::string& role);
	static const std::string& validateStatus(const std::string& status);
	static const std::string& validatePriority(const std::string& priority);
	static const std::string& validateType(const std::string& type);
	static const std::string& validateName(const std::string& name);
	static const std::string& validateDescription(const std::string& description);
	static const std::string& validateDate(const std::string& date);
	static unsigned dateToNumber(const std::string& date);
	static unsigned validatePoints(unsigned points);
	static unsigned validateGrade(unsigned grade);
	static const std::string& validateProjectStatus(const std::string& status);
	static const std::string& validateStageStatus(const std::string& status);
	template <typename T>
	static const std::shared_ptr<T>& validatePtr(const std::shared_ptr<T>& ptr);
};

template<typename T>
inline const std::shared_ptr<T>& Validator::validatePtr(const std::shared_ptr<T>& ptr)
{
	if (!ptr) {
		throw std::invalid_argument("Pointer cannot be null!");
	}
	return ptr;
}