#pragma once
#include <string>

bool readAllText(int fd, std::string &out);
bool writeAll(int fd, const std::string &s);
void handleClient(int cfd);
