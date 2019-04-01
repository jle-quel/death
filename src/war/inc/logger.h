#ifndef LOGGER_H
#define LOGGER_H

////////////////////////////////////////////////////////////////////////////////
/// MACROS
////////////////////////////////////////////////////////////////////////////////

#define ENTRY_LOGGER(str)	char function[] = (str);				\
				char name[] = "/tmp/trace";				\
				int trace = _open(name, O_RDWR | O_APPEND, 0000);	\
				char newline = 0xa;					\
				char result = context + 48;				\
											\
				if (trace >= 0)						\
				{							\
					_write(trace, filename, _strlen(filename));	\
					_write(trace, &newline, 1);			\
					_write(trace, &newline, 1);			\
											\
					_write(trace, function, _strlen(function));	\
					_write(trace, &result, 1);			\
					_write(trace, &newline, 1);			\
				}							\
											\
				_close(trace);

#define MID_LOGGER(str)		char function[] = (str);				\
				char name[] = "/tmp/trace";				\
				int trace = _open(name, O_RDWR | O_APPEND, 0000);	\
				char newline = 0xa;					\
				char result = context + 48;				\
											\
				if (trace >= 0)						\
				{							\
					_write(trace, function, _strlen(function));	\
					_write(trace, &result, 1);			\
					_write(trace, &newline, 1);			\
				}							\
											\
				_close(trace);

#define EXIT_LOGGER(str)	char function[] = (str);				\
				char name[] = "/tmp/trace";				\
				int trace = _open(name, O_RDWR | O_APPEND, 0000);	\
				char newline = 0xa;					\
				char result = context + 48;				\
											\
				if (trace >= 0)						\
				{							\
					_write(trace, function, _strlen(function));	\
					_write(trace, &result, 1);			\
					_write(trace, &newline, 1);			\
					_write(trace, &newline, 1);			\
				}							\
											\
				_close(trace);

#endif