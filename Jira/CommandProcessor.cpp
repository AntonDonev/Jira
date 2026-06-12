#include "CommandProcessor.h"
#include "Student.h"
#include <algorithm>

CommandProcessor::CommandProcessor(std::vector<std::shared_ptr<User>>& users, std::vector<Project>& projects, std::shared_ptr<User>& currentUser) :
	users(users), projects(projects), currentUser(currentUser)
{
}

std::vector<std::string> CommandProcessor::getArguments(const std::string& line) const
{
	std::vector<std::string> arguments;
	std::string currentArgument;

	for (char symbol : line) {
		if (symbol == ' ') {
			if (!currentArgument.empty()) {
				arguments.push_back(currentArgument);
				currentArgument.clear();
			}
		}
		else {
			currentArgument += symbol;
		}
	}

	if (!currentArgument.empty()) {
		arguments.push_back(currentArgument);
	}

	return arguments;
}

void CommandProcessor::requireLoggedUser() const
{
	if (currentUser == nullptr) {
		throw std::invalid_argument("You must be logged in");
	}
}

void CommandProcessor::requireRole(const std::string& role, const std::string& secondRole) const
{
	requireLoggedUser();
	if (!currentUser->hasRole(role) && (secondRole.empty() || !currentUser->hasRole(secondRole))) {
		throw std::invalid_argument("You do not have permission for this command");
	}
}

std::shared_ptr<User> CommandProcessor::findUser(const std::string& username) const
{
	for (const std::shared_ptr<User>& user : users) {
		if (user != nullptr && user->getUsername() == username) {
			return user;
		}
	}

	throw std::invalid_argument("Such user does not exist");
}

Project& CommandProcessor::findProject(const std::string& projectName) const
{
	for (Project& project : projects) {
		if (project.getName() == projectName) {
			return project;
		}
	}

	throw std::invalid_argument("Such project does not exist");
}

Project& CommandProcessor::findProjectByTaskId(const std::string& taskId) const
{
	for (Project& project : projects) {
		for (const std::shared_ptr<Task>& task : project.getTasks()) {
			if (task != nullptr && task->getId() == taskId) {
				return project;
			}
		}
	}

	throw std::invalid_argument("Such task does not exist");
}

void CommandProcessor::listAllTasks() const
{
	for (const Project& project : projects) {
		project.listAllTasks();
	}
}

void CommandProcessor::listTasksByEndDate(const std::string& date, bool afterDate) const
{
	const unsigned limit = Validator::dateToNumber(date);
	std::vector<const Task*> matchingTasks;

	for (const Project& project : projects) {
		for (const std::shared_ptr<Task>& task : project.getTasks()) {
			if (task == nullptr) {
				continue;
			}

			const unsigned taskEndDate = Validator::dateToNumber(task->getEndTime());
			if (task->isAssignedTo(currentUser) && ((afterDate && taskEndDate > limit) || (!afterDate && taskEndDate < limit))) {
				matchingTasks.push_back(task.get());
			}
		}
	}

	std::sort(matchingTasks.begin(), matchingTasks.end(), [](const Task* first, const Task* second) {
		return Validator::dateToNumber(first->getEndTime()) < Validator::dateToNumber(second->getEndTime());
		});

	for (const Task* task : matchingTasks) {
		task->printTask();
	}
}

void CommandProcessor::printHelp() const
{
	std::cout << "login, logout, register, list-projects, create-project, create-task" << std::endl;
	std::cout << "assign-task, change-status, add-comment, my-tasks, upcoming-tasks, overdue" << std::endl;
	std::cout << "add-tag, approve-task, review-task, grade-task, save, load, close" << std::endl;
}

