import os

# for each line in git status -s: ask the user what he did between 4 choices
# 1. new feature
# 2. bug fix
# 3. refactoring
# 4. Code Cleanup
# 5. Other

# if the user choose 1, ask him for the name of the feature
# if the user choose 2, ask him for the origin of the bug and what he did to fix it
# if the user choose 3, ask him for the scope of the refactoring
# if the user choose 4, ask him for the scope of the code cleanup
# if the user choose 5, ask him precisely what he did

commit_message = ""


def no_l_b(text_with_line_break):
    return text_with_line_break.replace("\n", "")


menu = {1: "New Feature", 2: "Bug Fix",
        3: "Refactoring", 4: "Code Cleanup", 5: "Other", 6: "Delete", 7: "Documentation", 8: "Configuration"}
# if it exists, use gitignore file to ignore files
if os.path.exists(".gitignore"):
    with open(".gitignore", "r") as f:
        gitignore = f.readlines()

git_status = os.popen('git status -s').readlines()
for line in git_status:
    # ignore file in gitignore
    if line[3:] in gitignore:
        continue
    else:
        print("for the file: " + line[3:] + " what did you do?")
        for key, value in menu.items():
            print(key, value)
        commit_message += no_l_b(line[3:]) + ": " + "\n"
        choice = input('What did you do? ')
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
            refactoring_scope = input('What is the scope of the refactoring? ')
            commit_message += "    refactor: " + refactoring_scope + " :recycle:\n"
        elif choice == '4':
            code_cleanup_scope = input(
                'What is the scope of the code cleanup? ')
            code_cleanup_details = input('What did you do to clean the code? ')
            commit_message += "    style: :wastebasket: Code cleanup: " + \
                code_cleanup_scope + ": " + code_cleanup_details + " :art:\n"
        elif choice == '5':
            other_summary = input(
                'What was the summary of your other changes? ')
            commit_message += "    chore: Other: " + \
                other_summary + " :adhesive_bandage:\n"
        elif choice == '6':
            deletion_reason = input('Why did you delete this file? ')
            commit_message += "    chore: Deletion: " + deletion_reason + " :fire:\n"
        elif choice == '7':
            documentation_summary = input('What did you document? ')
            commit_message += "    docs: Documentation: " + \
                documentation_summary + " :books:\n"
        elif choice == '8':
            config_name = input('What configuration did you change / add ? ')
            config_utility = input('What is the utility of this configuration? ')
            commit_message += "    chore: Configuration: " + config_name + ": " + \
                config_utility + " :wrench:\n"

commit_title = ":sparkles: " + input('What is the title of the commit? ')
final_commit_message = commit_title + "\n" + commit_message

# print the final commit message and ask the user if he wants to commit
print("\n" + final_commit_message)
commit_loop = True
while commit_loop:
    commit_choice = input('Do you want to commit? (y/n) ')
    if commit_choice == 'y':
        os.system('git add .')
        os.system('git commit -m "' + final_commit_message + '"')
        os.system('git pull')
        commit_loop = False
    elif commit_choice == 'n':
        commit_loop = False
        print('Commit aborted')
    else:
        print('Please enter y or n')
