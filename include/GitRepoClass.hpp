/*
Name: GitRepoClass.hpp                     
by: Maxime                                 
mail: maxime.le-besnerais@epitech.eu            
description:                               
created: 20/02/2025 23:04:19              
*/

#ifndef GITREPOCLASS_HPP_
    #define GITREPOCLASS_HPP_
    #include <git2.h>
    #include <string>
    #include <vector>
    #include <stdexcept>

    /*
     M  = Modified but not staged
    M   = Modified and staged
    MM  = Staged modificatiandons and new unstaged modifications
    A   = New file staged
    D   = Deleted file
    R   = Renamed file
    C   = Copied file
    U   = Updated but unmerged
    ??  = Untracked file
    !!  = Ignored file
    */

    typedef enum file_status {
        M,
        MM,
        A,
        D,
        R,
        C,
        U,
        UNTRACKED,
        IGNORED
    } file_status;

    typedef struct file_status_porcelain {
        std::string filepath;
        file_status status;
    } fs_porcelain;

    std::string statusToString(file_status status);

    class GitRepository {
    private:
        git_repository* repo;
        bool initialized;
        std::string rootPath;

        file_status convertGitStatus(const git_status_entry* entry);

    public:
        GitRepository();
        ~GitRepository();

        GitRepository(const GitRepository&) = delete;
        GitRepository& operator=(const GitRepository&) = delete;
        GitRepository(GitRepository&& other) noexcept;
        GitRepository& operator=(GitRepository&& other) noexcept;

        bool open(const std::string& path = ".");
        bool isInitialized() const {
            return initialized;
        }

        git_repository* get() {
            return repo;
        }

        const git_repository* get() const {
            return repo;
        }

        std::string getPath() const {
            if (!initialized) {
                throw std::runtime_error("Repository not initialized");
            }
            return git_repository_workdir(repo);
        };

        std::vector<fs_porcelain> getStatus();
        
        // Commit changes
        bool commit(const std::string& title, 
                const std::string& message, 
                const std::vector<std::string>& files);
    private:
        void cleanup() {
            if (repo) {
                git_repository_free(repo);
                repo = nullptr;
            }
            initialized = false;
        }
    };

#endif /* !GITREPOCLASS_HPP_ */
