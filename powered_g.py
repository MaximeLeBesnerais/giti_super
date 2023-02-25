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
    commit_line = []
    if len(git_log) == 0:
        print("There are no changes to commit")
        exit()
    for line in git_log:
        if line[3:].startswith('Makefile'):
            os.system(f'git add {line[3:]}')
            letter = line[:2].strip()
            commit_line.append(f"\t{line[3:]}: {status_letters[letter]}")
    if potential_title == "":
        commit_message = f"[MAKEFILE] {len(commit_line)} {'file' if len(commit_line) <= 1 else 'files'} changed\n" + \
                         "\n".join(commit_line)
    else:
        commit_message = f"[MAKEFILE] {potential_title}\n" + "\n".join(commit_line)
    os.system(f'git commit -m "{commit_message}"')


def giti_coding_style(files):
    commit_line = []
    for file in files:
        os.system(f'git add {file}')
        commit_line.append(f"\t{file}: Coding style related changes")
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


def giti_interactive():
    git_log = os.popen('git status -s').readlines()
    committing_files = []
    if len(git_log) == 0:
        print("There are no changes to commit")
        exit()
    adding_files = True
    if len(git_log) == 1:
        print("There is only one file to commit, adding it automatically")
        committing_files.append(git_log[0][3:])
        adding_files = False
    while adding_files:
        if len(git_log) == 1:
            print("There is only one file to commit, do you want to add it?(y/n)")
            if input() == "y":
                committing_files.append(git_log[0][3:])
                print("File added")
            adding_files = False
            break
        for i, line in enumerate(git_log):
            print(f"{i + 1}. {line[3:]}")
        input_number = input("Which file do you want to add to the commit? (0 to stop adding files) ")
        if input_number == "0" or input_number == "":
            adding_files = False
        else:
            try:
                input_number = int(input_number)
                if input_number <= 0 or input_number > len(git_log):
                    print("Invalid input")
                else:
                    committing_files.append(git_log[input_number - 1][3:])
                    git_log.pop(input_number - 1)
                    adding_files = len(git_log) != 0
            except ValueError:
                print("Invalid input")
    if len(committing_files) == 0:
        print("No files to commit")
        exit()
    commit_line = []
    for file in committing_files:
        os.system(f'git add {file}')
        letter = git_log[0][:2].strip()
        file = file[:-1]
        commit_line.append(f"\t{file}: {status_letters[letter]}")
    commit_type = input("What type of commit is this ? (default: [OTHER]) ")
    if commit_type == "":
        commit_type = "[OTHER]"
    else:
        commit_type = f"[{commit_type.upper()}]"
    commit_title = input("What is the title of the commit ? (default: giti default commit) ")
    if commit_title == "":
        commit_title = f"{commit_type} {len(committing_files)} {'file' if len(committing_files) <= 1 else 'files'}" \
                       f" changed"
    commit_comment = input("Do you want to add a comment to the commit ? (default: no comment) ")
    if commit_comment == "":
        commit_comment = "Committed without a comment"
    commit_message = f"{commit_type} {commit_title}\n" + "\n".join(commit_line) + f"\n\t{commit_comment}"
    os.system(f'git commit -m "{commit_message}"')


# def giti_header(potential_title):
#     selection = [".h", ".hpp", ".hxx", ".hh"]
#     log = os.popen('git status -s').readlines()
#     for line in log:
#         if not any(line.endswith(ext) for ext in selection):
#             log.remove(line)
#     if len(log) == 0:
#         print("There are no header files to commit")
#         exit()
#     commit_line = []
#     for line in log:
#         os.system(f'git add {line[3:]}')
#         letter = line[:2].strip()
#         commit_line.append(f"\t{line[3:]}: {status_letters[letter]}")
#     commit_message = f"[HEADER] {len(commit_line)} {'file' if len(commit_line) <= 1 else 'files'} changed\n" + \
#                      "\n".join(commit_line)
#     os.system(f'git commit -m "{commit_message}"')
