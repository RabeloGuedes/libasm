#include "libtest.h"
#include <limits.h>
#include <sys/wait.h>

// Weak symbol sentinel
#define WEAK_SENTINEL ((int)0xDEADBEEF)

// Function prototype (declared as weak in libasm_weak.c)
extern int ft_atoi_base(char *str, char *base);

// Reference implementation for comparison
static int is_valid_base(char *base)
{
	int i, j;
	int len = 0;
	
	if (!base)
		return 0;
	
	// Count base length and check for invalid chars
	while (base[len])
	{
		if (base[len] == '+' || base[len] == '-' || base[len] <= 32 || base[len] == 127)
			return 0;
		len++;
	}
	
	// Base must be at least 2
	if (len < 2)
		return 0;
	
	// Check for duplicates
	for (i = 0; i < len; i++)
	{
		for (j = i + 1; j < len; j++)
		{
			if (base[i] == base[j])
				return 0;
		}
	}
	
	return len;
}

static int get_digit_value(char c, char *base, int base_len)
{
	int i;
	
	for (i = 0; i < base_len; i++)
	{
		if (base[i] == c)
			return i;
	}
	return -1;
}

static int ref_atoi_base(char *str, char *base)
{
	int base_len = is_valid_base(base);
	int result = 0;
	int sign = 1;
	int i = 0;
	int digit;
	
	if (base_len == 0)
		return 0;
	
	if (!str)
		return 0;
	
	// Skip whitespace
	while (str[i] == ' ' || str[i] == '\t' || str[i] == '\n' ||
		   str[i] == '\v' || str[i] == '\f' || str[i] == '\r')
		i++;
	
	// Handle sign (only one allowed, like standard atoi)
	if (str[i] == '+' || str[i] == '-')
	{
		if (str[i] == '-')
			sign = -1;
		i++;
		// If there's another sign character, return 0 (invalid)
		if (str[i] == '+' || str[i] == '-')
			return 0;
	}
	
	// Convert
	while (str[i])
	{
		digit = get_digit_value(str[i], base, base_len);
		if (digit == -1)
			break;
		result = result * base_len + digit;
		i++;
	}
	
	return result * sign;
}

