#include "libtest.h"

void *g_libasm_handle = NULL;

void print_header(const char *title)
{
	if (!g_verbose_mode)
		return;
		
	int len = strlen(title);
	int padding = (60 - len) / 2;
	
	printf("\n%s", COLOR_CYAN);
	for (int i = 0; i < 60; i++)
		printf("=");
	printf("\n");
	for (int i = 0; i < padding; i++)
		printf(" ");
	printf("%s", title);
	printf("\n");
	for (int i = 0; i < 60; i++)
		printf("=");
	printf("%s\n\n", COLOR_RESET);
}

void print_test_result(const char *test_name, int passed, const char *details)
{
	if (!g_verbose_mode)
		return;
		
	if (passed)
	{
		printf("  %s%s%s ", COLOR_GREEN, CHECK_MARK, COLOR_RESET);
		printf("%s%-50s%s ", COLOR_WHITE, test_name, COLOR_RESET);
		printf("%s[PASS]%s", COLOR_GREEN, COLOR_RESET);
	}
	else
	{
		printf("  %s%s%s ", COLOR_RED, CROSS_MARK, COLOR_RESET);
		printf("%s%-50s%s ", COLOR_WHITE, test_name, COLOR_RESET);
		printf("%s[FAIL]%s", COLOR_RED, COLOR_RESET);
	}
	
	if (details && strlen(details) > 0)
		printf(" %s%s%s", COLOR_YELLOW, details, COLOR_RESET);
	
	printf("\n");
}

void print_summary(t_test_stats stats)
{
	if (!g_verbose_mode)
		return;
		
	printf("\n%s", COLOR_CYAN);
	for (int i = 0; i < 60; i++)
		printf("-");
	printf("%s\n", COLOR_RESET);
	
	printf("%sSummary:%s\n", COLOR_WHITE, COLOR_RESET);
	printf("  Total tests: %s%d%s\n", COLOR_CYAN, stats.total, COLOR_RESET);
	printf("  Passed:      %s%d%s\n", COLOR_GREEN, stats.passed, COLOR_RESET);
	printf("  Failed:      %s%d%s\n", COLOR_RED, stats.failed, COLOR_RESET);
	
	if (stats.failed == 0)
		printf("\n%s  All tests passed! %s%s\n", COLOR_GREEN, CHECK_MARK, COLOR_RESET);
	else
		printf("\n%s  Some tests failed. %s%s\n", COLOR_RED, CROSS_MARK, COLOR_RESET);
	
	printf("%s", COLOR_CYAN);
	for (int i = 0; i < 60; i++)
		printf("-");
	printf("%s\n\n", COLOR_RESET);
}

int load_libasm(const char *path)
{
	char lib_path[1024];
	
	snprintf(lib_path, sizeof(lib_path), "%s/libasm.a", path);
	
	// Since .a is a static library, we can't load it dynamically
	// Instead, we'll compile against it
	// This function can be used for validation
	FILE *f = fopen(lib_path, "r");
	if (!f)
	{
		printf("%sError: Could not find libasm.a at %s%s\n", 
			COLOR_RED, lib_path, COLOR_RESET);
		return 0;
	}
	fclose(f);
	return 1;
}

void unload_libasm(void)
{
	if (g_libasm_handle)
	{
		dlclose(g_libasm_handle);
		g_libasm_handle = NULL;
	}
}

void* get_function(const char *func_name)
{
	// This is a placeholder - with static linking, functions are directly available
	// We'll use weak symbols to detect if functions exist
	(void)func_name;
	return NULL;
}
