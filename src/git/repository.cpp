// src/git/repository.cpp
/*
* Author: Maxime Le Besnerais
* Created: 2/24/2025
* License: MIT
* GitHub: https://github.com/MaximeLeBesnerais
*/

#include "giti/git/repository.hpp"
#include <git2.h>
#include <stdexcept>
#include <algorithm>
#include <iostream>

namespace giti {
namespace git {

class Repository::Impl {
public:
    Impl() : repo(nullptr), initialized(false) {
        git_libgit2_init();
    }

    ~Impl() {
        cleanup();
        git_libgit2_shutdown();
    }

    void cleanup() {
        if (repo) {
            git_repository_free(repo);
            repo = nullptr;
        }
        initialized = false;
    }

    FileStatus convertGitStatus(const git_status_entry* entry) {
        if (entry->status == GIT_STATUS_IGNORED)
            return FileStatus::Ignored;
        if (entry->status == GIT_STATUS_WT_NEW)
            return FileStatus::Untracked;
        
        uint32_t status = entry->status;
        
        if ((status & GIT_STATUS_INDEX_NEW) != 0)
            return FileStatus::Added;
        if ((status & GIT_STATUS_INDEX_DELETED) != 0)
            return FileStatus::Deleted;
        if ((status & GIT_STATUS_INDEX_RENAMED) != 0)
            return FileStatus::Renamed;
        if ((status & GIT_STATUS_INDEX_TYPECHANGE) != 0)
            return FileStatus::Copied;
        if ((status & GIT_STATUS_CONFLICTED) != 0)
            return FileStatus::Unmerged;
        
        bool staged = (status & GIT_STATUS_INDEX_MODIFIED) != 0;
        bool unstaged = (status & GIT_STATUS_WT_MODIFIED) != 0;
        
        if (staged && unstaged)
            return FileStatus::ModifiedBoth;
        if (staged)
            return FileStatus::ModifiedStaged;
        if (unstaged)
            return FileStatus::Modified;
        
        return FileStatus::Untracked;
    }

