#include "terminal.hh"
#include "_components.hh"
#include "iohandle.hh"

void Terminal::Run() {
	IOHandle::Exit();
	printf("\e[2J \e[1;1f");
	printf("yedit has been suspended to the terminal\ntype \"exit\" to return to yedit\n");
	
	char* shell  = getenv("SHELL");
	char* argv[] = {shell, NULL};

	pid_t pid = fork();
	if (pid == 0) {
		execvp(shell, argv);
	}
	else if (pid > 0) {
		int status;
		pid = wait(&status);
	}

	IOHandle::Init();
}