# Giti Powercharged
<a href="https://gitmoji.dev">
  <img src="https://img.shields.io/badge/gitmoji-%20😜%20😍-FFDD67.svg?style=flat-square" alt="Gitmoji">
</a>

Python script to write commit message following the [Conventional Commits](https://www.conventionalcommits.org/en/v1.0.0/) specification.
This is a tool and not perfect, so please use it with caution.

## Installation

```bash
curl https://raw.githubusercontent.com/MaximeLeBesnerais/giti_super/main/install.sh | sh
```
## Usage

Default usage:
```bash
giti <type> [title] [commit message] <files>
```
to skip a field, use `-`:
- `-nt` to skip the title
- `-nc` to skip the commit message
- `-nct` to skip the title and the commit message (or `-ntc`)

You can't skip the type, nor the files.

### Examples

```bash
giti feat -nct README.md
giti fix -nt "Solved a bug" readme.md (no title)
giti bug -nc "Solved a bug" readme.md (no commit message)
```
