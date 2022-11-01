import os
import sys

from click import command
from giti_powercharged import powercharged
import json

# create a dictionary of commands with the commands/commands.json file
commands_list = []
description = {}

def load_commands():
    global commands_list
    file_data = json.load(open('commands/commands.json'))
    for entry in file_data:
        command_name = entry['command']
        if isinstance(command_name, list):
            for command in command_name:
                commands_list.append(command)
                description[command] = entry['description']
        else:
            commands_list.append(command_name)
            description[command_name] = entry['description']


def main(params):
    global commands_list, description
    if len(params) == 1 or params[1] == "powercharged":
        powercharged()
    elif params[1] in commands_list:
        print("received command: " + params[1])
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

