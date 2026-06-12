#include "Task.h"

unsigned Task::taskCount = 100;

std::string Task::getInChargeName() const
{
	const std::shared_ptr<User> inChargePtr = inCharge.lock();
	return inChargePtr ? inChargePtr->getUsername() : "No user assigned";
}

std::string Task::getTaskInfo() const
{
	return "ID: " + id
		+ ", title: " + title
		+ ", description: " + description
		+ ", type: " + type
		+ ", priority: " + priority
		+ ", status: " + status
		+ ", in charge: " + getInChargeName()
		+ ", starting date: " + startingDate
		+ ", end date: " + (endTime.empty() ? "No end date" : endTime)
		+ ", points: " + std::to_string(points)
		+ ", grade: " + (grade == 0 ? "Not graded" : std::to_string(grade));
}

void Task::addHistoryEntry(const std::shared_ptr<User>& user, const std::string& change)
{
	const std::string username = user ? user->getUsername() : "Unknown user";
	historyOfChanges.push_back(username + " " + change);
}

bool Task::containsText(const std::string& text, const std::string& keyword) const
{
	if (keyword.empty()) {
		return true;
	}

	if (keyword.length() > text.length()) {
		return false;
	}

	for (size_t i = 0; i <= text.length() - keyword.length(); i++) {
		bool found = true;

		for (size_t j = 0; j < keyword.length(); j++) {
			if (text[i + j] != keyword[j]) {
				found = false;
				break;
			}
		}

		if (found) {
			return true;
		}
	}

	return false;
}

Task::Task(const std::string& title, const std::string& description, const std::string& type, const std::string& taskPriority,
	const std::string& status, const std::shared_ptr<User>& inCharge, const std::string& startingDate,
	const std::string& endTime, unsigned points, unsigned grade) :
	id("FMI-" + std::to_string(taskCount++)),
	title(Validator::validateName(title)),
	description(Validator::validateDescription(description)),
	type(Validator::validateType(type)),
	priority(Validator::validatePriority(taskPriority)),
	status(Validator::validateStatus(status)),
	inCharge(Validator::validatePtr(inCharge)),
	startingDate(Validator::validateDate(startingDate)),
	endTime(Validator::validateDate(endTime)),
	points(Validator::validatePoints(points)),
	grade(Validator::validateGrade(grade))
{
	historyOfChanges.push_back("Task created by " + inCharge->getUsername() + ". Task info: " + getTaskInfo());
}

Task::Task(const std::string& id, const std::string& title, const std::string& description, const std::string& type,
	const std::string& taskPriority, const std::string& status, const std::shared_ptr<User>& inCharge,
	const std::string& startingDate, const std::string& endTime, unsigned points, unsigned grade) :
	id(Validator::validateName(id)),
	title(Validator::validateName(title)),
	description(Validator::validateDescription(description)),
	type(Validator::validateType(type)),
	priority(Validator::validatePriority(taskPriority)),
	status(Validator::validateStatus(status)),
	inCharge(Validator::validatePtr(inCharge)),
	startingDate(Validator::validateDate(startingDate)),
	endTime(Validator::validateDate(endTime)),
	points(Validator::validatePoints(points)),
	grade(Validator::validateGrade(grade)) 
{
	unsigned currentId = 0;

	for (size_t i = 4; i < id.length(); i++) {
		currentId = currentId * 10 + (id[i] - '0');
	}

	if (taskCount <= currentId) {
		taskCount = currentId + 1;
	}
}

std::string Task::getId() const
{
	return this->id;

}

const std::string& Task::getTitle() const
{
	return title;
}

const std::string& Task::getDescription() const
{
	return description;
}

const std::string& Task::getType() const
{
	return type;
}

const std::string& Task::getPriority() const
{
	return priority;
}

const std::string& Task::getStatus() const
{
	return status;
}

const std::string& Task::getStartingDate() const
{
	return startingDate;
}

const std::string& Task::getEndTime() const
{
	return endTime;
}

