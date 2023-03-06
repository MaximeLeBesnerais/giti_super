import os
import json
import datetime
import urllib.request

# https://raw.githubusercontent.com/MaximeLeBesnerais/giti_super/main/versions_changelog.json

script_path = os.path.dirname(os.path.realpath(__file__))


def connect(host="https://google.com"):
    try:
        urllib.request.urlopen(host)
        return True
    except urllib.error.URLError:
        return False


def get_today():
    return datetime.datetime.now().strftime("%d/%m/%Y")


def time_diff(date1, date2):
    date1 = datetime.datetime.strptime(date1, "%d/%m/%Y")
    date2 = datetime.datetime.strptime(date2, "%d/%m/%Y")
    return (date2 - date1).days


class GitiVersion:
    def __init__(self, version, changelog, date, last_update=None):
        self.major = version.split(".")[0]
        self.minor = version.split(".")[1]
        self.patch = version.split(".")[2]
        self.small_patch = "0"
        try:
            self.small_patch = version.split(".")[3]
        except IndexError:
            pass
        self.changelog = changelog
        self.last_update = last_update
        self.date = date

    def equals(self, other):
        return self.major == other.major and self.minor == other.minor and self.patch == other.patch

    def is_newer(self, other):
        here_properties = [self.major, self.minor, self.patch]
        other_properties = [other.major, other.minor, other.patch]
        for i in range(3):
            if here_properties[i] <= other_properties[i]:
                return False
        return True

    def is_older(self, other):
        here_properties = [self.major, self.minor, self.patch, self.small_patch]
        other_properties = [other.major, other.minor, other.patch, other.small_patch]
        for i in range(3):
            if here_properties[i] >= other_properties[i]:
                return False
        return True

    def changelog(self):
        return self.changelog

    def version(self):
        return f"{self.major}.{self.minor}.{self.patch}.{self.small_patch}"


def giti_get_version(json_file):
    return GitiVersion(json_file["version"], json_file["changelog"], json_file["date"], json_file["LUC"])


def proceed():
    if os.path.exists(f"{script_path}/update.sh"):
        os.system(f"{script_path}/update.sh")
        exit()
    print("The update script is missing at script location. Will now look in default location")
    if os.path.exists(f"{os.path.expanduser('~')}/.giti/update.sh"):
        os.system(f"{os.path.expanduser('~')}/.giti/update.sh")
        exit()
    print("The update script is missing at default location. Please contact the developer to fix this issue")


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


def giti_remove():
    if os.path.exists(f"{script_path}/remove.sh"):
        os.system(f"rm {script_path}/remove.sh")
        exit()
    print("The update script is missing at script location. Will now look in default location")
    if os.path.exists(f"{os.path.expanduser('~')}/.giti/remove.sh"):
        os.system(f"rm {os.path.expanduser('~')}/.giti/remove.sh")
        exit()
    print("The remove script is missing at default location. Please remove the folder ~/.giti manually")


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
    print(f"Last update check: {current_version.last_update}")
    giti_update_necessity()


def giti_update_necessity():
    current_version = giti_get_version(json.loads(open(f"{script_path}/versions_changelog.json").read()))
    last_update = current_version.last_update
    if last_update != "None":
        days_since_update = time_diff(last_update, get_today())
        if days_since_update < 3 or not connect():
            return

    json_file = os.popen(
        'curl -s https://raw.githubusercontent.com/MaximeLeBesnerais/giti_super/main/versions_changelog.json').read()
    json_file = json.loads(json_file)
    latest_version = GitiVersion(json_file["version"], json_file["changelog"], json_file["date"])
    if current_version.is_newer(latest_version) or current_version.equals(latest_version):
        json_file["LUC"] = get_today()
        with open(f"{script_path}/versions_changelog.json", "w") as f:
            f.write(json.dumps(json_file))
        return
    print(f"A new version of GITI is available ({latest_version.version()})")
    json_file["LUC"] = get_today()
    with open(f"{script_path}/versions_changelog.json", "w") as f:
        f.write(json.dumps(json_file))


def is_git():
    path = os.getcwd()
    valid = False
    while True:
        if os.path.exists(f"{path}/.git"):
            valid = True
            break
        if path == "/":
            break
        path = os.path.dirname(path)
    if not valid:
        print("Giti can only be used in a git repository")
        exit()
