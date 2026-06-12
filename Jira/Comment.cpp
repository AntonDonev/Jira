#include "Comment.h"

Comment::Comment(const std::shared_ptr<User>& author, const std::string& description, const std::string& date) : 
    author(Validator::validatePtr(author)), description(Validator::validateDescription(description)), date(Validator::validateDate(date))
{
}

const std::string& Comment::getDescription() const
{
    return description;
}

const std::string& Comment::getDate() const
{
    return date;
}

std::string Comment::getAuthorName() const
{
    return author.lock()->getUsername();
}
