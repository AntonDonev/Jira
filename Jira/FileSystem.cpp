#include "FileSystem.h"

std::vector<std::string> FileSystem::split(const std::string& text, char delimiter)
{
    std::vector<std::string> parts;
    std::string current;

    for (char symbol : text) {
        if (symbol == delimiter) {
            if (!current.empty()) {
                parts.push_back(current);
                current.clear();
            }
        }
        else {
            current += symbol;
        }
    }

    if (!current.empty()) {
        parts.push_back(current);
    }

    return parts;
}

std::vector<std::string> FileSystem::readList(const std::string& text)
{
    if (text == "-" || text.empty()) {
        return {};
    }

    return split(text, ';');
}

std::string FileSystem::writeList(const std::vector<std::string>& values)
{
    if (values.empty()) {
        return "-";
    }

    std::string result = values[0];
    for (size_t i = 1; i < values.size(); i++) {
        result += ";" + values[i];
    }

    return result;
}

std::shared_ptr<User> FileSystem::findUser(const std::vector<std::shared_ptr<User>>& users, const std::string& username)
{
    for (const std::shared_ptr<User>& user : users) {
        if (user != nullptr && user->getUsername() == username) {
            return user;
        }
    }

    return nullptr;
}

std::shared_ptr<User> FileSystem::adminUser()
{
    User* admin = &Admin::getInstance();
    return std::shared_ptr<User>(admin, [](User*) {});
}

Project* FileSystem::findProject(std::vector<Project>& projects, const std::string& projectName)
{
    for (Project& project : projects) {
        if (project.getName() == projectName) {
            return &project;
        }
    }

    return nullptr;
}

void FileSystem::saveUsers(const std::vector<std::shared_ptr<User>>& users)
{
    std::ofstream file("users.txt");

    for (const std::shared_ptr<User>& user : users) {
        if (user != nullptr) {
            file << user->getUsername() << "|" << user->getPassword() << "|" << user->getRole() << "\n";
        }
    }
}

void FileSystem::saveProjects(const std::vector<Project>& projects)
{
    std::ofstream file("projects.txt");

    for (const Project& project : projects) {
        std::vector<std::string> members;
        for (const std::weak_ptr<User>& user : project.getUsers()) {
            const std::shared_ptr<User> userPtr = user.lock();
            if (userPtr != nullptr) {
                members.push_back(userPtr->getUsername());
            }
        }

        file << "PROJECT|" << project.getName() << "|" << project.getDescription() << "|"
            << project.getStatus() << "|" << writeList(members) << "\n";

        for (const Stage& stage : project.getStages()) {
            file << "STAGE|" << project.getName() << "|" << stage.getName() << "|"
                << stage.getStartingDate() << "|" << (stage.getEndingDate().empty() ? "-" : stage.getEndingDate()) << "|"
                << stage.getStatus() << "|" << writeList(stage.getTaskIds()) << "\n";
        }
    }
}

void FileSystem::saveTasks(const std::vector<Project>& projects)
{
    std::ofstream file("tasks.txt");

    for (const Project& project : projects) {
        for (const std::shared_ptr<Task>& task : project.getTasks()) {
            if (task == nullptr) {
                continue;
            }

            std::string assignedUser = "-";
            const std::shared_ptr<User> user = task->getAssignedUser();
            if (user != nullptr) {
                assignedUser = user->getUsername();
            }

            std::vector<std::string> comments;
            for (const Comment& comment : task->getComments()) {
                comments.push_back(comment.getAuthorName() + "," + comment.getDate() + "," + comment.getDescription());
            }

            file << "TASK|" << project.getName() << "|" << task->getId() << "|" << task->getTitle() << "|"
                << task->getDescription() << "|" << task->getType() << "|" << task->getPriority() << "|"
                << task->getStatus() << "|" << assignedUser << "|" << task->getStartingDate() << "|"
                << task->getEndTime() << "|"
                << task->getPoints() << "|" << task->getGrade() << "|"
                << writeList(task->getTags()) << "|" << writeList(task->getHistoryOfChanges()) << "|"
                << writeList(comments) << "\n";
        }
    }
}