    git_repository* repo;
    bool initialized;
    std::filesystem::path rootPath;
};

// Constructor & Destructor
Repository::Repository() : pImpl(std::make_unique<Impl>()) {}
Repository::~Repository() = default;

// Move operations
Repository::Repository(Repository&&) noexcept = default;
Repository& Repository::operator=(Repository&&) noexcept = default;

bool Repository::open(const std::filesystem::path& path) {
    git_buf root = { nullptr, 0, 0 };
    int error = git_repository_discover(&root, path.string().c_str(), 0, nullptr);
    
    if (error == 0) {
        error = git_repository_open(&pImpl->repo, root.ptr);
        pImpl->initialized = (error == 0);
        if (pImpl->initialized) {
            pImpl->rootPath = git_repository_workdir(pImpl->repo);
        }
    }
    
    git_buf_dispose(&root);
    return pImpl->initialized;
}

bool Repository::isInitialized() const {
    return pImpl->initialized;
}

std::filesystem::path Repository::getPath() const {
    if (!pImpl->initialized) {
        throw std::runtime_error("Repository not initialized");
    }
    return pImpl->rootPath;
}

std::vector<FileStatusEntry> Repository::getStatus() const {
    if (!pImpl->initialized) {
        throw std::runtime_error("Repository not initialized");
    }

    std::vector<FileStatusEntry> result;
    git_status_list* status_list = nullptr;
    git_status_options opts = GIT_STATUS_OPTIONS_INIT;
    opts.show = GIT_STATUS_SHOW_INDEX_AND_WORKDIR;
    opts.flags = GIT_STATUS_OPT_INCLUDE_UNTRACKED |
                 GIT_STATUS_OPT_RENAMES_HEAD_TO_INDEX |
                 GIT_STATUS_OPT_SORT_CASE_SENSITIVELY;

    int error = git_status_list_new(&status_list, pImpl->repo, &opts);
    if (error != 0) {
        throw std::runtime_error("Failed to get status list");
    }

    try {
        size_t count = git_status_list_entrycount(status_list);
        for (size_t i = 0; i < count; i++) {
            const git_status_entry* entry = git_status_byindex(status_list, i);
            FileStatusEntry status_entry;
            
            status_entry.filepath = entry->head_to_index ? 
                entry->head_to_index->new_file.path :
                entry->index_to_workdir->new_file.path;
            
            status_entry.status = pImpl->convertGitStatus(entry);
            
            result.push_back(status_entry);
        }
    } catch (...) {
        git_status_list_free(status_list);
        throw;
    }

    git_status_list_free(status_list);
    return result;
}

std::vector<FileStatusEntry> Repository::getModifiedFiles() const {
    auto allStatus = getStatus();
    std::vector<FileStatusEntry> modified;
    
    std::copy_if(allStatus.begin(), allStatus.end(), 
                 std::back_inserter(modified),
                 [](const FileStatusEntry& entry) {
                     return entry.status == FileStatus::Modified ||
                            entry.status == FileStatus::ModifiedStaged ||
                            entry.status == FileStatus::ModifiedBoth;
                 });
    
    return modified;
}

std::vector<FileStatusEntry> Repository::getUntrackedFiles() const {
    auto allStatus = getStatus();
    std::vector<FileStatusEntry> untracked;
    
    std::copy_if(allStatus.begin(), allStatus.end(), 
                 std::back_inserter(untracked),
                 [](const FileStatusEntry& entry) {
                     return entry.status == FileStatus::Untracked;
                 });
    
    return untracked;
}

bool Repository::commit(const CommitOptions& options) {
    if (!pImpl->initialized) {
        throw std::runtime_error("Repository not initialized");
    }

    git_signature* signature = nullptr;
    git_index* index = nullptr;
    git_oid tree_id, commit_id;
    git_tree* tree = nullptr;
    git_commit* parent = nullptr;
    
    try {
        // Get the default signature
        if (git_signature_default(&signature, pImpl->repo) != 0) {
            throw std::runtime_error("Failed to create signature");
        }

        // Get the repository index
        if (git_repository_index(&index, pImpl->repo) != 0) {
            throw std::runtime_error("Failed to get repository index");
        }

        // Add specified files to the index
        for (const auto& file : options.files) {
            if (git_index_add_bypath(index, file.c_str()) != 0) {
                throw std::runtime_error("Failed to add file to index: " + file);
            }
        }

        // Write the index
        if (git_index_write(index) != 0) {
            throw std::runtime_error("Failed to write index");
        }

        // Create tree from index
        if (git_index_write_tree(&tree_id, index) != 0) {
            throw std::runtime_error("Failed to write tree");
        }

        if (git_tree_lookup(&tree, pImpl->repo, &tree_id) != 0) {
            throw std::runtime_error("Failed to look up tree");
        }

        // Get HEAD as parent commit
        git_reference* head = nullptr;
        if (git_repository_head(&head, pImpl->repo) == 0) {
            const git_oid* parent_id = git_reference_target(head);
            git_commit_lookup(&parent, pImpl->repo, parent_id);
            git_reference_free(head);
        }

        // Create commit
        std::string full_message = options.title + "\n\n" + options.message;
        
        const git_commit* parents[] = { parent };
        int error = git_commit_create(
            &commit_id,
            pImpl->repo,
            "HEAD",
            signature,
            signature,
            "UTF-8",
            full_message.c_str(),
            tree,
            parent ? 1 : 0,
            parent ? parents : nullptr
        );

        // Cleanup
        git_tree_free(tree);
        if (parent) git_commit_free(parent);
        git_index_free(index);
        git_signature_free(signature);

        return error == 0;
    } catch (...) {
        // Cleanup in case of error
        if (tree) git_tree_free(tree);
        if (parent) git_commit_free(parent);
        if (index) git_index_free(index);
        if (signature) git_signature_free(signature);
        throw;
    }
}
} // namespace git
} // namespace giti