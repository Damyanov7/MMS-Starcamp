#include <stdio.h>
#include <stdlib.h>


int longest_sequence(int *arr, size_t len);
void print_s(int *arr, int t);


int main() {
	int i;
	int arr[10] = { 1, 2, 3, 4, 1, 2, 3, 5, 8, 4 }; 
	int arr1[10] = { 1, 2, 3, 4, 5, 6, 3, 5, 8, 4 };
	int arr2[10] = { 1, 2, 3, 0, 1, 2, 3, 5, 8, 4 };
	int arr3[10] = { 1, 2, 2, 0, 1, 2, 3, 5, 4, 4 };
	int t0 = longest_sequence(arr, 10);
	int t1 = longest_sequence(arr1, 10);
	int t2 = longest_sequence(arr2, 10);
	int t3 = longest_sequence(arr3, 10);
	
	for(i = 0; i < 10; i++)  printf("----");
	printf("\n");
	print_s(arr, t0);
	print_s(arr1, t1);
	print_s(arr2, t2);
	print_s(arr3, t3);
	
	return 0;
}


/* Return starting index of logest sequence */
int longest_sequence(int *arr, size_t len) {
	size_t j;
	size_t count = 0;
	size_t max_count = 0;
	int k;
	
	if(!arr || !len) return -1;
	if(len < 2) return 0;
	
	for(j = 1; j < len; j++) {
		if(arr[j] > arr[j - 1]) {
			count++;
			if(count > max_count) {
				max_count = count;
				k = j - count;
			}
		} else count = 0;
	}
		
	return k;
}



/* Function printing the sequence */
void print_s(int *arr, int t){

	size_t i;
	
	/* printing index */
	printf("The longest seq starts at index: %d\n", t);
	
	/* printing the arr */
	for(i = 0; i < 10; i++) printf("%d", arr[i]);
	printf("\n");
	
	/* printing "^" at the index */
	for(i = 0; i < t; i++) printf(" ");  
	printf("^\n");
	
	/* printing the sequence */
	for(i = t; i < 10; i++) {
		if((i + 1) != 10) {
			if(arr[i] < arr[i + 1]) {
				printf("%d", arr[i]);
			} else {
				printf("%d", arr[i]);
				break;
			}
			
		} else if(arr[i] > arr[i-1]) {
				printf("%d", arr[i]);	
			}
	}
	
	printf("\n");
	for(i = 0; i < 10; i++)  printf("----");
	printf("\n");
	
	return;
}


