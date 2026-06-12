#include "Project.h"

bool Project::isValidEntry(const std::shared_ptr<User>& userToCheck)
{
	clearInvalidatedMembers();

	for (const std::weak_ptr<User>& user : users) {
		if (user.lock() == userToCheck) {
			return true;
		}
	}

	return false;
}

void Project::clearInvalidatedMembers()
{
	for (int i = 0; i < users.size(); ) {
		if (users[i].lock() == nullptr) {
			users.erase(users.begin() + i);
		}
		else i++;
	}
}

Project::Project(const std::string& name, const std::string& description, const std::string& status) :
	name(Validator::validateName(name)),
	description(Validator::validateDescription(description)),
	status(Validator::validateProjectStatus(status))
{
}

const std::string& Project::getName() const
{
	return name;
}

const std::string& Project::getDescription() const
{
	return description;
}

const std::string& Project::getStatus() const
{
	return status;
}

const std::vector<std::weak_ptr<User>>& Project::getUsers() const
{
	return users;
}

const std::vector<std::shared_ptr<Task>>& Project::getTasks() const
{
	return tasks;
}

const std::vector<Stage>& Project::getStages() const
{
	return stages;
}

void Project::addUser(const std::shared_ptr<User> newUser)
{
	clearInvalidatedMembers();
	if (newUser != nullptr && !isValidEntry(newUser)) {
		users.push_back(newUser);
	}
}

void Project::addTask(const Task& newTask)
{
	tasks.push_back(std::make_shared<Task>(newTask));
}

std::shared_ptr<Task> Project::getTaskPtrById(const std::string& taskId)
{
	for (const std::shared_ptr<Task>& task : tasks) {
		if (task != nullptr && task->getId() == taskId) {
			return task;
		}
	}

	throw std::invalid_argument("Such task does not exist in project");
}

std::shared_ptr<const Task> Project::getTaskPtrById(const std::string& taskId) const
{
	for (const std::shared_ptr<Task>& task : tasks) {
		if (task != nullptr && task->getId() == taskId) {
			return task;
		}
	}

	throw std::invalid_argument("Such task does not exist in project");
}

void Project::addStage(const Stage& newStage)
{
	stages.push_back(newStage);
}

Task& Project::getTaskById(const std::string& taskId)
{
	return *getTaskPtrById(taskId);
}

const Task& Project::getTaskById(const std::string& taskId) const
{
	return *getTaskPtrById(taskId);
}

Stage& Project::getStageByName(const std::string& stageName)
{
	for (Stage& stage : stages) {
		if (stage.getName() == stageName) {
			return stage;
		}
	}

	throw std::invalid_argument("Such stage does not exist in project");
}

const Stage& Project::getStageByName(const std::string& stageName) const
{
	for (const Stage& stage : stages) {
		if (stage.getName() == stageName) {
			return stage;
		}
	}

	throw std::invalid_argument("Such stage does not exist in project");
}

void Project::listTasks(const std::shared_ptr<User>& user) const
{
	for (const std::shared_ptr<Task>& task : tasks) {
		if (task != nullptr && task->isAssignedTo(user)) {
			task->printTask();
		}
	}
}

void Project::listAllTasks() const
{
	for (const std::shared_ptr<Task>& task : tasks) {
		if (task != nullptr) {
			task->printTask();
		}
	}
}

void Project::searchTasks(const std::string& keyword) const
{
	for (const std::shared_ptr<Task>& task : tasks) {
		if (task != nullptr && task->containsKeyword(keyword)) {
			task->printTask();
		}
	}
}

void Project::filterTasks(const std::string& criteria) const
{
	for (const std::shared_ptr<Task>& task : tasks) {
		if (task != nullptr && task->hasCriteria(criteria)) {
			task->printTask();
		}
	}
}

void Project::studentReport(const std::shared_ptr<User>& user) const
{
	unsigned completedTasks = 0;
	unsigned inProgressTasks = 0;

	for (const std::shared_ptr<Task>& task : tasks) {
		if (task == nullptr || !task->isAssignedTo(user)) {
			continue;
		}

		if (task->getStatus() == "Done") {
			completedTasks++;
		}
		else if (task->getStatus() == "InProgress") {
			inProgressTasks++;
		}
	}

	std::cout << "Completed tasks: " << completedTasks << std::endl;
	std::cout << "In progress: " << inProgressTasks << std::endl;
	std::cout << "Performance score: " << completedTasks * 10 << std::endl;
}

void Project::setStatus(const std::string& status)
{
	this->status = Validator::validateProjectStatus(status);
}

void Project::changeTaskStatus(Task& task, const std::string& newStatus, const std::shared_ptr<User>& changedBy)
{
	task.setStatus(newStatus, changedBy);
}

void Project::assignTask(Task& task, const std::shared_ptr<User>& newInCharge)
{
	task.assignTo(newInCharge, newInCharge);
}

void Project::addTagToTask(Task& task, const std::string& newTag, const std::shared_ptr<User>& addedBy)
{
	task.addTag(newTag, addedBy);
}

void Project::addComment(Task& task, const Comment& newComment)
{
	task.addComment(newComment);
}

void Project::reviewTask(Task& task, const std::shared_ptr<User>& reviewedBy)
{
	task.review(reviewedBy);
}

void Project::approveTask(Task& task, const std::shared_ptr<User>& approvedBy)
{
	task.approve(approvedBy);
}

void Project::changeTaskPriority(Task& task, const std::string& newPriority, const std::shared_ptr<User>& changedBy)
{
	task.setPriority(newPriority, changedBy);
}

void Project::gradeTask(Task& task, unsigned grade, const std::shared_ptr<User>& gradedBy)
{
	task.setGrade(grade, gradedBy);
}

void Project::startStage(Stage& stage)
{
	stage.setStatus("Active");
}

void Project::finishStage(Stage& stage)
{
	stage.setStatus("Finished");
}

void Project::moveTaskToStage(Task& task, Stage& stage)
{
	const std::string taskId = task.getId();
	const std::shared_ptr<Task> taskPtr = getTaskPtrById(taskId);
	for (Stage& currentStage : stages) {
		if (&currentStage != &stage && currentStage.containsTask(taskId)) {
			currentStage.removeTask(taskId);
		}
	}

	stage.addTask(taskPtr);
}

void Project::stageReport(const Stage& stage) const
{
	stage.printReport();
}

void Project::printProject() const
{
	std::cout << name << " | " << status << std::endl;
}
