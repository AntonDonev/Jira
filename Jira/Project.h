#pragma once
#include "Stage.h"

class Project {
	std::string name;
	std::string description;
	std::vector<std::weak_ptr<User>> users;
	std::vector<std::shared_ptr<Task>> tasks;
	std::vector<Stage> stages;
	std::string status;
	bool isValidEntry(const std::shared_ptr<User>& user);
	void clearInvalidatedMembers();
public:
	Project(const std::string& name, const std::string& description, const std::string& status = "Active");
	const std::string& getName() const;
	const std::string& getDescription() const;
	const std::string& getStatus() const;
	const std::vector<std::weak_ptr<User>>& getUsers() const;
	const std::vector<std::shared_ptr<Task>>& getTasks() const;
	const std::vector<Stage>& getStages() const;
	void addUser(const std::shared_ptr<User> newUser);
	void addTask(const Task& newTask);
	std::shared_ptr<Task> getTaskPtrById(const std::string& taskId);
	std::shared_ptr<const Task> getTaskPtrById(const std::string& taskId) const;
	void addStage(const Stage& newStage);
	Task& getTaskById(const std::string& taskId);
	const Task& getTaskById(const std::string& taskId) const;
	Stage& getStageByName(const std::string& stageName);
	const Stage& getStageByName(const std::string& stageName) const;
	void listTasks(const std::shared_ptr<User>& user) const;
	void listAllTasks() const;
	void searchTasks(const std::string& keyword) const;
	void filterTasks(const std::string& criteria) const;
	void studentReport(const std::shared_ptr<User>& user) const;
	void setStatus(const std::string& status);
	void changeTaskStatus(Task& task, const std::string& newStatus, const std::shared_ptr<User>& changedBy);
	void assignTask(Task& task, const std::shared_ptr<User>& newInCharge);
	void addTagToTask(Task& task, const std::string& newTag, const std::shared_ptr<User>& addedBy);
	void addComment(Task& task, const Comment& newComment);
	void reviewTask(Task& task, const std::shared_ptr<User>& reviewedBy);
	void approveTask(Task& task, const std::shared_ptr<User>& approvedBy);
	void changeTaskPriority(Task& task, const std::string& newPriority, const std::shared_ptr<User>& changedBy);
	void gradeTask(Task& task, unsigned grade, const std::shared_ptr<User>& gradedBy);
	void startStage(Stage& stage);
	void finishStage(Stage& stage);
	void moveTaskToStage(Task& task, Stage& stage);
	void stageReport(const Stage& stage) const;
	void printProject() const;
};
