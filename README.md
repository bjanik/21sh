# 21sh - UNIX Branch
by bjanik

Grade : 125/100

21sh is the second 42's shell project.

## Mandatory part :
          Line editing;
          Redirections : >, >>, >&, <, <<, <&;
          Pipes;
          Builtin : env, setenv, unsetenv, echo, cd
          
 ## Line editing
 
UP: Move up in command history

DOWN: Move down in command history

RIGHT: Move cursor to the right (standard AND selection mode)

LEFT: Move cursor to the left (standard AND selection mode)

HOME : cursor goes at the beginnig of the line

END : cursor goes at the end of the line

Ctrl + U : Delete the line before the cursor

Ctrl + L : Clear terminal without deleting the line

Ctrl + R : Switch to History Search

Shift + UP : Move the cursor one line up

Shift + DOWN : Move the cursor one line down

Shift + LEFT : Move the cursor to the end of the previous word

Shift + RIGHT : Move the cursor to the beginning of the next word

Ctrl + A : switch to selection/standard mode

Ctrl + F : copy the selected part of the input

Ctrl + E : Paste the copied buffer

Ctrl + X : Cut the selected part of the input


## Expansion

Environment variables :
if FOO=bar
$FOO expands to bar

Exit status:
$? expands to the last command exit status

Process id:
$$ expands to the shell's PID


