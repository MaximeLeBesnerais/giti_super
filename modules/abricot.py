import json
import os
import shutil
import subprocess

import markdown


def get_errors(report_: dict):
    major = 0
    minor = 0
    info = 0
    for file_ in report_:
        for error_ in file_["errors"]:
            match error_["severity"]:
                case "MAJOR":
                    major += 1
                case "MINOR":
                    minor += 1
                case "INFO":
                    info += 1
                case _:
                    pass
    return major, minor, info


def get_file_errors(file_: dict):
    major = 0
    minor = 0
    info = 0
    for error_ in file_["errors"]:
        match error_["severity"]:
            case "MAJOR":
                major += 1
            case "MINOR":
                minor += 1
            case "INFO":
                info += 1
            case _:
                pass
    return major, minor, info


class CodeError:
    def __init__(self, error_: dict = None):
        if error_ is None:
            self.file = ""
            self.line = 0
            self.code = ""
            self.message = ""
            self.severity = ""
            return
        self.file = error_["file"]
        self.line = error_["line"]
        self.code = error_["code"]
        self.message = error_["message"]
        self.severity = error_["severity"]


class FileReport:
    def __init__(self, file_: dict):
        self.json = file_
        self.name = file_["meta"]["file"]
        self.errors = []
        self.number_of_errors = len(self.errors)
        err = get_file_errors(file_)
        self.major = err[0]
        self.minor = err[1]
        self.info = err[2]

    def parse(self):
        for error in self.json["errors"]:
            self.errors.append(CodeError(error))


class AbricotReport:
    def __init__(self, report_: dict):
        self.json = report_
        self.number_of_files = len(report_)
        err = get_errors(report_)
        self.major = err[0]
        self.minor = err[1]
        self.info = err[2]
        self.files = []

    def parse(self):
        for file_ in self.json:
            self.files.append(FileReport(file_))


def generate_file_summary(report_: AbricotReport):
    md = ""
    for file_ in report_.files:
        file_.parse()
        total_errors = file_.major + file_.minor + file_.info
        # write in bold the file name
        md += f"- **{file_.name}**: {total_errors} errors: {file_.major} major, {file_.minor} minor, {file_.info} " \
              f"info\n\n"
    return md


def markdown_abricot(report_: AbricotReport):
    report_.parse()
    total_errors = report_.major + report_.minor + report_.info
    md = f"# Abricot report\n\n"
    md += f"## Summary\n\n"
    md += f"Number of files: {report_.number_of_files}\n\n"
    md += f"{total_errors} errors:\n\n"
    md += f"- {report_.major} major\n\n"
    md += f"- {report_.minor} minor\n\n"
    md += f"- {report_.info} info\n\n"
    md += f"## Files summary\n\n"
    md += generate_file_summary(report_)
    md += f"## Files\n\n"
    for file_ in report_.files:
        md += f"### {file_.name}\n\n"
        md += f"Number of errors: {file_.major} major, {file_.minor} minor, {file_.info} info\n\n"
        md += f"#### Errors\n\n"
        for error_ in file_.errors:
            md += f"- {error_.severity} - {error_.code} - {error_.message} - {error_.file}:{error_.line}\n\n"
    return markdown.markdown(md)


# search for .git folder
def get_asc():
    if os.path.isdir(".git"):
        return os.getcwd()
    git_f = ""
    while not os.path.isdir(".git"):
        git_f = os.path.join("..", git_f)
        if git_f == "../":
            print("No git repository found.")
            exit(1)
    return git_f


def giti_abricot():
    if not os.path.isfile("/usr/local/bin/abricot"):
        print("abricot not found. Please install it.")
        exit(1)
    git_f = get_asc()
    output = subprocess.check_output(["abricot", "--ignore", "--format", "json"], cwd=git_f).decode("utf-8")
    report_ = json.loads(output)
    report_ = AbricotReport(report_)
    md_file = markdown_abricot(report_)
    error_total = report_.major + report_.minor + report_.info
    if error_total == 0:
        return
    with open("report.md", "w") as file_:
        file_.write(md_file)


if __name__ == "__main__":
    print(f"\033[91mFor test purposes only. Do NOT use as main script.\033[0m")
    giti_abricot()
    print(f"\033[92mDone.\033[0m")
