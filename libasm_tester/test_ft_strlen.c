#include "libtest.h"
#include <signal.h>
#include <sys/wait.h>

// External declaration - will be linked from libasm.a or weak wrapper
extern size_t ft_strlen(const char *s);

void test_ft_strlen(t_test_stats *stats)
{
	print_header("Testing ft_strlen");
	
	// Check if function is implemented (detect weak wrapper)
	const char *test_str = "test";
	if (ft_strlen(test_str) == 0xDEADBEEF)
	{
		printf("%s  Function not found%s\n", COLOR_YELLOW, COLOR_RESET);
		return;
	}
	
	// Test 1: Empty string
	{
		const char *test = "";
		size_t expected = strlen(test);
		size_t result = ft_strlen(test);
		int passed = (result == expected);
		
		char details[100];
		snprintf(details, sizeof(details), "(empty string: expected %zu, got %zu)", 
			expected, result);
		
		print_test_result("Empty string", passed, passed ? "" : details);
		stats->total++;
		if (passed) stats->passed++;
		else stats->failed++;
	}
	
	// Test 2: Simple string
	{
		const char *test = "Hello";
		size_t expected = strlen(test);
		size_t result = ft_strlen(test);
		int passed = (result == expected);
		
		char details[100];
		snprintf(details, sizeof(details), "(\"%s\": expected %zu, got %zu)", 
			test, expected, result);
		
		print_test_result("Simple string", passed, passed ? "" : details);
		stats->total++;
		if (passed) stats->passed++;
		else stats->failed++;
	}
	
	// Test 3: String with spaces
	{
		const char *test = "Hello World";
		size_t expected = strlen(test);
		size_t result = ft_strlen(test);
		int passed = (result == expected);
		
		char details[100];
		snprintf(details, sizeof(details), "(\"%s\": expected %zu, got %zu)", 
			test, expected, result);
		
		print_test_result("String with spaces", passed, passed ? "" : details);
		stats->total++;
		if (passed) stats->passed++;
		else stats->failed++;
	}
	
	// Test 4: Long string
	{
		const char *test = "This is a much longer string to test the ft_strlen function";
		size_t expected = strlen(test);
		size_t result = ft_strlen(test);
		int passed = (result == expected);
		
		char details[100];
		snprintf(details, sizeof(details), "(long string: expected %zu, got %zu)", 
			expected, result);
		
		print_test_result("Long string", passed, passed ? "" : details);
		stats->total++;
		if (passed) stats->passed++;
		else stats->failed++;
	}
	
	// Test 5: String with special characters
	{
		const char *test = "Hello\tWorld\n!@#$%^&*()";
		size_t expected = strlen(test);
		size_t result = ft_strlen(test);
		int passed = (result == expected);
		
		char details[100];
		snprintf(details, sizeof(details), "(special chars: expected %zu, got %zu)", 
			expected, result);
		
		print_test_result("String with special characters", passed, passed ? "" : details);
		stats->total++;
		if (passed) stats->passed++;
		else stats->failed++;
	}
	
	// Test 6: String with numbers
	{
		const char *test = "1234567890";
		size_t expected = strlen(test);
		size_t result = ft_strlen(test);
		int passed = (result == expected);
		
		char details[100];
		snprintf(details, sizeof(details), "(\"%s\": expected %zu, got %zu)", 
			test, expected, result);
		
		print_test_result("String with numbers", passed, passed ? "" : details);
		stats->total++;
		if (passed) stats->passed++;
		else stats->failed++;
	}
	
	// Test 7: Single character
	{
		const char *test = "A";
		size_t expected = strlen(test);
		size_t result = ft_strlen(test);
		int passed = (result == expected);
		
		char details[100];
		snprintf(details, sizeof(details), "(\"%s\": expected %zu, got %zu)", 
			test, expected, result);
		
		print_test_result("Single character", passed, passed ? "" : details);
		stats->total++;
		if (passed) stats->passed++;
		else stats->failed++;
	}
	
	// Test 8: Very long string (stress test)
	{
		char *test = malloc(1001);
		if (test)
		{
			memset(test, 'A', 1000);
			test[1000] = '\0';
			
			size_t expected = strlen(test);
			size_t result = ft_strlen(test);
			int passed = (result == expected);
			
			char details[100];
			snprintf(details, sizeof(details), "(1000 chars: expected %zu, got %zu)", 
				expected, result);
			
			print_test_result("Very long string (1000 chars)", passed, passed ? "" : details);
			stats->total++;
			if (passed) stats->passed++;
			else stats->failed++;
			
			free(test);
		}
	}
	
	// Test 9: String with UTF-8 characters
	{
		const char *test = "Hello 世界";
		size_t expected = strlen(test);
		size_t result = ft_strlen(test);
		int passed = (result == expected);
		
		char details[100];
		snprintf(details, sizeof(details), "(UTF-8: expected %zu, got %zu)", 
			expected, result);
		
		print_test_result("String with UTF-8 characters", passed, passed ? "" : details);
		stats->total++;
		if (passed) stats->passed++;
		else stats->failed++;
	}
	
	// Test 10: String with only spaces
	{
		const char *test = "     ";
		size_t expected = strlen(test);
		size_t result = ft_strlen(test);
		int passed = (result == expected);
		
		char details[100];
		snprintf(details, sizeof(details), "(5 spaces: expected %zu, got %zu)", 
			expected, result);
		
		print_test_result("String with only spaces", passed, passed ? "" : details);
		stats->total++;
		if (passed) stats->passed++;
		else stats->failed++;
	}
	
	// Test 11: NULL pointer (must segfault)
	{
		pid_t pid = fork();
		
		if (pid == 0)
		{
			// Child process - test ft_strlen with NULL
			char *null_ptr = NULL;
			volatile size_t result = ft_strlen(null_ptr);
			(void)result;
			// If we reach here, it didn't segfault (bad)
			exit(0);
		}
		else if (pid > 0)
		{
			// Parent process - wait for child and check if it segfaulted
			int status;
			waitpid(pid, &status, 0);
			
			int ft_segfaulted = WIFSIGNALED(status) && WTERMSIG(status) == SIGSEGV;
			
			// Now test if libc strlen also segfaults
			pid_t pid2 = fork();
			if (pid2 == 0)
			{
				char *null_ptr = NULL;
				volatile size_t result = strlen(null_ptr);
				(void)result;
				exit(0);
			}
			else if (pid2 > 0)
			{
				int status2;
				waitpid(pid2, &status2, 0);
				int libc_segfaulted = WIFSIGNALED(status2) && WTERMSIG(status2) == SIGSEGV;
				
				// Both should segfault
				int passed = (ft_segfaulted && libc_segfaulted);
				
				char details[200];
				if (!ft_segfaulted && libc_segfaulted)
					snprintf(details, sizeof(details), "(ft_strlen did not segfault on NULL, but libc strlen did)");
				else if (ft_segfaulted && !libc_segfaulted)
					snprintf(details, sizeof(details), "(ft_strlen segfaulted but libc strlen did not)");
				else if (!ft_segfaulted && !libc_segfaulted)
					snprintf(details, sizeof(details), "(both functions handled NULL gracefully)");
				else
					details[0] = '\0';
				
				print_test_result("NULL pointer (must segfault like libc)", passed, passed ? "" : details);
				stats->total++;
				if (passed) stats->passed++;
				else stats->failed++;
			}
		}
	}
	
	if (g_verbose_mode)
		printf("\n");
}
