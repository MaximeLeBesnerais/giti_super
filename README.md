# Giti Powercharged

Python script to write commit message following the [Conventional Commits](https://www.conventionalcommits.org/en/v1.0.0/) specification.
This is a tool and not perfect, so please use it with caution.

## Installation

```bash
curl https://raw.githubusercontent.com/MaximeLeBesnerais/giti_super/main/install.sh | sh
```
## Usage

Default usage:
```bash
giti [skip options] <type> [files]
```
to skip a field, use `-`:
- `-nt` to skip the title
- `-nc` to skip the commit message
- `-nct` to skip the title and the commit message (or `-ntc`)

You can't skip the type, nor the files.

There is also an interactive mode. It will prompt you for each field.

- `giti -h` to show help

## Special Commands

**Utility commands:**
- `giti -a` to add all files
- `giti -m` to add all makefiles
- `giti -i` to add .gitiignore files
- `giti -d` to add deleted files
- `giti -l` to add header files

**Tool commands:**
- `giti -v` show version
- `giti -u` update giti
- `giti -U` Force update giti
- `giti -r` remove giti

- `giti --help2` show this help

### Examples

```bash
giti -nct feat README.md (no title & no commit message)
giti fix readme.md (title & commit message)
giti -nc bug readme.md (no commit message)
```
