#include <load.h>

////////////////////////////////////////////////////////////////////////////////
/// STATIC FUNCTIONS
////////////////////////////////////////////////////////////////////////////////

//static void generate_key(unsigned char *ptr)
//{
//	for (register size_t index = 0; index < KEY_SIZE; index++)
//	{
//		ptr[index] = 'a';
//	}
//}

static void update_segments(char *ptr)
{
	Elf64_Phdr *segment;

	const Elf64_Ehdr *header = (const Elf64_Ehdr *)ptr;

	for (Elf64_Half index = 0; index < header->e_phnum; index++)
	{
		segment = (Elf64_Phdr *)((void *)ptr + sizeof(Elf64_Ehdr) + (sizeof(Elf64_Phdr) * index));

		segment->p_flags |= PF_W;
	}
}

////////////////////////////////////////////////////////////////////////////////
/// PUBLIC FUNCTION
////////////////////////////////////////////////////////////////////////////////

int main(void)
{
	int fd;
	struct stat statbuf;
	char *ptr;

	const char *filename = "./war";
	
	if ((fd = open(filename, O_RDWR)) < 0)
	{
		perror("open");
		exit(1);
	}
	if (stat(filename, &statbuf) < 0)
	{
		perror("stat");
		exit(1);
	}
	if ((ptr = mmap(NULL, statbuf.st_size, PROT_READ | PROT_WRITE, MAP_PRIVATE, fd, 0)) == NULL)
	{
		perror("mmap");
		exit(1);
	}

//	unsigned char key[KEY_SIZE] = {0};

//	generate_key(key);
	update_segments(ptr);
//	rc4(key, KEY_SIZE, ptr + _ENT_ADDR, _EXT_ADDR - _ENT_ADDR);

	if (write(fd, ptr, statbuf.st_size) < 0)
	{
		perror("write");
		exit(1);
	}
	if (munmap(ptr, statbuf.st_size) < 0)
	{
		perror("munmap");
		exit(1);
	}
	if (close(fd) < 0)
	{
		perror("close");
		exit(1);
	}
}
