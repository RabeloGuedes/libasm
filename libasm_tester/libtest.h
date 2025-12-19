#ifndef LIBTEST_H
# define LIBTEST_H

# include <stdio.h>
# include <stdlib.h>
# include <unistd.h>
# include <string.h>
# include <fcntl.h>
# include <errno.h>
# include <dlfcn.h>
# include <sys/wait.h>
# include <signal.h>

// Global verbose mode flag
extern int g_verbose_mode;

// Color codes
# define COLOR_RESET   "\033[0m"
# define COLOR_RED     "\033[1;31m"
# define COLOR_GREEN   "\033[1;32m"
# define COLOR_YELLOW  "\033[1;33m"
# define COLOR_BLUE    "\033[1;34m"
# define COLOR_MAGENTA "\033[1;35m"
# define COLOR_CYAN    "\033[1;36m"
# define COLOR_WHITE   "\033[1;37m"

// Symbols
# define CHECK_MARK "✓"
# define CROSS_MARK "✗"

// List structure for bonus functions
typedef struct s_list
{
	void			*data;
	struct s_list	*next;
}	t_list;

// Test statistics
typedef struct s_test_stats
{
	int	total;
	int	passed;
	int	failed;
}	t_test_stats;

// Function pointers for libasm functions
typedef size_t	(*ft_strlen_t)(const char *);
typedef char*	(*ft_strcpy_t)(char *, const char *);
typedef int		(*ft_strcmp_t)(const char *, const char *);
typedef ssize_t	(*ft_write_t)(int, const void *, size_t);
typedef ssize_t	(*ft_read_t)(int, void *, size_t);
typedef char*	(*ft_strdup_t)(const char *);
typedef int		(*ft_atoi_base_t)(char *, char *);
typedef void	(*ft_list_push_front_t)(t_list **, void *);
typedef int		(*ft_list_size_t)(t_list *);
typedef void	(*ft_list_sort_t)(t_list **, int (*)());
typedef void	(*ft_list_remove_if_t)(t_list **, void *, int (*)(), void (*)(void *));

// Global handle for libasm
extern void *g_libasm_handle;

// Test framework functions
void	print_header(const char *title);
void	print_test_result(const char *test_name, int passed, const char *details);
void	print_summary(t_test_stats stats);
int		load_libasm(const char *path);
void	unload_libasm(void);
void*	get_function(const char *func_name);

// Test functions
void	test_ft_strlen(t_test_stats *stats);
void	test_ft_strcpy(t_test_stats *stats);
void	test_ft_strcmp(t_test_stats *stats);
void	test_ft_write(t_test_stats *stats);
void	test_ft_read(t_test_stats *stats);
void	test_ft_strdup(t_test_stats *stats);
void	test_ft_atoi_base(t_test_stats *stats);
void	test_ft_list_push_front(t_test_stats *stats);
void	test_ft_list_size(t_test_stats *stats);
void	test_ft_list_sort(t_test_stats *stats);
void	test_ft_list_remove_if(t_test_stats *stats);

#endif
