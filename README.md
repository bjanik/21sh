# 21sh - UNIX Branch
by bjanik

Grade : 125/100

21sh is, after minishell, the second 42's shell project.

## Mandatory part :
          Line editing;
          Redirections : >, >>, >&, <, <<, <&;
          Pipes;
          Builtin : env, setenv, unsetenv, echo, cd
          
## Bonus features
          History search
          Builtin export and history
          Expansions
          Tokens AND_IF ('&&') and OR_IF ('||')
          
          
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

Shift + LEFT : Move the cursor to the end of the next word

Shift + RIGHT : Move the cursor to the beginning of the previous word

Ctrl + A : switch to selection/standard mode

Ctrl + F : copy the selected part of the input

Ctrl + E : Paste the copied buffer

Ctrl + X : Cut the selected part of the input


## Builtins

#### Setenv
          setenv [name=value] ...          
          If name exists, modify the value of name. Otherwise, appends a new variable name to the LOCAL environment.
          
#### Unsetenv          
          unsetenv [name] ...          
          Delete name from the environment.          
          If name does not exist, goes to the next argument.
          
#### Env
          env [-i] [name=value] ... [utility [argument]]                           
              [-u] [name] ... [utility [argument]]              
              If i option is set, invoke utility with exactly the environment specified by the arguments; the  inher-
              ited environment shall be ignored completely.
              If u option is set, invoke utility with a modified environment resulting of the deletion of [name] variables
              
#### Export
          export [-n] [name[=value]] ...          
          If n option is set, makes the GLOBAL environment variable [name] LOCAL environment variable.          
          Otherwise, if name exists, changes the value of name. If it was previously LOCAL, makes it GLOBAL.
          
 #### Cd
          cd [arg]          
          Changes the shell's working directory to arg. 
          
#### History
          history [-c]          
          If c option is set, deletes the current history.          
          Otherwise, display the numerotated current history.
          
### Echo
          echo [-n] [args]
          Display on standard output the arguments. If n option is not set, display an additional  newline.

## Expansion
#### Environment variables :
          if FOO=bar
          If not single-quoted, "$FOO" expands to "bar"

#### Exit status:
          If not single-quoted, "$?" expands to the last command exit status

#### Process ID:

          "$$" expands to the shell's PID
