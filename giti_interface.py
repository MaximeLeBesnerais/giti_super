import os
import sys
from giti_powercharged import powercharged

def main(params):
    # if params[1] is null or equals to "powercharged":
    if len(params) == 1 or params[1] == "powercharged":
        powercharged()
    elif params[1] == 'interface':
        pass
    elif params[1] == 'help':
        print('''
        giti is a git commit interface that allows you to make commits without having to type them out.
        To use giti, type 'giti' followed by the command you want to run.
        The commands are:
        - powercharged (or none): a commit interface that allows you to make commits with extensive help.
        - feat [file_name]: a commit interface that allows you to make commits for new features
        - bug [file_name]: a commit interface that allows you to make commits for bug fixes
        - refactor [file_name]: a commit interface that allows you to make commits for refactoring
        - style [file_name]: a commit interface that allows you to make commits for code cleanup
        - chore [file_name]: a commit interface that allows you to make commits for other changes
        - docs [file_name]: a commit interface that allows you to make commits for documentation
        - config [file_name]: a commit interface that allows you to make commits for configuration changes
        - delete [file_name]: a commit interface that allows you to make commits for file deletions
        - help: displays this message.
        ''')
    else:
        print("Command not found. Type 'giti help' for help.")


if __name__ == '__main__':
    main(sys.argv)

