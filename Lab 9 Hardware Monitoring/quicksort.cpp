/* quicksort.cpp */
// Credits: https://www.geeksforgeeks.org/cpp-program-for-quicksort/
#include <bits/stdc++.h>
#include <stdlib.h>
#include <time.h>
#include <chrono>
using namespace std;

unsigned int const NUM_ELEMENTS = 50'000'000;


// A utility function to swap two elements
void swap(int* a, int* b) {
    int t = *a;
    *a = *b;
    *b = t;
}

/* This function takes last element as pivot, places
the pivot element at its correct position in sorted
array, and places all smaller (smaller than pivot)
to left of pivot and all greater elements to right
of pivot */
int partition(int arr[], int low, int high) {
    int pivot = arr[high];  // pivot
    int i = (low - 1);      // Index of smaller element and indicates the right position of pivot found so far

    for (int j = low; j <= high - 1; j++) {
        // If current element is smaller than the pivot
        if (arr[j] < pivot) {
            i++;  // increment index of smaller element
            swap(&arr[i], &arr[j]);
        }
    }
    swap(&arr[i + 1], &arr[high]);
    return (i + 1);
}

/* The main function that implements QuickSort
arr[] --> Array to be sorted,
low --> Starting index,
high --> Ending index */
void quickSort(int arr[], int low, int high) {
    if (low < high) {
        /* pi is partitioning index, arr[p] is now
        at right place */
        int pi = partition(arr, low, high);

        // Separately sort elements before
        // partition and after partition
        quickSort(arr, low, pi - 1);
        quickSort(arr, pi + 1, high);
    }
}

/* Function to print an array */
void printArray(int arr[], int size) {
    int i;
    for (i = 0; i < size; i++)
        cout << arr[i] << " ";
    cout << endl;
}

bool sort_verify(int arr[], int n) {
    for (int i = 1; i < n; ++i) {
        if (arr[i - 1] > arr[i]) {
            return false;
        }
    }

    return true;
}

bool quick_sort_top_level() {
    int* arr = new int[NUM_ELEMENTS];
    for (int i = 0; i < NUM_ELEMENTS; i++) {
        arr[i] = rand();
    }

    // cout << "Given array is \n";
    // printArray(arr, NUM_ELEMENTS);

    quickSort(arr, 0, NUM_ELEMENTS - 1);
    // cout << "\nSorted array is \n";
    // printArray(arr, NUM_ELEMENTS);

    bool isSorted = sort_verify(arr, NUM_ELEMENTS);
    delete[] arr;

    return isSorted;
}

// Driver Code
int main() {
    srand(time(NULL));

    int const TEST_TIMES = 5;
    int64_t test_results[TEST_TIMES];
    long long totalMilliseconds = 0;

    for (int i = 0; i < TEST_TIMES; i++) {
        auto start = std::chrono::system_clock::now();
        bool isSorted = quick_sort_top_level();
        auto end = std::chrono::system_clock::now();

        std::chrono::duration<double> elapsed = end - start;
        auto x = std::chrono::duration_cast<chrono::milliseconds>(elapsed);
        totalMilliseconds += x.count();
        cout << "Quick Sort [" << i + 1 << "]\tsorted: " << (isSorted ? "true" : "false") << "\tTook " << to_string(x.count()) << " ms\tn=" << NUM_ELEMENTS << endl;
    }
    
    long long average_milliseconds = (totalMilliseconds / TEST_TIMES);
    cout << "Average Milliseconds: " << average_milliseconds << " ms\n";
    cout << "Average Milliseconds per element: " << (double)average_milliseconds / NUM_ELEMENTS << " ms/element" << endl;
    
    return 0;
}
