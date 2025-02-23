// src/git/types.cpp
/*
* Author: Maxime Le Besnerais
* Created: 2/24/2025
* License: MIT
* GitHub: https://github.com/MaximeLeBesnerais
*/

#include "giti/git/types.hpp"

namespace giti {
namespace git {

std::string statusToString(FileStatus status) {
    switch (status) {
        case FileStatus::Modified: return "Modified";
        case FileStatus::ModifiedStaged: return "Modified and staged";
        case FileStatus::ModifiedBoth: return "Modified and staged with new changes";
        case FileStatus::Added: return "Added";
        case FileStatus::Deleted: return "Deleted";
        case FileStatus::Renamed: return "Renamed";
        case FileStatus::Copied: return "Copied";
        case FileStatus::Unmerged: return "Updated but unmerged";
        case FileStatus::Untracked: return "?? - Untracked";
        case FileStatus::Ignored: return "!! - Ignored";
    }
    return "Unknown";
}

} // namespace git
} // namespace giti
