#include "libtest.h"

// Weak symbol sentinel
extern void ft_list_sort(t_list **begin_list, int (*cmp)());

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

// Helper function to check if list is sorted
static int is_sorted(t_list *list, int (*cmp)(void *, void *))
{
	while (list && list->next)
	{
		if (cmp(list->data, list->next->data) > 0)
			return 0;
		list = list->next;
	}
	return 1;
}

// Helper function to duplicate a list
static t_list *duplicate_list(t_list *list)
{
	if (!list)
		return NULL;
	
	t_list *new_list = create_node(list->data);
	t_list *current = new_list;
	list = list->next;
	
	while (list)
	{
		current->next = create_node(list->data);
		current = current->next;
		list = list->next;
	}
	return new_list;
}

// Helper function to compare lists
static int lists_equal(t_list *list1, t_list *list2)
{
	while (list1 && list2)
	{
		if (list1->data != list2->data)
			return 0;
		list1 = list1->next;
		list2 = list2->next;
	}
	return (list1 == NULL && list2 == NULL);
}

// Comparison functions
static int cmp_int_asc(void *a, void *b)
{
	return (int)((long)a - (long)b);
}

static int cmp_int_desc(void *a, void *b)
{
	return (int)((long)b - (long)a);
}

static int cmp_str(void *a, void *b)
{
	return strcmp((char *)a, (char *)b);
}

// Reference implementation (bubble sort)
static void ref_list_sort(t_list **begin_list, int (*cmp)())
{
	if (!begin_list || !*begin_list || !cmp)
		return;
	
	int swapped;
	t_list *ptr1;
	t_list *lptr = NULL;
	
	do
	{
		swapped = 0;
		ptr1 = *begin_list;
		
		while (ptr1->next != lptr)
		{
			if (cmp(ptr1->data, ptr1->next->data) > 0)
			{
				void *tmp = ptr1->data;
				ptr1->data = ptr1->next->data;
				ptr1->next->data = tmp;
				swapped = 1;
			}
			ptr1 = ptr1->next;
		}
		lptr = ptr1;
	} while (swapped);
}

