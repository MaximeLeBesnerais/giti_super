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
}


def giti_all(potential_title):
    os.system('git add .')
    git_status = os.popen('git status -s').readlines()
    commit_line = []
    if len(git_status) == 0:
        print("There are no changes to commit")
        exit()
    for line in git_status:
        status_letter = line[:2]
        status_comment = status_letters[status_letter]
        commit_line.append(f"\t{line[3:]}: {status_comment}")
    if potential_title == "":
        commit_message = f"[ALL] {len(git_status)} files changed:\n" + "\n".join(commit_line)
    else:
        commit_message = f"[ALL] {potential_title}:\n" + "\n".join(commit_line)
    os.system(f'git commit -m "{commit_message}"')


def giti_makefile(potential_title):
    git_log = os.popen('git status -s').readlines()
    commit_line = []
    if len(git_log) == 0:
        print("There are no changes to commit")
        exit()
    for line in git_log:
        if line[3:].startswith('Makefile'):
            os.system(f'git add {line[3:]}')
            commit_line.append(f"\t{line[3:]}: {status_letters[line[:2]]}")
    if potential_title == "":
        commit_message = f"[MAKEFILE] {len(commit_line)} files changed:\n" + "\n".join(commit_line)
    else:
        commit_message = f"[MAKEFILE] {potential_title}:\n" + "\n".join(commit_line)
    os.system(f'git commit -m "{commit_message}"')


def giti_coding_style(files):
    commit_line = []
    for file in files:
        os.system(f'git add {file}')
        commit_line.append(f"\t{file}: Coding style related changes")
    commit_message = f"[CODING STYLE] {len(commit_line)} files changed:\n" + "\n".join(commit_line)
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
            commit_line.append(f"\t{line[3:]}: {status_letters[line[:2]]}")
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
        commit_message = f"[DELETION] {len(commit_line)} files removed:\n" + "\n".join(commit_line)
    else:
        commit_message = f"[DELETION] {potential_title}:\n" + "\n".join(commit_line)
    os.system(f'git commit -m "{commit_message}"')


def generic_giti(tag_key, files):
    if files == "":
        print("No files to commit")
        exit()
    commit_line = []
    git_log = os.popen('git status -s').readlines()
    tag = command_types[tag_key] if tag_key in command_types else tag_key.upper()
    for file in files:
        os.system(f'git add {file}')
        commit_line.append(f"\t{file}: {status_letters[git_log[0][:2]]}")
    potential_title = f"[{tag}] {len(files.split(' '))} files changed:\n"
    for file in files.split(' '):
        commit_line.append(f"\t{file}")
    commit_line.append(f"\t\tCommitted without a comment")
    commit_message = potential_title + "\n".join(commit_line)
    os.system(f'git commit -m "{commit_message}"')
