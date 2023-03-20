import os
from .commit import make_commit
from .configs import print_error
from argparse import ArgumentParser as ap

def do_commit(tag, title, comment, files, f = "git status --porcelain"):
    if make_commit(tag, title, comment, files, f):
        print("Commit status: \033[92mSuccess\033[0m")
        exit(0)
    print("Commit status: \033[91mFailed\033[0m")
    exit(1)

def giti_all():
    git_log = os.popen('git status -s').readlines()
    if len(git_log) == 0:
        print("There are no changes to commit")
        exit()
    title = f"{len(git_log)} {'file' if len(git_log) <= 1 else 'files'} were committed"
    comment = "Committed all files"
    files = [line[3:-1] for line in git_log]
    do_commit("ALL", title, comment, files)

def giti_makefile():
    git_log = os.popen('git status -s').readlines()
    git_log = [line[3:-1] for line in git_log]
    makefiles = []
    for line in git_log:
        if "Makefile" in line:
            makefiles.append(line)
    if len(makefiles) == 0:
        print("There are no Makefiles to commit")
        exit()
    title = f"{len(makefiles)} {'file' if len(makefiles) <= 1 else 'files'} were committed"
    comment = "Committed Makefiles"
    do_commit("MAKE", title, comment, makefiles)
    


def giti_header():
    git_log = os.popen('git status -s').readlines()
    git_log = [line[3:-1] for line in git_log]
    header_extensions = [".h", ".hpp", ".hxx", ".hh"]
    headers = []
    for line in git_log:
        if any(line.endswith(ext) for ext in header_extensions):
            headers.append(line)
    if len(headers) == 0:
        print("There are no header files to commit")
        exit()
    title = f"{len(headers)} {'file' if len(headers) <= 1 else 'files'} were committed"
    comment = "Committed header files"
    do_commit("HEADER", title, comment, headers)


def giti_ignore():
    git_log = os.popen('git status --porcelain').readlines()
    git_log = [line[3:-1] for line in git_log]
    gitignore = []
    for line in git_log:
        if '.gitignore' in line:
            gitignore.append(line)
    if len(gitignore) == 0:
        print("There are no .gitignore files to commit")
        exit()
    title = f"Modified .gitignore file"
    do_commit("IGNORE", title, "", gitignore)


def giti_del():
    git_log = os.popen('git ls-files --deleted').readlines()
    git_log = [line[3:-1] for line in git_log]
    if len(git_log) == 0:
        print("There are no deleted files to commit")
        exit()
    title = f"{len(git_log)} {'file' if len(git_log) <= 1 else 'files'} were deleted"
    comment = "Committed files deletion"
    do_commit("DEL", title, comment, git_log, "git ls-files --deleted")


def generic_giti(tag_key, files:list, title = "", comment = ""):
    log = os.popen('git status -s').readlines()
    log = [line[3:-1] for line in log]
    for file in files:
        if file not in any(log):
            print(f"\033[91m{file}\033[0m is not a valid file")
            exit(1)
    title = f"{len(files)} {'file' if len(files) <= 1 else 'files'} were committed" if title == "" else title
    comment = "Committed files" if comment == "" else comment
    do_commit(tag_key, title, comment, files)

def power_action_picker(args: ap):
    if args.all:
        giti_all()
    if args.make:
        giti_makefile()
    if args.header:
        giti_header()
    if args.igit:
        giti_ignore()
    if args.deleted:
        giti_del()
    print_error("No valid action was found")
