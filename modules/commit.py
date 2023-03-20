import subprocess

letters = ["A", "M", "D", "R", "C", "U", "?", "!"]

git_dict = {"A": "Added", "M": "Modified", "D": "Deleted", "R": "Renamed", "C": "Copied", "U": "Updated", "?": "Untracked", "!": "Ignored"}

def associate(letters):
    text = ""
    if letters[0] == letters[1]:
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
    return f"{tab[1]}: {associate(tab[0])}"


class Commit:
    def __init__(self, tag, title, comment, files):
        self.tag: str = tag
        self.title: str = title
        self.comment: str = comment
        self.files: list = files
        self.valid : bool = False
        self.status = subprocess.run(["git", "status", "--porcelain"], stdout=subprocess.PIPE).stdout.decode("utf-8")
    
    def check_files(self):
        for file in self.files:
            if file not in self.status:
                self.valid = False
        self.valid = True
    
    def __str__(self):
        tag = f"[{self.tag.upper()}]"
        title = f"{self.title}"
        comment = f"{self.comment}"
        files = [file_line(file) for file in self.status.split() if file != ""]
        files_msg = f"Files:" + "\n\t\t".join(files)
        commit_msg = f"{tag} {title}\n{files_msg}\n\n{comment}"
        return commit_msg
    
    def commit(self):
        if self.valid:
            subprocess.run(["git", "add", self.files])
            subprocess.run(["git", "commit", "-m", self.__str__()])
            return True
        print(f"Could not proceed with this commit")
        print(f"Reason: A file is not in the git status")
        return False
    
def make_commit(tag, title, comment, files):
    commit = Commit(tag, title, comment, files)
    if commit.check_files():
        commit.commit()
        return True
    return False
