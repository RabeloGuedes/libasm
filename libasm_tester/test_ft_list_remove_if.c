#include "libtest.h"

// Weak symbol sentinel
extern void ft_list_remove_if(t_list **begin_list, void *data_ref, int (*cmp)(), void (*free_fct)(void *));

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

// Helper function to count list size
static int list_size(t_list *list)
{
	int count = 0;
	
	while (list)
	{
		count++;
		list = list->next;
	}
	return count;
}

// Helper function to check if element exists in list
static int list_contains(t_list *list, void *data)
{
	while (list)
	{
		if (list->data == data)
			return 1;
		list = list->next;
	}
	return 0;
}

// Comparison function for integers
static int cmp_int(void *a, void *b)
{
	return (int)((long)a - (long)b);
}

// Comparison function for strings
static int cmp_str(void *a, void *b)
{
	return strcmp((char *)a, (char *)b);
}

// Free function that does nothing (for non-allocated data)
static void free_nothing(void *data)
{
	(void)data;
}

// Free function for dynamically allocated strings (reserved for future tests)
__attribute__((unused)) static void free_string(void *data)
{
	free(data);
}

// Reference implementation
static void ref_list_remove_if(t_list **begin_list, void *data_ref, int (*cmp)(), void (*free_fct)(void *))
{
	if (!begin_list || !*begin_list || !cmp)
		return;
	
	t_list *current = *begin_list;
	t_list *prev = NULL;
	
	while (current)
	{
		if (cmp(current->data, data_ref) == 0)
		{
			t_list *to_remove = current;
			
			if (prev)
				prev->next = current->next;
			else
				*begin_list = current->next;
			
			current = current->next;
			
			if (free_fct)
				free_fct(to_remove->data);
			free(to_remove);
		}
		else
		{
			prev = current;
			current = current->next;
		}
	}
}

