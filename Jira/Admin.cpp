#include "Admin.h"

Admin::Admin(const std::string& username, const std::string& password) : User(username, password, "Administrator")
{
}

Admin& Admin::getInstance()
{
    static Admin instance("admin", "admin");
    return instance;
}
