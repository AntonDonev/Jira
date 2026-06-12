#include "Admin.h"

Admin& Admin::getInstance()
{
    static Admin instance("admin", "admin");
    return instance;
}
