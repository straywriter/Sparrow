#pragma once

namespace FileSystem
{

#include <memory>
#include <string>


#include "fileinfo.h"

class DirEntry
{
  public:
    ~DirEntry();
    bool is_directory() const;
    bool is_regular_file() const;
    bool is_symlink() const;
    std::string path() const;

  private:
    friend class DirIterator;
    DirEntry();
    struct Private;
    std::unique_ptr<Private> p;
};

class DirIterator
{
  public:
    using value_type = DirEntry;
    using difference_type = std::ptrdiff_t;
    using pointer = value_type *;
    using reference = value_type &;
    using iterator_category = std::input_iterator_tag;
    DirIterator(const DirIterator &it);
    DirIterator &operator=(const DirIterator &it);
    DirIterator operator++();
    const value_type &operator*() const;
    const value_type *operator->() const;

    friend bool operator==(const DirIterator &it1, const DirIterator &it2);
    friend bool operator!=(const DirIterator &it1, const DirIterator &it2);
    friend DirIterator begin(DirIterator it) noexcept;
    friend DirIterator end(const DirIterator &) noexcept;
    ~DirIterator();

  private:
    friend class Dir;
    DirIterator();
    DirIterator(const std::string &path);
    struct Private;
    std::unique_ptr<Private> p;
};

/** Class representing a directory in the file system */
class Dir final
{
  public:
    Dir();
    Dir(const std::string &path);
    Dir(const Dir &d);
    Dir &operator=(const Dir &d);
    ~Dir();
    void setPath(const std::string &path);
    std::string path() const;

    DirIterator iterator() const;

    bool exists() const;
    std::string filePath(const std::string &path, bool acceptsAbsPath = true) const;
    bool exists(const std::string &path, bool acceptsAbsPath = true) const;
    bool mkdir(const std::string &path, bool acceptsAbsPath = true) const;
    bool rmdir(const std::string &path, bool acceptsAbsPath = true) const;
    bool remove(const std::string &path, bool acceptsAbsPath = true) const;
    bool rename(const std::string &orgName, const std::string &newName, bool acceptsAbsPath = true) const;
    bool copy(const std::string &src, const std::string &dest, bool acceptsAbsPath = true) const;
    std::string absPath() const;

    bool isRelative() const;

    static bool isRelativePath(const std::string &path);
    static std::string currentDirPath();
    static bool setCurrent(const std::string &path);
    static std::string cleanDirPath(const std::string &path);

  private:
    struct Private;
    std::unique_ptr<Private> p;
};

} // namespace FileSystem