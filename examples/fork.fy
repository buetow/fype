#*
 * Examples of how to use fork
 *#

my pid = fork;

if pid {
	put "I am the parent process and the child has the pid ";
	say pid;
}

ifnot pid {
	say "I am the child process";
}
