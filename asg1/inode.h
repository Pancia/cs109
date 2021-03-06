#ifndef __INODE_H__
#define __INODE_H__

#include <exception>
#include <iostream>
#include <memory>
#include <map>
#include <vector>
#include <numeric>

#include "util.h"

/**
 * inode_t -
 *    An inode is either a directory or a plain file.
 */
enum inode_t {PLAIN_INODE, DIR_INODE};
class inode;
class file_base;
class plain_file;
class directory;
using inode_ptr = std::shared_ptr<inode>;
using file_base_ptr = std::shared_ptr<file_base>;
using plain_file_ptr = std::shared_ptr<plain_file>;
using directory_ptr = std::shared_ptr<directory>;
using dir_map = std::map<std::string,inode_ptr>;

/**
 * inode_state -
 *    A small convenient class to maintain the state of the simulated
 *    process:  the root(/), the current directory(.), and the
 *    prompt.
 */
class inode_state {
    friend class inode;
    friend std::ostream& operator<<(std::ostream& out,
                               const inode_state& state);
    private:
        inode_state(const inode_state&) = delete; // copy ctor
        inode_state& operator=(const inode_state&) = delete;
        inode_ptr root {nullptr};
        inode_ptr cwd {nullptr};
        std::string prompt {"% "};
    public:
        inode_state();
        ~inode_state();
        void set_prompt(std::string new_prompt);
        std::string get_prompt();
        std::string get_wd();
        void cat  (const util::wordvec& words);
        void cd   (const util::wordvec& words);
        void ls   (const util::wordvec& words, bool recursive);
        void make (const util::wordvec& words);
        void mkdir(const util::wordvec& words);
        void rm   (const util::wordvec& words, bool recursive);
};

/**
 * class inode -
 *
 * inode ctor -
 *    Create a new inode of the given type.
 * get_inode_nr -
 *    Retrieves the serial number of the inode.  Inode numbers are
 *    allocated in sequence by small integer.
 * size -
 *    Returns the size of an inode.  For a directory, this is the
 *    number of dirents.  For a text file, the number of characters
 *    when printed(the sum of the lengths of each word, plus the
 *    number of words.
 */
class inode {
    friend class inode_state;
    friend std::ostream& operator<<(std::ostream& out,
                                     const inode& inode);
    private:
        static int next_inode_nr;
        int inode_nr;
    public:
        inode_t type;
        file_base_ptr contents;
        inode(inode_t init_type);
        int get_inode_nr() const;
};

/**
 * class file_base -
 *
 * Just a base class at which an inode can point.  No data or
 * functions.  Makes the synthesized members useable only from
 * the derived classes.
 */
class file_base {
    friend class inode_state;
    friend void print_inode(inode_ptr inode, std::string dirname);
    friend inode::~inode();
    protected:
        file_base() = default;
        file_base(const file_base&) = default;
        file_base(file_base&&) = default;
        file_base& operator=(const file_base&) = default;
        file_base& operator=(file_base&&) = default;
        virtual ~file_base() = default;
        virtual size_t size() const = 0;
    public:
        friend plain_file_ptr plain_file_ptr_of(file_base_ptr);
        friend directory_ptr directory_ptr_of(file_base_ptr);
};

/**
 * class plain_file -
 *
 * Used to hold data.
 * synthesized default ctor -
 *    Default vector<std::string> is a an empty vector.
 * readfile -
 *    Returns a copy of the contents of the util::wordvec in the file.
 *    Throws an yshell_exn for a directory.
 * writefile -
 *    Replaces the contents of a file with new contents.
 *    Throws an yshell_exn for a directory.
 */
class plain_file: public file_base {
    friend std::ostream& operator<<(std::ostream& out,
                                const plain_file& file);
    private:
        util::wordvec data;
    public:
        size_t size() const override;
        const util::wordvec& readfile() const;
        void writefile(const util::wordvec& newdata);
};

/**
 * class directory -
 *
 * Used to map filenames onto inode pointers.
 * default ctor -
 *    Creates a new map with keys "." and "..".
 * remove -
 *    Removes the file or subdirectory from the current inode.
 *    Throws an yshell_exn if this is not a directory, the file
 *    does not exist, or the subdirectory is not empty.
 *    Here empty means the only entries are dot(.) and dotdot(..).
 * mkdir -
 *    Creates a new directory under the current directory and
 *    immediately adds the directories dot(.) and dotdot(..) to it.
 *    Note that the parent(..) of / is / itself.  It is an error
 *    if the entry already exists.
 * mkfile -
 *    Create a new empty text file with the given name.  Error if
 *    a dirent with that name exists.
 * df_clear -
 *    Performs a depth-first clear()-ing of all directories in dirents.
 *    You should probably call this on the root dir
 *    to free all the memory.
 */
class directory: public file_base {
    friend class inode_state;
    friend std::ostream& operator<<(std::ostream& out,
                                 const directory& dir);
    private:
        dir_map dirents;
    public:
        size_t size() const override;
        void remove(const std::string& filename);
        inode& mkdir(inode_ptr inode, const std::string& dirname);
        inode& mkfile(const std::string& filename);
        void df_clear();
};

#endif

