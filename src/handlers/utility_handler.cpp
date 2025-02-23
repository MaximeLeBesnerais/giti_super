// src/handlers/utility_handler.cpp
/*
 * Author: Maxime Le Besnerais
 * Created: 2/24/2025
 * License: MIT
 * GitHub: https://github.com/MaximeLeBesnerais
 */

#include "giti/handlers/utility_handler.hpp"
#include "giti/commit/message.hpp"
#include <algorithm>
#include <cassert>
#include <iostream>

namespace giti
{
    namespace handlers
    {

        class UtilityHandler::Impl
        {
        public:
            Impl(git::Repository &r, const config::Config &cfg)
                : repo(r), config(cfg) {}

            std::vector<commit::FileEntry> getMatchingFiles(const cli::UtilityFlags &flags)
            {
                std::vector<commit::FileEntry> result;
                auto allFiles = repo.getStatus();

                for (const auto &status : allFiles)
                {
                    bool shouldInclude = false;

                    if (flags.all)
                    {
                        shouldInclude = true;
                    }
                    else
                    {
                        if (flags.build && isMatchingExtension(status.filepath,
                                                               config.getFileSetup().build_file_ext))
                        {
                            shouldInclude = true;
                        }
                        if (flags.ignore && isMatchingExtension(status.filepath,
                                                                config.getFileSetup().ignore_file_ext))
                        {
                            shouldInclude = true;
                        }
                        if (flags.other && isMatchingExtension(status.filepath,
                                                               config.getFileSetup().other_file_ext))
                        {
                            shouldInclude = true;
                        }
                        if (flags.deleted && status.status == git::FileStatus::Deleted)
                        {
                            shouldInclude = true;
                        }
                    }

                    if (shouldInclude)
                    {
                        result.push_back({status.filepath, status.status});
                    }
                }

                return result;
            }

            bool commitFiles(const std::vector<commit::FileEntry> &files)
            {
                if (files.empty())
                {
                    std::cout << "No files to commit" << std::endl;
                    return false;
                }

                std::vector<std::string> filePaths;
                for (const auto &file : files)
                {
                    filePaths.push_back(file.path);
                }

                std::string message = commit::MessageFormatter::formatCommitMessage(
                    "ALL", // Auto tag for utility commits
                    "",    // Empty title for auto-generated message
                    files,
                    "Committed all matching files");

                git::Repository::CommitOptions options{
                    .title = message,
                    .message = "",
                    .files = filePaths};

                return repo.commit(options);
            }

        private:
            bool isMatchingExtension(const std::string &filepath,
                                     const std::vector<std::string> &extensions)
            {
                return std::any_of(extensions.begin(), extensions.end(),
                                   [&filepath](const std::string &ext)
                                   {
                                       if (ext.find("*") != std::string::npos)
                                       {
                                           std::string actual_ext = ext.substr(ext.find("*") + 1);
                                           if (filepath.length() >= actual_ext.length())
                                           {
                                               return filepath.compare(filepath.length() - actual_ext.length(),
                                                                       actual_ext.length(), actual_ext) == 0;
                                           }
                                       }
                                       return filepath.length() >= ext.length() &&
                                              filepath.compare(filepath.length() - ext.length(),
                                                               ext.length(), ext) == 0;
                                   });
            }

            git::Repository &repo;
            const config::Config &config;
        };

        // Constructor & Destructor
        UtilityHandler::UtilityHandler(git::Repository &repo, const config::Config &config)
            : pImpl(std::make_unique<Impl>(repo, config)) {}
        UtilityHandler::~UtilityHandler() = default;

        // Move operations
        UtilityHandler::UtilityHandler(UtilityHandler &&) noexcept = default;
        UtilityHandler &UtilityHandler::operator=(UtilityHandler &&) noexcept = default;

        bool UtilityHandler::handle(const cli::UtilityFlags &flags)
        {
            auto files = pImpl->getMatchingFiles(flags);
            return pImpl->commitFiles(files);
        }

    } // namespace handlers
} // namespace giti
