#include "libtest.h"
#include <signal.h>
#include <sys/wait.h>

// External declaration - will be linked from libasm.a or weak wrapper
extern char *ft_strcpy(char *dst, const char *src);

void test_ft_strcpy(t_test_stats *stats)
{
	print_header("Testing ft_strcpy");
	
	// Check if function is implemented (detect weak wrapper)
	char test_dst[10];
	if (ft_strcpy(test_dst, "test") == (char *)0xDEADBEEF)
	{
		printf("%s  Function not found%s\n", COLOR_YELLOW, COLOR_RESET);
		return;
	}
	
	// Test 1: Simple copy
	{
		char dst1[50] = {0};
		char dst2[50] = {0};
		const char *src = "Hello";
		
		char *result1 = strcpy(dst1, src);
		char *result2 = ft_strcpy(dst2, src);
		
		int passed = (strcmp(dst1, dst2) == 0) && (result1 == dst1) && (result2 == dst2);
		
		char details[200];
		snprintf(details, sizeof(details), "(\"%s\": expected \"%s\", got \"%s\")", 
			src, dst1, dst2);
		
		print_test_result("Simple copy", passed, passed ? "" : details);
		stats->total++;
		if (passed) stats->passed++;
		else stats->failed++;
	}
	
	// Test 2: Empty string
	{
		char dst1[50] = {0};
		char dst2[50] = {0};
		const char *src = "";
		
		char *result1 = strcpy(dst1, src);
		char *result2 = ft_strcpy(dst2, src);
		
		int passed = (strcmp(dst1, dst2) == 0) && (result1 == dst1) && (result2 == dst2);
		
		char details[200];
		snprintf(details, sizeof(details), "(empty string: expected \"%s\", got \"%s\")", 
			dst1, dst2);
		
		print_test_result("Empty string", passed, passed ? "" : details);
		stats->total++;
		if (passed) stats->passed++;
		else stats->failed++;
	}
	
	// Test 3: Long string
	{
		char dst1[200] = {0};
		char dst2[200] = {0};
		const char *src = "This is a much longer string to test the ft_strcpy function properly";
		
		char *result1 = strcpy(dst1, src);
		char *result2 = ft_strcpy(dst2, src);
		
		int passed = (strcmp(dst1, dst2) == 0) && (result1 == dst1) && (result2 == dst2);
		
		char details[200];
		if (!passed)
			snprintf(details, sizeof(details), "(long string copy failed)");
		else
			details[0] = '\0';
		
		print_test_result("Long string", passed, passed ? "" : details);
		stats->total++;
		if (passed) stats->passed++;
		else stats->failed++;
	}
	
	// Test 4: String with special characters
	{
		char dst1[100] = {0};
		char dst2[100] = {0};
		const char *src = "Hello\tWorld\n!@#$%^&*()";
		
		strcpy(dst1, src);
		ft_strcpy(dst2, src);
		
		int passed = (strcmp(dst1, dst2) == 0);
		
		char details[200];
		if (!passed)
			snprintf(details, sizeof(details), "(special chars: strings don't match)");
		else
			details[0] = '\0';
		
		print_test_result("String with special characters", passed, passed ? "" : details);
		stats->total++;
		if (passed) stats->passed++;
		else stats->failed++;
	}
	
	// Test 5: Single character
	{
		char dst1[10] = {0};
		char dst2[10] = {0};
		const char *src = "A";
		
		strcpy(dst1, src);
		ft_strcpy(dst2, src);
		
		int passed = (strcmp(dst1, dst2) == 0);
		
		char details[200];
		snprintf(details, sizeof(details), "(\"%s\": expected \"%s\", got \"%s\")", 
			src, dst1, dst2);
		
		print_test_result("Single character", passed, passed ? "" : details);
		stats->total++;
		if (passed) stats->passed++;
		else stats->failed++;
	}
	
	// Test 6: Dst same size as src
	{
		char dst1[6] = {0};
		char dst2[6] = {0};
		const char *src = "Hello";
		
		strcpy(dst1, src);
		ft_strcpy(dst2, src);
		
		int passed = (strcmp(dst1, dst2) == 0);
		
		char details[200];
		snprintf(details, sizeof(details), "(same size: expected \"%s\", got \"%s\")", 
			dst1, dst2);
		
		print_test_result("Dst same size as src", passed, passed ? "" : details);
		stats->total++;
		if (passed) stats->passed++;
		else stats->failed++;
	}
	
	// Test 7: Dst bigger than src
	{
		char dst1[100] = "XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX";
		char dst2[100] = "XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX";
		const char *src = "Hi";
		
		strcpy(dst1, src);
		ft_strcpy(dst2, src);
		
		// Check that string was copied and null terminator placed correctly
		int passed = (strcmp(dst1, dst2) == 0) && (strlen(dst1) == strlen(src));
		
		char details[300];
		snprintf(details, sizeof(details), "(dst bigger: expected \"%s\", got \"%s\")", 
			dst1, dst2);
		
		print_test_result("Dst bigger than src", passed, passed ? "" : details);
		stats->total++;
		if (passed) stats->passed++;
		else stats->failed++;
	}
	
	// Test 8: String with numbers
	{
		char dst1[50] = {0};
		char dst2[50] = {0};
		const char *src = "1234567890";
		
		strcpy(dst1, src);
		ft_strcpy(dst2, src);
		
		int passed = (strcmp(dst1, dst2) == 0);
		
		char details[200];
		snprintf(details, sizeof(details), "(\"%s\": expected \"%s\", got \"%s\")", 
			src, dst1, dst2);
		
		print_test_result("String with numbers", passed, passed ? "" : details);
		stats->total++;
		if (passed) stats->passed++;
		else stats->failed++;
	}
	
	// Test 9: Very long string (stress test)
	{
		char dst1[1050] = {0};
		char dst2[1050] = {0};
		char src[1001];
		memset(src, 'A', 1000);
		src[1000] = '\0';
		
		strcpy(dst1, src);
		ft_strcpy(dst2, src);
		
		int passed = (strcmp(dst1, dst2) == 0);
		
		char details[200];
		if (!passed)
			snprintf(details, sizeof(details), "(1000 chars: copy failed)");
		else
			details[0] = '\0';
		
		print_test_result("Very long string (1000 chars)", passed, passed ? "" : details);
		stats->total++;
		if (passed) stats->passed++;
		else stats->failed++;
	}
	
	// Test 10: String with only spaces
	{
		char dst1[20] = {0};
		char dst2[20] = {0};
		const char *src = "     ";
		
		strcpy(dst1, src);
		ft_strcpy(dst2, src);
		
		int passed = (strcmp(dst1, dst2) == 0);
		
		char details[200];
		snprintf(details, sizeof(details), "(5 spaces: expected \"%s\", got \"%s\")", 
			dst1, dst2);
		
		print_test_result("String with only spaces", passed, passed ? "" : details);
		stats->total++;
		if (passed) stats->passed++;
		else stats->failed++;
	}
	
	// Test 11: NULL src pointer (must segfault)
	{
		pid_t pid = fork();
		
		if (pid == 0)
		{
			char dst[50];
			char *null_ptr = NULL;
			volatile char *result = ft_strcpy(dst, null_ptr);
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
				char dst[50];
				char *null_ptr = NULL;
				volatile char *result = strcpy(dst, null_ptr);
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
					snprintf(details, sizeof(details), "(ft_strcpy did not segfault on NULL src)");
				else if (ft_segfaulted && !libc_segfaulted)
					snprintf(details, sizeof(details), "(ft_strcpy segfaulted but libc did not)");
				else if (!ft_segfaulted && !libc_segfaulted)
					snprintf(details, sizeof(details), "(both handled NULL src gracefully)");
				else
					details[0] = '\0';
				
				print_test_result("NULL src pointer (must segfault like libc)", passed, passed ? "" : details);
				stats->total++;
				if (passed) stats->passed++;
				else stats->failed++;
			}
		}
	}
	
	// Test 12: NULL dst pointer (must segfault)
	{
		pid_t pid = fork();
		
		if (pid == 0)
		{
			char *null_ptr = NULL;
			const char *src = "Hello";
			volatile char *result = ft_strcpy(null_ptr, src);
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
				const char *src = "Hello";
				volatile char *result = strcpy(null_ptr, src);
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
					snprintf(details, sizeof(details), "(ft_strcpy did not segfault on NULL dst)");
				else if (ft_segfaulted && !libc_segfaulted)
					snprintf(details, sizeof(details), "(ft_strcpy segfaulted but libc did not)");
				else if (!ft_segfaulted && !libc_segfaulted)
					snprintf(details, sizeof(details), "(both handled NULL dst gracefully)");
				else
					details[0] = '\0';
				
				print_test_result("NULL dst pointer (must segfault like libc)", passed, passed ? "" : details);
				stats->total++;
				if (passed) stats->passed++;
				else stats->failed++;
			}
		}
	}
	
	// Test 13: Both NULL (must segfault)
	{
		pid_t pid = fork();
		
		if (pid == 0)
		{
			char *null_dst = NULL;
			char *null_src = NULL;
			volatile char *result = ft_strcpy(null_dst, null_src);
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
				char *null_dst = NULL;
				char *null_src = NULL;
				volatile char *result = strcpy(null_dst, null_src);
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
					snprintf(details, sizeof(details), "(ft_strcpy did not segfault on both NULL)");
				else if (ft_segfaulted && !libc_segfaulted)
					snprintf(details, sizeof(details), "(ft_strcpy segfaulted but libc did not)");
				else if (!ft_segfaulted && !libc_segfaulted)
					snprintf(details, sizeof(details), "(both handled both NULL gracefully)");
				else
					details[0] = '\0';
				
				print_test_result("Both pointers NULL (must segfault like libc)", passed, passed ? "" : details);
				stats->total++;
				if (passed) stats->passed++;
				else stats->failed++;
			}
		}
	}
	
	// Test 14: Buffer overflow (must segfault or crash)
	{
		pid_t pid = fork();
		
		if (pid == 0)
		{
			// Deliberately create a buffer overflow scenario
			// Use malloc to avoid compile-time overflow detection
			char *dst = malloc(5);
			if (dst)
			{
				// Create source string dynamically to avoid compiler warnings
				char src[60];
				strcpy(src, "This is a very long string that will overflow");
				volatile char *result = ft_strcpy(dst, src);
				(void)result;
				free(dst);
			}
			exit(0);
		}
		else if (pid > 0)
		{
			int status;
			waitpid(pid, &status, 0);
			// Check if it crashed (segfault, abort, or any signal)
			int ft_crashed = WIFSIGNALED(status);
			
			pid_t pid2 = fork();
			if (pid2 == 0)
			{
				char *dst = malloc(5);
				if (dst)
				{
					char src[60];
					strcpy(src, "This is a very long string that will overflow");
					volatile char *result = strcpy(dst, src);
					(void)result;
					free(dst);
				}
				exit(0);
			}
			else if (pid2 > 0)
			{
				int status2;
				waitpid(pid2, &status2, 0);
				int libc_crashed = WIFSIGNALED(status2);
				
				// Both should behave the same way (crash or not crash)
				int passed = (ft_crashed == libc_crashed);
				
				char details[200];
				if (!ft_crashed && libc_crashed)
					snprintf(details, sizeof(details), "(ft_strcpy did not crash on overflow, but libc did)");
				else if (ft_crashed && !libc_crashed)
					snprintf(details, sizeof(details), "(ft_strcpy crashed but libc did not)");
				else if (!ft_crashed && !libc_crashed)
					snprintf(details, sizeof(details), "(both allowed overflow without crashing)");
				else
					details[0] = '\0';
				
				print_test_result("Buffer overflow (behavior must match libc)", passed, passed ? "" : details);
				stats->total++;
				if (passed) stats->passed++;
				else stats->failed++;
			}
		}
	}
	
	if (g_verbose_mode)
		printf("\n");
}
