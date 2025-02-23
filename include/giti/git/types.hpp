// include/giti/git/types.hpp
/*
* Author: Maxime Le Besnerais
* Created: 2/24/2025
* License: MIT
* GitHub: https://github.com/MaximeLeBesnerais
*/

#ifndef GITI_GIT_TYPES_HPP_
#define GITI_GIT_TYPES_HPP_

#include <string>
#include <vector>

namespace giti {
namespace git {

enum class FileStatus {
    Modified,
    ModifiedStaged,
    ModifiedBoth,
    Added,
    Deleted,
    DeletedStaged,
    Renamed,
    Copied,
    Unmerged,
    Untracked,
    Ignored
};

struct FileStatusEntry {
    std::string filepath;
    FileStatus status;
};

std::string statusToString(FileStatus status);

} // namespace git
} // namespace giti

#endif // GITI_GIT_TYPES_HPP_
