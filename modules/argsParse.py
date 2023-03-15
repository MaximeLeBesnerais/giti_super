import argparse as ap
import textwrap


parser = ap.ArgumentParser(prog="giti", formatter_class=ap.RawTextHelpFormatter,
                           description=textwrap.dedent('''\
                           GITI: Interactive commit tool
                            ----------------------------
                            Made by: Bandana
                            Note: Using without arguments will open the interactive mode'''))

# Regular: giti <tag> [title & comment] <files...>
# special: giti <option>

parser.add_argument("tag", help="The tag of the commit", nargs="?")

s = parser.add_argument_group("Title & Comment")
m = s.add_mutually_exclusive_group()
m.add_argument("-nt", "--no-title", action="store_true", help="Use default title")
m.add_argument("-nc", "--no-comment", action="store_true", help="Use default comment")
m.add_argument("-nct", "-ntc", "--no-comment-title", action="store_true", help="Use default title and comment")

parser.add_argument("files", nargs="*", help="The files to commit")

# Special
special = ap.ArgumentParser(prog="giti", formatter_class=ap.RawTextHelpFormatter,
                           description=textwrap.dedent('''\
                           GITI: Interactive commit tool
                            ----------------------------
                            Made by: Bandana
                            Note: Using without arguments will open the interactive mode'''), add_help=False)

x = special.add_mutually_exclusive_group()
az = x.add_argument_group("Informations")
info = az.add_mutually_exclusive_group()
info.add_argument("-v", "--version", action="store_true", help="Show the version of giti")
info.add_argument("-u", "--update", action="store_true", help="Update giti")
info.add_argument("-U", "--force-update", action="store_true", help="Force update giti")
info.add_argument("-r", "--remove", action="store_true", help="Remove giti")

wk = x.add_argument_group("Tools")
tools = wk.add_mutually_exclusive_group() 
tools.add_argument("-a", "--all", action="store_true", help="Commit all files")
tools.add_argument("-i", "--igit", action="store_true", help="Commit .gitignore")
tools.add_argument("-d", "--deleted", action="store_true", help="Commit deleted files")
tools.add_argument("-l", "--header", action="store_true", help="Commit header files")
tools.add_argument("-m", "--make", action="store_true", help="Commit makefile")

def special_help():
    print('''\
GITI: Interactive commit tool
----------------------------
Made by: Bandana
Note: Using without arguments will open the interactive mode
                            
    Information:
    -v, --version: Show the version of giti
    -u, --update: Update giti
    -U, --force-update: Force update giti
    -r, --remove: Remove giti

    Tools:
    -a, --all: Commit all files
    -i, --igit: Commit .gitignore
    -d, --deleted: Commit deleted files
    -l, --header: Commit header files
    -m, --make: Commit makefile
        ''')

x.add_argument("--help2", action="store_true", help="Show the help of the special flags")

def multiple_true(*args):
    kwargs = vars(args[0])
    num = 0
    for i in kwargs:
        if kwargs[i] == True:
            num += 1
    if num > 1:
        print("Error: Multiple special flags")
        exit(1)
