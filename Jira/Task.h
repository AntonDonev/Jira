#pragma once
#include "Comment.h"
class Task {
	static unsigned taskCount;
	std::string title;
	std::string description;
	std::string type;
	std::string priority;
	std::string status;
	std::weak_ptr<User> inCharge;
	std::string startingDate;
	std::string endTime;
	unsigned points;
	unsigned grade;
	std::vector<Comment> comments;
	std::vector<std::string> tags;
	std::vector<std::string> historyOfChanges;
	std::string id;

	std::string getInChargeName() const;
	std::string getTaskInfo() const;
	void addHistoryEntry(const std::shared_ptr<User>& user, const std::string& change);
	bool containsText(const std::string& text, const std::string& keyword) const;

public:
	Task(const std::string& title, const std::string& description, const std::string& type, const std::string& taskPriority,
		const std::string& status, const std::shared_ptr<User>& inCharge, const std::string& startingDate,
		const std::string& endTime, unsigned points = 0, unsigned grade = 0);
	std::string getId() const;
	const std::string& getTitle() const;
	const std::string& getDescription() const;
	const std::string& getType() const;
	const std::string& getPriority() const;
	const std::string& getStatus() const;
	const std::string& getStartingDate() const;
	const std::string& getEndTime() const;
	unsigned getPoints() const;
	unsigned getGrade() const;
	std::shared_ptr<User> getAssignedUser() const;
	const std::vector<Comment>& getComments() const;
	const std::vector<std::string>& getTags() const;
	const std::vector<std::string>& getHistoryOfChanges() const;
	void setTags(const std::vector<std::string>& newTags);
	void setHistory(const std::vector<std::string>& newHistory);
	void addLoadedComment(const Comment& newComment);
	void setPriority(const std::string& newPriority, const std::shared_ptr<User>& changedBy);
	void setStatus(const std::string& newStatus, const std::shared_ptr<User>& changedBy);
	void assignTo(const std::shared_ptr<User>& newInCharge, const std::shared_ptr<User>& changedBy);
	void setGrade(unsigned newGrade, const std::shared_ptr<User>& changedBy);
	void review(const std::shared_ptr<User>& reviewedBy);
	void approve(const std::shared_ptr<User>& approvedBy);
	void addTag(const std::string& newTag, const std::shared_ptr<User>& addedBy);
	void addComment(const Comment& newComment);
	bool isAssignedTo(const std::shared_ptr<User>& user) const;
	bool containsKeyword(const std::string& keyword) const;
	bool hasCriteria(const std::string& criteria) const;
	void printTask() const;
};