# Code Reorganization and Optimization Recommendations

## 1. Project Structure
Suggested directory structure:
```
giti/
├── include/             # Public headers
├── src/                 # Source files
│   ├── config/         # Configuration related code
│   ├── git/            # Git operations related code
│   ├── parsers/        # Command line parsing related code
│   └── utils/          # Utility functions and helpers
├── tests/              # Unit tests
└── CMakeLists.txt      # Build system
```

## 2. Component-wise Reorganization

### 2.1 Git Operations Component
Create a dedicated namespace `giti::git` for all git-related operations:

```cpp
// include/giti/git/repository.hpp
namespace giti::git {
    enum class FileStatus {
        Modified,
        ModifiedStaged,
        ModifiedBoth,
        Added,
        Deleted,
        Renamed,
        Copied,
        Unmerged,
        Untracked,
        Ignored
    };

    class Repository {
    public:
        // ... existing GitRepository methods ...
    private:
        struct Impl;  // PIMPL idiom
        std::unique_ptr<Impl> pImpl;
    };
}
```

### 2.2 Configuration Component
Move configuration into its own namespace with better encapsulation:

```cpp
// include/giti/config/config.hpp
namespace giti::config {
    struct Version {
        std::string name;
        std::string changelog;
        semver::version version;  // Use a proper semantic versioning type
    };

    class Config {
    public:
        static Config loadFromFile(const std::filesystem::path& path);
        static Config createDefault();
        
        // Getters for configuration sections
        const Version& getVersion() const;
        const FileSetup& getFileSetup() const;
        const CommitSettings& getCommitSettings() const;
        
    private:
        Version version;
        FileSetup fileSetup;
        CommitSettings commitSettings;
    };
}
```

### 2.3 Command Line Interface Component
Organize CLI handling into a dedicated component:

```cpp
// include/giti/cli/command.hpp
namespace giti::cli {
    class Command {
    public:
        virtual ~Command() = default;
        virtual int execute() = 0;
    };

    class CommitCommand : public Command {
    public:
        CommitCommand(std::string type, 
                     std::string title,
                     std::string message,
                     std::vector<std::string> files);
        int execute() override;
    };

    // Other commands...
}
```

## 3. Specific Optimizations

### 3.1 Memory Management
- Use smart pointers consistently for RAII
- Implement PIMPL idiom for better ABI stability
- Use std::string_view for string parameters that aren't stored

### 3.2 Error Handling
Create a consistent error handling strategy:

```cpp
// include/giti/core/error.hpp
namespace giti {
    class Error : public std::runtime_error {
    public:
        enum class Code {
            Success,
            GitError,
            ConfigError,
            ParseError
        };

        Error(Code code, std::string message);
        Code code() const;
    };
}
```

### 3.3 Configuration Improvements
- Use filesystem::path instead of raw strings for paths
- Add validation for configuration values
- Implement proper versioning for config files

### 3.4 Git Operations Improvements
- Add batch operations for multiple files
- Implement proper cleanup using RAII
- Add support for async operations where appropriate

## 4. Modern C++ Features to Adopt

1. Use std::optional for optional values
2. Use std::variant for union-like behavior
3. Use std::span for array views
4. Use constexpr where possible
5. Use structured bindings for better readability

Example implementation:

```cpp
// src/git/repository.cpp
namespace giti::git {
    std::optional<Repository> Repository::open(const std::filesystem::path& path) {
        try {
            Repository repo;
            if (repo.initializeFromPath(path)) {
                return repo;
            }
            return std::nullopt;
        } catch (const Error& e) {
            return std::nullopt;
        }
    }

    std::vector<FileStatus> Repository::getModifiedFiles() const {
        return getStatus() | std::views::filter(isModified) 
                          | std::views::transform(toFileStatus);
    }
}
```

## 5. Testing Strategy

1. Add unit tests for each component
2. Use mock objects for git operations
3. Add integration tests for end-to-end workflows
4. Implement proper test fixtures

Example test:

```cpp
// tests/git/repository_test.cpp
TEST_CASE("Repository::getStatus returns correct statuses") {
    auto repo = GitTestFixture::createTestRepo();
    
    SECTION("New file is marked as untracked") {
        Files::createFile(repo.path() / "new.txt");
        auto status = repo.getStatus();
        REQUIRE(status.size() == 1);
        CHECK(status[0].status == FileStatus::Untracked);
    }
}
```

## 6. Build System Improvements

1. Use modern CMake practices
2. Add proper dependency management
3. Configure installation targets
4. Add version information to builds

Example CMakeLists.txt:

```cmake
cmake_minimum_required(VERSION 3.15)
project(giti VERSION 3.0.0)

# Add compile commands export for tooling
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

# Use modern C++
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Add library targets
add_library(giti_core
    src/git/repository.cpp
    src/config/config.cpp
    src/parsers/cli_parser.cpp
)

# Configure installation
install(TARGETS giti_core
    LIBRARY DESTINATION lib
    ARCHIVE DESTINATION lib
    RUNTIME DESTINATION bin
)
```

## Implementation Priority

1. Reorganize project structure
2. Implement core error handling
3. Refactor git operations component
4. Update configuration handling
5. Implement new CLI structure
6. Add tests
7. Update build system

This will provide a more maintainable and robust codebase while maintaining the existing functionality.