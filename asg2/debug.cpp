#include <climits>
#include <iostream>
#include <vector>

#include "debug.h"
#include "util.h"

std::vector<bool> debugflags::flags (UCHAR_MAX + 1, false);

void debugflags::setflags (const std::string& initflags) {
    for (const unsigned char flag: initflags) {
        if (flag == '@') flags.assign (flags.size(), true);
        else flags[flag] = true;
    }
    // Note that DEBUGF can trace setflags.
    if (getflag ('x')) {
        std::string flag_chars;
        for (size_t index = 0; index < flags.size(); ++index) {
            if (getflag (index)) flag_chars += (char) index;
        }
        DEBUGF ('x', "debugflags::flags = " << flag_chars);
    }
}

//
// getflag -
//    Check to see if a certain flag is on.
//

bool debugflags::getflag (char flag) {
    return flags[static_cast<unsigned char> (flag)];
}

void debugflags::where (char flag, const char* file, int line,
        const char* func) {
    std::cout << util::sys_info::execname()
        << ": DEBUG(" << flag << ") "
        << file << "[" << line << "] " << func << "()" << std::endl;
}
