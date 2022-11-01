import os
import sys
from giti_powercharged import powercharged
import json

# create a dictionary of commands with the commands/commands.json file
commands_list = []
description = {}
command_input = {}
message = {}

os.popen('clear')
os.popen('git add .').readlines()
git_status = os.popen('git status -s').readlines()

def git_follow(file_name):
    global git_status
    for line in git_status:
        if line[3:] == file_name:
            return True
    return False


def load_commands():
    global commands_list, description, command_input, message
    file_data = json.load(open('commands/commands.json'))
    for entry in file_data:
        command_name = entry['command']
        if isinstance(command_name, list):
            for command in command_name:
                commands_list.append(command)
                description[command] = entry['description']
                command_input[command] = entry['input']
                message[command] = entry['message']
        else:
            commands_list.append(command_name)
            description[command_name] = entry['description']
            command_input[command_name] = entry['input']
            message[command_name] = entry['message']


def command_interpreter(command, file):
    print("received command: " + command)
    print("Evaluated file: " + file)
    if git_follow(file):
        print("File is interesting for git")
    else:
        print("Git doesn't care about this file")



def main(params):
    global commands_list, description, command_input, message

    if len(params) == 1 or params[1] == "powercharged":
        powercharged()
    elif params[1] in commands_list:
        if len(params) < 3:
            print("You need to specify a file")
            return
        command_interpreter(params[1], params[2])
    elif params[1] == 'help':
        print('''
        giti is a git commit interface that allows you to make commits without having to type them out.
        To use giti, type 'giti' followed by the command you want to run.
        The commands are:
        - powercharged (or none): a commit interface that allows you to make commits with extensive help.
        - help: displays this message.
        ''')
        x = 0
        for command in commands_list:
            print(f'''- {command}: {description[command]}''')
            x += 1
            if x == 6:
                x = 0
                input("----------")
    else:
        print("Command not found. Type 'giti help' for help.")


if __name__ == '__main__':
    load_commands()
    main(sys.argv)

