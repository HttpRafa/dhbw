#ifndef MAIL_HANDLER_H
#define MAIL_HANDLER_H

#include <string>

#include "Person.hpp"

class MailHandler {
public:
    void writeMail(Person receiver, std::string text);
};

#endif