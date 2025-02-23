# 🚀 Giti C++ Overload (3.0.0)

Giti has evolved! Now rewritten in blazing-fast C++, this commit message powerhouse helps you create perfect [Conventional Commits](https://www.conventionalcommits.org/en/v1.0.0/) with minimal effort. Experience Git workflow automation like never before!

## ⚡ Features

- **Lightning Fast**: Completely rewritten in C++ for maximum performance
- **Smart File Detection**: Automatically categorizes your changes
- **Conventional Commits**: Built-in support for standard commit types
- **Flexible Configuration**: Easy to customize via JSON config
- **Native Git Integration**: Direct libgit2 integration for reliable operations

## 🔧 Prerequisites

Before installing Giti, ensure you have:
- C++ compiler with C++17 support
- CMake (3.15 or higher)
- Make
- libgit2 development files
- Git

### Installing Prerequisites

#### Ubuntu/Debian
```bash
sudo apt update
sudo apt install build-essential cmake libgit2-dev git
```

#### Fedora
```bash
sudo dnf install gcc-c++ cmake libgit2-devel git
```

#### macOS
```bash
brew install cmake libgit2 git
```

## 📦 Installation

### One-Command Install (Recommended)
```bash
curl -sSL https://raw.githubusercontent.com/MaximeLeBesnerais/giti/main/install.sh | sudo bash
```

### Manual Installation
```bash
git clone https://github.com/MaximeLeBesnerais/giti.git
cd giti
make install
```

## 🎯 Usage

### Basic Command Syntax
```bash
giti <type> [files...]
```

### Options
- `-t, --title <title>`: Set commit title
- `-m, --message <message>`: Set commit message
- `--type <type>`: Specify commit type

### Quick Commands
| Command     | Description                         |
|-------------|-------------------------------------|
| `giti -a`   | Select all modified files           |
| `giti -b`   | Select build-related files          |
| `giti -i`   | Select .gitignore files             |
| `giti -d`   | Select deleted files                |
| `giti -o`   | Select other tracked files          |

### Tool Commands
| Command       | Description                     |
|---------------|---------------------------------|
| `giti -v`     | Display version information     |
| `giti -u`     | Check for updates               |
| `giti -f`     | Force reinstall                 |
| `giti -r`     | Remove Giti                     |

## 📝 Examples

```bash
# Commit all modified files
giti -a

# Feature commit with specific files
giti feat src/main.cpp include/giti/config.hpp

# Bug fix with title and message
giti fix -t "Fix memory leak" -m "Resolved issue in config parser" src/config.cpp

# Build-related changes
giti -b
```

## ⚙️ Configuration

Giti uses a JSON configuration file located at `~/.giti/config.json`. You can customize:
- Allowed commit types
- File extensions for different categories
- Commit message templates
- And more!

## 🛠️ Building from Source

```bash
git clone https://github.com/MaximeLeBesnerais/giti.git
cd giti
make
make install  # Requires sudo
```

## 📜 License

MIT License - see the [LICENSE](LICENSE) file for details.

---

⭐ Star us on GitHub if Giti makes your Git workflow easier!