void test_ft_list_sort(t_test_stats *stats)
{
	char detail[256];
	int passed;
	t_list *list_ft;
	t_list *list_ref;
	
	print_header("Testing ft_list_sort [BONUS]");
	
	// Detection: Try sorting a 3-element list
	// Weak symbol does nothing, real implementation should change it
	list_ft = create_node((void *)3);
	list_ft->next = create_node((void *)1);
	list_ft->next->next = create_node((void *)2);
	
	// Call the function with a real comparator
	ft_list_sort(&list_ft, cmp_int_asc);
	
	// Check if list is unchanged
	// Weak symbol: list stays [3, 1, 2] (unchanged)
	// Real implementation: list becomes [1, 2, 3] or any other order
	if (list_ft && list_ft->data == (void *)3 && 
		list_ft->next && list_ft->next->data == (void *)1 &&
		list_ft->next->next && list_ft->next->next->data == (void *)2)
	{
		// List unchanged - weak symbol (not implemented)
		free_list(list_ft);
		printf("%s  Function not found or not compiled with 'make bonus'%s\n", COLOR_YELLOW, COLOR_RESET);
		if (g_verbose_mode)
			printf("\n");
		return;
	}
	free_list(list_ft);
	
	// Test 1: NULL pointer parameter - should not crash
	ft_list_sort(NULL, cmp_int_asc);
	ref_list_sort(NULL, cmp_int_asc);
	passed = 1;  // If we reach here, both handled NULL correctly
	snprintf(detail, sizeof(detail), "Both handled NULL without crashing");
	print_test_result("NULL begin_list parameter", passed, detail);
	stats->total++;
	if (passed) stats->passed++;
	else stats->failed++;
	
	// Test 2: NULL comparison function - should not crash
	list_ft = create_node((void *)1);
	list_ref = create_node((void *)1);
	ft_list_sort(&list_ft, NULL);
	ref_list_sort(&list_ref, NULL);
	passed = 1;  // If we reach here, both handled NULL cmp correctly
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
	ft_list_sort(&list_ft, cmp_int_asc);
	ref_list_sort(&list_ref, cmp_int_asc);
	passed = (list_ft == NULL && list_ref == NULL);
	snprintf(detail, sizeof(detail), "Empty list remains NULL");
	print_test_result("Empty list", passed, detail);
	stats->total++;
	if (passed) stats->passed++;
	else stats->failed++;
	
	// Test 4: Single element
	list_ft = create_node((void *)42);
	list_ref = create_node((void *)42);
	ft_list_sort(&list_ft, cmp_int_asc);
	ref_list_sort(&list_ref, cmp_int_asc);
	passed = lists_equal(list_ft, list_ref) && list_size(list_ft) == 1;
	snprintf(detail, sizeof(detail), "Single element list unchanged");
	print_test_result("Single element", passed, detail);
	stats->total++;
	if (passed) stats->passed++;
	else stats->failed++;
	free_list(list_ft);
	free_list(list_ref);
	
	// Test 5: Two elements (ascending order)
	list_ft = create_node((void *)5);
	list_ft->next = create_node((void *)3);
	list_ref = duplicate_list(list_ft);
	ft_list_sort(&list_ft, cmp_int_asc);
	ref_list_sort(&list_ref, cmp_int_asc);
	passed = lists_equal(list_ft, list_ref) && is_sorted(list_ft, cmp_int_asc);
	snprintf(detail, sizeof(detail), "Sorted: 3, 5");
	print_test_result("Two elements (ascending)", passed, detail);
	stats->total++;
	if (passed) stats->passed++;
	else stats->failed++;
	free_list(list_ft);
	free_list(list_ref);
	
	// Test 6: Two elements (descending order)
	list_ft = create_node((void *)3);
	list_ft->next = create_node((void *)5);
	list_ref = duplicate_list(list_ft);
	ft_list_sort(&list_ft, cmp_int_desc);
	ref_list_sort(&list_ref, cmp_int_desc);
	passed = lists_equal(list_ft, list_ref) && is_sorted(list_ft, cmp_int_desc);
	snprintf(detail, sizeof(detail), "Sorted: 5, 3");
	print_test_result("Two elements (descending)", passed, detail);
	stats->total++;
	if (passed) stats->passed++;
	else stats->failed++;
	free_list(list_ft);
	free_list(list_ref);
	
	// Test 7: Already sorted list (ascending)
	list_ft = create_node((void *)1);
	list_ft->next = create_node((void *)2);
	list_ft->next->next = create_node((void *)3);
	list_ft->next->next->next = create_node((void *)4);
	list_ref = duplicate_list(list_ft);
	ft_list_sort(&list_ft, cmp_int_asc);
	ref_list_sort(&list_ref, cmp_int_asc);
	passed = lists_equal(list_ft, list_ref) && is_sorted(list_ft, cmp_int_asc);
	snprintf(detail, sizeof(detail), "Already sorted: 1, 2, 3, 4");
	print_test_result("Already sorted (ascending)", passed, detail);
	stats->total++;
	if (passed) stats->passed++;
	else stats->failed++;
	free_list(list_ft);
	free_list(list_ref);
	
	// Test 8: Reverse sorted list
	list_ft = create_node((void *)4);
	list_ft->next = create_node((void *)3);
	list_ft->next->next = create_node((void *)2);
	list_ft->next->next->next = create_node((void *)1);
	list_ref = duplicate_list(list_ft);
	ft_list_sort(&list_ft, cmp_int_asc);
	ref_list_sort(&list_ref, cmp_int_asc);
	passed = lists_equal(list_ft, list_ref) && is_sorted(list_ft, cmp_int_asc);
	snprintf(detail, sizeof(detail), "Sorted: 1, 2, 3, 4");
	print_test_result("Reverse sorted list", passed, detail);
	stats->total++;
	if (passed) stats->passed++;
	else stats->failed++;
	free_list(list_ft);
	free_list(list_ref);
	
	// Test 9: Random order (5 elements)
	list_ft = create_node((void *)3);
	list_ft->next = create_node((void *)1);
	list_ft->next->next = create_node((void *)4);
	list_ft->next->next->next = create_node((void *)2);
	list_ft->next->next->next->next = create_node((void *)5);
	list_ref = duplicate_list(list_ft);
	ft_list_sort(&list_ft, cmp_int_asc);
	ref_list_sort(&list_ref, cmp_int_asc);
	passed = lists_equal(list_ft, list_ref) && is_sorted(list_ft, cmp_int_asc);
	snprintf(detail, sizeof(detail), "Sorted: 1, 2, 3, 4, 5");
	print_test_result("Random order (5 elements)", passed, detail);
	stats->total++;
	if (passed) stats->passed++;
	else stats->failed++;
	free_list(list_ft);
	free_list(list_ref);
	
	// Test 10: Duplicate values
	list_ft = create_node((void *)3);
	list_ft->next = create_node((void *)1);
	list_ft->next->next = create_node((void *)3);
	list_ft->next->next->next = create_node((void *)2);
	list_ft->next->next->next->next = create_node((void *)1);
	list_ref = duplicate_list(list_ft);
	ft_list_sort(&list_ft, cmp_int_asc);
	ref_list_sort(&list_ref, cmp_int_asc);
	passed = lists_equal(list_ft, list_ref) && is_sorted(list_ft, cmp_int_asc);
	snprintf(detail, sizeof(detail), "Sorted: 1, 1, 2, 3, 3");
	print_test_result("Duplicate values", passed, detail);
	stats->total++;
	if (passed) stats->passed++;
	else stats->failed++;
	free_list(list_ft);
	free_list(list_ref);
	
	// Test 11: String comparison
	list_ft = create_node("zebra");
	list_ft->next = create_node("apple");
	list_ft->next->next = create_node("mango");
	list_ft->next->next->next = create_node("banana");
	list_ref = duplicate_list(list_ft);
	ft_list_sort(&list_ft, cmp_str);
	ref_list_sort(&list_ref, cmp_str);
	passed = lists_equal(list_ft, list_ref) && is_sorted(list_ft, cmp_str);
	snprintf(detail, sizeof(detail), "Sorted: apple, banana, mango, zebra");
	print_test_result("String comparison", passed, detail);
	stats->total++;
	if (passed) stats->passed++;
	else stats->failed++;
	free_list(list_ft);
	free_list(list_ref);
	
	// Test 12: Large list (20 elements)
	list_ft = create_node((void *)15);
	t_list *current = list_ft;
	int values[] = {8, 23, 4, 16, 42, 11, 7, 19, 3, 28, 13, 6, 31, 9, 21, 2, 17, 5, 12};
	for (int i = 0; i < 19; i++)
	{
		current->next = create_node((void *)(long)values[i]);
		current = current->next;
	}
	list_ref = duplicate_list(list_ft);
	ft_list_sort(&list_ft, cmp_int_asc);
	ref_list_sort(&list_ref, cmp_int_asc);
	passed = lists_equal(list_ft, list_ref) && is_sorted(list_ft, cmp_int_asc);
	snprintf(detail, sizeof(detail), "20 elements correctly sorted");
	print_test_result("Large list (20 elements)", passed, detail);
	stats->total++;
	if (passed) stats->passed++;
	else stats->failed++;
	free_list(list_ft);
	free_list(list_ref);
	
	if (g_verbose_mode)
		printf("\n");
}
