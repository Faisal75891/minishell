*This project has been created as part of the 42 curriculum by fbaras, samamaev AND COPILOT*

# minishell 🐚

## what is this

basic shell clone, bash but make it baby mode.

## Instructions

```bash
make
./minishell
```
### To exit
```bash
exit or CTRL-D
```
### file structure

```
src/
├── main/           → entry point, main loop
├── parsing/        → lexer, parser, quote/expansion flex
├── execution/      → executor, pipes, redirects go brr
├── builtins/       → cd, echo, pwd, export, unset, env, exit
├── signals/        → Ctrl+C/D/\ handling
└── utils/          → PATH search, env vars, cleanup, history
include/            → minishell.h gang
libft/              → custom C library (ft_* functions)
```

### what actually works:
- basic main loop with `readline("$ ")`
- `t_shell` with:
  - `env` copied from `envp`
  - `last_status` tracked after every command/pipeline
- variable expansion:
  - `$VAR` from `env`
  - `$?` from `shell->last_status`
  - `$$` from `getpid()`
- argument handling:
  - simple split by spaces (no quotes yet, cope)
  - each arg goes through expansion
- execution:
  - builtins (`cd`, `echo`, `env`, etc.) bypass forks and run in the parent thread
  - external commands dynamically translate through `PATH` env and run safely via `execve`
  - pipelines via `run_pipeline` and `wait_all` (last command status bubbles up)
- exit status logic:
  - normal exit → code as is
  - killed by signal → `128 + signal` (bash style)
- Lexer:
  - Creates a linked list of command
  - tokenizes input into tokens or words
  - Each node has the type of token and the type of quote Which makes it easier to do variable expansions.
- Parser:
  - Parses the values inside the lexer.
  - It handles syntax errors in the command.
  - Commands are split into smaller parts.
  - Commands are split by the '|' delimiter
  - each command has ints own argv, redirections array.
  - e.g:
  - Input: Cat > out.txt | grep a >> EOF | ls
  - Will become 3 commands split as follow: 
  -     argv 1: ["cat", NULL], redirections: [">", "out.txt"]
  -     argv 2: ["grep", "a", NULL], redirections: ["<<", "EOF"]
  -     argv 3: ["ls", NULL], redirections: NULL
- New_executor :
  - This new executor uses the new parsed structure
  - It executes commands. typeshit.
  - It still needs work like error detection and memory leaks.

- New_main :
  - Inside it is a todo list of all the shortcomings of the program.
  - It uses new_executor and new parser.
