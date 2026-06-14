#include "Stage.h"

Stage::Stage(const std::string& name, const std::string& startingDate, const std::string& endingDate, const std::string& status) :
	name(Validator::validateName(name)),
	startingDate(Validator::validateDate(startingDate)),
	endingDate(endingDate.empty() ? endingDate : Validator::validateDate(endingDate)),
	status(Validator::validateStageStatus(status))
{
}

const std::string& Stage::getName() const
{
	return name;
}

const std::string& Stage::getStartingDate() const
{
	return startingDate;
}

const std::string& Stage::getEndingDate() const
{
	return endingDate;
}

const std::string& Stage::getStatus() const
{
	return status;
}

const std::vector<std::weak_ptr<Task>>& Stage::getTasks() const
{
	return tasks;
}

std::vector<std::string> Stage::getTaskIds() const
{
	std::vector<std::string> taskIds;
	for (const std::weak_ptr<Task>& task : tasks) {
		const std::shared_ptr<Task> taskPtr = task.lock();
		if (taskPtr != nullptr) {
			taskIds.push_back(taskPtr->getId());
		}
	}

	return taskIds;
}

void Stage::setStatus(const std::string& newStatus)
{
	status = Validator::validateStageStatus(newStatus);
}

void Stage::addTask(const std::shared_ptr<Task>& task)
{
	if (task == nullptr) {
		throw std::invalid_argument("Cannot add missing task to stage");
	}

	if (!containsTask(task->getId())) {
		tasks.push_back(task);
	}
}

void Stage::removeTask(const std::string& taskId)
{
	const std::string validatedTaskId = Validator::validateName(taskId);
	size_t i = 0;
	while (i < tasks.size()) {
		const std::shared_ptr<Task> taskPtr = tasks[i].lock();
		if (taskPtr == nullptr || taskPtr->getId() == validatedTaskId) {
			tasks.erase(tasks.begin() + i);
			if (taskPtr != nullptr) {
				return;
			}
		}
		else {
			i++;
		}
	}

	throw std::invalid_argument("Such task does not exist in stage");
}

bool Stage::containsTask(const std::string& taskId) const
{
	const std::string validatedTaskId = Validator::validateName(taskId);
	for (const std::weak_ptr<Task>& task : tasks) {
		const std::shared_ptr<Task> taskPtr = task.lock();
		if (taskPtr != nullptr && taskPtr->getId() == validatedTaskId) {
			return true;
		}
	}

	return false;
}

void Stage::printReport() const
{
	std::cout << "Stage: " << name << std::endl;
	std::cout << "Status: " << status << std::endl;
	std::cout << "Tasks: " << getTaskIds().size() << std::endl;
}
