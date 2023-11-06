#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <fcntl.h>
#include <wait.h>
#include <string.h>

int main(int argc, char *argv[])
{
	if (argc <= 1)
	{
		printf("arguments should be at least 1\n");
		return -1;
	}

	pid_t *pids = (pid_t *)calloc(argc, sizeof(pid_t));

	/* output fd of the previous process, initialized to
	 * the stdout fd the parent process pointed to
	 */
	int previous_out_fd = dup(STDIN_FILENO);
	/* we should not close STDIN here otherwise the child will close it again */

	int fds[2];
	for (int i = 1; i < argc; i++)
	{
		if (pipe(fds) == -1)
		{
			perror("pipe create error");
			return errno;
		}
		int ret = fork();
		if (ret < 0)
		{
			perror("fork error");
			return errno;
		}
		else if (ret == 0) // child
		{
			pids[i] = getpid();

			/* assign the previous process's output fd to the stdin of the child process */
			dup2(previous_out_fd, STDIN_FILENO);
			close(previous_out_fd);

			close(fds[0]); // close read end

			/* if it is not the last process, assgin stdout of the child process
			 * to the write end of the pipe, which connects the next child process.
			 */
			if (i != argc - 1)
				dup2(fds[1], STDOUT_FILENO);
			close(fds[1]); // close write end

			if (execlp(argv[i], argv[i], (char *)NULL) == -1)
			{
				fprintf(stderr, "pipe: %s: %s\n", argv[i], strerror(errno));
				return errno;
			}
			return 0;
		}
		else // parent
		{
			/* save the read end, which is the input of the next process */
			previous_out_fd = fds[0];
			
			close(fds[1]); // close write end

			/* we should'nt close the read end's fd before the child duplicated it */
		}
	}

	int status;
	for (int i = 1; i < argc; i++)
	{
		waitpid(pids[i], &status, 0);
		int ret = WEXITSTATUS(status);
		if (ret != 0)
			return ret;
	}

	return 0;
}
