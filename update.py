import os
import json
import datetime

# https://raw.githubusercontent.com/MaximeLeBesnerais/giti_super/main/versions_changelog.json

script_path = os.path.dirname(os.path.realpath(__file__))


def get_today():
    return datetime.datetime.now().strftime("%d/%m/%Y")


def time_diff(date1, date2):
    date1 = datetime.datetime.strptime(date1, "%d/%m/%Y")
    date2 = datetime.datetime.strptime(date2, "%d/%m/%Y")
    return (date2 - date1).days


class GitiVersion:
    def __init__(self, version, changelog, date):
        self.major = version.split(".")[0]
        self.minor = version.split(".")[1]
        self.patch = version.split(".")[2]
        self.changelog = changelog
        self.date = date

    def equals(self, other):
        return self.major == other.major and self.minor == other.minor and self.patch == other.patch

    def is_newer(self, other):
        here_properties = [self.major, self.minor, self.patch]
        other_properties = [other.major, other.minor, other.patch]
        for i in range(3):
            if here_properties[i] > other_properties[i]:
                return True
            if here_properties[i] < other_properties[i]:
                return False
        return False

    def is_older(self, other):
        return not self.is_newer(other) and not self.equals(other)

    def changelog(self):
        return self.changelog

    def version(self):
        return f"{self.major}.{self.minor}.{self.patch}"


def giti_get_version(json_file):
    return GitiVersion(json_file["version"], json_file["changelog"], json_file["date"])


def proceed():
    if os.path.exists(f"{script_path}/update.sh"):
        os.system(f"{script_path}/update.sh")
        exit()
    print("The update script is missing at script location. Will now look in default location")
    if os.path.exists(f"{os.path.expanduser('~')}/.giti/update.sh"):
        os.system(f"{os.path.expanduser('~')}/.giti/update.sh")
        exit()


def giti_update(silent=False):
    current_version = giti_get_version(json.loads(open(f"{script_path}/versions_changelog.json").read()))
    print(f"Current version: {current_version.version()}")
    json_file = os.popen(
        'curl -s https://raw.githubusercontent.com/MaximeLeBesnerais/giti_super/main/versions_changelog.json').read()
    json_file = json.loads(json_file)
    latest_version = GitiVersion(json_file["version"], json_file["changelog"], json_file["date"])
    if current_version.is_newer(latest_version):
        print("You are using a newer version of GITI than the latest one")
        exit()
    if current_version.equals(latest_version):
        print("You are using the latest version of GITI")
        exit()
    print(f"Latest version: {latest_version.version()}")
    print("The old version of GITI will be deleted and the new one will be downloaded")
    proceed()


def giti_version():
    current_version = giti_get_version(json.loads(open(f"{script_path}/versions_changelog.json").read()))
    print(f"Current version: {current_version.version()}")
    days_since_update = time_diff(current_version.date, get_today())
    txt = ""
    match days_since_update:
        case 0:
            txt = "today"
        case 1:
            txt = "yesterday"
        case _:
            txt = f"{days_since_update} days ago"
    print(f"Last updated {txt} ({current_version.date})")
    print(f"Changelog: {current_version.changelog}")
