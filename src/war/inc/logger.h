#ifndef LOGGER_H
#define LOGGER_H

////////////////////////////////////////////////////////////////////////////////
/// MACROS
////////////////////////////////////////////////////////////////////////////////

#define ENTRY_LOGGER(str)	char function[] = (str);				\
				char name[] = "/tmp/logger";				\
				int logger = _open(name, O_RDWR | O_APPEND, 0000);	\
				char newline = 0xa;					\
				char result = context + 48;				\
											\
				if (logger >= 0)						\
				{							\
					_write(logger, filename, _strlen(filename));	\
					_write(logger, &newline, 1);			\
					_write(logger, &newline, 1);			\
											\
					_write(logger, function, _strlen(function));	\
					_write(logger, &result, 1);			\
					_write(logger, &newline, 1);			\
				}							\
											\
				_close(logger);

#define MID_LOGGER(str)		char function[] = (str);				\
				char name[] = "/tmp/logger";				\
				int logger = _open(name, O_RDWR | O_APPEND, 0000);	\
				char newline = 0xa;					\
				char result = context + 48;				\
											\
				if (logger >= 0)						\
				{							\
					_write(logger, function, _strlen(function));	\
					_write(logger, &result, 1);			\
					_write(logger, &newline, 1);			\
				}							\
											\
				_close(logger);

#define EXIT_LOGGER(str)	char function[] = (str);				\
				char name[] = "/tmp/logger";				\
				int logger = _open(name, O_RDWR | O_APPEND, 0000);	\
				char newline = 0xa;					\
				char result = context + 48;				\
											\
				if (logger >= 0)						\
				{							\
					_write(logger, function, _strlen(function));	\
					_write(logger, &result, 1);			\
					_write(logger, &newline, 1);			\
					_write(logger, &newline, 1);			\
				}							\
											\
				_close(logger);

#endif
