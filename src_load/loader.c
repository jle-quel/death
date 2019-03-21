#include "../inc/war.h"

#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <stdio.h>
#include <sys/mman.h>
#include <fcntl.h>
#include <stdlib.h>
#include <elf.h>

#define ENT_ADDR 0x1135
#define F1_ADDR 0x1404
#define F2_ADDR 0x1b20
#define F3_ADDR 0x2086
#define F4_ADDR 0x23e2
#define F5_ADDR 0x2661
#define F6_ADDR 0x2bbf
#define F7_ADDR 0x2ee6
#define F8_ADDR 0x37ef
#define F9_ADDR 0x3be4

#define ENT_SIZE F1_ADDR - ENT_ADDR
#define F1_SIZE F2_ADDR - F1_ADDR
#define F2_SIZE F3_ADDR - F2_ADDR
#define F3_SIZE F4_ADDR - F3_ADDR
#define F4_SIZE F5_ADDR - F4_ADDR
#define F5_SIZE F6_ADDR - F5_ADDR
#define F6_SIZE F7_ADDR - F6_ADDR
#define F7_SIZE F8_ADDR - F7_ADDR
#define F8_SIZE F9_ADDR - F8_ADDR

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

void update_keychain_left(struct s_keychain *keychain, const char *caller, const size_t size)
{
	int key = 0;
	size_t junk[2] = {23, 32};

	for (register size_t index = 0; index < size; index++)
	{
//		key = caller[index];
//		junk[index % 2] = index ^ key;
//		key += caller[index] ^ (junk[index % 2] + 1);
//		junk[index % 2] |= key;
//		key += junk[index % 2];
		key += caller[index];
	}

	keychain->key[LEFT] = key;
	keychain->junk[LEFT][0] = junk[0];
	keychain->junk[LEFT][1] = junk[1];
}

void update_keychain_right(struct s_keychain *keychain, const char *caller, const size_t size)
{
	int key = 0;
	size_t junk[2] = {23, 32};

	for (register size_t index = 0; index < size; index++)
	{
//		key += junk[index % 2];
//		junk[index % 2] = index ^ key;
//		key = caller[index];
//		junk[index % 2] |= key;
		key += caller[index];
	}
	printf("%u\n", key);

	keychain->key[RIGHT] = key;
	keychain->junk[RIGHT][0] = junk[0];
	keychain->junk[RIGHT][1] = junk[1];
}

void decrypt_left(const struct s_keychain *keychain, char *callee, const size_t size)
{
	for (register size_t index = 0; index < size; index++)
	{
		callee[index] ^= keychain->key[LEFT];
	}
}

void decrypt_right(const struct s_keychain *keychain, char *callee, const size_t size)
{
	for (register size_t index = 0; index < size; index++)
	{
		callee[index] ^= keychain->key[RIGHT];
	}
}

////////////////////////////////////////////////////////////////////////////////
/// PUBLIC FUNCTION
////////////////////////////////////////////////////////////////////////////////

int main(void)
{
	struct s_keychain keychain = {0};
	char *filename = "./war";
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

	update_keychain_right(&keychain, ptr + F7_ADDR, F7_SIZE);
	decrypt_right(&keychain, ptr + F8_ADDR, F8_SIZE);

	update_keychain_left(&keychain, ptr + F6_ADDR, F6_SIZE);
	decrypt_left(&keychain, ptr + F7_ADDR, F7_SIZE);

	update_keychain_right(&keychain, ptr + F5_ADDR, F5_SIZE);
	decrypt_right(&keychain, ptr + F6_ADDR, F6_SIZE);

	update_keychain_left(&keychain, ptr + F4_ADDR, F4_SIZE);
	decrypt_left(&keychain, ptr + F5_ADDR, F5_SIZE);

	update_keychain_right(&keychain, ptr + F3_ADDR, F3_SIZE);
	decrypt_right(&keychain, ptr + F4_ADDR, F4_SIZE);

	update_keychain_left(&keychain, ptr + F2_ADDR, F2_SIZE);
	decrypt_left(&keychain, ptr + F3_ADDR, F3_SIZE);

	update_keychain_right(&keychain, ptr + F1_ADDR, F1_SIZE);
	decrypt_right(&keychain, ptr + F2_ADDR, F2_SIZE);

	update_keychain_left(&keychain, ptr + ENT_ADDR, ENT_SIZE);
	decrypt_left(&keychain, ptr + F1_ADDR, F1_SIZE);

	write(fd, ptr, statbuf.st_size);
	munmap(ptr, statbuf.st_size);
	close(fd);
}
