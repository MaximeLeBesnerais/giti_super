#!/usr/bin/python3
import os


def no_l_b(text_with_line_break):
    return text_with_line_break.replace("\n", "")


def powercharged():

    commit_message = ""

    menu = {1: "New Feature", 2: "Bug Fix", 3: "Documentations", 4: "Refactoring"}

    git_status = os.popen('git status -s').readlines()
    if len(git_status) == 0:
        print("There are no changes to commit")
        exit()
    for line in git_status:
        if line.startswith(' D') or line.startswith('D'):
            print("The file " + line[3:] + " was deleted")
            commit_message += no_l_b(line[3:]) + ": " + "\n"
            deleted_file_comment = input("Why did you delete it? ")
            commit_message += "     deletion: " + deleted_file_comment + " :fire:\n"
        else:
            print("for the file: " + line[3:] + " what did you do?")
            for key, value in menu.items():
                print(key, value)
            commit_message += no_l_b(line[3:]) + ": " + "\n"
            choice = str(input('What did you do? '))

            if choice == '1':
                feature_explication = input('What is the feature about? ')
                optional_feature_scope = input(
                    'What is the scope of the feature? (ie: lang) ')
                if optional_feature_scope != "":
                    optional_feature_scope = "(" + optional_feature_scope + ")"
                commit_message += "    feat" + optional_feature_scope + ": New feature: " + \
                    feature_explication + " :zap:\n"

            elif choice == '2':
                bug_fix = input('What bug was fixed ? ')
                commit_message += "    fix: " + bug_fix + " :bug:\n"

            elif choice == '3':
                commit_message += "    docs: - Documentation was altered - refer to commit title :memo:\n"

            elif choice == '4':
                commit_message += "    refactor: - Code was refactored - refer to commit title :recycle:\n"
            else:
                weird_comment = choice
                commit_message += "    chore: " + weird_comment + " :question:\n"
    commit_title = ":sparkles: " + input('What is the title of the commit? ')
    final_commit_message = commit_title + "\n" + commit_message
    os.wait()
    os.popen('git add .')
    os.wait()
    os.system('git commit -m "' + final_commit_message + '"')
    os.system('git pull')


if __name__ == "__main__":
    powercharged()
