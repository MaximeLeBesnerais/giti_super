/*
Name: git_lib_tools.cpp                    
by: Maxime                                 
mail: maxime.le-besnerais@epitech.eu           
description:                               
created: 20/02/2025 22:57:34              
*/

#include <GitRepoClass.hpp>

std::string statusToString(file_status status) {
    switch (status) {
        case M: return "Modified";
        case MM: return "Modified and staged";
        case A: return "Added";
        case D: return "Deleted";
        case R: return "Renamed";
        case C: return "Copied";
        case U: return "Updated but unmerged";
        case UNTRACKED: return "?? - Untracked";
        case IGNORED: return "!! - Ignored";
    }
    return "";
}

GitRepository::GitRepository() : repo(nullptr), initialized(false) {
    git_libgit2_init();
}

GitRepository::~GitRepository() {
    cleanup();
    git_libgit2_shutdown();
}

GitRepository::GitRepository(GitRepository&& other) noexcept
    : repo(other.repo), initialized(other.initialized) {
    other.repo = nullptr;
    other.initialized = false;
}

GitRepository& GitRepository::operator=(GitRepository&& other) noexcept {
    if (this != &other) {
        cleanup();
        repo = other.repo;
        initialized = other.initialized;
        other.repo = nullptr;
        other.initialized = false;
    }
    return *this;
}

bool GitRepository:: open(const std::string& path) {
    cleanup();
    
    git_buf root = { nullptr, 0, 0 };
    int error = git_repository_discover(&root, path.c_str(), 0, nullptr);
    
    if (error == 0) {
        error = git_repository_open(&repo, root.ptr);
        initialized = (error == 0);
    }
    
    git_buf_dispose(&root);
    return initialized;
}

file_status GitRepository::convertGitStatus(const git_status_entry* entry) {
    if (entry->status == GIT_STATUS_IGNORED)
        return IGNORED;
    if (entry->status == GIT_STATUS_WT_NEW)
        return UNTRACKED;
    
    uint32_t status = entry->status;
    
    if ((status & GIT_STATUS_INDEX_NEW) != 0)
        return A;
    if ((status & GIT_STATUS_INDEX_DELETED) != 0)
        return D;
    if ((status & GIT_STATUS_INDEX_RENAMED) != 0)
        return R;
    if ((status & GIT_STATUS_INDEX_TYPECHANGE) != 0)
        return C;
    if ((status & GIT_STATUS_CONFLICTED) != 0)
        return U;
    
    bool staged = (status & GIT_STATUS_INDEX_MODIFIED) != 0;
    bool unstaged = (status & GIT_STATUS_WT_MODIFIED) != 0;
    
    if (staged && unstaged)
        return MM;
    if (staged || unstaged)
        return M;
    
    return UNTRACKED;
}

std::vector<fs_porcelain> GitRepository::getStatus() {
    if (!initialized) {
        throw std::runtime_error("Repository not initialized");
    }

    std::vector<fs_porcelain> result;
    git_status_list* status_list = nullptr;
    git_status_options opts = GIT_STATUS_OPTIONS_INIT;
    opts.show = GIT_STATUS_SHOW_INDEX_AND_WORKDIR;
    opts.flags = GIT_STATUS_OPT_INCLUDE_UNTRACKED |
                 GIT_STATUS_OPT_RENAMES_HEAD_TO_INDEX |
                 GIT_STATUS_OPT_SORT_CASE_SENSITIVELY;

    int error = git_status_list_new(&status_list, repo, &opts);
    if (error != 0) {
        throw std::runtime_error("Failed to get status list");
    }

    size_t count = git_status_list_entrycount(status_list);
    for (size_t i = 0; i < count; i++) {
        const git_status_entry* entry = git_status_byindex(status_list, i);
        fs_porcelain status_entry;
        
        // Get the filepath
        status_entry.filepath = entry->head_to_index ? 
            entry->head_to_index->new_file.path :
            entry->index_to_workdir->new_file.path;
        
        // Get the status
        status_entry.status = convertGitStatus(entry);
        
        result.push_back(status_entry);
    }

    git_status_list_free(status_list);
    return result;
}

bool GitRepository::commit(const std::string& title,
                         const std::string& message,
                         const std::vector<std::string>& files) {
    if (!initialized) {
        throw std::runtime_error("Repository not initialized");
    }

    git_signature* signature = nullptr;
    git_index* index = nullptr;
    git_oid tree_id, commit_id;
    git_tree* tree = nullptr;
    git_commit* parent = nullptr;
    
    try {
        // Get the default signature
        if (git_signature_default(&signature, repo) != 0) {
            throw std::runtime_error("Failed to create signature");
        }

        // Get the repository index
        if (git_repository_index(&index, repo) != 0) {
            throw std::runtime_error("Failed to get repository index");
        }

        // Add specified files to the index
        for (const auto& file : files) {
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

        if (git_tree_lookup(&tree, repo, &tree_id) != 0) {
            throw std::runtime_error("Failed to look up tree");
        }

        // Get HEAD as parent commit
        git_reference* head = nullptr;
        if (git_repository_head(&head, repo) == 0) {
            const git_oid* parent_id = git_reference_target(head);
            git_commit_lookup(&parent, repo, parent_id);
            git_reference_free(head);
        }

        // Create commit
        std::string full_message = title + "\n\n" + message;
        
        const git_commit* parents[] = { parent };
        int error = git_commit_create(
            &commit_id,
            repo,
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
