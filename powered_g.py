#!/usr/bin/python3
import os


def no_l_b(text_with_line_break):
    return text_with_line_break.replace("\n", "")

def powercharged():

    commit_message = ""

    menu = {1: "New Feature", 2: "Bug Fix", 3: "Documentations"}

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
                feature_name = input('What is the name of the feature? ')
                feature_explication = input('What is the feature about? ')
                optional_feature_scope = input(
                    'What is the scope of the feature? (ie: lang) ')
                if optional_feature_scope != "":
                    optional_feature_scope = "(" + optional_feature_scope + ")"
                commit_message += "    feat" + optional_feature_scope + ": New feature: " + \
                    feature_name + ": " + feature_explication + " :zap:\n"
            elif choice == '2':
                bug_origin = input('What is the origin of the bug? ')
                bug_fix = input('What did you do to fix it? ')
                commit_message += "    fix: " + bug_origin + ": " + bug_fix + ":bug:\n"
            elif choice == '3':
                documentation_summary = input('What did you document? ')
                commit_message += "    docs: Documentation: " + \
                    documentation_summary + " :memo:\n"
            else:
                weird_comment = choice
                commit_message += "    chore: " + weird_comment + " :question:\n"
    commit_title = ":sparkles: " + input('What is the title of the commit? ')
    final_commit_message = commit_title + "\n" + commit_message

    print("\n" + final_commit_message)
    commit_loop = True
    while commit_loop:
        commit_choice = input('Do you want to commit? (y/n) ')
        if commit_choice == 'y':
            os.wait()
            os.popen('git add .')
            os.wait()
            os.system('git commit -m "' + final_commit_message + '"')
            os.system('git pull')
            commit_loop = False
        elif commit_choice == 'n':
            commit_loop = False
            print('Commit aborted')
        else:
            print('Please enter y or n')

if __name__ == "__main__":
    powercharged()