void test_ft_atoi_base(t_test_stats *stats)
{
	char detail[256];
	int ft_result, ref_result;
	int passed;
	
	print_header("Testing ft_atoi_base [BONUS]");
	
	// Check if function exists (weak symbol detection)
	ft_result = ft_atoi_base("0", "01");
	if (ft_result == WEAK_SENTINEL)
	{
		printf("%s  Function not found or not compiled with 'make bonus'%s\n", COLOR_YELLOW, COLOR_RESET);
		if (g_verbose_mode)
			printf("\n");
		return;
	}
	
	// Test 1: Simple binary conversion
	ft_result = ft_atoi_base("101", "01");
	ref_result = ref_atoi_base("101", "01");
	passed = (ft_result == ref_result);
	snprintf(detail, sizeof(detail), "(\"101\" base \"01\": expected %d, got %d)", ref_result, ft_result);
	print_test_result("Binary: 101 = 5", passed, detail);
	stats->total++;
	if (passed) stats->passed++;
	else stats->failed++;
	
	// Test 2: Decimal conversion
	ft_result = ft_atoi_base("42", "0123456789");
	ref_result = ref_atoi_base("42", "0123456789");
	passed = (ft_result == ref_result);
	snprintf(detail, sizeof(detail), "(\"42\" base \"0123456789\": expected %d, got %d)", ref_result, ft_result);
	print_test_result("Decimal: 42", passed, detail);
	stats->total++;
	if (passed) stats->passed++;
	else stats->failed++;
	
	// Test 3: Hexadecimal lowercase
	ft_result = ft_atoi_base("2a", "0123456789abcdef");
	ref_result = ref_atoi_base("2a", "0123456789abcdef");
	passed = (ft_result == ref_result);
	snprintf(detail, sizeof(detail), "(\"2a\" base \"0123456789abcdef\": expected %d, got %d)", ref_result, ft_result);
	print_test_result("Hexadecimal: 2a = 42", passed, detail);
	stats->total++;
	if (passed) stats->passed++;
	else stats->failed++;
	
	// Test 4: Hexadecimal uppercase
	ft_result = ft_atoi_base("FF", "0123456789ABCDEF");
	ref_result = ref_atoi_base("FF", "0123456789ABCDEF");
	passed = (ft_result == ref_result);
	snprintf(detail, sizeof(detail), "(\"FF\" base \"0123456789ABCDEF\": expected %d, got %d)", ref_result, ft_result);
	print_test_result("Hexadecimal: FF = 255", passed, detail);
	stats->total++;
	if (passed) stats->passed++;
	else stats->failed++;
	
	// Test 5: Negative number
	ft_result = ft_atoi_base("-101", "01");
	ref_result = ref_atoi_base("-101", "01");
	passed = (ft_result == ref_result);
	snprintf(detail, sizeof(detail), "(\"-101\" base \"01\": expected %d, got %d)", ref_result, ft_result);
	print_test_result("Negative binary: -101 = -5", passed, detail);
	stats->total++;
	if (passed) stats->passed++;
	else stats->failed++;
	
	// Test 6: Multiple signs
	ft_result = ft_atoi_base("--42", "0123456789");
	ref_result = ref_atoi_base("--42", "0123456789");
	passed = (ft_result == ref_result);
	snprintf(detail, sizeof(detail), "(\"--42\" base \"0123456789\": expected %d, got %d)", ref_result, ft_result);
	print_test_result("Multiple signs: --42 = 0", passed, detail);
	stats->total++;
	if (passed) stats->passed++;
	else stats->failed++;
	
	// Test 7: With leading whitespace
	ft_result = ft_atoi_base("   \t\n42", "0123456789");
	ref_result = ref_atoi_base("   \t\n42", "0123456789");
	passed = (ft_result == ref_result);
	snprintf(detail, sizeof(detail), "(\"   \\t\\n42\" base \"0123456789\": expected %d, got %d)", ref_result, ft_result);
	print_test_result("Leading whitespace", passed, detail);
	stats->total++;
	if (passed) stats->passed++;
	else stats->failed++;
	
	// Test 8: Invalid base (too short)
	ft_result = ft_atoi_base("5", "0");
	ref_result = ref_atoi_base("5", "0");
	passed = (ft_result == ref_result);
	snprintf(detail, sizeof(detail), "(\"5\" base \"0\": expected %d, got %d)", ref_result, ft_result);
	print_test_result("Invalid base (single char)", passed, detail);
	stats->total++;
	if (passed) stats->passed++;
	else stats->failed++;
	
	// Test 9: Invalid base (duplicates)
	ft_result = ft_atoi_base("5", "0123456709");
	ref_result = ref_atoi_base("5", "0123456709");
	passed = (ft_result == ref_result);
	snprintf(detail, sizeof(detail), "(\"5\" base \"0123456709\": expected %d, got %d)", ref_result, ft_result);
	print_test_result("Invalid base (duplicates)", passed, detail);
	stats->total++;
	if (passed) stats->passed++;
	else stats->failed++;
	
	// Test 10: Invalid base (contains +)
	ft_result = ft_atoi_base("5", "01+3");
	ref_result = ref_atoi_base("5", "01+3");
	passed = (ft_result == ref_result);
	snprintf(detail, sizeof(detail), "(\"5\" base \"01+3\": expected %d, got %d)", ref_result, ft_result);
	print_test_result("Invalid base (contains +)", passed, detail);
	stats->total++;
	if (passed) stats->passed++;
	else stats->failed++;
	
	// Test 11: Invalid base (contains -)
	ft_result = ft_atoi_base("5", "01-3");
	ref_result = ref_atoi_base("5", "01-3");
	passed = (ft_result == ref_result);
	snprintf(detail, sizeof(detail), "(\"5\" base \"01-3\": expected %d, got %d)", ref_result, ft_result);
	print_test_result("Invalid base (contains -)", passed, detail);
	stats->total++;
	if (passed) stats->passed++;
	else stats->failed++;
	
	// Test 12: Invalid base (contains whitespace)
	ft_result = ft_atoi_base("5", "01 3");
	ref_result = ref_atoi_base("5", "01 3");
	passed = (ft_result == ref_result);
	snprintf(detail, sizeof(detail), "(\"5\" base \"01 3\": expected %d, got %d)", ref_result, ft_result);
	print_test_result("Invalid base (contains space)", passed, detail);
	stats->total++;
	if (passed) stats->passed++;
	else stats->failed++;
	
	// Test 13: Octal conversion
	ft_result = ft_atoi_base("52", "01234567");
	ref_result = ref_atoi_base("52", "01234567");
	passed = (ft_result == ref_result);
	snprintf(detail, sizeof(detail), "(\"52\" base \"01234567\": expected %d, got %d)", ref_result, ft_result);
	print_test_result("Octal: 52 = 42", passed, detail);
	stats->total++;
	if (passed) stats->passed++;
	else stats->failed++;
	
	// Test 14: Custom base (poney)
	ft_result = ft_atoi_base("yep", "poney");
	ref_result = ref_atoi_base("yep", "poney");
	passed = (ft_result == ref_result);
	snprintf(detail, sizeof(detail), "(\"yep\" base \"poney\": expected %d, got %d)", ref_result, ft_result);
	print_test_result("Custom base: yep in poney = 103", passed, detail);
	stats->total++;
	if (passed) stats->passed++;
	else stats->failed++;
	
	// Test 15: Custom base (special symbols)
	ft_result = ft_atoi_base("!@!", "!@#$%");
	ref_result = ref_atoi_base("!@!", "!@#$%");
	passed = (ft_result == ref_result);
	snprintf(detail, sizeof(detail), "(\"!@!\" base \"!@#$%%\": expected %d, got %d)", ref_result, ft_result);
	print_test_result("Custom base: !@! in symbols (base-5)", passed, detail);
	stats->total++;
	if (passed) stats->passed++;
	else stats->failed++;
	
	// Test 16: Custom base (alphabet)
	ft_result = ft_atoi_base("hello", "abcdefghijklmnopqrstuvwxyz");
	ref_result = ref_atoi_base("hello", "abcdefghijklmnopqrstuvwxyz");
	passed = (ft_result == ref_result);
	snprintf(detail, sizeof(detail), "(\"hello\" base \"abc...xyz\": expected %d, got %d)", ref_result, ft_result);
	print_test_result("Custom base: hello in alphabet", passed, detail);
	stats->total++;
	if (passed) stats->passed++;
	else stats->failed++;
	
	// Test 17: Custom base (vowels)
	ft_result = ft_atoi_base("ouuio", "aeiou");
	ref_result = ref_atoi_base("ouuio", "aeiou");
	passed = (ft_result == ref_result);
	snprintf(detail, sizeof(detail), "(\"ouuio\" base \"aeiou\": expected %d, got %d)", ref_result, ft_result);
	print_test_result("Custom base: ouuio in aeiou", passed, detail);
	stats->total++;
	if (passed) stats->passed++;
	else stats->failed++;
	
	// Test 18: NULL string pointer (fork-protected)
	{
		pid_t pid = fork();
		if (pid == 0)
		{
			volatile int result = ft_atoi_base(NULL, "0123456789");
			(void)result;
			exit(0);
		}
		else if (pid > 0)
		{
			int status;
			waitpid(pid, &status, 0);
			int ft_crashed = WIFSIGNALED(status);
			
			pid_t pid2 = fork();
			if (pid2 == 0)
			{
				volatile int result = ref_atoi_base(NULL, "0123456789");
				(void)result;
				exit(0);
			}
			else if (pid2 > 0)
			{
				int status2;
				waitpid(pid2, &status2, 0);
				int ref_crashed = WIFSIGNALED(status2);
				
				passed = (ft_crashed == ref_crashed);
				if (ft_crashed && ref_crashed)
					snprintf(detail, sizeof(detail), "(NULL string: both segfault as expected)");
				else if (!ft_crashed && !ref_crashed)
					snprintf(detail, sizeof(detail), "(NULL string: both return 0 safely)");
				else if (ft_crashed)
					snprintf(detail, sizeof(detail), "(NULL string: ft segfaults but ref returns 0)");
				else
					snprintf(detail, sizeof(detail), "(NULL string: ft returns 0 but ref segfaults)");
			}
			else
			{
				passed = 0;
				snprintf(detail, sizeof(detail), "(fork failed for ref test)");
			}
		}
		else
		{
			passed = 0;
			snprintf(detail, sizeof(detail), "(fork failed for ft test)");
		}
		print_test_result("NULL string pointer (protected)", passed, detail);
		stats->total++;
		if (passed) stats->passed++;
		else stats->failed++;
	}
	
	// Test 19: NULL base pointer (fork-protected)
	{
		pid_t pid = fork();
		if (pid == 0)
		{
			volatile int result = ft_atoi_base("42", NULL);
			(void)result;
			exit(0);
		}
		else if (pid > 0)
		{
			int status;
			waitpid(pid, &status, 0);
			int ft_crashed = WIFSIGNALED(status);
			
			pid_t pid2 = fork();
			if (pid2 == 0)
			{
				volatile int result = ref_atoi_base("42", NULL);
				(void)result;
				exit(0);
			}
			else if (pid2 > 0)
			{
				int status2;
				waitpid(pid2, &status2, 0);
				int ref_crashed = WIFSIGNALED(status2);
				
				passed = (ft_crashed == ref_crashed);
				if (ft_crashed && ref_crashed)
					snprintf(detail, sizeof(detail), "(NULL base: both segfault as expected)");
				else if (!ft_crashed && !ref_crashed)
					snprintf(detail, sizeof(detail), "(NULL base: both return 0 safely)");
				else if (ft_crashed)
					snprintf(detail, sizeof(detail), "(NULL base: ft segfaults but ref returns 0)");
				else
					snprintf(detail, sizeof(detail), "(NULL base: ft returns 0 but ref segfaults)");
			}
			else
			{
				passed = 0;
				snprintf(detail, sizeof(detail), "(fork failed for ref test)");
			}
		}
		else
		{
			passed = 0;
			snprintf(detail, sizeof(detail), "(fork failed for ft test)");
		}
		print_test_result("NULL base pointer (protected)", passed, detail);
		stats->total++;
		if (passed) stats->passed++;
		else stats->failed++;
	}
	
	// Test 20: Both NULL pointers (fork-protected)
	{
		pid_t pid = fork();
		if (pid == 0)
		{
			volatile int result = ft_atoi_base(NULL, NULL);
			(void)result;
			exit(0);
		}
		else if (pid > 0)
		{
			int status;
			waitpid(pid, &status, 0);
			int ft_crashed = WIFSIGNALED(status);
			
			pid_t pid2 = fork();
			if (pid2 == 0)
			{
				volatile int result = ref_atoi_base(NULL, NULL);
				(void)result;
				exit(0);
			}
			else if (pid2 > 0)
			{
				int status2;
				waitpid(pid2, &status2, 0);
				int ref_crashed = WIFSIGNALED(status2);
				
				passed = (ft_crashed == ref_crashed);
				if (ft_crashed && ref_crashed)
					snprintf(detail, sizeof(detail), "(both NULL: both segfault as expected)");
				else if (!ft_crashed && !ref_crashed)
					snprintf(detail, sizeof(detail), "(both NULL: both return 0 safely)");
				else if (ft_crashed)
					snprintf(detail, sizeof(detail), "(both NULL: ft segfaults but ref returns 0)");
				else
					snprintf(detail, sizeof(detail), "(both NULL: ft returns 0 but ref segfaults)");
			}
			else
			{
				passed = 0;
				snprintf(detail, sizeof(detail), "(fork failed for ref test)");
			}
		}
		else
		{
			passed = 0;
			snprintf(detail, sizeof(detail), "(fork failed for ft test)");
		}
		print_test_result("Both NULL pointers (protected)", passed, detail);
		stats->total++;
		if (passed) stats->passed++;
		else stats->failed++;
	}
	
	// Test 21: Zero
	ft_result = ft_atoi_base("0", "0123456789");
	ref_result = ref_atoi_base("0", "0123456789");
	passed = (ft_result == ref_result);
	snprintf(detail, sizeof(detail), "(\"0\" base \"0123456789\": expected %d, got %d)", ref_result, ft_result);
	print_test_result("Zero", passed, detail);
	stats->total++;
	if (passed) stats->passed++;
	else stats->failed++;
	
	// Test 22: Empty string
	ft_result = ft_atoi_base("", "0123456789");
	ref_result = ref_atoi_base("", "0123456789");
	passed = (ft_result == ref_result);
	snprintf(detail, sizeof(detail), "(\"\" base \"0123456789\": expected %d, got %d)", ref_result, ft_result);
	print_test_result("Empty string", passed, detail);
	stats->total++;
	if (passed) stats->passed++;
	else stats->failed++;
	
	// Test 23: String with invalid chars (stops at first invalid)
	ft_result = ft_atoi_base("101x", "01");
	ref_result = ref_atoi_base("101x", "01");
	passed = (ft_result == ref_result);
	snprintf(detail, sizeof(detail), "(\"101x\" base \"01\": expected %d, got %d)", ref_result, ft_result);
	print_test_result("Invalid char (stops at x)", passed, detail);
	stats->total++;
	if (passed) stats->passed++;
	else stats->failed++;
	
	// Test 24: Only whitespace
	ft_result = ft_atoi_base("   \t\n", "0123456789");
	ref_result = ref_atoi_base("   \t\n", "0123456789");
	passed = (ft_result == ref_result);
	snprintf(detail, sizeof(detail), "(\"   \\t\\n\" base \"0123456789\": expected %d, got %d)", ref_result, ft_result);
	print_test_result("Only whitespace", passed, detail);
	stats->total++;
	if (passed) stats->passed++;
	else stats->failed++;
	
	// Test 25: Large number
	ft_result = ft_atoi_base("7FFFFFFF", "0123456789ABCDEF");
	ref_result = ref_atoi_base("7FFFFFFF", "0123456789ABCDEF");
	passed = (ft_result == ref_result);
	snprintf(detail, sizeof(detail), "(\"7FFFFFFF\" base \"0123456789ABCDEF\": expected %d, got %d)", ref_result, ft_result);
	print_test_result("Large number: 7FFFFFFF", passed, detail);
	stats->total++;
	if (passed) stats->passed++;
	else stats->failed++;
	
	// Test 26: Sign only
	ft_result = ft_atoi_base("-", "0123456789");
	ref_result = ref_atoi_base("-", "0123456789");
	passed = (ft_result == ref_result);
	snprintf(detail, sizeof(detail), "(\"-\" base \"0123456789\": expected %d, got %d)", ref_result, ft_result);
	print_test_result("Sign only", passed, detail);
	stats->total++;
	if (passed) stats->passed++;
	else stats->failed++;
	
	// Test 27: Ternary base (base 3)
	ft_result = ft_atoi_base("120", "012");
	ref_result = ref_atoi_base("120", "012");
	passed = (ft_result == ref_result);
	snprintf(detail, sizeof(detail), "(\"120\" base \"012\": expected %d, got %d)", ref_result, ft_result);
	print_test_result("Ternary: 120 = 15", passed, detail);
	stats->total++;
	if (passed) stats->passed++;
	else stats->failed++;
	
	// Test 28: Base 36 (max standard base)
	ft_result = ft_atoi_base("ZZ", "0123456789ABCDEFGHIJKLMNOPQRSTUVWXYZ");
	ref_result = ref_atoi_base("ZZ", "0123456789ABCDEFGHIJKLMNOPQRSTUVWXYZ");
	passed = (ft_result == ref_result);
	snprintf(detail, sizeof(detail), "(\"ZZ\" base36: expected %d, got %d)", ref_result, ft_result);
	print_test_result("Base 36: ZZ = 1295", passed, detail);
	stats->total++;
	if (passed) stats->passed++;
	else stats->failed++;
	
	if (g_verbose_mode)
		printf("\n");
}