unsigned Task::getPoints() const
{
	return points;
}

unsigned Task::getGrade() const
{
	return grade;
}

std::shared_ptr<User> Task::getAssignedUser() const
{
	return inCharge.lock();
}

const std::vector<Comment>& Task::getComments() const
{
	return comments;
}

const std::vector<std::string>& Task::getTags() const
{
	return tags;
}

const std::vector<std::string>& Task::getHistoryOfChanges() const
{
	return historyOfChanges;
}

void Task::setTags(const std::vector<std::string>& newTags)
{
	tags = newTags;
}

void Task::setHistory(const std::vector<std::string>& newHistory)
{
	historyOfChanges = newHistory;
}

void Task::addLoadedComment(const Comment& newComment)
{
	comments.push_back(newComment);
}

void Task::setPriority(const std::string& newPriority, const std::shared_ptr<User>& changedBy)
{
	const std::string validatedPriority = Validator::validatePriority(newPriority);
	const std::string oldPriority = priority;
	priority = validatedPriority;
	addHistoryEntry(changedBy, "changed task priority from " + oldPriority + " to " + priority + ".");
}

void Task::setStatus(const std::string& newStatus, const std::shared_ptr<User>& changedBy)
{
	const std::string validatedStatus = Validator::validateStatus(newStatus);
	const std::string oldStatus = status;
	status = validatedStatus;
	addHistoryEntry(changedBy, "changed task status from " + oldStatus + " to " + status + ".");
}

void Task::assignTo(const std::shared_ptr<User>& newInCharge, const std::shared_ptr<User>& changedBy)
{
	const std::string oldInChargeName = getInChargeName();
	inCharge = newInCharge;
	addHistoryEntry(changedBy, "assigned task from " + oldInChargeName + " to " + getInChargeName() + ".");
}

void Task::setGrade(unsigned newGrade, const std::shared_ptr<User>& changedBy)
{
	const unsigned validatedGrade = Validator::validateGrade(newGrade);
	const unsigned oldGrade = grade;
	grade = validatedGrade;
	addHistoryEntry(changedBy, "changed task grade from "
		+ (oldGrade == 0 ? "Not graded" : std::to_string(oldGrade))
		+ " to " + std::to_string(grade) + ".");
}

void Task::review(const std::shared_ptr<User>& reviewedBy)
{
	addHistoryEntry(reviewedBy, "reviewed task.");
}

void Task::approve(const std::shared_ptr<User>& approvedBy)
{
	addHistoryEntry(approvedBy, "approved task.");
}

void Task::addTag(const std::string& newTag, const std::shared_ptr<User>& addedBy)
{
	const std::string validatedTag = Validator::validateName(newTag);
	tags.push_back(validatedTag);
	addHistoryEntry(addedBy, "added tag \"" + validatedTag + "\".");
}

void Task::addComment(const Comment& newComment)
{
	comments.push_back(newComment);
	historyOfChanges.push_back(newComment.getAuthorName() + " commented on task: " + newComment.getDescription() + " | " + newComment.getDate());
}

bool Task::isAssignedTo(const std::shared_ptr<User>& user) const
{
	const std::shared_ptr<User> inChargePtr = inCharge.lock();
	return user != nullptr && inChargePtr != nullptr && inChargePtr->getId() == user->getId();
}

bool Task::containsKeyword(const std::string& keyword) const
{
	return containsText(title, keyword)
		|| containsText(description, keyword)
		|| containsText(type, keyword)
		|| containsText(priority, keyword)
		|| containsText(status, keyword);
}

bool Task::hasCriteria(const std::string& criteria) const
{
	if (type == criteria || priority == criteria || status == criteria) {
		return true;
	}

	for (const std::string& tag : tags) {
		if (tag == criteria) {
			return true;
		}
	}

	return false;
}

void Task::printTask() const
{
	std::cout << id << " | " << status << " | " << priority << " | " << getInChargeName() << std::endl;
}
