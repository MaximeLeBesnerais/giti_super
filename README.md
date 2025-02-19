# ⚡ Giti Powercharged

A Python script for generating commit messages following the [Conventional Commits](https://www.conventionalcommits.org/en/v1.0.0/) specification. While helpful, this tool isn't perfect—use with caution!

---

## 🚀 Installation

Install Giti Powercharged with a single command:

```bash
curl https://raw.githubusercontent.com/MaximeLeBesnerais/giti_super/main/install.sh | sh
```

---

## 📌 Usage

### Basic Syntax
```bash
giti [skip options] <type> [files]
```
- **`<type>`**: Required. The type of commit.
- **`[files]`**: Required. The files to commit.
- **Skip Options**: Use `-` to skip specific fields.

### Skipping Fields
You can skip fields using these options:
- `-nt` → Skip the title
- `-nc` → Skip the commit message
- `-nct` or `-ntc` → Skip both title and commit message

❗ **You cannot skip the type or files.**

### Interactive Mode
If no skip options are provided, the tool will prompt you for each field.

#### Help Command
- `giti -h` → Display help menu

---

## 🛠 Special Commands

### Utility Commands
| Command       | Description               |
|--------------|---------------------------|
| `giti -a`   | Add all files              |
| `giti -m`   | Add all makefiles          |
| `giti -i`   | Add `.gitiignore` files    |
| `giti -d`   | Add deleted files          |
| `giti -l`   | Add header files           |

### Tool Commands
| Command       | Description               |
|--------------|---------------------------|
| `giti -v`   | Show version               |
| `giti -u`   | Update Giti                |
| `giti -U`   | Force update Giti          |
| `giti -r`   | Remove Giti                |
| `giti --help2` | Show extended help      |

---

## 📖 Examples

```bash
giti -nct feat README.md   # No title & no commit message
giti fix readme.md         # Includes title & commit message
giti -nc bug readme.md     # No commit message
```

---

Enhance your Git workflow with **Giti Powercharged**! 🚀

