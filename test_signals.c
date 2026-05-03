#include <stdio.h>
#include <signal.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <signal.h>
#include <termios.h>
#include <sys/wait.h>

static struct   termios g_old;
int last_status = 0;

char *strjoin(const char *s1, const char *s2)
{
    char *result = malloc(strlen(s1) + strlen(s2) + 1);
    
    if (result) {
        strcpy(result, s1);
        strcat(result, s2);
    }
    return result;
}


void    handle_quit_parent(int sig)
{
    if (isatty(STDIN_FILENO) && isatty(STDERR_FILENO))
    {
        write(1, "Interactive!\n", 14);
    }
    else
        write(1, "Not Interactive!\n", 18);
}

int main(int argc, char **argv, char **envp)
{
    // removing ^C shi
    struct  termios t;

    tcgetattr(STDIN_FILENO, &g_old);
    if (t.c_lflag & ECHO) {
        puts("ECHO is enabled.");
    } else {
        puts("ECHO is disabled.");
    }
    t = g_old;
    t.c_lflag &= ~ECHOCTL;
    tcsetattr(STDIN_FILENO, TCSANOW, &t);

    // sigaction shi
    sigset_t    s;

    sigemptyset(&s);
    sigaddset(&s, SIGINT);
    struct sigaction    sa_action = {0};
    sa_action.sa_handler = &handle_quit_parent;
    sa_action.sa_flags = SA_RESTART;
    sigaction(SIGINT, &sa_action, NULL);
    sigaction(SIGQUIT, &sa_action, NULL);

    if (argv[1])
    {
        char    *args[] = {argv[1], NULL};
        char    *cmd = strjoin("/bin/", argv[1]);
        pid_t pid = fork();
        if (!pid)
        {
            kill(getppid(), SIGINT);
            execve(cmd, args, envp);
            exit(127);
        }
    }
    write(1, "Parent process\n", 16);
    wait(NULL);
    write(1, "\n", 1);
    write(1, &last_status, 1);
    tcsetattr(STDIN_FILENO, TCSANOW, &g_old);
}
