/* insertionsort.cpp */
// Credits: https://www.geeksforgeeks.org/insertion-sort/
#include <bits/stdc++.h>
#include <stdlib.h>
#include <time.h>
#include <chrono>
using namespace std;

unsigned int const NUM_ELEMENTS = 140'000;


/* Function to sort an array using insertion sort*/
void insertionSort(int arr[], int n) {
    int i, key, j;
    for (i = 1; i < n; i++) {
        key = arr[i];
        j = i - 1;

        /* Move elements of arr[0..i-1], that are
        greater than key, to one position ahead
        of their current position */
        while (j >= 0 && arr[j] > key) {
            arr[j + 1] = arr[j];
            --j;
        }
        arr[j + 1] = key;
    }
}

// A utility function to print an array of size n
void printArray(int arr[], int n) {
    int i;
    for (i = 0; i < n; i++)
        cout << arr[i] << " ";
    cout << endl;
}

bool sort_verify(int arr[], int n) {
    for (int i=1; i < n; ++i) {
        if (arr[i-1] > arr[i]) {
            return false;
        }
    }
    
    return true;
}

bool insertion_sort_top_level() {
    int* arr = new int[NUM_ELEMENTS];
    for (int i=0; i < NUM_ELEMENTS; i++) {
        arr[i] = rand();
    }
    
    // cout << "Given array is \n";
    // printArray(arr, NUM_ELEMENTS);

    insertionSort(arr, NUM_ELEMENTS);
    
    // cout << "\nSorted array is \n";
    // printArray(arr, NUM_ELEMENTS);

    bool isSorted = sort_verify(arr, NUM_ELEMENTS);
    delete[] arr;

    return isSorted;
}



/* Driver code */
int main() {
    srand(time(NULL));

    int const TEST_TIMES = 5;
    int64_t test_results[TEST_TIMES];
    long long totalMilliseconds = 0;

    for (int i = 0; i < TEST_TIMES; i++) {
        auto start = std::chrono::system_clock::now();
        bool isSorted = insertion_sort_top_level();
        auto end = std::chrono::system_clock::now();

        std::chrono::duration<double> elapsed = end - start;
        auto x = std::chrono::duration_cast<chrono::milliseconds>(elapsed);
        totalMilliseconds += x.count();
        cout << "Insertion Sort [" << i + 1 << "]\tsorted: " << (isSorted ? "true" : "false") << "\tTook " << to_string(x.count()) << " ms\tn=" << NUM_ELEMENTS << endl;
    }
        
    long long average_milliseconds = (totalMilliseconds / TEST_TIMES);
    cout << "Average Milliseconds: " << average_milliseconds << " ms\n";
    cout << "Average Milliseconds per element: " << (double)average_milliseconds / NUM_ELEMENTS << " ms/element" << endl;
    
}