import os
import json

# raw changelog file address:
# https://raw.githubusercontent.com/MaximeLeBesnerais/giti_super/main/versions_changelog.json

script_path = os.path.dirname(os.path.realpath(__file__))


class GitiVersion:
    def __init__(self, version, changelog):
        self.major = version.split(".")[0]
        self.minor = version.split(".")[1]
        self.patch = version.split(".")[2]
        self.changelog = changelog

    def equals(self, other):
        return self.major == other.major and self.minor == other.minor and self.patch == other.patch

    def is_newer(self, other):
        if self.major > other.major:
            return True
        elif self.major == other.major:
            if self.minor > other.minor:
                return True
            elif self.minor == other.minor:
                if self.patch > other.patch:
                    return True
        return False

    def is_older(self, other):
        return not self.is_newer(other) and not self.equals(other)

    def changelog(self):
        return self.changelog

    def version(self):
        return f"{self.major}.{self.minor}.{self.patch}"


def giti_get_version():
    json_file = os.popen("~/.giti/versions_changelog.json").read()
    json_file = json.loads(json_file)
    return GitiVersion(json_file["version"], json_file["changelog"])


def proceed(none):
    if os.path.exists(f"{script_path}/update.sh"):
        os.system(f"{script_path}/update.sh")
        exit()
    print("The update script is missing at script location. Will now look in default location")
    if os.path.exists(f"{os.path.expanduser('~')}/.giti/update.sh"):
        os.system(f"{os.path.expanduser('~')}/.giti/update.sh")
        exit()


def giti_update():
    current_version = giti_get_version()
    print(f"Current version: {current_version.version()}")
    # get the latest version of giti from the GitHub repository
    json_file = os.popen(
        'curl https://raw.githubusercontent.com/MaximeLeBesnerais/giti_super/main/versions_changelog.json').read()
    json_file = json.loads(json_file)
    latest_version = GitiVersion(json_file["version"], json_file["changelog"])
    if current_version.is_newer(latest_version):
        print("You are using a newer version of GITI than the latest one")
        exit()
    if current_version.equals(latest_version):
        print("You are using the latest version of GITI")
        exit()
    print(f"Latest version: {latest_version.version()}")
    print("The old version of GITI will be deleted and the new one will be downloaded")
    proceed(None)
