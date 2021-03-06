#ifndef __COMMANDS_H__
#define __COMMANDS_H__

#include <map>

#include "inode.h"
#include "util.h"

namespace cmd {
    using command_fn = void(*)(inode_state& state,
                               const util::wordvec& words);
    using command_map = std::map<std::string,command_fn>;

    //
    // commands -
    //    A class to hold and dispatch each of the command functions.
    //    Each command "foo" is interpreted by a command_fn fn_foo.
    //
    class commands {
        private:
            commands(const inode&) = delete; // copy ctor
            commands& operator=(const inode&) = delete; // operator=
            command_map map;
        public:
            commands();
            command_fn at(const std::string& cmd);
    };

    //
    // execution functions -
    //    See the man page for a description of each of these functions.
    //
    void fn_cat    (inode_state& state, const util::wordvec& words);
    void fn_cd     (inode_state& state, const util::wordvec& words);
    void fn_echo   (inode_state& state, const util::wordvec& words);
    void fn_exit   (inode_state& state, const util::wordvec& words);
    void fn_ls     (inode_state& state, const util::wordvec& words);
    void fn_lsr    (inode_state& state, const util::wordvec& words);
    void fn_make   (inode_state& state, const util::wordvec& words);
    void fn_mkdir  (inode_state& state, const util::wordvec& words);
    void fn_prompt (inode_state& state, const util::wordvec& words);
    void fn_pwd    (inode_state& state, const util::wordvec& words);
    void fn_rm     (inode_state& state, const util::wordvec& words);
    void fn_rmr    (inode_state& state, const util::wordvec& words);

    //
    // exit_status_message -
    //   Prints an exit message and returns the exit status, as recorded
    //   by any of the functions.
    //
    int exit_status_message();

    class ysh_exit_exn: public std::exception {
        public:
            explicit ysh_exit_exn(int exit_status);
    };
}

#endif

