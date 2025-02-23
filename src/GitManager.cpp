/*  GitManager.cpp
  by: Maxime Le Besnerais                              
  mail: maxoulebesnerais@gmail.com                              
  created: 24/02/2025 00:18:29                   */

#include "GitManager.hpp"

GitManager::GitManager(GitRepository& repo) : repo(repo) {}

bool GitManager::initialize() {
    try {
        return repo.open();
    } catch (const std::exception& e) {
        std::cerr << "Git initialization error: " << e.what() << std::endl;
        return false;
    }
}

void GitManager::printRepositoryStatus() const {
    std::string path = repo.getPath();
    std::cout << "Repository path: " << path << std::endl;
    
    auto status = repo.getStatus();
    for (const auto& file : status) {
        std::cout << file.filepath << " : " 
                 << statusToString(file.status) << std::endl;
    }
}
