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
    # for each line that contains a Makefile, add it to the commit
    for line in git_log:
        if line[3:].startswith('Makefile'):
            os.system(f'git add {line[3:]}')
            commit_line.append(f"\t{line[3:]}: {status_letters[line[:2]]}")
    if potential_title == "":
        commit_message = f"[MAKEFILE] {len(commit_line)} files changed:\n" + "\n".join(commit_line)
    else:
        commit_message = f"[MAKEFILE] {potential_title}:\n" + "\n".join(commit_line)
    os.system(f'git commit -m "{commit_message}"')


def giti_coding_style(potential_title, files):
    commit_line = []
    for file in files:
        os.system(f'git add {file}')
        commit_line.append(f"\t{file}: Coding style related changes")
    if potential_title == "":
        commit_message = f"[CODING STYLE] {len(commit_line)} files changed:\n" + "\n".join(commit_line)
    else:
        commit_message = f"[CODING STYLE] {potential_title}:\n" + "\n".join(commit_line)
    os.system(f'git commit -m "{commit_message}"')
