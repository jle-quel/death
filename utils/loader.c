#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <stdio.h>
#include <sys/mman.h>
#include <fcntl.h>
#include <stdlib.h>
#include <elf.h>

#define ENT_ADDR 0x1145
#define FOO_ADDR 0x1192
#define BAR_ADDR 0x11df
#define EXI_ADDR 0x11f2

#define ENT_SIZE FOO_ADDR - ENT_ADDR
#define FOO_SIZE BAR_ADDR - FOO_ADDR
#define BAR_SIZE EXI_ADDR - BAR_ADDR

char *ptr;

////////////////////////////////////////////////////////////////////////////////
/// STATIC FUNCTION
////////////////////////////////////////////////////////////////////////////////

void segment_write(char *ptr)
{
	const Elf64_Ehdr *header = (const Elf64_Ehdr *)ptr;
	Elf64_Phdr *segment;

	for (Elf64_Half index = 0; index < header->e_phnum; index++)
	{
		segment = (Elf64_Phdr *)((void *)ptr + sizeof(Elf64_Ehdr) + (sizeof(Elf64_Phdr) * index));

		segment->p_flags |= PF_W;
	}
}

void decrypt(const char *caller, char *callee, const size_t caller_size, const size_t callee_size, size_t n)
{
	unsigned int key = 0;
	size_t junk[] = {13, 23, -23, -13};

	for (size_t index = 0; index < caller_size; index++)
	{
		key += caller[index];
		junk[index % 4] = index ^ key;
		junk[index % 4] ^= junk[index % 4] + 1;
		key += caller[index] ^ junk[index % 4];
	}

	if (n)
		decrypt(ptr + FOO_ADDR, ptr + BAR_ADDR, FOO_SIZE, BAR_SIZE, n - 1);

	for (register size_t index = 0; index < callee_size; index++)
	{
		junk[index % 4] <<= 2;
		callee[index] ^= (key / junk[index % 4]);
		junk[index % 4] >>= 1;
	}
}


////////////////////////////////////////////////////////////////////////////////
/// PUBLIC FUNCTION
////////////////////////////////////////////////////////////////////////////////

int main(void)
{
	char *filename = "../war";
	int fd;
	struct stat statbuf;

	if ((fd = open(filename, O_RDWR)) < 0)
	{
		perror("open");
		exit(0);
	}
	if (stat(filename, &statbuf) < 0)
	{
		perror("stat");
		exit(0);
	}
	if ((ptr = mmap(NULL, statbuf.st_size, PROT_READ | PROT_WRITE, MAP_PRIVATE, fd, 0)) == NULL)
	{
		perror("mmap");
		exit(0);
	}

	segment_write(ptr);
	decrypt(ptr + ENT_ADDR, ptr + FOO_ADDR, ENT_SIZE, FOO_SIZE, 1);

	write(fd, ptr, statbuf.st_size);
	munmap(ptr, statbuf.st_size);
	close(fd);
}
