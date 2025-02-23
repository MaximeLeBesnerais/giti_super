// src/handlers/commit_handlers.cpp
/*
 * Author: Maxime Le Besnerais
 * Created: 2/24/2025
 * License: MIT
 * GitHub: https://github.com/MaximeLeBesnerais
 */

#include "giti/handlers/commit_handler.hpp"
#include "giti/commit/message.hpp"
#include <iostream>
#include <algorithm>

namespace giti
{
    namespace handlers
    {

        class CommitHandler::Impl
        {
        public:
            Impl(git::Repository &r, const config::Config &cfg)
                : repo(r), config(cfg) {}

            bool validateAndGetFiles(std::vector<std::string> &files,
                                     std::vector<commit::FileEntry> &fileEntries)
            {
                auto status = repo.getStatus();
                bool allValid = true;
                for (auto &file : files)
                {
                    if (file.find("./") == 0)
                    {
                        file = file.substr(2);
                    }
                }

                for (const auto &filepath : files)
                {
                    auto it = std::find_if(status.begin(), status.end(),
                                           [&filepath](const auto &entry)
                                           {
                                               return entry.filepath == filepath;
                                           });

                    if (it != status.end())
                    {
                        fileEntries.push_back({filepath, it->status});
                    }
                    else
                    {
                        std::cerr << "Error: File '" << filepath
                                  << "' not found in git status" << std::endl;
                        allValid = false;
                    }
                }

                return allValid;
            }

            bool commitFiles(const cli::CommitCommand &cmd,
                             const std::vector<commit::FileEntry> &fileEntries)
            {
                std::string tag = cmd.type;
                std::transform(tag.begin(), tag.end(), tag.begin(), ::toupper);
                std::string message = commit::MessageFormatter::formatCommitMessage(
                    tag,
                    cmd.title, // Can be empty
                    fileEntries,
                    cmd.message // Can be empty
                );

                git::Repository::CommitOptions options{
                    .title = message,
                    .message = "",
                    .files = cmd.files};

                return repo.commit(options);
            }

        private:
            git::Repository &repo;
            const config::Config &config;
        };

        // Constructor & Destructor
        CommitHandler::CommitHandler(git::Repository &repo, const config::Config &config)
            : pImpl(std::make_unique<Impl>(repo, config)) {}
        CommitHandler::~CommitHandler() = default;

        // Move operations
        CommitHandler::CommitHandler(CommitHandler &&) noexcept = default;
        CommitHandler &CommitHandler::operator=(CommitHandler &&) noexcept = default;

        bool CommitHandler::handle(cli::CommitCommand &cmd)
        {
            std::vector<commit::FileEntry> fileEntries;

            if (!pImpl->validateAndGetFiles(cmd.files, fileEntries))
            {
                return false;
            }

            if (pImpl->commitFiles(cmd, fileEntries))
            {
                std::cout << "Successfully committed changes" << std::endl;
                return true;
            }

            return false;
        }

    } // namespace handlers
} // namespace giti