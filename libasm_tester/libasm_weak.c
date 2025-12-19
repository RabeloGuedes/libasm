#include <stddef.h>
#include <sys/types.h>

// Magic sentinel value to detect weak symbols
#define WEAK_SENTINEL 0xDEADBEEF

// Weak wrapper functions that allow linking even when libasm functions are missing
// These will be overridden by strong symbols from libasm.a if they exist
// They return sentinel values so we can detect if the real function was linked

size_t __attribute__((weak)) ft_strlen(const char *s)
{
	(void)s;
	return WEAK_SENTINEL;
}

char __attribute__((weak)) *ft_strcpy(char *dst, const char *src)
{
	(void)dst;
	(void)src;
	return (char *)WEAK_SENTINEL;
}

int __attribute__((weak)) ft_strcmp(const char *s1, const char *s2)
{
	(void)s1;
	(void)s2;
	return WEAK_SENTINEL;
}

ssize_t __attribute__((weak)) ft_write(int fd, const void *buf, size_t count)
{
	(void)fd;
	(void)buf;
	(void)count;
	return WEAK_SENTINEL;
}

ssize_t __attribute__((weak)) ft_read(int fd, void *buf, size_t count)
{
	(void)fd;
	(void)buf;
	(void)count;
	return WEAK_SENTINEL;
}

char __attribute__((weak)) *ft_strdup(const char *s)
{
	(void)s;
	return (char *)WEAK_SENTINEL;
}

// Bonus functions weak symbols
int __attribute__((weak)) ft_atoi_base(char *str, char *base)
{
	(void)str;
	(void)base;
	return WEAK_SENTINEL;
}

typedef struct s_list t_list;

void __attribute__((weak)) ft_list_push_front(t_list **begin_list, void *data)
{
	(void)begin_list;
	(void)data;
}

int __attribute__((weak)) ft_list_size(t_list *begin_list)
{
	(void)begin_list;
	return WEAK_SENTINEL;
}

void __attribute__((weak)) ft_list_sort(t_list **begin_list, int (*cmp)())
{
	(void)begin_list;
	(void)cmp;
}

void __attribute__((weak)) ft_list_remove_if(t_list **begin_list, void *data_ref, int (*cmp)(), void (*free_fct)(void *))
{
	(void)begin_list;
	(void)data_ref;
	(void)cmp;
	(void)free_fct;
}
