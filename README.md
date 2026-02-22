# minishell 🐚

## what is this

basic shell clone, bash but make it baby mode. only runs commands rn.

## how to use

```bash
make        # build it
./m         # run it
exit        # peace out
```

## what's happening

### the vibe
- read input → parse it → fork it → execute it
- saves ur commands to `.history` (just logging for now)
- finds commands in PATH automatically

### file structure

```
minishell.c      → main loop, fork/execve magic
read_commands.c  → parsing commands, reading input
utils.c          → helper functions (PATH search, memory cleanup)
history.c        → command history dump
```

## what got fixed

### bugs squashed:
- ✅ memory leaks in `read_command()` 
- ✅ `fork()` with no error handling -> low IQ move
- ✅ typo `.PHONEY` → `.PHONY`

### refactor for norminette (≤25 lines):
- split `main()` into 3 functions
- broke down `read_command()` (extracted `is_history_key()`)
- moved `get_paths()` and `get_full_path()` to `utils.c`

### what actually works:
- basic commands (`ls`, `pwd`, `cat` etc)
- `exit` command
- PATH lookup
- history (literally just a log file)

## todo later

- [ ] history navigation (up/down arrows)
- [ ] pipes `|`
- [ ] redirects `>`, `<`, `>>`
- [ ] builtin commands (`cd`, `export`, `env`)
- [ ] quotes and escaping

---

**status:** base version slaps, ready to build on 💀💀💀
