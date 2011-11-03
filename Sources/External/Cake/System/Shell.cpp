#include <iostream>
#include <cstdlib>
#include <cstdio>

#include "Shell.hpp"
#include "AtomicSyscall.hpp"

using namespace Cake::System;

ShellResult Shell::execute(const std::string & cmd)
{
    FILE * f;
    ATOMIC_SYSCALL(popen(cmd.c_str(), "r"), f, == 0);

    std::string out;
    while (!::feof(f))
    {
        char c;
        int length = fread(&c, sizeof(c), 1, f);
        if (length > 0)
        {
            out += c;
        }
    }
    int status = ::pclose(f);
    int ret = WEXITSTATUS(status);

    // TODO: std err
    return ShellResult(ret, out, "");
}