void test_ft_list_remove_if(t_test_stats *stats)
{
	char detail[256];
	int passed;
	t_list *list_ft;
	t_list *list_ref;
	
	print_header("Testing ft_list_remove_if [BONUS]");
	
	// Detection: Try removing from a simple list
	// Weak symbol does nothing, real implementation should change the list
	list_ft = create_node((void *)1);
	list_ft->next = create_node((void *)2);
	list_ft->next->next = create_node((void *)3);
	
	ft_list_remove_if(&list_ft, (void *)2, cmp_int, free_nothing);
	
	// Check if list is unchanged
	// Weak symbol: list stays [1, 2, 3]
	// Real implementation: list becomes [1, 3] (2 removed)
	if (list_ft && list_ft->data == (void *)1 &&
		list_ft->next && list_ft->next->data == (void *)2 &&
		list_ft->next->next && list_ft->next->next->data == (void *)3)
	{
		free_list(list_ft);
		printf("%s  Function not found or not compiled with 'make bonus'%s\n", COLOR_YELLOW, COLOR_RESET);
		if (g_verbose_mode)
			printf("\n");
		return;
	}
	free_list(list_ft);
	
	// Test 1: NULL begin_list parameter - should not crash
	ft_list_remove_if(NULL, (void *)1, cmp_int, free_nothing);
	ref_list_remove_if(NULL, (void *)1, cmp_int, free_nothing);
	passed = 1;
	snprintf(detail, sizeof(detail), "Both handled NULL without crashing");
	print_test_result("NULL begin_list parameter", passed, detail);
	stats->total++;
	if (passed) stats->passed++;
	else stats->failed++;
	
	// Test 2: NULL comparison function - should not crash
	list_ft = create_node((void *)1);
	list_ref = create_node((void *)1);
	ft_list_remove_if(&list_ft, (void *)1, NULL, free_nothing);
	ref_list_remove_if(&list_ref, (void *)1, NULL, free_nothing);
	passed = 1;
	snprintf(detail, sizeof(detail), "Both handled NULL cmp without crashing");
	print_test_result("NULL comparison function", passed, detail);
	stats->total++;
	if (passed) stats->passed++;
	else stats->failed++;
	free_list(list_ft);
	free_list(list_ref);
	
	// Test 3: Empty list
	list_ft = NULL;
	list_ref = NULL;
	ft_list_remove_if(&list_ft, (void *)1, cmp_int, free_nothing);
	ref_list_remove_if(&list_ref, (void *)1, cmp_int, free_nothing);
	passed = (list_ft == NULL && list_ref == NULL);
	snprintf(detail, sizeof(detail), "Empty list remains NULL");
	print_test_result("Empty list", passed, detail);
	stats->total++;
	if (passed) stats->passed++;
	else stats->failed++;
	
	// Test 4: Remove first element
	list_ft = create_node((void *)1);
	list_ft->next = create_node((void *)2);
	list_ft->next->next = create_node((void *)3);
	list_ref = create_node((void *)1);
	list_ref->next = create_node((void *)2);
	list_ref->next->next = create_node((void *)3);
	
	ft_list_remove_if(&list_ft, (void *)1, cmp_int, free_nothing);
	ref_list_remove_if(&list_ref, (void *)1, cmp_int, free_nothing);
	
	passed = (list_size(list_ft) == list_size(list_ref) && 
			  list_ft && list_ft->data == (void *)2 &&
			  !list_contains(list_ft, (void *)1));
	snprintf(detail, sizeof(detail), "Removed first element, list: [2, 3]");
	print_test_result("Remove first element", passed, detail);
	stats->total++;
	if (passed) stats->passed++;
	else stats->failed++;
	free_list(list_ft);
	free_list(list_ref);
	
	// Test 5: Remove middle element
	list_ft = create_node((void *)1);
	list_ft->next = create_node((void *)2);
	list_ft->next->next = create_node((void *)3);
	list_ref = create_node((void *)1);
	list_ref->next = create_node((void *)2);
	list_ref->next->next = create_node((void *)3);
	
	ft_list_remove_if(&list_ft, (void *)2, cmp_int, free_nothing);
	ref_list_remove_if(&list_ref, (void *)2, cmp_int, free_nothing);
	
	passed = (list_size(list_ft) == list_size(list_ref) &&
			  list_ft && list_ft->data == (void *)1 &&
			  list_ft->next && list_ft->next->data == (void *)3 &&
			  !list_contains(list_ft, (void *)2));
	snprintf(detail, sizeof(detail), "Removed middle element, list: [1, 3]");
	print_test_result("Remove middle element", passed, detail);
	stats->total++;
	if (passed) stats->passed++;
	else stats->failed++;
	free_list(list_ft);
	free_list(list_ref);
	
	// Test 6: Remove last element
	list_ft = create_node((void *)1);
	list_ft->next = create_node((void *)2);
	list_ft->next->next = create_node((void *)3);
	list_ref = create_node((void *)1);
	list_ref->next = create_node((void *)2);
	list_ref->next->next = create_node((void *)3);
	
	ft_list_remove_if(&list_ft, (void *)3, cmp_int, free_nothing);
	ref_list_remove_if(&list_ref, (void *)3, cmp_int, free_nothing);
	
	passed = (list_size(list_ft) == list_size(list_ref) &&
			  list_ft && list_ft->data == (void *)1 &&
			  list_ft->next && list_ft->next->data == (void *)2 &&
			  !list_contains(list_ft, (void *)3));
	snprintf(detail, sizeof(detail), "Removed last element, list: [1, 2]");
	print_test_result("Remove last element", passed, detail);
	stats->total++;
	if (passed) stats->passed++;
	else stats->failed++;
	free_list(list_ft);
	free_list(list_ref);
	
	// Test 7: Remove all elements
	list_ft = create_node((void *)5);
	list_ft->next = create_node((void *)5);
	list_ft->next->next = create_node((void *)5);
	list_ref = create_node((void *)5);
	list_ref->next = create_node((void *)5);
	list_ref->next->next = create_node((void *)5);
	
	ft_list_remove_if(&list_ft, (void *)5, cmp_int, free_nothing);
	ref_list_remove_if(&list_ref, (void *)5, cmp_int, free_nothing);
	
	passed = (list_ft == NULL && list_ref == NULL);
	snprintf(detail, sizeof(detail), "All elements removed, list is NULL");
	print_test_result("Remove all elements", passed, detail);
	stats->total++;
	if (passed) stats->passed++;
	else stats->failed++;
	
	// Test 8: Remove no elements (not found)
	list_ft = create_node((void *)1);
	list_ft->next = create_node((void *)2);
	list_ft->next->next = create_node((void *)3);
	list_ref = create_node((void *)1);
	list_ref->next = create_node((void *)2);
	list_ref->next->next = create_node((void *)3);
	
	ft_list_remove_if(&list_ft, (void *)99, cmp_int, free_nothing);
	ref_list_remove_if(&list_ref, (void *)99, cmp_int, free_nothing);
	
	passed = (list_size(list_ft) == 3 && list_size(list_ref) == 3);
	snprintf(detail, sizeof(detail), "No elements removed, list: [1, 2, 3]");
	print_test_result("Remove non-existent element", passed, detail);
	stats->total++;
	if (passed) stats->passed++;
	else stats->failed++;
	free_list(list_ft);
	free_list(list_ref);
	
	// Test 9: Remove multiple occurrences
	list_ft = create_node((void *)1);
	list_ft->next = create_node((void *)2);
	list_ft->next->next = create_node((void *)1);
	list_ft->next->next->next = create_node((void *)3);
	list_ft->next->next->next->next = create_node((void *)1);
	list_ref = create_node((void *)1);
	list_ref->next = create_node((void *)2);
	list_ref->next->next = create_node((void *)1);
	list_ref->next->next->next = create_node((void *)3);
	list_ref->next->next->next->next = create_node((void *)1);
	
	ft_list_remove_if(&list_ft, (void *)1, cmp_int, free_nothing);
	ref_list_remove_if(&list_ref, (void *)1, cmp_int, free_nothing);
	
	passed = (list_size(list_ft) == list_size(list_ref) &&
			  list_ft && list_ft->data == (void *)2 &&
			  list_ft->next && list_ft->next->data == (void *)3 &&
			  !list_contains(list_ft, (void *)1));
	snprintf(detail, sizeof(detail), "Removed all occurrences of 1, list: [2, 3]");
	print_test_result("Remove multiple occurrences", passed, detail);
	stats->total++;
	if (passed) stats->passed++;
	else stats->failed++;
	free_list(list_ft);
	free_list(list_ref);
	
	// Test 10: Remove with string comparison
	list_ft = create_node("apple");
	list_ft->next = create_node("banana");
	list_ft->next->next = create_node("cherry");
	list_ref = create_node("apple");
	list_ref->next = create_node("banana");
	list_ref->next->next = create_node("cherry");
	
	ft_list_remove_if(&list_ft, "banana", cmp_str, free_nothing);
	ref_list_remove_if(&list_ref, "banana", cmp_str, free_nothing);
	
	passed = (list_size(list_ft) == list_size(list_ref) &&
			  list_ft && strcmp((char *)list_ft->data, "apple") == 0 &&
			  list_ft->next && strcmp((char *)list_ft->next->data, "cherry") == 0);
	snprintf(detail, sizeof(detail), "Removed 'banana', list: [apple, cherry]");
	print_test_result("String comparison", passed, detail);
	stats->total++;
	if (passed) stats->passed++;
	else stats->failed++;
	free_list(list_ft);
	free_list(list_ref);
	
	// Test 11: Single element removal
	list_ft = create_node((void *)42);
	list_ref = create_node((void *)42);
	
	ft_list_remove_if(&list_ft, (void *)42, cmp_int, free_nothing);
	ref_list_remove_if(&list_ref, (void *)42, cmp_int, free_nothing);
	
	passed = (list_ft == NULL && list_ref == NULL);
	snprintf(detail, sizeof(detail), "Single element removed, list is NULL");
	print_test_result("Single element removal", passed, detail);
	stats->total++;
	if (passed) stats->passed++;
	else stats->failed++;
	
	// Test 12: Large list with multiple removals
	list_ft = create_node((void *)1);
	t_list *current_ft = list_ft;
	list_ref = create_node((void *)1);
	t_list *current_ref = list_ref;
	
	// Create list: [1, 2, 5, 3, 5, 4, 5, 5, 6]
	int values[] = {2, 5, 3, 5, 4, 5, 5, 6};
	for (int i = 0; i < 8; i++)
	{
		current_ft->next = create_node((void *)(long)values[i]);
		current_ft = current_ft->next;
		current_ref->next = create_node((void *)(long)values[i]);
		current_ref = current_ref->next;
	}
	
	ft_list_remove_if(&list_ft, (void *)5, cmp_int, free_nothing);
	ref_list_remove_if(&list_ref, (void *)5, cmp_int, free_nothing);
	
	passed = (list_size(list_ft) == list_size(list_ref) &&
			  list_size(list_ft) == 5 &&
			  !list_contains(list_ft, (void *)5));
	snprintf(detail, sizeof(detail), "Removed all 5s, list: [1, 2, 3, 4, 6]");
	print_test_result("Large list multiple removals", passed, detail);
	stats->total++;
	if (passed) stats->passed++;
	else stats->failed++;
	free_list(list_ft);
	free_list(list_ref);
	
	if (g_verbose_mode)
		printf("\n");
}
