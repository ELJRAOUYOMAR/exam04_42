# include <stdio.h>
# include <unistd.h>
# include <stdlib.h>

int	ft_popen(const char *file, char *const argv[], char type)
{
    if (!file || !argv || (type != 'r' && type != 'w'))
        return -1;

    int fds[2];
    if (pipe(fds) == -1) return -1;

    if (type == 'w')
    {
        if (fork() == 0)
        {
            dup2(fds[0], 0);
            close(fds[0]);
            close(fds[1]);
            execvp(file, argv);
            exit(-1);
        }
        close(fds[0]);
        return fds[1];
    }
    else if (type == 'r')
    {
        if (fork() == 0)
        {
            dup2(fds[1], 1);
            close(fds[1]);
            close(fds[0]);
            execvp(file, argv);
            exit(-1);
        }
        close(fds[1]);
        return fds[0];
    }

    return -1;
}


char	*get_next_line(int fd)
{
	char	*ret = malloc(9999);
	char	c;
	int		i = 0;
	int		cpt = 0;

	if (fd < 0)
		return (NULL);
	while ((cpt = read(fd, &c, 1)) > 0)
	{
		ret[i] = c;
		i++;
		if (c == '\n')
			break ;
	}
	if (i == 0 || cpt < 0)
	{
		free(ret);
		return (NULL);
	}
	ret[i] = '\0';
	return (ret);
}


void ft_putstr(const char *s)
{
	if (!s)
		return;
	while (*s)
		write(STDOUT_FILENO, s++, 1);
}
int main() {
	int fd = ft_popen("ls", (char *const[]){"ls", NULL}, 'r');

	char *line;
	while(line = get_next_line(fd))
		ft_putstr(line);
}