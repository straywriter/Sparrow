#pragma once

#include <string>
namespace FileSystem

{

/** @brief Minimal replacement for QFileInfo. */
class FileInfo
{
  public:
    explicit FileInfo(const std::string &name) : m_name(name)
    {
    }

    /**
     * retrun file is exists
     *
     * @return true
     * @return false
     */
    bool exists() const;

    /**
     *
     *
     * @return size_t
     */
    size_t size() const;
    bool isWritable() const;
    bool isReadable() const;
    bool isExecutable() const;
    bool isRelative() const;
    bool isFile() const;
    bool isDir() const;
    bool isSymLink() const;
    std::string readLink() const;
    std::string filePath() const;
    std::string absFilePath() const;
    std::string fileName() const;
    std::string baseName() const;
    std::string extension(bool complete) const;
    std::string dirPath(bool absPath = true) const;

  private:
    static void correctPath(std::string &s);
    std::string m_name;
};

} // namespace FileSystem