/*
 * A very simple unit test for our JDArrayStack implementation.
 */

#include <stdio.h>
#include <stdlib.h>
#include "../structures/JDArrayStack.h"

struct Complex_Structure {
	int* x;
	int* y;
};

// Called by JDArrayStack_dealloc to free individual elements remaining in its array. Ownership of popped elements transfer to client.
void custom_deallocator(void* elem)
{
	free(((struct Complex_Structure*)elem)->x);
	free(((struct Complex_Structure*)elem)->y);
}

int main(void)
{
	struct JDArrayStack rs;
	int i;
	int j;
	int useful_dummy = 0;
	int num_test  = 0;
	int pass_rate = 0;
	
	printf("Initializing JDArrayStack variable of integers: rs\n");
	JDArrayStack_init(&rs,sizeof(int),NULL);
	
	// Test #1
	printf("Now pushing 10 integer values onto stack..\n");
	num_test++;
	for (i = 0; i < 1000; ++i) {
		int val = i * i; // 0, 1, 4, 9, 16, 25, 36, 49, 64, 81, ... , 1,000,000
		JDArrayStack_push(&rs,&val);
	}
	useful_dummy = JDArrayStack_count(&rs);
	printf("Count should return the value 1000: %d\n", useful_dummy);
	if (useful_dummy == 1000) pass_rate++;
	
	// Test #2
	num_test++;
	useful_dummy = JDArrayStack_empty(&rs);
	printf("Empty should return false(0): %d\n", useful_dummy);
	if (useful_dummy == 0) pass_rate++;

	// Test #3
	num_test++;
	JDArrayStack_pop(&rs,&useful_dummy);
	printf("The top of the stack should be 998001: %d\n", useful_dummy);
	if (useful_dummy == 998001) pass_rate++;

	// Test #4
	num_test++;
	useful_dummy = JDArrayStack_count(&rs);
	printf("Count should return the value 999: %d\n", useful_dummy);
	if (useful_dummy == 999) pass_rate++;

	// Test #5
	num_test++;
	j = JDArrayStack_count(&rs);
	for (i = 0; i < j; ++i)
		JDArrayStack_pop(&rs, &useful_dummy);
	useful_dummy = JDArrayStack_count(&rs);
	printf("After popping all elements, count should now be 0: %d\n", useful_dummy);
	if (!useful_dummy) pass_rate++;

	// Test #6
	num_test++;
	useful_dummy = JDArrayStack_empty(&rs);
	printf("Our stack should also be empty or TRUE (1): %d\n", useful_dummy);
	if (useful_dummy == 1) pass_rate++;

	JDArrayStack_dealloc(&rs);

	printf("Passed %d of %d test.\n\n", pass_rate, num_test);
	
	/***********************************************************************************/
	/* Now testing an aggregate type. Test uses valgrind to certify we don't have leaks*/

	struct Complex_Structure cs1;
	cs1.x = malloc(sizeof(int) * 9000);
	cs1.y = malloc(sizeof(int) * 9000);
	
	struct Complex_Structure cs2;
	cs2.x = malloc(sizeof(int) * 9000);
	cs2.y = malloc(sizeof(int) * 9000);

	JDArrayStack_init(&rs, sizeof(struct Complex_Structure), &custom_deallocator); // lets pass a custom dealloc function for our elements.
	JDArrayStack_push(&rs, &cs1);
	JDArrayStack_push(&rs, &cs2);
	
	JDArrayStack_dealloc(&rs);

	return 0;
}	
