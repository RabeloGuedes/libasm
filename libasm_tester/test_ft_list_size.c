#include "libtest.h"

// Weak symbol sentinel
extern int ft_list_size(t_list *begin_list);

// Helper function to create a new list node
static t_list *create_node(void *data)
{
	t_list *node = (t_list *)malloc(sizeof(t_list));
	if (!node)
		return NULL;
	node->data = data;
	node->next = NULL;
	return node;
}

// Helper function to free entire list
static void free_list(t_list *list)
{
	t_list *tmp;
	
	while (list)
	{
		tmp = list;
		list = list->next;
		free(tmp);
	}
}

// Reference implementation
static int ref_list_size(t_list *begin_list)
{
	int count = 0;
	
	while (begin_list)
	{
		count++;
		begin_list = begin_list->next;
	}
	return count;
}

void test_ft_list_size(t_test_stats *stats)
{
	char detail[256];
	int passed;
	int result_ft;
	int result_ref;
	t_list *list;
	
	print_header("Testing ft_list_size [BONUS]");
	
	// Check if function exists (weak sentinel is -559038737)
	list = create_node("test");
	result_ft = ft_list_size(list);
	free_list(list);
	if (result_ft == -559038737)
	{
		printf("%s  Function not found or not compiled with 'make bonus'%s\n", COLOR_YELLOW, COLOR_RESET);
		if (g_verbose_mode)
			printf("\n");
		return;
	}
	
	// Test 1: NULL pointer parameter (fork-protected)
	{
		pid_t pid = fork();
		if (pid == 0)
		{
			result_ft = ft_list_size(NULL);
			(void)result_ft;
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
				result_ref = ref_list_size(NULL);
				(void)result_ref;
				exit(0);
			}
			else if (pid2 > 0)
			{
				waitpid(pid2, &status, 0);
				int ref_crashed = WIFSIGNALED(status);
				
				passed = (ft_crashed == ref_crashed);
				if (passed && !ft_crashed)
				{
					// Both returned normally - check values
					result_ft = ft_list_size(NULL);
					result_ref = ref_list_size(NULL);
					passed = (result_ft == result_ref);
					snprintf(detail, sizeof(detail), "ft_list_size(NULL) = %d (expected %d)", 
							result_ft, result_ref);
				}
				else
				{
					snprintf(detail, sizeof(detail), "ft_list_size(NULL): %s", 
							passed ? "both handled NULL" : "different behavior");
				}
				print_test_result("NULL pointer parameter", passed, detail);
				stats->total++;
				if (passed) stats->passed++;
				else stats->failed++;
			}
		}
	}
	
	// Test 2: Empty list (size 0)
	list = NULL;
	result_ft = ft_list_size(list);
	result_ref = ref_list_size(list);
	passed = (result_ft == result_ref);
	snprintf(detail, sizeof(detail), "ft_list_size(empty list) = %d (expected %d)",
			result_ft, result_ref);
	print_test_result("Empty list (size 0)", passed, detail);
	stats->total++;
	if (passed) stats->passed++;
	else stats->failed++;
	
	// Test 3: Single element
	list = create_node("first");
	result_ft = ft_list_size(list);
	result_ref = ref_list_size(list);
	passed = (result_ft == result_ref);
	snprintf(detail, sizeof(detail), "ft_list_size(1 element) = %d (expected %d)",
			result_ft, result_ref);
	print_test_result("Single element", passed, detail);
	stats->total++;
	if (passed) stats->passed++;
	else stats->failed++;
	free_list(list);
	
	// Test 4: Two elements
	list = create_node("first");
	list->next = create_node("second");
	result_ft = ft_list_size(list);
	result_ref = ref_list_size(list);
	passed = (result_ft == result_ref);
	snprintf(detail, sizeof(detail), "ft_list_size(2 elements) = %d (expected %d)",
			result_ft, result_ref);
	print_test_result("Two elements", passed, detail);
	stats->total++;
	if (passed) stats->passed++;
	else stats->failed++;
	free_list(list);
	
	// Test 5: Five elements
	list = create_node("first");
	list->next = create_node("second");
	list->next->next = create_node("third");
	list->next->next->next = create_node("fourth");
	list->next->next->next->next = create_node("fifth");
	result_ft = ft_list_size(list);
	result_ref = ref_list_size(list);
	passed = (result_ft == result_ref);
	snprintf(detail, sizeof(detail), "ft_list_size(5 elements) = %d (expected %d)",
			result_ft, result_ref);
	print_test_result("Five elements", passed, detail);
	stats->total++;
	if (passed) stats->passed++;
	else stats->failed++;
	free_list(list);
	
	// Test 6: Ten elements
	list = create_node("0");
	t_list *current = list;
	for (int i = 1; i < 10; i++)
	{
		current->next = create_node("node");
		current = current->next;
	}
	result_ft = ft_list_size(list);
	result_ref = ref_list_size(list);
	passed = (result_ft == result_ref);
	snprintf(detail, sizeof(detail), "ft_list_size(10 elements) = %d (expected %d)",
			result_ft, result_ref);
	print_test_result("Ten elements", passed, detail);
	stats->total++;
	if (passed) stats->passed++;
	else stats->failed++;
	free_list(list);
	
	// Test 7: Large list (100 elements)
	list = create_node("0");
	current = list;
	for (int i = 1; i < 100; i++)
	{
		current->next = create_node("node");
		current = current->next;
	}
	result_ft = ft_list_size(list);
	result_ref = ref_list_size(list);
	passed = (result_ft == result_ref);
	snprintf(detail, sizeof(detail), "ft_list_size(100 elements) = %d (expected %d)",
			result_ft, result_ref);
	print_test_result("Large list (100 elements)", passed, detail);
	stats->total++;
	if (passed) stats->passed++;
	else stats->failed++;
	free_list(list);
	
	// Test 8: Very large list (1000 elements)
	list = create_node("0");
	current = list;
	for (int i = 1; i < 1000; i++)
	{
		current->next = create_node("node");
		current = current->next;
	}
	result_ft = ft_list_size(list);
	result_ref = ref_list_size(list);
	passed = (result_ft == result_ref);
	snprintf(detail, sizeof(detail), "ft_list_size(1000 elements) = %d (expected %d)",
			result_ft, result_ref);
	print_test_result("Very large list (1000 elements)", passed, detail);
	stats->total++;
	if (passed) stats->passed++;
	else stats->failed++;
	free_list(list);
	
	// Test 9: List with NULL data pointers
	list = create_node(NULL);
	list->next = create_node(NULL);
	list->next->next = create_node(NULL);
	result_ft = ft_list_size(list);
	result_ref = ref_list_size(list);
	passed = (result_ft == result_ref);
	snprintf(detail, sizeof(detail), "ft_list_size(3 nodes with NULL data) = %d (expected %d)",
			result_ft, result_ref);
	print_test_result("List with NULL data pointers", passed, detail);
	stats->total++;
	if (passed) stats->passed++;
	else stats->failed++;
	free_list(list);
	
	// Test 10: Mixed data types
	list = create_node((void *)42);
	list->next = create_node("string");
	list->next->next = create_node((void *)0);
	list->next->next->next = create_node((void *)12345);
	result_ft = ft_list_size(list);
	result_ref = ref_list_size(list);
	passed = (result_ft == result_ref);
	snprintf(detail, sizeof(detail), "ft_list_size(4 mixed types) = %d (expected %d)",
			result_ft, result_ref);
	print_test_result("Mixed data types", passed, detail);
	stats->total++;
	if (passed) stats->passed++;
	else stats->failed++;
	free_list(list);
	
	if (g_verbose_mode)
		printf("\n");
}
