#pragma once
#include "Task.h"

class Stage {
	std::string name;
	std::string startingDate;
	std::string endingDate;
	std::vector<std::weak_ptr<Task>> tasks;
	std::string status;
public:
	Stage(const std::string& name, const std::string& startingDate, const std::string& endingDate, const std::string& status);
	const std::string& getName() const;
	const std::string& getStartingDate() const;
	const std::string& getEndingDate() const;
	const std::string& getStatus() const;
	const std::vector<std::weak_ptr<Task>>& getTasks() const;
	void setStatus(const std::string& newStatus);
	void addTask(const std::shared_ptr<Task>& task);
	void removeTask(const std::string& task);
	bool containsTask(const std::string& task) const;
	void printReport() const;
	std::vector<std::string> getTaskIds() const;

};