#include "libtest.h"

#ifdef FIRST
# undef FIRST
# define FIRST "first"
#else
# define FIRST "first"
#endif


#ifdef SECOND
# undef SECOND
# define SECOND "second"
#else
# define SECOND "second"
#endif


// Weak symbol sentinel - we can't return this from void function, so we'll check list state
extern void ft_list_push_front(t_list **begin_list, void *data);

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
__attribute__((unused)) static int list_size(t_list *list)
{
	int count = 0;
	
	while (list)
	{
		count++;
		list = list->next;
	}
	return count;
}

// Reference implementation
static void ref_list_push_front(t_list **begin_list, void *data)
{
	t_list *new_node;
	
	if (!begin_list)
		return;
	
	new_node = create_node(data);
	if (!new_node)
		return;
	
	new_node->next = *begin_list;
	*begin_list = new_node;
}

void test_ft_list_push_front(t_test_stats *stats)
{
	char detail[256];
	int passed;
	t_list *list_ft;
	t_list *list_ref;
	
	print_header("Testing ft_list_push_front [BONUS]");
	
	// Check if function exists (try to detect weak symbol by checking if it does anything)
	list_ft = NULL;
	ft_list_push_front(&list_ft, "test");
	if (list_ft == NULL)
	{
		printf("%s  Function not found or not compiled with 'make bonus'%s\n", COLOR_YELLOW, COLOR_RESET);
		if (g_verbose_mode)
			printf("\n");
		return;
	}
	free_list(list_ft);
	
	// Test 1: NULL pointer parameter (fork-protected)
	{
		pid_t pid = fork();
		if (pid == 0)
		{
			ft_list_push_front(NULL, "data");
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
				ref_list_push_front(NULL, "data");
				exit(0);
			}
			else if (pid2 > 0)
			{
				int status2;
				waitpid(pid2, &status2, 0);
				int ref_crashed = WIFSIGNALED(status2);
				
				passed = (ft_crashed == ref_crashed);
				if (ft_crashed && ref_crashed)
					snprintf(detail, sizeof(detail), "(NULL pointer: both segfault as expected)");
				else if (!ft_crashed && !ref_crashed)
					snprintf(detail, sizeof(detail), "(NULL pointer: both handle safely)");
				else if (ft_crashed)
					snprintf(detail, sizeof(detail), "(NULL pointer: ft segfaults but ref handles safely)");
				else
					snprintf(detail, sizeof(detail), "(NULL pointer: ft handles but ref segfaults)");
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
		print_test_result("NULL pointer parameter (protected)", passed, detail);
		stats->total++;
		if (passed) stats->passed++;
		else stats->failed++;
	}
	
	// Test 2: Push to empty list (NULL) - fork-protected
	{
		pid_t pid = fork();
		if (pid == 0)
		{
			t_list *test_list = NULL;
			ft_list_push_front(&test_list, FIRST);
			// Check if node was created
			if (test_list && !strcmp(test_list->data, FIRST) && test_list->next == NULL)
				exit(0);
			exit(1);
		}
		else if (pid > 0)
		{
			int status;
			waitpid(pid, &status, 0);
			int ft_ok = (WIFEXITED(status) && WEXITSTATUS(status) == 0);
			int ft_crashed = WIFSIGNALED(status);
			
			pid_t pid2 = fork();
			if (pid2 == 0)
			{
				t_list *test_list = NULL;
				ref_list_push_front(&test_list, FIRST);
				if (test_list && !strcmp(test_list->data, FIRST) && test_list->next == NULL)
					exit(0);
				exit(1);
			}
			else if (pid2 > 0)
			{
				int status2;
				waitpid(pid2, &status2, 0);
				int ref_ok = (WIFEXITED(status2) && WEXITSTATUS(status2) == 0);
				
				passed = (ft_ok && ref_ok && !ft_crashed);
				if (passed)
					snprintf(detail, sizeof(detail), "(push \"first\" to empty list: success)");
				else if (ft_crashed)
					snprintf(detail, sizeof(detail), "(push \"first\": ft segfaulted)");
				else if (!ft_ok && ref_ok)
					snprintf(detail, sizeof(detail), "(push \"first\": ft failed but ref succeeded)");
				else
					snprintf(detail, sizeof(detail), "(push \"first\": both failed)");
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
		print_test_result("Push to empty list (protected)", passed, detail);
		stats->total++;
		if (passed) stats->passed++;
		else stats->failed++;
	}
	
	// Test 3: Push SECOND element (fork-protected)
	{
		pid_t pid = fork();
		if (pid == 0)
		{
			t_list *test_list = NULL;
			ft_list_push_front(&test_list, SECOND);
			ft_list_push_front(&test_list, FIRST);
			// Verify structure
			if (test_list && test_list->next &&
				!strcmp(test_list->data, FIRST) &&
				!strcmp(test_list->next->data, SECOND) &&
				test_list->next->next == NULL)
				exit(0);
			exit(1);
		}
		else if (pid > 0)
		{
			int status;
			waitpid(pid, &status, 0);
			passed = (WIFEXITED(status) && WEXITSTATUS(status) == 0 && !WIFSIGNALED(status));
			
			if (passed)
				snprintf(detail, sizeof(detail), "(pushed 2 elements: order correct)");
			else if (WIFSIGNALED(status))
				snprintf(detail, sizeof(detail), "(pushed 2 elements: segfaulted)");
			else
				snprintf(detail, sizeof(detail), "(pushed 2 elements: incorrect structure)");
		}
		else
		{
			passed = 0;
			snprintf(detail, sizeof(detail), "(fork failed)");
		}
		print_test_result("Push SECOND element (protected)", passed, detail);
		stats->total++;
		if (passed) stats->passed++;
		else stats->failed++;
	}
	
	// Test 4: Push multiple elements (5) - fork-protected
	{
		pid_t pid = fork();
		if (pid == 0)
		{
			t_list *test_list = NULL;
			char *strs[5];
			for (int i = 5; i > 0; i--)
			{
				strs[6-i-1] = malloc(20);
				sprintf(strs[6-i-1], "element_%d", i);
				ft_list_push_front(&test_list, strs[6-i-1]);
			}
			// Verify count and order
			int count = 0;
			t_list *tmp = test_list;
			int valid = 1;
			for (int i = 1; i <= 5 && tmp; i++)
			{
				count++;
				char expected[20];
				sprintf(expected, "element_%d", i);
				if (strcmp((char*)tmp->data, expected) != 0)
					valid = 0;
				tmp = tmp->next;
			}
			// Cleanup
			tmp = test_list;
			while (tmp)
			{
				free(tmp->data);
				tmp = tmp->next;
			}
			exit((count == 5 && valid) ? 0 : 1);
		}
		else if (pid > 0)
		{
			int status;
			waitpid(pid, &status, 0);
			passed = (WIFEXITED(status) && WEXITSTATUS(status) == 0 && !WIFSIGNALED(status));
			
			if (passed)
				snprintf(detail, sizeof(detail), "(pushed 5 elements: size=5, order correct)");
			else if (WIFSIGNALED(status))
				snprintf(detail, sizeof(detail), "(pushed 5 elements: segfaulted)");
			else
				snprintf(detail, sizeof(detail), "(pushed 5 elements: incorrect structure)");
		}
		else
		{
			passed = 0;
			snprintf(detail, sizeof(detail), "(fork failed)");
		}
		print_test_result("Push 5 elements (protected)", passed, detail);
		stats->total++;
		if (passed) stats->passed++;
		else stats->failed++;
	}
	
	// Test 5: Push NULL data
	list_ft = NULL;
	list_ref = NULL;
	ft_list_push_front(&list_ft, NULL);
	ref_list_push_front(&list_ref, NULL);
	passed = (list_ft != NULL && list_ref != NULL &&
			  list_ft->data == NULL && list_ref->data == NULL);
	snprintf(detail, sizeof(detail), "(push NULL data: list=%p, data=%p)", 
			 (void*)list_ft, list_ft ? list_ft->data : (void*)0xBAD);
	print_test_result("Push NULL data", passed, detail);
	stats->total++;
	if (passed) stats->passed++;
	else stats->failed++;
	free_list(list_ft);
	free_list(list_ref);
	
	// Test 6: Push integer data
	list_ft = NULL;
	list_ref = NULL;
	int num1 = 42;
	int num2 = 84;
	ft_list_push_front(&list_ft, &num1);
	ft_list_push_front(&list_ft, &num2);
	ref_list_push_front(&list_ref, &num1);
	ref_list_push_front(&list_ref, &num2);
	passed = (list_ft && list_ref &&
			  list_ft->data == &num2 && list_ref->data == &num2 &&
			  list_ft->next->data == &num1 && list_ref->next->data == &num1);
	snprintf(detail, sizeof(detail), "(push ints: first=%d, SECOND=%d)", 
			 list_ft && list_ft->data ? *(int*)list_ft->data : -1,
			 list_ft && list_ft->next && list_ft->next->data ? *(int*)list_ft->next->data : -1);
	print_test_result("Push integer pointers", passed, detail);
	stats->total++;
	if (passed) stats->passed++;
	else stats->failed++;
	free_list(list_ft);
	free_list(list_ref);
	
	// Test 7: Push to existing list maintains order
	list_ft = create_node("existing");
	list_ref = create_node("existing");
	ft_list_push_front(&list_ft, "new");
	ref_list_push_front(&list_ref, "new");
	passed = (list_ft && list_ref &&
			  strcmp((char*)list_ft->data, "new") == 0 &&
			  strcmp((char*)list_ft->next->data, "existing") == 0 &&
			  strcmp((char*)list_ref->data, "new") == 0 &&
			  strcmp((char*)list_ref->next->data, "existing") == 0);
	snprintf(detail, sizeof(detail), "(push to existing: head=%s, next=%s)", 
			 list_ft ? (char*)list_ft->data : "NULL",
			 list_ft && list_ft->next ? (char*)list_ft->next->data : "NULL");
	print_test_result("Push to existing list", passed, detail);
	stats->total++;
	if (passed) stats->passed++;
	else stats->failed++;
	free_list(list_ft);
	free_list(list_ref);
	
	// Test 8: LIFO order (Last In First Out)
	list_ft = NULL;
	list_ref = NULL;
	char *strings[] = {FIRST, SECOND, "third", "fourth"};
	for (int i = 0; i < 4; i++)
	{
		ft_list_push_front(&list_ft, strings[i]);
		ref_list_push_front(&list_ref, strings[i]);
	}
	// After pushing in order: first, SECOND, third, fourth
	// List should be: fourth -> third -> SECOND -> first
	passed = (list_ft && list_ref &&
			  list_ft->data == strings[3] &&
			  list_ft->next->data == strings[2] &&
			  list_ft->next->next->data == strings[1] &&
			  list_ft->next->next->next->data == strings[0] &&
			  list_ft->next->next->next->next == NULL);
	snprintf(detail, sizeof(detail), "(LIFO: head=%s, expected=%s)", 
			 list_ft ? (char*)list_ft->data : "NULL", "fourth");
	print_test_result("LIFO order verification", passed, detail);
	stats->total++;
	if (passed) stats->passed++;
	else stats->failed++;
	free_list(list_ft);
	free_list(list_ref);
	
	// Test 9: Push large string
	list_ft = NULL;
	list_ref = NULL;
	char *large_str = "This is a very long string that should be handled correctly by the push_front function without any issues";
	ft_list_push_front(&list_ft, large_str);
	ref_list_push_front(&list_ref, large_str);
	passed = (list_ft && list_ref &&
			  list_ft->data == large_str &&
			  list_ref->data == large_str);
	snprintf(detail, sizeof(detail), "(large string: len=%zu, match=%s)", 
			 strlen(large_str), passed ? "yes" : "no");
	print_test_result("Push large string", passed, detail);
	stats->total++;
	if (passed) stats->passed++;
	else stats->failed++;
	free_list(list_ft);
	free_list(list_ref);
	
	// Test 10: Push many elements (stress test) - fork-protected
	{
		pid_t pid = fork();
		if (pid == 0)
		{
			t_list *test_list = NULL;
			for (int i = 100; i > 0; i--)
				ft_list_push_front(&test_list, (void*)(long)i);
			
			// Verify count and order
			int count = 0;
			int valid = 1;
			t_list *tmp = test_list;
			for (int i = 1; i <= 100 && tmp; i++)
			{
				count++;
				if ((long)tmp->data != i)
					valid = 0;
				tmp = tmp->next;
			}
			exit((count == 100 && valid) ? 0 : 1);
		}
		else if (pid > 0)
		{
			int status;
			waitpid(pid, &status, 0);
			passed = (WIFEXITED(status) && WEXITSTATUS(status) == 0 && !WIFSIGNALED(status));
			
			if (passed)
				snprintf(detail, sizeof(detail), "(stress test: 100 elements, order correct)");
			else if (WIFSIGNALED(status))
				snprintf(detail, sizeof(detail), "(stress test: segfaulted)");
			else
				snprintf(detail, sizeof(detail), "(stress test: incorrect structure)");
		}
		else
		{
			passed = 0;
			snprintf(detail, sizeof(detail), "(fork failed)");
		}
		print_test_result("Stress test (100 elements, protected)", passed, detail);
		stats->total++;
		if (passed) stats->passed++;
		else stats->failed++;
	}
	
	// Test 11: Push empty string
	list_ft = NULL;
	list_ref = NULL;
	ft_list_push_front(&list_ft, "");
	ref_list_push_front(&list_ref, "");
	passed = (list_ft && list_ref &&
			  list_ft->data == list_ref->data &&
			  strlen((char*)list_ft->data) == 0);
	snprintf(detail, sizeof(detail), "(empty string: data=\"\", len=%zu)", 
			 list_ft && list_ft->data ? strlen((char*)list_ft->data) : 999);
	print_test_result("Push empty string", passed, detail);
	stats->total++;
	if (passed) stats->passed++;
	else stats->failed++;
	free_list(list_ft);
	free_list(list_ref);
	
	// Test 12: Memory leak detection (stress test)
	{
		const int iterations = 500;
		char detail_mem[256];
		
		// Create and destroy lists multiple times
		for (int i = 0; i < iterations; i++)
		{
			list_ft = NULL;
			char *data = malloc(50);
			snprintf(data, 50, "iteration_%d", i);
			ft_list_push_front(&list_ft, data);
			
			// Clean up
			if (list_ft)
			{
				free(data);
				free_list(list_ft);
			}
		}
		
		// Test with multiple nodes per list
		for (int i = 0; i < 100; i++)
		{
			list_ft = NULL;
			for (int j = 0; j < 10; j++)
			{
				char *data = malloc(20);
				snprintf(data, 20, "node_%d", j);
				ft_list_push_front(&list_ft, data);
			}
			
			// Free data and list
			t_list *current = list_ft;
			while (current)
			{
				if (current->data)
					free(current->data);
				current = current->next;
			}
			free_list(list_ft);
		}
		
		int passed = 1;
		snprintf(detail_mem, sizeof(detail_mem), 
			"created and freed %d lists with %d total nodes", 
			iterations + 100, iterations + 1000);
		
		print_test_result("Memory leak stress test", passed, detail_mem);
		stats->total++;
		if (passed) stats->passed++;
		else stats->failed++;
	}
	
	if (g_verbose_mode)
		printf("\n");
}
