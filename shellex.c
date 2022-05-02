/******************************************************
 * 
 * Author: Vivian Vu
 *
 * Project 3 - Simple Shell
 * Project Description   : Create your own simple shell
 *                         with basic functionality.
 * Date Modified         : 11/24/20
 * Class                 : CMSC257 - Sonmez
 *
 * ****************************************************/

/* $begin shellmain */
#include "csapp.h"
#define MAXARGS   128

/* Function prototypes */
void eval(char *cmdline);
int parseline(char *buf, char **argv);
int builtin_command(char **argv); 

/* COMMENTS: function for signal call */
void flush (int signal) 
{
	fflush(stdout);
	return;
}

/* COMMENTS: printf statements for help messages */
void help ()
{
	printf("\nDeveloper: Vivian Vu\n\n");
	printf("TO CHANGE SHELL PROMPT:\n");
	printf("   ./my257sh -p <prompt>\n\n");
	printf("BUILT-IN COMMANDS\n");   
	printf("   pid\n   ppid\n   cd\n   cd <path>\n   exit\n   help\n\n");
	printf("ADDITIONAL HELP\n");
	printf("   man\n\n");	
}

/* COMMENTS: main method taking in command line arguments */
int main(int argc, char *argv[]) 
{
	char cmdline[MAXLINE]; /* Command line */
	signal(SIGINT, flush);
	while (1) {
		/* Read */	
		if (argc > 2) { 
			printf("%s> ", argv[2]);
		}
		else {
			printf("my257sh> ");
		}                 
		Fgets(cmdline, MAXLINE, stdin); 
		if (feof(stdin))
			exit(0);

		/* Evaluate */
		eval(cmdline);
	} 
}
/* $end shellmain */

/* $begin eval */
/* eval - Evaluate a command line */
void eval(char *cmdline) 
{
	char *argv[MAXARGS]; /* Argument list execve() */
	char buf[MAXLINE];   /* Holds modified command line */
	int bg;              /* Should the job run in bg or fg? */
	pid_t pid;           /* Process id */

	strcpy(buf, cmdline);
	bg = parseline(buf, argv); 
	if (argv[0] == NULL)  
		return;   /* Ignore empty lines */

	if (!builtin_command(argv)) { 
		if ((pid = Fork()) == 0) {   /* Child runs user job */
			if (execvp(argv[0], argv) < 0) {
				printf("Execution failed (in fork)\n: No such file or directory\n");
				exit(1);
			}
		}

		/* Parent waits for foreground job to terminate */
		if (!bg) {
			/* COMMENTS: declare int status and exit variables for child exit status */
			int status;
			int exit;

			if (waitpid(pid, &status, 0) < 0)
				unix_error("waitfg: waitpid error");
		
			/* COMMENTS: print the exit status after WIFEXITED status */
			waitpid(pid, &status, 0);
					if (WIFEXITED(status)) {
				exit = WEXITSTATUS(status);
				printf("Child exit status: %d\n", exit);
			}
			else
				printf("%d %s", pid, cmdline);
		}
		return;
	}
}

/* If first arg is a builtin command, run it and return true */
int builtin_command(char **argv) 
{
	/* COMMENTS: exit command*/
	if (!strcmp(argv[0], "exit")) { /* quit command */
		int up = raise(SIGTERM);
		if (up != 0) {
			exit(0);
		}
	}
	/* COMMENTS: pid command */
	if (!strcmp(argv[0], "pid")) {
		printf("%d\n", getpid());
		return 1;
	}
	/* COMMENTS: ppid command */
	if (!strcmp(argv[0], "ppid")) {
		printf("%d\n", getppid());
		return 1;
	}
	/* COMMENTS: help command w/ help function */
	if (!strcmp(argv[0], "help")) {
		help();
		return 1;
	}
	/* COMMENTS: change directory command including path */
	if (!strcmp(argv[0], "cd")) {
		char path[100];
		if (argv[1]) {
			chdir(argv[1]);
			return 1;
		}
		else {
			printf("%s\n", getcwd(path, sizeof(path)));
			return 1;
		}
	}
	if (!strcmp(argv[0], "&")) {   /* Ignore singleton & */
		return 1;
	}
	return 0;                     /* Not a builtin command */

}
/* $end eval */

/* $begin parseline */
/* parseline - Parse the command line and build the argv array */
int parseline(char *buf, char **argv) 
{
	char *delim;         /* Points to first space delimiter */
	int argc;            /* Number of args */
	int bg;              /* Background job? */

	buf[strlen(buf)-1] = ' ';  /* Replace trailing '\n' with space */
	while (*buf && (*buf == ' ')) /* Ignore leading spaces */
		buf++;

	/* Build the argv list */
	argc = 0;
	while ((delim = strchr(buf, ' '))) {
		argv[argc++] = buf;
		*delim = '\0';
		buf = delim + 1;
		while (*buf && (*buf == ' ')) /* Ignore spaces */
			buf++;
	}
	argv[argc] = NULL;

	if (argc == 0)  /* Ignore blank line */
		return 1;

	/* Should the job run in the background? */
	if ((bg = (*argv[argc-1] == '&')) != 0)
		argv[--argc] = NULL;

	return bg;
}
/* $end parseline */
