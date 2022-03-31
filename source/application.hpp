#pragma once

#include <iostream>

namespace Journey {
class Application
{
public:
    Application(const std::string GameName);

    void Start() const;

private:
    std::string mName;
};
}