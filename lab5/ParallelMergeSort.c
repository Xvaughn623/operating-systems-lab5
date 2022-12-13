#include <stdio.h>
#include <pthread.h>
#include <stdlib.h>
#include <time.h>
#include <unistd.h>

#define SIZE 100
#define THREAD_MAX 2
/*
Xavier Vaughn
@02928026
*/
int array[SIZE];
int part = 0;

void fillArrayWithRandomNumbers(int arr[SIZE]) {
    for(int i = 0; i<SIZE; i++) array[i] = rand()%100;
}

void printArray(int arr[SIZE]) {
    for(int i = 0; i < SIZE; i++) printf("%5d", arr[i]);
    printf("\n");
}

typedef struct StartEndIndexes {
    int start;
    int end;
} StartEndIndexes;

// Runs mergesort on the array segment described in the
// argument. Spawns two threads to mergesort each half
// of the array segment, and then merges the results.
void merge(int low, int mid, int high){
    int left[mid - low + 1];
    int right[high - mid];
 
    // n1 is size of left part and n2 is size
    // of right part
    int n1 = mid - low + 1;
    int n2 = high - mid; 
    int i;
    int j;
 
    // storing values in left part
    for (i = 0; i < n1; i++)
        left[i] = array[i + low];
 
    // storing values in right part
    for (i = 0; i < n2; i++)
        right[i] = array[i + mid + 1];
 
    int k = low;
    i = j = 0;
 
    // merge left and right in ascending order
    while (i < n1 && j < n2) {
        if (left[i] <= right[j])
            array[k++] = left[i++];
        else
            array[k++] = right[j++];
    }
 
    // insert remaining values from left
    while (i < n1) {
        array[k++] = left[i++];
    }
 
    // insert remaining values from right
    while (j < n2) {
        array[k++] = right[j++];
    }
}
void merge_sort(int low, int high){
    int mid = low + (high - low) / 2;

    merge_sort(low,mid);

    merge_sort(mid + 1, high);

    merge(low,mid,high);
}
void* threadMergeSort(void* args) {
    int thread_part = part++;

    int low = thread_part * (SIZE/2);
    int high = (thread_part + 1) * (SIZE/2) - 1;

    int mid = low + (high - low) / 2;

    if(low < high){
        merge_sort(low,mid);
        merge_sort(mid + 1, high);
        merge(low,mid,high);
    }
    return NULL;
}

int main() {
    srand(time(0));
    StartEndIndexes sei;
    sei.start = 0;
    sei.end = SIZE - 1;
    
    // 1. Fill array with random numbers.
    fillArrayWithRandomNumbers(array);
    
    // 2. Print the array.
    printf("Unsorted array: ");
    printArray(array);
    
    // 3. Create a 2 threads for merge sort.
    pthread_t threads[THREAD_MAX];
    for (int i = 0; i < THREAD_MAX; i++){
        pthread_create(&threads[i], NULL, merge_sort,(void*)NULL);

        for(int i = 0; i < THREAD_MAX; i++){
            pthread_join(threads[i], NULL);
        }
        merge(0, (THREAD_MAX / 2 - 1) / 2, THREAD_MAX / 2 - 1);
        merge(THREAD_MAX / 2, THREAD_MAX/2 + (THREAD_MAX-1-THREAD_MAX/2)/2, THREAD_MAX - 1);
        merge(0, (THREAD_MAX - 1)/2, THREAD_MAX - 1);
    }
    // 4. Wait for mergesort to finish.
    
    // 5. Print the sorted array.
    printf("Sorted array:   ");
    printArray(array);
}
