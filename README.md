# OS-Lab-MyShell

## Description
myshell - a command line interpreter designed to interface with the UNIX Operating System

## Copyright

## Synopsis

[myshell] [options] [command_string | file]

# Options
These non-character commands refer to the options as indicated within the [Synopsis](##Synopsis) can be used when the shell environment has been invoked:

| Internal Command  | Description |
| ------------- | ------------- |
| cd <directory>  | Change the current default directory to <directory>. If the <directory> argument is not present, report the current directory. If the directory does not exist an appropriate error message will be reported  |
| clr  | Clear the screen  |
| dir <directory> | List the contents of directory <directory>  |
| environ | List all the environment strings  |
| echo <comment>  | Display <comment> on the display followed by a new line (multiple spaces/tabs may be reduced to a single space) |
| help  | Display the user manual using the more filter |
| pause | Pause operation of the shell until 'Enter' is pressed |
| quit  | Quit the shell  |

## I/O Redirection
myshell supports i/o-redirection on either or both stdin and/or stdout. i.e.:
<br> ```programname arg1 arg2 < inputfile > outputfile```<br>
With output redirection, if the redirection character is `>` then the outputfile is created if it does not exist and truncated if it does. If the redirection token is `>>` then outputfile is created if it does not exist and appended to if it does.

### Redirection Symbols
These redirection symbols are used in relation to the command line arguments:
| Symbol  | Purpose |
| ------------- | ------------- |
| `<` |  read file |
| `>` | file created/truncated if file exists |
| `&` | background execution  |
| `>>` | create file/append to it |
