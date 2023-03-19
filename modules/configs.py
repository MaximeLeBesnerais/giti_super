import os
import json

script_path = os.path.dirname(os.path.realpath(__file__))
script_path = "/".join(script_path.split("/")[:-1])


def print_error(msg):
    print(f"\033[91mError: \033[0m{msg}")
    exit(1)


def extract_conf(json_data):
    active = json_data["Active"]
    report = json_data["Report"]
    hook = json_data["Hook"]
    summary = json_data["Summary"]
    return active, report, hook, summary


class Abricot:
    def __init__(self):
        self.active = True
        self.report = False
        self.Hook = False
        self.Summary = False

    def load(self, json_data):
        self.active, self.report, self.Hook, self.Summary = extract_conf(json_data)

    def report(self):
        return self.active and self.report

    def hook(self):
        return self.active and self.Hook

    def summary(self):
        return self.active and self.Summary

    def toggle(self):
        self.active = not self.active


class Unit:
    def __init__(self):
        self.active = False
        self.report = False
        self.Hook = False
        self.Summary = False

    def load(self, json_data):
        self.active, self.report, self.Hook, self.Summary = extract_conf(json_data)

    def report(self):
        return self.active and self.report


class Clang:
    def __init__(self):
        self.active = False

    def fetch_active(self, json_data):
        self.active = json_data["Active"]


class Config:
    def __init__(self):
        self.Abricot = Abricot()
        self.Unit = Unit()
        self.Clang = Clang()

    def load(self, json_data):
        self.Abricot.load(json_data["Abricot"])
        self.Unit.load(json_data["Unit"])
        self.Clang.fetch_active(json_data["Clang"])


def fetch_config():
    if os.path.isfile(".giti.json"):
        with open(".giti.json", "r") as f:
            json_data = json.load(f)
            config = Config()
            config.load(json_data)
        return config
    if os.path.isfile(f"{script_path}/jsons/.giti.json"):
        with open(f"{script_path}/jsons/.giti.json", "r") as f:
            json_data = json.load(f)
            config = Config()
            config.load(json_data)
        return config
    print("No config file found - Use 'giti --config' to create one")
    config = Config()
    return config
