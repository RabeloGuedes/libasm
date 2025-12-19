#include "libtest.h"
#include <fcntl.h>
#include <unistd.h>
#include <string.h>
#include <errno.h>
#include <sys/wait.h>
#include <stdlib.h>

// External declaration - will be linked from libasm.a or weak wrapper
extern ssize_t ft_read(int fd, void *buf, size_t count);

void test_ft_read(t_test_stats *stats)
{
	print_header("Testing ft_read");
	
	// Check if function is implemented (detect weak wrapper)
	char test_buf[10];
	if (ft_read(0, test_buf, 0) == (ssize_t)0xDEADBEEF)
	{
		printf("%s  Function not found%s\n", COLOR_YELLOW, COLOR_RESET);
		return;
	}
	
	// Test 1: Read from file
	{
		const char *filename = "/tmp/test_ft_read.txt";
		const char *content = "Hello, World!";
		
		// Create test file
		int fd_write = open(filename, O_WRONLY | O_CREAT | O_TRUNC, 0644);
		write(fd_write, content, strlen(content));
		close(fd_write);
		
		// Read with libc
		char buf1[100] = {0};
		int fd1 = open(filename, O_RDONLY);
		ssize_t expected = read(fd1, buf1, strlen(content));
		close(fd1);
		
		// Read with ft_read
		char buf2[100] = {0};
		int fd2 = open(filename, O_RDONLY);
		ssize_t result = ft_read(fd2, buf2, strlen(content));
		close(fd2);
		
		int passed = (result == expected && strcmp(buf1, buf2) == 0);
		unlink(filename);
		print_test_result("Read from file", passed,
			passed ? "" : "Return values or content don't match");
		stats->total++;
		if (passed) stats->passed++;
		else stats->failed++;
	}
	
	// Test 2: Read empty file
	{
		const char *filename = "/tmp/test_ft_read_empty.txt";
		
		// Create empty file
		int fd_write = open(filename, O_WRONLY | O_CREAT | O_TRUNC, 0644);
		close(fd_write);
		
		// Read with libc
		char buf1[100] = {0};
		int fd1 = open(filename, O_RDONLY);
		ssize_t expected = read(fd1, buf1, 10);
		close(fd1);
		
		// Read with ft_read
		char buf2[100] = {0};
		int fd2 = open(filename, O_RDONLY);
		ssize_t result = ft_read(fd2, buf2, 10);
		close(fd2);
		
		int passed = (result == expected && result == 0);
		unlink(filename);
		print_test_result("Read empty file", passed,
			passed ? "" : "Should return 0 for EOF");
		stats->total++;
		if (passed) stats->passed++;
		else stats->failed++;
	}
	
	// Test 3: Read long content
	{
		const char *filename = "/tmp/test_ft_read_long.txt";
		char content[1001];
		memset(content, 'A', 1000);
		content[1000] = '\0';
		
		// Create test file
		int fd_write = open(filename, O_WRONLY | O_CREAT | O_TRUNC, 0644);
		write(fd_write, content, 1000);
		close(fd_write);
		
		// Read with libc
		char buf1[1001] = {0};
		int fd1 = open(filename, O_RDONLY);
		ssize_t expected = read(fd1, buf1, 1000);
		close(fd1);
		
		// Read with ft_read
		char buf2[1001] = {0};
		int fd2 = open(filename, O_RDONLY);
		ssize_t result = ft_read(fd2, buf2, 1000);
		close(fd2);
		
		int passed = (result == expected && memcmp(buf1, buf2, 1000) == 0);
		unlink(filename);
		print_test_result("Read long content (1000 chars)", passed,
			passed ? "" : "Return values or content don't match");
		stats->total++;
		if (passed) stats->passed++;
		else stats->failed++;
	}
	
	// Test 4: Read with count smaller than file size
	{
		const char *filename = "/tmp/test_ft_read_partial.txt";
		const char *content = "HelloWorld";
		
		// Create test file
		int fd_write = open(filename, O_WRONLY | O_CREAT | O_TRUNC, 0644);
		write(fd_write, content, strlen(content));
		close(fd_write);
		
		// Read with libc
		char buf1[100] = {0};
		int fd1 = open(filename, O_RDONLY);
		ssize_t expected = read(fd1, buf1, 5);
		close(fd1);
		
		// Read with ft_read
		char buf2[100] = {0};
		int fd2 = open(filename, O_RDONLY);
		ssize_t result = ft_read(fd2, buf2, 5);
		close(fd2);
		
		int passed = (result == expected && result == 5 && memcmp(buf1, buf2, 5) == 0);
		unlink(filename);
		print_test_result("Read partial content (5 bytes)", passed,
			passed ? "" : "Should read only 5 bytes");
		stats->total++;
		if (passed) stats->passed++;
		else stats->failed++;
	}
	
	// Test 5: Invalid file descriptor
	{
		char buf[100];
		errno = 0;
		ssize_t expected = read(-1, buf, 10);
		int expected_errno = errno;
		
		errno = 0;
		ssize_t result = ft_read(-1, buf, 10);
		int result_errno = errno;
		
		int passed = (result == expected && result == -1 && 
		              expected_errno == result_errno);
		char detail[200];
		if (!passed)
			snprintf(detail, sizeof(detail), 
				"Expected: %zd (errno %d), Got: %zd (errno %d)", 
				expected, expected_errno, result, result_errno);
		print_test_result("Invalid file descriptor (-1)", passed,
			passed ? "" : detail);
		stats->total++;
		if (passed) stats->passed++;
		else stats->failed++;
	}
	
	// Test 6: Read with count = 0
	{
		const char *filename = "/tmp/test_ft_read_zero.txt";
		const char *content = "Hello";
		
		// Create test file
		int fd_write = open(filename, O_WRONLY | O_CREAT | O_TRUNC, 0644);
		write(fd_write, content, strlen(content));
		close(fd_write);
		
		// Read with libc
		char buf1[100] = {0};
		int fd1 = open(filename, O_RDONLY);
		ssize_t expected = read(fd1, buf1, 0);
		close(fd1);
		
		// Read with ft_read
		char buf2[100] = {0};
		int fd2 = open(filename, O_RDONLY);
		ssize_t result = ft_read(fd2, buf2, 0);
		close(fd2);
		
		int passed = (result == expected && result == 0);
		unlink(filename);
		print_test_result("Read with count = 0", passed,
			passed ? "" : "Should return 0");
		stats->total++;
		if (passed) stats->passed++;
		else stats->failed++;
	}
	
	// Test 7: Read binary data
	{
		const char *filename = "/tmp/test_ft_read_binary.txt";
		unsigned char content[256];
		for (int i = 0; i < 256; i++)
			content[i] = (unsigned char)i;
		
		// Create test file
		int fd_write = open(filename, O_WRONLY | O_CREAT | O_TRUNC, 0644);
		write(fd_write, content, 256);
		close(fd_write);
		
		// Read with libc
		unsigned char buf1[256] = {0};
		int fd1 = open(filename, O_RDONLY);
		ssize_t expected = read(fd1, buf1, 256);
		close(fd1);
		
		// Read with ft_read
		unsigned char buf2[256] = {0};
		int fd2 = open(filename, O_RDONLY);
		ssize_t result = ft_read(fd2, buf2, 256);
		close(fd2);
		
		int passed = (result == expected && memcmp(buf1, buf2, 256) == 0);
		unlink(filename);
		print_test_result("Read binary data (256 bytes)", passed,
			passed ? "" : "Binary data doesn't match");
		stats->total++;
		if (passed) stats->passed++;
		else stats->failed++;
	}
	
	// Test 8: Multiple reads from same file
	{
		const char *filename = "/tmp/test_ft_read_multi.txt";
		const char *content = "HelloWorld";
		
		// Create test file
		int fd_write = open(filename, O_WRONLY | O_CREAT | O_TRUNC, 0644);
		write(fd_write, content, strlen(content));
		close(fd_write);
		
		// Read with libc
		char buf1[6] = {0};
		char buf2[6] = {0};
		int fd1 = open(filename, O_RDONLY);
		read(fd1, buf1, 5);
		read(fd1, buf2, 5);
		close(fd1);
		
		// Read with ft_read
		char buf3[6] = {0};
		char buf4[6] = {0};
		int fd2 = open(filename, O_RDONLY);
		ft_read(fd2, buf3, 5);
		ft_read(fd2, buf4, 5);
		close(fd2);
		
		int passed = (strcmp(buf1, buf3) == 0 && strcmp(buf2, buf4) == 0);
		unlink(filename);
		print_test_result("Multiple reads from same file", passed,
			passed ? "" : "Sequential reads don't match");
		stats->total++;
		if (passed) stats->passed++;
		else stats->failed++;
	}
	
	// Test 9: Read from closed file descriptor
	{
		const char *filename = "/tmp/test_ft_read_closed.txt";
		const char *content = "Hello";
		
		// Create test file
		int fd_write = open(filename, O_WRONLY | O_CREAT | O_TRUNC, 0644);
		write(fd_write, content, strlen(content));
		close(fd_write);
		
		char buf[100];
		int fd = open(filename, O_RDONLY);
		close(fd);
		
		errno = 0;
		ssize_t expected = read(fd, buf, 10);
		int expected_errno = errno;
		
		errno = 0;
		ssize_t result = ft_read(fd, buf, 10);
		int result_errno = errno;
		
		int passed = (result == expected && result == -1 && 
		              expected_errno == result_errno);
		unlink(filename);
		char detail[200];
		if (!passed)
			snprintf(detail, sizeof(detail), 
				"Expected: %zd (errno %d), Got: %zd (errno %d)", 
				expected, expected_errno, result, result_errno);
		print_test_result("Read from closed file descriptor", passed,
			passed ? "" : detail);
		stats->total++;
		if (passed) stats->passed++;
		else stats->failed++;
	}
	
	// Test 10: Read with special characters
	{
		const char *filename = "/tmp/test_ft_read_special.txt";
		const char *content = "Tab\tNewline\nReturn\rNull";
		
		// Create test file
		int fd_write = open(filename, O_WRONLY | O_CREAT | O_TRUNC, 0644);
		write(fd_write, content, strlen(content));
		close(fd_write);
		
		// Read with libc
		char buf1[100] = {0};
		int fd1 = open(filename, O_RDONLY);
		ssize_t expected = read(fd1, buf1, strlen(content));
		close(fd1);
		
		// Read with ft_read
		char buf2[100] = {0};
		int fd2 = open(filename, O_RDONLY);
		ssize_t result = ft_read(fd2, buf2, strlen(content));
		close(fd2);
		
		int passed = (result == expected && strcmp(buf1, buf2) == 0);
		unlink(filename);
		print_test_result("Read with special characters", passed,
			passed ? "" : "Special characters don't match");
		stats->total++;
		if (passed) stats->passed++;
		else stats->failed++;
	}
	
	// Test 11: NULL buffer (must segfault like libc)
	{
		const char *filename = "/tmp/test_ft_read_null.txt";
		const char *content = "Hello";
		
		// Create test file
		int fd_write = open(filename, O_WRONLY | O_CREAT | O_TRUNC, 0644);
		write(fd_write, content, strlen(content));
		close(fd_write);
		
		// Use volatile to prevent compiler from detecting NULL at compile time
		volatile void *null_ptr = NULL;
		
		pid_t pid = fork();
		if (pid == 0)
		{
			// Child process - test ft_read
			int fd = open(filename, O_RDONLY);
			ft_read(fd, (void *)null_ptr, 10);
			close(fd);
			exit(0);
		}
		else if (pid > 0)
		{
			// Parent process
			int status;
			waitpid(pid, &status, 0);
			int ft_segfaulted = WIFSIGNALED(status) && WTERMSIG(status) == SIGSEGV;
			
			// Test libc read
			pid = fork();
			if (pid == 0)
			{
				int fd = open(filename, O_RDONLY);
				read(fd, (void *)null_ptr, 10);
				close(fd);
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
							"ft_read did not segfault on NULL buffer");
					else
						snprintf(detail, sizeof(detail), 
							"ft_read segfaulted but libc read did not");
				}
				print_test_result("NULL buffer (must segfault like libc)", 
					passed, passed ? "" : detail);
				stats->total++;
				if (passed) stats->passed++;
				else stats->failed++;
			}
		}
		unlink(filename);
	}
	
	if (g_verbose_mode)
		printf("\n");
}
