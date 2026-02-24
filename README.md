# minishell 🐚

## what is this

basic shell clone, bash but make it baby mode. only runs commands rn.

## how to use

```bash
make
./m
exit        
```

## what's happening

### the vibe
- read input → parse it → fork it → execute it
- saves ur commands to `.history` (just logging for now)
- finds commands in PATH automatically

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

### big refactor:
- reorganized into clean folder structure (src/, include/, libft/)
- split code into modules (parsing, execution, builtins, utils)

### what actually works:
- basic commands (`ls`, `pwd`, `cat`, etc.)
- `exit` command 
- PATH lookup (finds ur binaries automatically)
- command history (logs to `.history` file)

## coming soon

- [ ] pipes `|` (chain commands fr fr)
- [ ] redirects `>`, `<`, `>>` (file I/O szn)
- [ ] builtin commands (`cd`, `echo`, `pwd`, `export`, `unset`, `env`, `exit`)
- [ ] quotes handling (single + double quotes)
- [ ] environment variable expansion (`$VAR`, `$?`)
- [ ] signals (Ctrl+C, Ctrl+D, Ctrl+\ no cap)

---

**status:** project restructured and clean af, ready to code the actual features 💀🔥
