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
- ✅ typo `.PHONEY` → `.PHONY`
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
- Lexer *(NEW)*:
  - Creates a linked list of command
  - tokenizes input into tokens or words
  - Each node has the type of token and the type of quote Which makes it easier to do variable expansions.
  - NOTE: It is still not added to the code. Parsing will need to be redone to be compatible with the lexer.

## coming soon

- [ ] pipes `|` (chain commands fr fr) **– core already there, needs polish & testing**
- [ ] redirects `>`, `<`, `>>` (file I/O szn)
- [ ] quotes handling (single + double quotes)
- [ ] environment variable expansion (`$VAR`, `$?`) **– mostly done, edge-cases & quotes left**
- [ ] signals (Ctrl+C, Ctrl+D, Ctrl+\ no cap) **– handlers exist, disabled in main for now**
- [ ] yeet debug spam once exec layer is stable

---

**status:** project restructured and clean af, executor pipeline is getting un-cooked. builtins and external `PATH` mapping are fully online, so `ls` and `cd` actually do things now instead of segfaulting. execve is fixed but we still gotta lock in pipes & redirects to make it a real shell vibe.
