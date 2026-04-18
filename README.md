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
- tries to find commands in `PATH` automatically (now hooked up properly no cap)

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
- ✅ main loop, now using `t_shell` (env + last_status)
- ✅ wired builtins to the executor so they actually run instead of getting eaten by `execve`
- ✅ fixed `PATH` resolution so `execve` stops malding and actually runs external commands like `ls`

### big refactor:
- reorganized into clean folder structure (src/, include/, libft/)
- split code into modules (parsing, execution, builtins, utils)
- added env/utils helpers (`env_utils`, `path_utils`, `char_utils`, `str_utils`, `error_utils`)
- wired `$VAR`, `$?`, `$$` expansion into the arg parser
- hooked up all builtins (`cd`, `pwd`, `echo`, `env`, `export`, `unset`, `exit`) natively inside the main shell process

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
- New_executor *(NEW)*:
  - This new executor uses the new parsed structure
  - It executes commands. typeshit.
  - It still needs work like error detection and memory leaks.

# What i Did:
- I added -g flag in the makefile for better debugging,
- I added free_parser() function.
- I fixed a minor bug in copy_env().
- NOTE: memory leaks are mostly from the executor not checking stuff.
- I Wired the new_executor all the way up to `handle_command` so the shell uses the new pipeline fully
- Pulled redirections `<` `>` `>>` setup off the bench and plugged into `new_executor`
- Deleted the `redirects.c` file entirely. `new_executor` is the only big boss now
- Re-tested quote matching `""` and ``, all good
- Wired `$VAR`, `$?`, `$$` into the parser flow too

## coming soon

- [ ] pipes `|` (chain commands fr fr) **– core already there, needs polish & testing**
- [x] environment variable expansion (`$VAR`, `$?`) **– mostly done, edge-cases & quotes left**
- [ ] signals (Ctrl+C, Ctrl+D, Ctrl+\ no cap) **– handlers exist, disabled in main for now**
- [ ] yeet debug spam once exec layer is stable
- [ ] wire built-ins `cd`, `echo`, `export`, etc. into `new_executor` bc old executor handles it rn!

---
---

**status:** project restructured and clean af. `new_executor` runs shell commands and file overrides like a real shell. old `executor.c` is just chilling carrying our builtin commands rn until we wrap that into the new file. But it does not mean that we are leaving the old executor js to be there.
Rn, we switched to new_executor.c, we bypassed execute_command() from executor.c entirely, so we need to copy that check over to new_executor.c so that our shell actually handles our custom functions (cd, pwd, echo, export, etc.) again!

--------------
js to understand what is happening, i need to follow the structure first and go one by one, spicially in terms of programming! I don't know why, but its completly the opposite if we talk about not proramming languages, even though - they are also laguages -> XD. This is my method of thinking, i can't get the whole idea, if i am not focused from the biggening.

