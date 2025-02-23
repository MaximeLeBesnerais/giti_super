// include/giti/handlers/commit_handler.hpp
/*
 * Author: Maxime Le Besnerais
 * Created: 2/24/2025
 * License: MIT
 * GitHub: https://github.com/MaximeLeBesnerais
 */

#ifndef GITI_HANDLERS_COMMIT_HANDLER_HPP_
#define GITI_HANDLERS_COMMIT_HANDLER_HPP_

#include "giti/cli/types.hpp"
#include "giti/git/repository.hpp"
#include "giti/config/config.hpp"
#include <memory>

namespace giti
{
    namespace handlers
    {

        class CommitHandler
        {
        public:
            CommitHandler(git::Repository &repo, const config::Config &config);
            ~CommitHandler();

            CommitHandler(const CommitHandler &) = delete;
            CommitHandler &operator=(const CommitHandler &) = delete;

            CommitHandler(CommitHandler &&) noexcept;
            CommitHandler &operator=(CommitHandler &&) noexcept;

            bool handle(cli::CommitCommand &cmd);

        private:
            class Impl;
            std::unique_ptr<Impl> pImpl;
        };

    } // namespace handlers
} // namespace giti

#endif // GITI_HANDLERS_COMMIT_HANDLER_HPP_
