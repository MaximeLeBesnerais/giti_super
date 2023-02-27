import os
from powered_g import status_letters


def parse_selection(selection: str, upper_limit) -> [int]:
    if selection == "":
        return []
    selection = selection.split()
    try:
        selection = [int(s) for s in selection]
    except ValueError:
        print("Selection must be a number")
        return []
    if max(selection) > upper_limit:
        print("Selection out of range")
        exit()
    return selection


def get_files_selection(file_list) -> [int]:
    selection: [int] = []
    adding = True
    while adding:
        print("Select a file to commit")
        for i, file in enumerate(file_list):
            print(f"\t{i + 1}. {file}")
        print("\t0. Done (or press enter)")
        selection_input = input("Selection: ")
        selection_input = parse_selection(selection_input, len(file_list))
        if len(selection_input) == 0:
            adding = False
        else:
            selection += selection_input
            for i in selection_input:
                file_list.remove(file_list[i - 1])
            if len(file_list) == 0:
                adding = False
                print("All files have been selected")
            if len(file_list) == 1:
                print("There is only one file left to select")
                print(f"Do you want to select {file_list[0]}? (y/n)")
                selection_input = input("Selection: ")
                if selection_input.lower() == "y":
                    selection.append(1)
                adding = False
    return selection


def giti_interactive():
    git_log = os.popen('git status -s').readlines()
    git_log = [line[:-1] for line in git_log]
    print(git_log)
    commit_line = []
    if len(git_log) == 0:
        print("There are no changes to commit")
        exit()
    if len(git_log) > 30:
        print("There are too many changes to commit with interactive mode")
        print("Please use the all mode instead, or individually commit files")
        exit()
    file_list = [line[3:] for line in git_log]
    selection = get_files_selection(file_list)
    if len(selection) == 0:
        print("There are no files to commit")
        exit()
    for i in selection:
        os.system(f'git add {git_log[i - 1][3:]}')
        letter = git_log[i - 1][:2].strip()
        commit_line.append(f"\t{git_log[i - 1][3:]}: {status_letters[letter]}")
    type_commit = input("What type of commit is this?")
    if type_commit == "":
        type_commit = "OTHER"
    title = input("Title: ")
    if title == "":
        title = f"[{type_commit.upper()}] For {len(selection)} {'file' if len(selection) == 1 else 'files'}"
    else:
        title = f"[{type_commit.upper()}] {title}"
    comment = input("Comment: ")
    if comment == "":
        comment = f"Committed without comment"
    commit_message = f"{title}\n" + "\n".join(commit_line) + f"\n\t\t{comment}"
    os.system(f'git commit -m "{commit_message}"')