void CommandProcessor::execute(const std::string& line)
{
	const std::vector<std::string> arguments = getArguments(line);
	if (arguments.empty()) {
		return;
	}

	const std::string command = arguments[0];

	if (command == "login") {
		if (arguments.size() != 3) throw std::invalid_argument("login expects username and password");
		currentUser = findUser(arguments[1]);
		if (!currentUser->checkPassword(arguments[2])) {
			currentUser = nullptr;
			throw std::invalid_argument("Wrong password");
		}
		std::cout << "[System] Successfully logged in. Role: " << currentUser->getRole() << std::endl;
	}
	else if (command == "logout") {
		requireLoggedUser();
		currentUser = nullptr;
		std::cout << "[System] Logged out." << std::endl;
	}
	else if (command == "help") {
		printHelp();
	}
	else if (command == "view-profile") {
		requireLoggedUser();
		std::cout << "Username: " << currentUser->getUsername() << std::endl;
		std::cout << "Role: " << currentUser->getRole() << std::endl;
	}
	else if (command == "close") {
		closeRequested = true;
		std::cout << "[System] System shutting down..." << std::endl;
	}
	else if (command == "register") {
		requireRole("Administrator");
		if (arguments.size() != 4) throw std::invalid_argument("register expects username, password and role");
		users.push_back(ObjectFactory::createUser(arguments[1], arguments[2], arguments[3]));
		std::cout << "[System] User registered." << std::endl;
	}
	else if (command == "list-projects") {
		requireLoggedUser();
		for (const auto& project : projects) {
			for (const auto& currentCandidate : project.getUsers()) {
				if (currentCandidate.lock() == currentUser) {
					project.printProject();
				}
			}
		}
	}
	else if (command == "list-tasks") {
		requireLoggedUser();
		for (Project& project : projects) project.listTasks(currentUser);
	}
	else if (command == "list-all-projects") {
		requireLoggedUser();
		for (const auto& project : projects) {
			project.printProject();
		}
	}
	else if (command == "list-all-tasks") {
		requireRole("Lecturer");
		listAllTasks();
	}
	else if (command == "join-project") {
		requireRole("Student");
		if (arguments.size() != 2) throw std::invalid_argument("join-project expects project name");
		std::shared_ptr<Student> student = std::dynamic_pointer_cast<Student>(currentUser);
		if (student == nullptr) throw std::invalid_argument("Current user is not a student");
		student->enterProject(findProject(arguments[1]), student);
		std::cout << "[System] Joined project." << std::endl;
	}
	else if (command == "create-project") {
		requireRole("Administrator");
		if (arguments.size() != 2) throw std::invalid_argument("create-project expects name");
		{
			std::string description;
			std::cout << "Description: ";
			std::getline(std::cin, description);
			projects.push_back(ObjectFactory::createProject(arguments[1], description));
			std::cout << "[System] Project created." << std::endl;
		}
	}
	else if (command == "archive-project") {
		requireRole("Administrator");
		if (arguments.size() != 2) throw std::invalid_argument("archive-project expects name");
		findProject(arguments[1]).setStatus("Finished");
		std::cout << "[System] Project archived." << std::endl;
	}
	else if (command == "add-user-to-project") {
		requireRole("Administrator");
		if (arguments.size() != 3) throw std::invalid_argument("add-user-to-project expects username and project");
		findProject(arguments[2]).addUser(findUser(arguments[1]));
		std::cout << "[System] User added to project." << std::endl;
	}
	else if (command == "remove-user") {
		requireRole("Administrator");
		if (arguments.size() != 2) throw std::invalid_argument("remove-user expects username");
		if (arguments[1] == "admin") throw std::invalid_argument("Admin cannot be removed");
		{
			const size_t oldSize = users.size();
			size_t index = 0;
			while (index < users.size()) {
				if (users[index] != nullptr && users[index]->getUsername() == arguments[1]) {
					users.erase(users.begin() + index);
				}
				else {
					index++;
				}
			}

			if (oldSize == users.size()) throw std::invalid_argument("Such user does not exist");
			std::cout << "[System] User removed." << std::endl;
		}
	}
	else if (command == "create-task") {
		requireRole("Student");
		if (arguments.size() != 6) {
			throw std::invalid_argument("create-task expects project, type, priority, start date and end date");
		}
		{
			std::string title;
			std::string description;
			std::cout << "Title: ";
			std::getline(std::cin, title);
			std::cout << "Description: ";
			std::getline(std::cin, description);
			Task task = ObjectFactory::createTask(title, description, arguments[2], arguments[3], arguments[4], currentUser, arguments[5]);
			findProject(arguments[1]).addTask(task);
			std::cout << "[System] Task created successfully. ID: " << task.getId() << std::endl;
		}
	}
	else if (command == "assign-task") {
		requireRole("Student");
		if (arguments.size() != 2) throw std::invalid_argument("assign-task expects task id");
		Project& project = findProjectByTaskId(arguments[1]);
		project.assignTask(project.getTaskById(arguments[1]), currentUser);
		std::cout << "[System] Task assigned." << std::endl;
	}
	else if (command == "change-status") {
		requireLoggedUser();
		if (arguments.size() != 3) throw std::invalid_argument("change-status expects task id and status");
		Project& project = findProjectByTaskId(arguments[1]);
		project.changeTaskStatus(project.getTaskById(arguments[1]), arguments[2], currentUser);
		std::cout << "[System] Status changed." << std::endl;
	}
	else if (command == "add-comment") {
		requireLoggedUser();
		if (arguments.size() != 2) throw std::invalid_argument("add-comment expects task id");
		Project& project = findProjectByTaskId(arguments[1]);
		{
			std::string description;
			std::string date;
			std::cout << "Comment: ";
			std::getline(std::cin, description);
			std::cout << "Date: ";
			std::getline(std::cin, date);
			Comment comment(currentUser, description, date);
			project.addComment(project.getTaskById(arguments[1]), comment);
			std::cout << "[System] Comment added." << std::endl;
		}
	}
	else if (command == "my-tasks") {
		requireLoggedUser();
		for (Project& project : projects) project.studentReport(currentUser);
	}
	else if (command == "upcoming-tasks") {
		requireLoggedUser();
		if (arguments.size() != 2) throw std::invalid_argument("upcoming-tasks expects date");
		listTasksByEndDate(arguments[1], true);
	}
	else if (command == "overdue") {
		requireLoggedUser();
		if (arguments.size() != 2) throw std::invalid_argument("overdue expects date");
		listTasksByEndDate(arguments[1], false);
	}
	else if (command == "search-tasks") {
		requireLoggedUser();
		if (arguments.size() != 2) throw std::invalid_argument("search-tasks expects keyword");
		for (Project& project : projects) project.searchTasks(arguments[1]);
	}
	else if (command == "filter-tasks") {
		requireLoggedUser();
		if (arguments.size() != 2) throw std::invalid_argument("filter-tasks expects criteria");
		for (Project& project : projects) project.filterTasks(arguments[1]);
	}
	else if (command == "add-tag") {
		requireLoggedUser();
		if (arguments.size() != 3) throw std::invalid_argument("add-tag expects task id and tag");
		Project& project = findProjectByTaskId(arguments[1]);
		project.addTagToTask(project.getTaskById(arguments[1]), arguments[2], currentUser);
		std::cout << "[System] Tag added." << std::endl;
	}
	else if (command == "approve-task") {
		requireRole("TeachingAssistant", "Lecturer");
		if (arguments.size() != 2) throw std::invalid_argument("approve-task expects task id");
		Project& project = findProjectByTaskId(arguments[1]);
		project.approveTask(project.getTaskById(arguments[1]), currentUser);
		std::cout << "[System] Task approved." << std::endl;
	}
	else if (command == "review-task") {
		requireRole("TeachingAssistant", "Lecturer");
		if (arguments.size() != 2) throw std::invalid_argument("review-task expects task id");
		Project& project = findProjectByTaskId(arguments[1]);
		project.reviewTask(project.getTaskById(arguments[1]), currentUser);
		std::cout << "[System] Task reviewed." << std::endl;
	}
	else if (command == "change-priority") {
		requireRole("TeachingAssistant", "Lecturer");
		if (arguments.size() != 3) throw std::invalid_argument("change-priority expects task id and priority");
		Project& project = findProjectByTaskId(arguments[1]);
		project.changeTaskPriority(project.getTaskById(arguments[1]), arguments[2], currentUser);
		std::cout << "[System] Priority changed." << std::endl;
	}
	else if (command == "create-stage") {
		requireRole("TeachingAssistant", "Lecturer");
		if (arguments.size() != 4 && arguments.size() != 5) throw std::invalid_argument("create-stage expects project, stage, start date and optional end date");
		const std::string endDate = arguments.size() == 5 && arguments[4] != "-" ? arguments[4] : "";
		findProject(arguments[1]).addStage(ObjectFactory::createStage(arguments[2], arguments[3], endDate));
		std::cout << "[System] Stage created." << std::endl;
	}
	else if (command == "start-stage") {
		requireRole("TeachingAssistant", "Lecturer");
		if (arguments.size() != 3) throw std::invalid_argument("start-stage expects project and stage");
		Project& project = findProject(arguments[1]);
		project.startStage(project.getStageByName(arguments[2]));
		std::cout << "[System] Stage started." << std::endl;
	}
	else if (command == "finish-stage") {
		requireRole("TeachingAssistant", "Lecturer");
		if (arguments.size() != 3) throw std::invalid_argument("finish-stage expects project and stage");
		Project& project = findProject(arguments[1]);
		project.finishStage(project.getStageByName(arguments[2]));
		std::cout << "[System] Stage finished." << std::endl;
	}
	else if (command == "stage-report") {
		requireRole("TeachingAssistant", "Lecturer");
		if (arguments.size() != 3) throw std::invalid_argument("stage-report expects project and stage");
		Project& project = findProject(arguments[1]);
		project.stageReport(project.getStageByName(arguments[2]));
	}
	else if (command == "move-task-to-stage") {
		requireRole("TeachingAssistant", "Lecturer");
		if (arguments.size() != 3) throw std::invalid_argument("move-task-to-stage expects task id and stage");
		{
			Project& project = findProjectByTaskId(arguments[1]);
			project.moveTaskToStage(project.getTaskById(arguments[1]), project.getStageByName(arguments[2]));
			std::cout << "[System] Task moved to stage." << std::endl;
		}
	}
	else if (command == "grade-task") {
		requireRole("Lecturer");
		if (arguments.size() != 3) throw std::invalid_argument("grade-task expects task id and grade");
		Project& project = findProjectByTaskId(arguments[1]);
		project.gradeTask(project.getTaskById(arguments[1]), std::stoul(arguments[2]), currentUser);
		std::cout << "[System] Task graded." << std::endl;
	}
	else if (command == "student-report") {
		requireRole("Lecturer");
		if (arguments.size() != 2) throw std::invalid_argument("student-report expects username");
		for (Project& project : projects) project.studentReport(findUser(arguments[1]));
	}
	else if (command == "finalize-project") {
		requireRole("Lecturer");
		if (arguments.size() != 2) throw std::invalid_argument("finalize-project expects project");
		findProject(arguments[1]).setStatus("Finished");
		std::cout << "[System] Project finalized." << std::endl;
	}
	else if (command == "save") {
		requireRole("Administrator");
		FileSystem::save(users, projects);
		std::cout << "[System] Data saved." << std::endl;
	}
	else if (command == "load") {
		requireRole("Administrator");
		FileSystem::load(users, projects);
		currentUser = nullptr;
		std::cout << "[System] Data loaded. Please login again." << std::endl;
	}
	else {
		throw std::invalid_argument("Unknown command");
	}
}

bool CommandProcessor::shouldClose() const
{
	return closeRequested;
}
