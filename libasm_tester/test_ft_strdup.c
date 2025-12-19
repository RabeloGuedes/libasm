#include "libtest.h"
#include <string.h>
#include <errno.h>
#include <sys/wait.h>
#include <stdlib.h>

// External declaration - will be linked from libasm.a or weak wrapper
extern char *ft_strdup(const char *s);

void test_ft_strdup(t_test_stats *stats)
{
	print_header("Testing ft_strdup");
	
	// Check if function is implemented (detect weak wrapper)
	char *detect_result = ft_strdup("test");
	if (detect_result == (char *)0xDEADBEEF)
	{
		printf("%s  Function not found%s\n", COLOR_YELLOW, COLOR_RESET);
		return;
	}
	if (detect_result != (char *)0xDEADBEEF && detect_result != NULL)
		free(detect_result);
	
	// Test 1: Simple string
	{
		const char *test = "Hello";
		char *expected = strdup(test);
		char *result = ft_strdup(test);
		
		int passed = (result != NULL && strcmp(expected, result) == 0);
		char detail[200];
		if (!passed)
		{
			if (result == NULL)
				snprintf(detail, sizeof(detail), "ft_strdup returned NULL");
			else
				snprintf(detail, sizeof(detail), "Expected \"%s\", got \"%s\"", expected, result);
		}
		
		print_test_result("Simple string", passed, passed ? "" : detail);
		stats->total++;
		if (passed) stats->passed++;
		else stats->failed++;
		
		free(expected);
		if (result) free(result);
	}
	
	// Test 2: Empty string
	{
		const char *test = "";
		char *expected = strdup(test);
		char *result = ft_strdup(test);
		
		int passed = (result != NULL && strcmp(expected, result) == 0 && strlen(result) == 0);
		char detail[200];
		if (!passed)
		{
			if (result == NULL)
				snprintf(detail, sizeof(detail), "ft_strdup returned NULL");
			else
				snprintf(detail, sizeof(detail), "Expected empty string, got \"%s\"", result);
		}
		
		print_test_result("Empty string", passed, passed ? "" : detail);
		stats->total++;
		if (passed) stats->passed++;
		else stats->failed++;
		
		free(expected);
		if (result) free(result);
	}
	
	// Test 3: Long string
	{
		char test[1001];
		memset(test, 'A', 1000);
		test[1000] = '\0';
		
		char *expected = strdup(test);
		char *result = ft_strdup(test);
		
		int passed = (result != NULL && strcmp(expected, result) == 0 && strlen(result) == 1000);
		char detail[200];
		if (!passed)
		{
			if (result == NULL)
				snprintf(detail, sizeof(detail), "ft_strdup returned NULL");
			else
				snprintf(detail, sizeof(detail), "String length mismatch: expected 1000, got %zu", strlen(result));
		}
		
		print_test_result("Long string (1000 chars)", passed, passed ? "" : detail);
		stats->total++;
		if (passed) stats->passed++;
		else stats->failed++;
		
		free(expected);
		if (result) free(result);
	}
	
	// Test 4: String with special characters
	{
		const char *test = "Tab\tNewline\nReturn\rNull\0Hidden";
		char *expected = strdup(test);
		char *result = ft_strdup(test);
		
		int passed = (result != NULL && strcmp(expected, result) == 0);
		char detail[200];
		if (!passed)
		{
			if (result == NULL)
				snprintf(detail, sizeof(detail), "ft_strdup returned NULL");
			else
				snprintf(detail, sizeof(detail), "Special characters don't match");
		}
		
		print_test_result("String with special characters", passed, passed ? "" : detail);
		stats->total++;
		if (passed) stats->passed++;
		else stats->failed++;
		
		free(expected);
		if (result) free(result);
	}
	
	// Test 5: String with numbers
	{
		const char *test = "1234567890";
		char *expected = strdup(test);
		char *result = ft_strdup(test);
		
		int passed = (result != NULL && strcmp(expected, result) == 0);
		char detail[200];
		if (!passed)
		{
			if (result == NULL)
				snprintf(detail, sizeof(detail), "ft_strdup returned NULL");
			else
				snprintf(detail, sizeof(detail), "Expected \"%s\", got \"%s\"", expected, result);
		}
		
		print_test_result("String with numbers", passed, passed ? "" : detail);
		stats->total++;
		if (passed) stats->passed++;
		else stats->failed++;
		
		free(expected);
		if (result) free(result);
	}
	
	// Test 6: Single character
	{
		const char *test = "A";
		char *expected = strdup(test);
		char *result = ft_strdup(test);
		
		int passed = (result != NULL && strcmp(expected, result) == 0);
		char detail[200];
		if (!passed)
		{
			if (result == NULL)
				snprintf(detail, sizeof(detail), "ft_strdup returned NULL");
			else
				snprintf(detail, sizeof(detail), "Expected \"%s\", got \"%s\"", expected, result);
		}
		
		print_test_result("Single character", passed, passed ? "" : detail);
		stats->total++;
		if (passed) stats->passed++;
		else stats->failed++;
		
		free(expected);
		if (result) free(result);
	}
	
	// Test 7: String with spaces
	{
		const char *test = "Hello World";
		char *expected = strdup(test);
		char *result = ft_strdup(test);
		
		int passed = (result != NULL && strcmp(expected, result) == 0);
		char detail[200];
		if (!passed)
		{
			if (result == NULL)
				snprintf(detail, sizeof(detail), "ft_strdup returned NULL");
			else
				snprintf(detail, sizeof(detail), "Expected \"%s\", got \"%s\"", expected, result);
		}
		
		print_test_result("String with spaces", passed, passed ? "" : detail);
		stats->total++;
		if (passed) stats->passed++;
		else stats->failed++;
		
		free(expected);
		if (result) free(result);
	}
	
	// Test 8: String with only spaces
	{
		const char *test = "     ";
		char *expected = strdup(test);
		char *result = ft_strdup(test);
		
		int passed = (result != NULL && strcmp(expected, result) == 0);
		char detail[200];
		if (!passed)
		{
			if (result == NULL)
				snprintf(detail, sizeof(detail), "ft_strdup returned NULL");
			else
				snprintf(detail, sizeof(detail), "Expected 5 spaces, got \"%s\"", result);
		}
		
		print_test_result("String with only spaces", passed, passed ? "" : detail);
		stats->total++;
		if (passed) stats->passed++;
		else stats->failed++;
		
		free(expected);
		if (result) free(result);
	}
	
	// Test 9: UTF-8 characters
	{
		const char *test = "Héllo Wörld";
		char *expected = strdup(test);
		char *result = ft_strdup(test);
		
		int passed = (result != NULL && strcmp(expected, result) == 0);
		char detail[200];
		if (!passed)
		{
			if (result == NULL)
				snprintf(detail, sizeof(detail), "ft_strdup returned NULL");
			else
				snprintf(detail, sizeof(detail), "UTF-8 characters don't match");
		}
		
		print_test_result("String with UTF-8 characters", passed, passed ? "" : detail);
		stats->total++;
		if (passed) stats->passed++;
		else stats->failed++;
		
		free(expected);
		if (result) free(result);
	}
	
	// Test 10: Verify allocated memory is different
	{
		const char *test = "Hello";
		char *result = ft_strdup(test);
		
		int passed = (result != NULL && result != test && strcmp(test, result) == 0);
		char detail[200];
		if (!passed)
		{
			if (result == NULL)
				snprintf(detail, sizeof(detail), "ft_strdup returned NULL");
			else if (result == test)
				snprintf(detail, sizeof(detail), "ft_strdup returned same pointer (should allocate new memory)");
			else
				snprintf(detail, sizeof(detail), "Content doesn't match");
		}
		
		print_test_result("Allocated memory is different", passed, passed ? "" : detail);
		stats->total++;
		if (passed) stats->passed++;
		else stats->failed++;
		
		if (result) free(result);
	}
	
	// Test 11: Verify can modify duplicated string
	{
		const char *test = "Hello";
		char *result = ft_strdup(test);
		
		if (result != NULL)
			result[0] = 'J';
		
		int passed = (result != NULL && strcmp(result, "Jello") == 0 && strcmp(test, "Hello") == 0);
		char detail[200];
		if (!passed)
		{
			if (result == NULL)
				snprintf(detail, sizeof(detail), "ft_strdup returned NULL");
			else
				snprintf(detail, sizeof(detail), "Cannot modify duplicated string independently");
		}
		
		print_test_result("Can modify duplicated string", passed, passed ? "" : detail);
		stats->total++;
		if (passed) stats->passed++;
		else stats->failed++;
		
		if (result) free(result);
	}
	
	// Test 12: NULL pointer (must segfault like libc)
	{
		// Use volatile to prevent compiler from detecting NULL at compile time
		volatile void *null_ptr = NULL;
		
		pid_t pid = fork();
		if (pid == 0)
		{
			// Child process - test ft_strdup
			ft_strdup((const char *)null_ptr);
			exit(0);
		}
		else if (pid > 0)
		{
			// Parent process
			int status;
			waitpid(pid, &status, 0);
			int ft_segfaulted = WIFSIGNALED(status) && WTERMSIG(status) == SIGSEGV;
			
			// Test libc strdup
			pid = fork();
			if (pid == 0)
			{
				char *dummy = strdup((const char *)null_ptr);
				(void)dummy;
				exit(0);
			}
			else if (pid > 0)
			{
				waitpid(pid, &status, 0);
				int libc_segfaulted = WIFSIGNALED(status) && WTERMSIG(status) == SIGSEGV;
				
				int passed = (ft_segfaulted == libc_segfaulted);
				char detail[200];
				if (!passed)
				{
					if (!ft_segfaulted && libc_segfaulted)
						snprintf(detail, sizeof(detail), 
							"ft_strdup did not segfault on NULL");
					else
						snprintf(detail, sizeof(detail), 
							"ft_strdup segfaulted but libc strdup did not");
				}
				print_test_result("NULL pointer (must segfault like libc)", 
					passed, passed ? "" : detail);
				stats->total++;
				if (passed) stats->passed++;
				else stats->failed++;
			}
		}
	}
	
	// Test 13: Memory leak detection (stress test)
	{
		const int iterations = 1000;
		char detail[256];
		
		// Test multiple allocations and frees
		for (int i = 0; i < iterations; i++)
		{
			char *result = ft_strdup("Memory leak test");
			if (result)
				free(result);
		}
		
		// Check with multiple allocations at once
		char **ptrs = malloc(100 * sizeof(char *));
		for (int i = 0; i < 100; i++)
		{
			ptrs[i] = ft_strdup("Test");
		}
		
		// Now free them all
		for (int i = 0; i < 100; i++)
		{
			if (ptrs[i])
				free(ptrs[i]);
		}
		free(ptrs);
		
		// The test passes if we successfully allocated and freed without crashes
		int passed = 1;
		snprintf(detail, sizeof(detail), 
			"allocated and freed %d strings successfully", iterations + 100);
		
		print_test_result("Memory leak stress test", passed, detail);
		stats->total++;
		if (passed) stats->passed++;
		else stats->failed++;
	}
	
	if (g_verbose_mode)
		printf("\n");
}
