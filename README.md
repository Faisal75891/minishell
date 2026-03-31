# minishell 🐚

## what is this

basic shell clone, bash but make it baby mode. only runs commands rn. typeshit

## how to use

```bash
make
./m
exit        
```

## what's happening

### the vibe
- read input → parse it → **expand `$`** → fork it → execute it
- saves ur commands to `.history` (just logging for now)
- tries to find commands in `PATH` automatically (but PATH/env is a bit cooked rn)

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

### bugs squashed:
- ✅ memory leaks in `read_command()` 
- ✅ `fork()` with no error handling
- ✅ typo `.PHONEY` → `.PHONY`
- ✅ main loop, now using `t_shell` (env + last_status)

### big refactor:
- reorganized into clean folder structure (src/, include/, libft/)
- split code into modules (parsing, execution, builtins, utils)
- added env/utils helpers (`env_utils`, `path_utils`, `char_utils`, `str_utils`, `error_utils`)
- wired `$VAR`, `$?`, `$$` expansion into the arg parser

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
  - single commands go through `execute_command` (fork + execve + wait)
  - pipelines via `run_pipeline` and `wait_all` (last command status bubbles up)
- exit status logic:
  - normal exit → code as is
  - killed by signal → `128 + signal` (bash style)
- Lexer *(NEW)*:
  - Creates a linked list of command
  - tokenizes input into tokens or words
  - Each node has the type of token and the type of quote Which makes it easier to do variable expansions.
- Parser *(NEW)*:
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

## coming soon

- [ ] pipes `|` (chain commands fr fr) **– core already there, needs polish & testing**
- [ ] redirects `>`, `<`, `>>` (file I/O szn)
- [ ] builtin commands (`cd`, `echo`, `pwd`, `export`, `unset`, `env`, `exit`) **– files exist, logic WIP**
- [ ] quotes handling (single + double quotes)
- [ ] environment variable expansion (`$VAR`, `$?`) **– mostly done, edge-cases & quotes left**
- [ ] signals (Ctrl+C, Ctrl+D, Ctrl+\ no cap) **– handlers exist, disabled in main for now**
- [ ] fix PATH/env so `execve` stops malding on `ls`/`pwd`
- [ ] yeet debug spam once exec layer is stable

---

**status:** project restructured and clean af, parsing/expansion/executor pipeline mostly built, but env/PATH is half-baked so `execve` is still salty. it runs, but it’s trash
