#include <iostream>

#include "MailHandler.hpp"

void MailHandler::writeMail(Person receiver, std::string text) {
    std::cout << "Mail to " << receiver.getName() << ": " << text << std::endl;
}