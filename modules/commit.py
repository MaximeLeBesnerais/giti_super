from .configs import fetch_config
import subprocess
from .abricot import giti_abricot

letters = ["A", "M", "D", "R", "C", "U", "?", "!", "T"]

git_dict = {"A": "Added", "M": "Modified", "D": "Deleted", "R": "Renamed", "C": "Copied", "U": "Updated", "?": "Untracked", "!": "Ignored", "T": "Type change"}

config = fetch_config()


def associate(letters):
    text = ""
    if len(letters) > 1 and letters[0] == letters[1]:
        if letters[0] in letters:
            return git_dict[letters[0]]
        return "Unknown"
    for i in range(len(letters)):
        if i != len(letters) - 1:
                text += " & "
        if letters[i] in letters:
            text += git_dict[letters[i]]
            continue
        text += "Unknown"
    return text


def file_line(log_line):
    tab = log_line.split(" ")
    try:
        text = f"{tab[1]}: {associate(tab[0])}"
    except IndexError:
        print(f"Error: {log_line}")
        print(f"Error: {tab}")
        exit(84)
    return text


def array_accept(array, values):
    new_array = []
    for i in range(len(array)):
        if any (value in array[i] for value in values):
            new_array.append(array[i])
    return new_array


class Commit:
    def __init__(self, tag, title, comment, files,
                 status = "git status --porcelain"):
        self.tag: str = tag
        self.title: str = title
        self.comment: str = comment
        self.files: list = files
        self.valid : bool = False
        self.status = subprocess.run(status.split(), stdout=subprocess.PIPE).stdout.decode("utf-8")
    
    def __str__(self):
        tag = f"[{self.tag.upper()}]"
        if self.title != "":
            title = f"{self.title}"
        else:
            title = f"{len(self.files)} {'file' if len(self.files) <= 1 else 'files'} changed"
        array = self.status.splitlines()
        array = array_accept(array, self.files)
        files = [file_line(line.strip()) for line in array]
        files_msg = f"\tFiles:\n\t\t" + "\n\t\t".join(files)
        if self.comment != "":
            comment = f"\tComment:\n\t\t{self.comment}"
        else:
            comment = "\tComment:\n\t\tCommit made without comment"
        commit_msg = f"{tag} {title}\n{files_msg}\n\n{comment}"
        return commit_msg
    
    def commit(self):
        if self.valid:
            array = ["git", "add"]
            array.extend(self.files)
            subprocess.run(array)
            subprocess.run(["git", "commit", f"-m {self}"])
            return True
        print(f"Could not proceed with this commit")
        print(f"Reason: A file is not in the git status")
        return False


def make_commit(tag, title, comment, files, status = "git status --porcelain"):
    commit = Commit(tag, title, comment, files, status)
    commit.valid = True
    commit.commit()
    return True


def abricoting_exiting(abricot = True):
    if config.Abricot.active and abricot:
        giti_abricot()
    exit()