void FileSystem::loadUsers(std::vector<std::shared_ptr<User>>& users)
{
    users.clear();

    std::ifstream file("users.txt");
    if (!file.is_open()) {
        users.push_back(adminUser());
        return;
    }

    bool hasAdmin = false;
    std::string line;
    while (std::getline(file, line)) {
        const std::vector<std::string> fields = split(line, '|');
        if (fields.size() != 3) {
            continue;
        }

        if (fields[2] == "Administrator") {
            if (!hasAdmin) {
                users.push_back(adminUser());
                hasAdmin = true;
            }
        }
        else {
            users.push_back(ObjectFactory::createUser(fields[0], fields[1], fields[2]));
        }
    }

    if (!hasAdmin) {
        users.push_back(adminUser());
    }
}

void FileSystem::loadProjects(std::vector<std::shared_ptr<User>>& users, std::vector<Project>& projects)
{
    projects.clear();

    std::ifstream file("projects.txt");
    if (!file.is_open()) {
        return;
    }

    std::string line;
    while (std::getline(file, line)) {
        const std::vector<std::string> fields = split(line, '|');
        if (fields.empty()) {
            continue;
        }

        if (fields[0] == "PROJECT" && fields.size() == 5) {
            Project project(fields[1], fields[2], fields[3]);
            for (const std::string& username : readList(fields[4])) {
                project.addUser(findUser(users, username));
            }
            projects.push_back(project);
        }

    }
}

void FileSystem::loadTasks(std::vector<std::shared_ptr<User>>& users, std::vector<Project>& projects)
{
    std::ifstream file("tasks.txt");
    if (!file.is_open()) {
        return;
    }

    std::string line;
    while (std::getline(file, line)) {
        std::vector<std::string> fields = split(line, '|');
        if (fields.size() != 16 || fields[0] != "TASK") {
            continue;
        }

        Project* project = findProject(projects, fields[1]);
        if (project == nullptr) {
            continue;
        }

        if (fields[10] == "-") {
            continue;
        }

        const std::string endDate = fields[10];
        Task task(fields[2], fields[3], fields[4], fields[5], fields[6], fields[7], findUser(users, fields[8]),
            fields[9], endDate, std::stoul(fields[11]), std::stoul(fields[12]));

        task.setTags(readList(fields[13]));
        task.setHistory(readList(fields[14]));

        for (const std::string& commentText : readList(fields[15])) {
            const std::vector<std::string> commentFields = split(commentText, ',');
            if (commentFields.size() == 3) {
                task.addLoadedComment(Comment(findUser(users, commentFields[0]), commentFields[2], commentFields[1]));
            }
        }

        project->addTask(task);
    }
}

void FileSystem::loadStages(std::vector<Project>& projects)
{
    std::ifstream file("projects.txt");
    if (!file.is_open()) {
        return;
    }

    std::string line;
    while (std::getline(file, line)) {
        const std::vector<std::string> fields = split(line, '|');
        if (fields.size() != 7 || fields[0] != "STAGE") {
            continue;
        }

        Project* project = findProject(projects, fields[1]);
        if (project == nullptr) {
            continue;
        }

        const std::string endDate = fields[4] == "-" ? "" : fields[4];
        Stage stage(fields[2], fields[3], endDate, fields[5]);
        for (const std::string& taskId : readList(fields[6])) {
            try {
                stage.addTask(project->getTaskPtrById(taskId));
            }
            catch (const std::invalid_argument&) {
            }
        }

        project->addStage(stage);
    }
}

void FileSystem::save(const std::vector<std::shared_ptr<User>>& users, const std::vector<Project>& projects)
{
    saveUsers(users);
    saveProjects(projects);
    saveTasks(projects);
}

void FileSystem::load(std::vector<std::shared_ptr<User>>& users, std::vector<Project>& projects)
{
    loadUsers(users);
    loadProjects(users, projects);
    loadTasks(users, projects);
    loadStages(projects);
}
