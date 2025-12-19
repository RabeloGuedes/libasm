#include "libtest.h"
#include <signal.h>
#include <sys/wait.h>

// External declaration - will be linked from libasm.a or weak wrapper
extern int ft_strcmp(const char *s1, const char *s2);

void test_ft_strcmp(t_test_stats *stats)
{
	print_header("ft_strcmp");
	
	// Check if function is implemented (detect weak wrapper)
	if (ft_strcmp("test", "test") == (int)0xDEADBEEF)
	{
		printf("%s  Function not found%s\n", COLOR_YELLOW, COLOR_RESET);
		return;
	}
	
	// Test 1: Identical strings
	{
		const char *s1 = "Hello";
		const char *s2 = "Hello";
		
		int result1 = strcmp(s1, s2);
		int result2 = ft_strcmp(s1, s2);
		
		int passed = ((result1 == 0 && result2 == 0) || 
		              (result1 > 0 && result2 > 0) || 
		              (result1 < 0 && result2 < 0));
		
		char details[200];
		snprintf(details, sizeof(details), "(\"%s\" vs \"%s\": expected %d, got %d)", 
			s1, s2, result1, result2);
		
		print_test_result("Identical strings", passed, passed ? "" : details);
		stats->total++;
		if (passed) stats->passed++;
		else stats->failed++;
	}
	
	// Test 2: First string greater
	{
		const char *s1 = "Hello";
		const char *s2 = "Hella";
		
		int result1 = strcmp(s1, s2);
		int result2 = ft_strcmp(s1, s2);
		
		int passed = ((result1 == 0 && result2 == 0) || 
		              (result1 > 0 && result2 > 0) || 
		              (result1 < 0 && result2 < 0));
		
		char details[200];
		snprintf(details, sizeof(details), "(\"%s\" vs \"%s\": expected %s, got %s)", 
			s1, s2, result1 > 0 ? "positive" : "negative", result2 > 0 ? "positive" : "negative");
		
		print_test_result("First string greater", passed, passed ? "" : details);
		stats->total++;
		if (passed) stats->passed++;
		else stats->failed++;
	}
	
	// Test 3: Second string greater
	{
		const char *s1 = "Hello";
		const char *s2 = "World";
		
		int result1 = strcmp(s1, s2);
		int result2 = ft_strcmp(s1, s2);
		
		int passed = ((result1 == 0 && result2 == 0) || 
		              (result1 > 0 && result2 > 0) || 
		              (result1 < 0 && result2 < 0));
		
		char details[200];
		snprintf(details, sizeof(details), "(\"%s\" vs \"%s\": expected %s, got %s)", 
			s1, s2, result1 < 0 ? "negative" : "positive", result2 < 0 ? "negative" : "positive");
		
		print_test_result("Second string greater", passed, passed ? "" : details);
		stats->total++;
		if (passed) stats->passed++;
		else stats->failed++;
	}
	
	// Test 4: Empty strings
	{
		const char *s1 = "";
		const char *s2 = "";
		
		int result1 = strcmp(s1, s2);
		int result2 = ft_strcmp(s1, s2);
		
		int passed = (result1 == result2) && (result1 == 0);
		
		char details[200];
		snprintf(details, sizeof(details), "(both empty: expected %d, got %d)", 
			result1, result2);
		
		print_test_result("Both empty strings", passed, passed ? "" : details);
		stats->total++;
		if (passed) stats->passed++;
		else stats->failed++;
	}
	
	// Test 5: First string empty
	{
		const char *s1 = "";
		const char *s2 = "Hello";
		
		int result1 = strcmp(s1, s2);
		int result2 = ft_strcmp(s1, s2);
		
		int passed = ((result1 == 0 && result2 == 0) || 
		              (result1 > 0 && result2 > 0) || 
		              (result1 < 0 && result2 < 0));
		
		char details[200];
		snprintf(details, sizeof(details), "(\"\" vs \"%s\": expected %s, got %s)", 
			s2, result1 < 0 ? "negative" : "positive", result2 < 0 ? "negative" : "positive");
		
		print_test_result("First string empty", passed, passed ? "" : details);
		stats->total++;
		if (passed) stats->passed++;
		else stats->failed++;
	}
	
	// Test 6: Second string empty
	{
		const char *s1 = "Hello";
		const char *s2 = "";
		
		int result1 = strcmp(s1, s2);
		int result2 = ft_strcmp(s1, s2);
		
		int passed = ((result1 == 0 && result2 == 0) || 
		              (result1 > 0 && result2 > 0) || 
		              (result1 < 0 && result2 < 0));
		
		char details[200];
		snprintf(details, sizeof(details), "(\"%s\" vs \"\": expected %s, got %s)", 
			s1, result1 > 0 ? "positive" : "negative", result2 > 0 ? "positive" : "negative");
		
		print_test_result("Second string empty", passed, passed ? "" : details);
		stats->total++;
		if (passed) stats->passed++;
		else stats->failed++;
	}
	
	// Test 7: First string is prefix of second
	{
		const char *s1 = "Hello";
		const char *s2 = "Hello World";
		
		int result1 = strcmp(s1, s2);
		int result2 = ft_strcmp(s1, s2);
		
		int passed = ((result1 == 0 && result2 == 0) || 
		              (result1 > 0 && result2 > 0) || 
		              (result1 < 0 && result2 < 0));
		
		char details[200];
		snprintf(details, sizeof(details), "(\"%s\" vs \"%s\": expected %s, got %s)", 
			s1, s2, result1 < 0 ? "negative" : "positive", result2 < 0 ? "negative" : "positive");
		
		print_test_result("First is prefix of second", passed, passed ? "" : details);
		stats->total++;
		if (passed) stats->passed++;
		else stats->failed++;
	}
	
	// Test 8: Second string is prefix of first
	{
		const char *s1 = "Hello World";
		const char *s2 = "Hello";
		
		int result1 = strcmp(s1, s2);
		int result2 = ft_strcmp(s1, s2);
		
		int passed = ((result1 == 0 && result2 == 0) || 
		              (result1 > 0 && result2 > 0) || 
		              (result1 < 0 && result2 < 0));
		
		char details[200];
		snprintf(details, sizeof(details), "(\"%s\" vs \"%s\": expected %s, got %s)", 
			s1, s2, result1 > 0 ? "positive" : "negative", result2 > 0 ? "positive" : "negative");
		
		print_test_result("Second is prefix of first", passed, passed ? "" : details);
		stats->total++;
		if (passed) stats->passed++;
		else stats->failed++;
	}
	
	// Test 9: Case sensitivity
	{
		const char *s1 = "Hello";
		const char *s2 = "hello";
		
		int result1 = strcmp(s1, s2);
		int result2 = ft_strcmp(s1, s2);
		
		int passed = ((result1 == 0 && result2 == 0) || 
		              (result1 > 0 && result2 > 0) || 
		              (result1 < 0 && result2 < 0));
		
		char details[200];
		snprintf(details, sizeof(details), "(\"%s\" vs \"%s\": expected %s, got %s)", 
			s1, s2, result1 < 0 ? "negative" : "positive", result2 < 0 ? "negative" : "positive");
		
		print_test_result("Case sensitivity (H vs h)", passed, passed ? "" : details);
		stats->total++;
		if (passed) stats->passed++;
		else stats->failed++;
	}
	
	// Test 10: Special characters
	{
		const char *s1 = "Hello\tWorld";
		const char *s2 = "Hello\nWorld";
		
		int result1 = strcmp(s1, s2);
		int result2 = ft_strcmp(s1, s2);
		
		int passed = ((result1 == 0 && result2 == 0) || 
		              (result1 > 0 && result2 > 0) || 
		              (result1 < 0 && result2 < 0));
		
		char details[200];
		if (!passed)
			snprintf(details, sizeof(details), "(special chars: signs don't match)");
		else
			details[0] = '\0';
		
		print_test_result("Special characters (\\t vs \\n)", passed, passed ? "" : details);
		stats->total++;
		if (passed) stats->passed++;
		else stats->failed++;
	}
	
	// Test 11: Numbers as strings
	{
		const char *s1 = "123";
		const char *s2 = "124";
		
		int result1 = strcmp(s1, s2);
		int result2 = ft_strcmp(s1, s2);
		
		int passed = ((result1 == 0 && result2 == 0) || 
		              (result1 > 0 && result2 > 0) || 
		              (result1 < 0 && result2 < 0));
		
		char details[200];
		snprintf(details, sizeof(details), "(\"%s\" vs \"%s\": expected %s, got %s)", 
			s1, s2, result1 < 0 ? "negative" : "positive", result2 < 0 ? "negative" : "positive");
		
		print_test_result("Numbers as strings", passed, passed ? "" : details);
		stats->total++;
		if (passed) stats->passed++;
		else stats->failed++;
	}
	
	// Test 12: Long identical strings
	{
		const char *s1 = "This is a very long string to test the ft_strcmp function with identical content";
		const char *s2 = "This is a very long string to test the ft_strcmp function with identical content";
		
		int result1 = strcmp(s1, s2);
		int result2 = ft_strcmp(s1, s2);
		
		int passed = (result1 == 0) && (result2 == 0);
		
		char details[200];
		snprintf(details, sizeof(details), "(long strings: expected 0, got %d)", result2);
		
		print_test_result("Long identical strings", passed, passed ? "" : details);
		stats->total++;
		if (passed) stats->passed++;
		else stats->failed++;
	}
	
	// Test 13: Difference at the end
	{
		const char *s1 = "HelloWorld";
		const char *s2 = "HelloWorle";
		
		int result1 = strcmp(s1, s2);
		int result2 = ft_strcmp(s1, s2);
		
		int passed = ((result1 == 0 && result2 == 0) || 
		              (result1 > 0 && result2 > 0) || 
		              (result1 < 0 && result2 < 0));
		
		char details[200];
		snprintf(details, sizeof(details), "(\"%s\" vs \"%s\": expected %s, got %s)", 
			s1, s2, result1 < 0 ? "negative" : "positive", result2 < 0 ? "negative" : "positive");
		
		print_test_result("Difference at the end", passed, passed ? "" : details);
		stats->total++;
		if (passed) stats->passed++;
		else stats->failed++;
	}
	
	// Test 14: Single character difference
	{
		const char *s1 = "a";
		const char *s2 = "b";
		
		int result1 = strcmp(s1, s2);
		int result2 = ft_strcmp(s1, s2);
		
		int passed = ((result1 == 0 && result2 == 0) || 
		              (result1 > 0 && result2 > 0) || 
		              (result1 < 0 && result2 < 0));
		
		char details[200];
		snprintf(details, sizeof(details), "(\"%s\" vs \"%s\": expected %s, got %s)", 
			s1, s2, result1 < 0 ? "negative" : "positive", result2 < 0 ? "negative" : "positive");
		
		print_test_result("Single character difference", passed, passed ? "" : details);
		stats->total++;
		if (passed) stats->passed++;
		else stats->failed++;
	}
	
	// Test 15: Same single character
	{
		const char *s1 = "x";
		const char *s2 = "x";
		
		int result1 = strcmp(s1, s2);
		int result2 = ft_strcmp(s1, s2);
		
		int passed = (result1 == 0) && (result2 == 0);
		
		char details[200];
		snprintf(details, sizeof(details), "(\"%s\" vs \"%s\": expected 0, got %d)", 
			s1, s2, result2);
		
		print_test_result("Same single character", passed, passed ? "" : details);
		stats->total++;
		if (passed) stats->passed++;
		else stats->failed++;
	}
	
	// Test 16: NULL first string (must segfault)
	{
		pid_t pid = fork();
		
		if (pid == 0)
		{
			char *null_ptr = NULL;
			const char *s2 = "Hello";
			volatile int result = ft_strcmp(null_ptr, s2);
			(void)result;
			exit(0);
		}
		else if (pid > 0)
		{
			int status;
			waitpid(pid, &status, 0);
			int ft_segfaulted = WIFSIGNALED(status) && WTERMSIG(status) == SIGSEGV;
			
			pid_t pid2 = fork();
			if (pid2 == 0)
			{
				char *null_ptr = NULL;
				const char *s2 = "Hello";
				volatile int result = strcmp(null_ptr, s2);
				(void)result;
				exit(0);
			}
			else if (pid2 > 0)
			{
				int status2;
				waitpid(pid2, &status2, 0);
				int libc_segfaulted = WIFSIGNALED(status2) && WTERMSIG(status2) == SIGSEGV;
				
				int passed = (ft_segfaulted && libc_segfaulted);
				
				char details[200];
				if (!ft_segfaulted && libc_segfaulted)
					snprintf(details, sizeof(details), "(ft_strcmp did not segfault on NULL s1)");
				else if (ft_segfaulted && !libc_segfaulted)
					snprintf(details, sizeof(details), "(ft_strcmp segfaulted but libc did not)");
				else if (!ft_segfaulted && !libc_segfaulted)
					snprintf(details, sizeof(details), "(both handled NULL s1 gracefully)");
				else
					details[0] = '\0';
				
				print_test_result("NULL first string (must segfault like libc)", passed, passed ? "" : details);
				stats->total++;
				if (passed) stats->passed++;
				else stats->failed++;
			}
		}
	}
	
	// Test 17: NULL second string (must segfault)
	{
		pid_t pid = fork();
		
		if (pid == 0)
		{
			const char *s1 = "Hello";
			char *null_ptr = NULL;
			volatile int result = ft_strcmp(s1, null_ptr);
			(void)result;
			exit(0);
		}
		else if (pid > 0)
		{
			int status;
			waitpid(pid, &status, 0);
			int ft_segfaulted = WIFSIGNALED(status) && WTERMSIG(status) == SIGSEGV;
			
			pid_t pid2 = fork();
			if (pid2 == 0)
			{
				const char *s1 = "Hello";
				char *null_ptr = NULL;
				volatile int result = strcmp(s1, null_ptr);
				(void)result;
				exit(0);
			}
			else if (pid2 > 0)
			{
				int status2;
				waitpid(pid2, &status2, 0);
				int libc_segfaulted = WIFSIGNALED(status2) && WTERMSIG(status2) == SIGSEGV;
				
				int passed = (ft_segfaulted && libc_segfaulted);
				
				char details[200];
				if (!ft_segfaulted && libc_segfaulted)
					snprintf(details, sizeof(details), "(ft_strcmp did not segfault on NULL s2)");
				else if (ft_segfaulted && !libc_segfaulted)
					snprintf(details, sizeof(details), "(ft_strcmp segfaulted but libc did not)");
				else if (!ft_segfaulted && !libc_segfaulted)
					snprintf(details, sizeof(details), "(both handled NULL s2 gracefully)");
				else
					details[0] = '\0';
				
				print_test_result("NULL second string (must segfault like libc)", passed, passed ? "" : details);
				stats->total++;
				if (passed) stats->passed++;
				else stats->failed++;
			}
		}
	}
	
	// Test 18: Both NULL (must segfault)
	{
		pid_t pid = fork();
		
		if (pid == 0)
		{
			char *null1 = NULL;
			char *null2 = NULL;
			volatile int result = ft_strcmp(null1, null2);
			(void)result;
			exit(0);
		}
		else if (pid > 0)
		{
			int status;
			waitpid(pid, &status, 0);
			int ft_segfaulted = WIFSIGNALED(status) && WTERMSIG(status) == SIGSEGV;
			
			pid_t pid2 = fork();
			if (pid2 == 0)
			{
				char *null1 = NULL;
				char *null2 = NULL;
				volatile int result = strcmp(null1, null2);
				(void)result;
				exit(0);
			}
			else if (pid2 > 0)
			{
				int status2;
				waitpid(pid2, &status2, 0);
				int libc_segfaulted = WIFSIGNALED(status2) && WTERMSIG(status2) == SIGSEGV;
				
				int passed = (ft_segfaulted && libc_segfaulted);
				
				char details[200];
				if (!ft_segfaulted && libc_segfaulted)
					snprintf(details, sizeof(details), "(ft_strcmp did not segfault on both NULL)");
				else if (ft_segfaulted && !libc_segfaulted)
					snprintf(details, sizeof(details), "(ft_strcmp segfaulted but libc did not)");
				else if (!ft_segfaulted && !libc_segfaulted)
					snprintf(details, sizeof(details), "(both handled both NULL gracefully)");
				else
					details[0] = '\0';
				
				print_test_result("Both strings NULL (must segfault like libc)", passed, passed ? "" : details);
				stats->total++;
				if (passed) stats->passed++;
				else stats->failed++;
			}
		}
	}
	
	if (g_verbose_mode)
		printf("\n");
}
