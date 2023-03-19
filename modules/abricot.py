# [
#     {
#         "title": "\u2023 In File tests2.c",
#         "group": "tests2.c",
#         "meta": {
#             "file": "tests2.c"
#         },
#         "errors": [
#             {
#                 "file": "tests2.c",
#                 "line": 14,
#                 "code": "C-A3",
#                 "message": "Files must end with a line break",
#                 "severity": "INFO"
#             },
#             {
#                 "file": "tests2.c",
#                 "line": 10,
#                 "code": "C-F6",
#                 "message": "A function taking no parameters must take void as a parameter",
#                 "severity": "MAJOR"
#             },
#             {
#                 "file": "tests2.c",
#                 "line": 12,
#                 "code": "C-FN",
#                 "message": "This syscall is usually banned in projects",
#                 "severity": "MAJOR"
#             }
#         ]
#     },
#     {
#         "title": "\u2023 In File tests.c",
#         "group": "tests.c",
#         "meta": {
#             "file": "tests.c"
#         },
#         "errors": [
#             {
#                 "file": "tests.c",
#                 "line": 33,
#                 "code": "C-A3",
#                 "message": "Files must end with a line break",
#                 "severity": "INFO"
#             },
#             {
#                 "file": "tests.c",
#                 "line": 32,
#                 "code": "C-F4",
#                 "message": "The body of a function must not exceed 20 lines",
#                 "severity": "MAJOR"
#             },
#             {
#                 "file": "tests.c",
#                 "line": 31,
#                 "code": "C-F4",
#                 "message": "The body of a function must not exceed 20 lines",
#                 "severity": "MAJOR"
#             },
#             {
#                 "file": "tests.c",
#                 "line": 30,
#                 "code": "C-F4",
#                 "message": "The body of a function must not exceed 20 lines",
#                 "severity": "MAJOR"
#             },
#             {
#                 "file": "tests.c",
#                 "line": 29,
#                 "code": "C-F4",
#                 "message": "The body of a function must not exceed 20 lines",
#                 "severity": "MAJOR"
#             },
#             {
#                 "file": "tests.c",
#                 "line": 28,
#                 "code": "C-F4",
#                 "message": "The body of a function must not exceed 20 lines",
#                 "severity": "MAJOR"
#             },
#             {
#                 "file": "tests.c",
#                 "line": 27,
#                 "code": "C-F4",
#                 "message": "The body of a function must not exceed 20 lines",
#                 "severity": "MAJOR"
#             },
#             {
#                 "file": "tests.c",
#                 "line": 26,
#                 "code": "C-F4",
#                 "message": "The body of a function must not exceed 20 lines",
#                 "severity": "MAJOR"
#             },
#             {
#                 "file": "tests.c",
#                 "line": 25,
#                 "code": "C-F4",
#                 "message": "The body of a function must not exceed 20 lines",
#                 "severity": "MAJOR"
#             },
#             {
#                 "file": "tests.c",
#                 "line": 24,
#                 "code": "C-F4",
#                 "message": "The body of a function must not exceed 20 lines",
#                 "severity": "MAJOR"
#             },
#             {
#                 "file": "tests.c",
#                 "line": 1,
#                 "code": "C-G1",
#                 "message": "Files must always start with the standard header",
#                 "severity": "MINOR"
#             },
#             {
#                 "file": "tests.c",
#                 "line": 1,
#                 "code": "C-G8",
#                 "message": "No empty lines must be present",
#                 "severity": "MINOR"
#             }
#         ]
#     },
#     {
#         "title": "\u2023 In File tests3.h",
#         "group": "tests3.h",
#         "meta": {
#             "file": "tests3.h"
#         },
#         "errors": [
#             {
#                 "file": "tests3.h",
#                 "line": 9,
#                 "code": "C-G3",
#                 "message": "The preprocessor directives must be indented",
#                 "severity": "MINOR"
#             },
#             {
#                 "file": "tests3.h",
#                 "line": 10,
#                 "code": "C-G3",
#                 "message": "The preprocessor directives must be indented",
#                 "severity": "MINOR"
#             }
#         ]
#     }
# ]
# To be parsed by the plugin

import json


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


if __name__ == "__main__":
    # print in red: \033[91m
    print(f"\033[91mFor test purposes only. Do NOT use as main script.\033[0m")
    with open("report.json", "r") as f:
        report = json.load(f)
    report = AbricotReport(report)
    report.parse()
    for file in report.files:
        print(f"File: {file.name} - {file.number_of_errors} errors: {file.major} major, {file.minor} minor, {file.info} info")
        file.parse()
        for error in file.errors:
            print(f"\t{error.severity} - {error.code} - {error.message} - {error.file}:{error.line}")
