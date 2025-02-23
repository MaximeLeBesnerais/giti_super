// include/giti/commit/messages.hpp
/*
* Author: Maxime Le Besnerais
* Created: 2/24/2025
* License: MIT
* GitHub: https://github.com/MaximeLeBesnerais
*/


#ifndef GITI_COMMIT_MESSAGE_HPP_
#define GITI_COMMIT_MESSAGE_HPP_

#include "giti/git/types.hpp"
#include <string>
#include <vector>
#include <sstream>

namespace giti {
namespace commit {

struct FileEntry {
    std::string path;
    git::FileStatus status;
};

class MessageFormatter {
public:
    static std::string formatCommitMessage(
        const std::string& tag,
        const std::string& title,
        const std::vector<FileEntry>& files,
        const std::string& comment
    ) {
        std::stringstream ss;
        
        // Format header
        if (title.empty()) {
            ss << "[" << tag << "] " << files.size() << " file" 
               << (files.size() > 1 ? "s" : "") << " were committed\n";
        } else {
            ss << "[" << tag << "] " << title << "\n";
        }
        
        // Format files section
        ss << "\tFiles:\n";
        for (const auto& file : files) {
            ss << "\t\t" << file.path << ": " 
               << statusToString(file.status) << "\n";
        }
        
        // Format comment section
        ss << "\n\tComment:\n";
        ss << "\t\t" << (comment.empty() ? "Commit made without comment" : comment) 
           << "\n";
        
        return ss.str();
    }

private:
    static std::string statusToString(git::FileStatus status) {
        switch (status) {
            case git::FileStatus::Modified: return "Modified";
            case git::FileStatus::ModifiedStaged: return "Modified and staged";
            case git::FileStatus::ModifiedBoth: return "Modified and staged with new changes";
            case git::FileStatus::Added: return "Added";
            case git::FileStatus::Deleted: return "Deleted";
            case git::FileStatus::Renamed: return "Renamed";
            case git::FileStatus::Copied: return "Copied";
            case git::FileStatus::Unmerged: return "Unmerged";
            case git::FileStatus::Untracked: return "Untracked";
            case git::FileStatus::Ignored: return "Ignored";
            default: return "Unknown";
        }
    }
};

} // namespace commit
} // namespace giti

#endif // GITI_COMMIT_MESSAGE_HPP_
