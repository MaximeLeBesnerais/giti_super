import os

status_letters = {
    'M': 'This file has been modified',
    'A': 'This is a new file',
    'D': 'This is a file suppression',
    'R': 'This file has been renamed or moved',
    'C': 'This file has been copied',
    'U': 'Due to a merge conflict, this file is unmerged',
    '??': 'As far as git is concerned, this file is untracked'
}

command_types = {
    "fix": "[BUG FIX]",
    "bug": "[BUG FIX]",
    "feat": "[FEAT]",
    "docs": "[DOCUMENTATION]",
    "wip": "[WIP]",
    "header": "[HEADER]",
    "assets": "[ASSETS]",
    "make": "[MAKEFILE]"
}


def giti_all(potential_title):
    os.system('git add .')
    git_status = os.popen('git status -s').readlines()
    git_status = [line[:-1] for line in git_status]
    commit_line = []
    if len(git_status) == 0:
        print("There are no changes to commit")
        exit()
    for line in git_status:
        status_letter = line[:2].strip()
        status_comment = status_letters[status_letter]
        commit_line.append(f"\t{line[3:]}: {status_comment}")
    if potential_title == "":
        commit_message = f"[ALL] {len(git_status)} {'file' if len(git_status) <= 1 else 'files'} changed\n" + \
                         "\n".join(commit_line)
    else:
        commit_message = f"[ALL] {potential_title}\n" + "\n".join(commit_line)
    os.system(f'git commit -m "{commit_message}"')


def giti_makefile(potential_title):
    git_log = os.popen('git status -s').readlines()
    git_log = [line[:-1] for line in git_log]
    commit_line = []
    if len(git_log) == 0:
        print("There are no changes to commit")
        exit()
    for line in git_log:
        if line[3:].startswith('Makefile'):
            os.system(f'git add {line[3:]}')
            letter = line[:2].strip()
            commit_line.append(f"\t{line[3:]}: {status_letters[letter]}")
    if len(commit_line) == 0:
        print("There are no Makefile to commit")
        exit()
    if potential_title == "":
        commit_message = f"[MAKEFILE] {len(commit_line)} {'file' if len(commit_line) <= 1 else 'files'} changed\n" + \
                         "\n".join(commit_line)
    else:
        commit_message = f"[MAKEFILE] {potential_title}\n" + "\n".join(commit_line)
    os.system(f'git commit -m "{commit_message}"')


def giti_header(potential_title):
    git_log = os.popen('git status -s').readlines()
    git_log = [line[:-1] for line in git_log]
    header_extensions = [".h", ".hpp", ".hxx", ".hh"]
    commit_line = []
    if len(git_log) == 0:
        print("There are no changes to commit")
        exit()
    for line in git_log:
        if line[3:].endswith(tuple(header_extensions)):
            os.system(f'git add {line[3:]}')
            letter = line[:2].strip()
            commit_line.append(f"\t{line[3:]}: {status_letters[letter]}")
            continue
    if len(commit_line) == 0:
        print("There are no header files to commit")
        exit()
    if potential_title == "":
        commit_message = f"[HEADER] {len(commit_line)} {'file' if len(commit_line) <= 1 else 'files'} changed\n" + \
                         "\n".join(commit_line)
    else:
        commit_message = f"[HEADER] {potential_title}\n" + "\n".join(commit_line)
    os.system(f'git commit -m "{commit_message}"')


def giti_coding_style(files):
    git_log = os.popen('git status -s').readlines()
    git_log = [line[:-1] for line in git_log]
    commit_line = []
    if len(git_log) == 0:
        print("There are no changes to commit")
        exit()
    for line in git_log:
        if line[3:] in files:
            os.system(f'git add {line[3:]}')
            letter = line[:2].strip()
            commit_line.append(f"\t{line[3:]}: {status_letters[letter]}")
            continue
    if len(commit_line) == 0:
        print("There are no files to commit")
        exit()
    commit_message = f"[CODING STYLE] {len(commit_line)} {'file' if len(commit_line) <= 1 else 'files'} changed\n" + \
                     "\n".join(commit_line)
    os.system(f'git commit -m "{commit_message}"')


def giti_ignore(potential_title):
    if not os.path.exists('.gitignore'):
        print("There is no .gitignore file to commit")
        exit()
    git_log = os.popen('git status -s').readlines()
    commit_line = []
    for line in git_log:
        if ".gitignore" in line:
            os.system(f'git add {line[3:]}')
            letter = line[:2].strip()
            commit_line.append(f"\t{line[3:]}: {status_letters[letter]}")
    if potential_title == "":
        commit_message = f"[IGNORE] Modified .gitignore"
    else:
        commit_message = f"[IGNORE] {potential_title}"
    os.system(f'git commit -m "{commit_message}"')


def giti_del(potential_title):
    git_log = os.popen('git ls-files --deleted').readlines()
    if len(git_log) == 0:
        print("There are no deleted files to commit")
        exit()
    commit_line = []
    for line in git_log:
        os.system(f'git add {line}')
        commit_line.append(f"\t{line}: Deleted file")
    if potential_title == "":
        commit_message = f"[DELETION] {len(commit_line)} {'file' if len(commit_line) <= 1 else 'files'} removed\n" + \
                         "\n".join(commit_line)
    else:
        commit_message = f"[DELETION] {potential_title}\n" + "\n".join(commit_line)
    os.system(f'git commit -m "{commit_message}"')


def generic_giti(tag_key, title, comment, files: list):
    if files == "":
        print("No files to commit")
        exit()
    commit_line = []
    git_log = os.popen('git status -s').readlines()
    git_log = [line[:-1] for line in git_log]
    tag = command_types[tag_key] if tag_key in command_types else f"[{tag_key.upper()}]"
    for file in files:
        os.system(f'git add {file}')
        letter = git_log[0][:2].strip()
        commit_line.append(f"\t{file}: {status_letters[letter]}")
    if title == "":
        potential_title = f"{tag} {len(files)} {'file' if len(files) <= 1 else 'files'} changed\n"
    else:
        potential_title = f"{tag} {title}\n"
    if comment != "":
        commit_line.append(f"\t\t{comment}")
    else:
        commit_line.append(f"\tCommitted without a comment")
    commit_message = potential_title + "\n".join(commit_line)
    os.system(f'git commit -m "{commit_message}"')
