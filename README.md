*This project has been created as part of the 42 curriculum by fbaras, samamaev*

# minishell 🐚

## Description

- Minishell is a 42 school team project to create a basic shell program in C. It implements redirections and pipes, as well as environment variable expansions and the cd, echo, env, exit, export, pwd and unset builtin commands.

- in short, it is a basic shell clone, bash but make it baby mode.

## Instructions
git clone the project
```bash
git@vogsphere.42abudhabi.ae:vogsphere/intra-uuid-cf989cf4-6bd0-4a1b-91f0-cac1c3f0d7bb-7399502-fbaras
```
to compile
```bash
make
```
to run the program
```bash
./minishell
```
To exit
```bash
type exit or press CTRL-D
```
### file structure

```
src/
├── main/
├── parsing/
├── execution/
├── builtins/
├── signals/
└── utils/
include/
libft/
```

## Some features
- variable expansion:
  - `$VAR` from `env`
  - `$?` from `shell->last_status`
  - `$$` from `getpid()`
- Lexer:
  - Creates a linked list of command
  - tokenizes input into operators or words
- Parser:
  - Parses the values inside the lexer.
  - It handles syntax errors in the command.
  - Commands are split into smaller parts.
  - Commands are split by the '|' delimiter
  - each command has ints own argv and redirections array.
  - e.g:
  - Input: Cat > out.txt | grep a >> EOF | ls
  - Will become 3 commands split as follow: 
  -     argv 1: ["cat", NULL], redirections: [">", "out.txt"]
  -     argv 2: ["grep", "a", NULL], redirections: ["<<", "EOF"]
  -     argv 3: ["ls", NULL], redirections: NULL

## Resources
  ### Youtube Links
  #### Core Dumped videos
  - https://www.youtube.com/watch?v=SwIPOf2YAgI
  - https://www.youtube.com/watch?v=m6WXrC9Mxzo&t=635s
  - https://www.youtube.com/watch?v=Y2mDwW2pMv4
  ### hhp3
  - https://www.youtube.com/watch?v=ubt-UjcQUYg
  - https://www.youtube.com/watch?v=ZjzMdsTWF0U&t=1411s
  ### Bash Manual
  - https://www.gnu.org/software/bash/manual/bash.html
- https://www.cyberciti.biz/faq/linux-bash-exit-status-set-exit-statusin-bash/

### AI Usage
  - AI was used in many ways in the duration of this project, For example, It was very helpful to pinpoint bugs and speed up the debugging process. In addition, Ai Assisted in big refactors and assisted in making changes in all the files.
  - Ai helped in providing ideas for the structure of the code, providing a feedback loop on how things should.
  - Ai Provided great resources and was a huge help in finding the right information.
  - Ai Helped in managing git branches and handling git merges without issues.
