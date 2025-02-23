/*  GitManager.hpp
  by: Maxime Le Besnerais
  mail: maxoulebesnerais@gmail.com
  created: 24/02/2025 00:14:11                   */

#ifndef GITMANAGER_HPP
#define GITMANAGER_HPP
#include "GitRepoClass.hpp"

class GitManager
{
private:
    GitRepository &repo;

public:
    explicit GitManager(GitRepository &repo);
    bool initialize();
    void printRepositoryStatus() const;
};

#endif // GITMANAGER_HPP
