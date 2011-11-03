#pragma once

#include "Cake/System/IShell.hpp"

class IShellMock : public Cake::System::IShell
{
public:
    MOCK_METHOD1(execute, Cake::System::ShellResult(const std::string & cmd));
};
