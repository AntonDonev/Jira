#include "Validator.h"

const std::string& Validator::validatePassword(const std::string& password)
{
	if (password == "admin") return password;
	if (password.length() <= 7) throw std::invalid_argument("Password must be at least 8 symbols long");
	return password;
}

const std::string& Validator::validateUsername(const std::string& username)
{
	if (username == "admin") return username;
	if (username.length() <= 5) throw std::invalid_argument("Username must be at least 6 symbols long");
	return username;
}

const std::string& Validator::validateRole(const std::string& role)
{
	if (role != "Student" && role != "TeachingAssistant" && role != "Lecturer" && role != "Administrator") throw std::invalid_argument("Invalid role type");
	return role;
}

const std::string& Validator::validateStatus(const std::string& status)
{
	if (status != "ToDo" && status != "InReview" && status != "InProgress" && status != "Done") throw std::invalid_argument("Invalid status");
	return status;
}

const std::string& Validator::validatePriority(const std::string& priority)
{
	if (priority != "Critical" && priority != "High" && priority != "Low" && priority != "Medium") throw std::invalid_argument("Invalid priority");
	return priority;
}

const std::string& Validator::validateType(const std::string& type)
{
	if (type != "Bug" && type != "Feature" && type != "Improvement" && type != "Task") throw std::invalid_argument("Invalid task type");
	return type;
}

const std::string& Validator::validateName(const std::string& name)
{
	if (name.empty()) throw std::invalid_argument("Name cannot be empty");
	return name;
}

const std::string& Validator::validateDescription(const std::string& description)
{
	if (description.length() <= 14) throw std::invalid_argument("Description must be at least 15 symbols long");
	return description;
}

const std::string& Validator::validateDate(const std::string& date)
{
	// DD/MM/YEAR

	if (date.length() != 10)
		throw std::invalid_argument("Invalid date format");

	if (date[2] != '/' || date[5] != '/')
		throw std::invalid_argument("Invalid date format");

	for (int i = 0; i < date.length(); i++)
	{
		if (i == 2 || i == 5)
			continue;

		if (!std::isdigit(static_cast<unsigned char>(date[i])))
			throw std::invalid_argument("Invalid date format");
	}

	int day = std::stoi(date.substr(0, 2));
	int month = std::stoi(date.substr(3, 2));
	int year = std::stoi(date.substr(6, 4));

	if (year <= 0)
		throw std::invalid_argument("Invalid year");

	if (month < 1 || month > 12)
		throw std::invalid_argument("Invalid month");

	int daysInMonth[] = {
		31, 28, 31, 30, 31, 30,
		31, 31, 30, 31, 30, 31
	};

	if (day < 1 || day > daysInMonth[month - 1])
		throw std::invalid_argument("Invalid day");

	return date;

}

unsigned Validator::dateToNumber(const std::string& date)
{
	const std::string& validatedDate = validateDate(date);
	const unsigned day = std::stoul(validatedDate.substr(0, 2));
	const unsigned month = std::stoul(validatedDate.substr(3, 2));
	const unsigned year = std::stoul(validatedDate.substr(6, 4));

	return year * 10000 + month * 100 + day;
}

unsigned Validator::validatePoints(unsigned points)
{
	if (points > 100) throw std::invalid_argument("Points must be between 0 and 100");
	return points;
}

unsigned Validator::validateGrade(unsigned grade)
{
	if (grade != 0 && (grade < 2 || grade > 6)) throw std::invalid_argument("Grade must be 0 or between 2 and 6");
	return grade;
}

const std::string& Validator::validateProjectStatus(const std::string& status)
{
	if (status != "Active" && status != "Finished") throw std::invalid_argument("Invalid Project Status");
	return status;
}

const std::string& Validator::validateStageStatus(const std::string& status)
{
	if (status != "Planned" && status != "Active" && status != "Finished") throw std::invalid_argument("Invalid Stage Status");
	return status;
}

const std::shared_ptr<User>& Validator::validatePtr(const std::shared_ptr<User>& ptr)
{
	if (ptr == nullptr) throw std::invalid_argument("A nullptr object has been passed");
	return ptr;
}
