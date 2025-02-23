// include/giti/git/repository.hpp
/*
* Author: Maxime Le Besnerais
* Created: 2/24/2025
* License: MIT
* GitHub: https://github.com/MaximeLeBesnerais
*/

#ifndef GITI_GIT_REPOSITORY_HPP_
#define GITI_GIT_REPOSITORY_HPP_

#include "giti/git/types.hpp"
#include <memory>
#include <string>
#include <vector>
#include <filesystem>
#include <optional>

namespace giti {
namespace git {

class Repository {
public:
    // Constructor & Destructor
    Repository();
    ~Repository();
    
    // Delete copy operations
    Repository(const Repository&) = delete;
    Repository& operator=(const Repository&) = delete;
    
    // Move operations
    Repository(Repository&&) noexcept;
    Repository& operator=(Repository&&) noexcept;

    // Core operations
    bool open(const std::filesystem::path& path = ".");
    bool isInitialized() const;
    std::filesystem::path getPath() const;

    // Status operations
    std::vector<FileStatusEntry> getStatus() const;
    std::vector<FileStatusEntry> getModifiedFiles() const;
    std::vector<FileStatusEntry> getUntrackedFiles() const;

    // Commit operations
    struct CommitOptions {
        std::string title;
        std::string message;
        std::vector<std::string> files;
        bool allowEmpty = false;
    };

    bool commit(const CommitOptions& options);

private:
    class Impl;
    std::unique_ptr<Impl> pImpl;
};

} // namespace git
} // namespace giti

#endif // GITI_GIT_REPOSITORY_HPP_
