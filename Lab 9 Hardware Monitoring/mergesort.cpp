/* mergesort.cpp */
// Credits: https://www.edureka.co/blog/merge-sort-in-cpp
#include <iostream>
#include <stdlib.h>
#include <time.h>
#include <chrono>
using namespace std;

unsigned int const NUM_ELEMENTS = 50'000'000;


void merge(int a[], int Firstindex, int m, int Lastindex); //merges the sub-arrays which are created while division

void mergeSort(int a[], int Firstindex, int Lastindex)
{
    if (Firstindex < Lastindex)
    {

        int m = Firstindex + (Lastindex - Firstindex) / 2;


        mergeSort(a, Firstindex, m);
        mergeSort(a, m + 1, Lastindex);

        merge(a, Firstindex, m, Lastindex);
    }
}

void merge(int a[], int Firstindex, int m, int Lastindex)
{
    int x;
    int y;
    int z;
    int sub1 = m - Firstindex + 1;
    int sub2 = Lastindex - m;


    int* First = new int[sub1];

    int* Second = new int[sub2];

    // copying data to temp arrays
    for (x = 0; x < sub1; x++)
    {
        First[x] = a[Firstindex + x];
    }
    for (y = 0; y < sub2; y++)
    {
        Second[y] = a[m + 1 + y];
    }


    x = 0;
    y = 0;
    z = Firstindex;
    while (x < sub1 && y < sub2)
    {
        if (First[x] <= Second[y])
        {
            a[z] = First[x];
            x++;
        }
        else
        {
            a[z] = Second[y];
            y++;
        }
        z++;
    }
    while (x < sub1)
    {
        a[z] = First[x];
        x++;
        z++;
    }
    while (y < sub2)
    {
        a[z] = Second[y];
        y++;
        z++;
    }

    delete[] First;
    delete[] Second;
}

// UTILITY FUNCTIONS
// Function to print an array
void printArray(int A[], int size) {
    for (int i = 0; i < size; i++)
        cout << A[i] << " ";
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

bool merge_sort_top_level() {
    int* arr = new int[NUM_ELEMENTS];
    for (int i = 0; i < NUM_ELEMENTS; i++) {
        arr[i] = rand();
    }

    // cout << "Given array is \n";
    // printArray(arr, NUM_ELEMENTS);

    mergeSort(arr, 0, NUM_ELEMENTS - 1);

    // cout << "\nSorted array is \n";
    // printArray(arr, NUM_ELEMENTS);

    bool isSorted = sort_verify(arr, NUM_ELEMENTS);
    delete[] arr;

    return isSorted;
}

// Driver code
int main() {
    srand(time(NULL));

    int const TEST_TIMES = 5;
    int64_t test_results[TEST_TIMES];
    long long totalMilliseconds = 0;

    for (int i = 0; i < TEST_TIMES; i++) {
        auto start = std::chrono::system_clock::now();
        bool isSorted = merge_sort_top_level();
        auto end = std::chrono::system_clock::now();

        std::chrono::duration<double> elapsed = end - start;
        auto x = std::chrono::duration_cast<chrono::milliseconds>(elapsed);
        totalMilliseconds += x.count();
        cout << "Merge Sort [" << i + 1 << "]\tsorted: " << (isSorted ? "true" : "false") << "\tTook " << to_string(x.count()) << " ms\tn=" << NUM_ELEMENTS << endl;
    }
        
    long long average_milliseconds = (totalMilliseconds / TEST_TIMES);
    cout << "Average Milliseconds: " << average_milliseconds << " ms\n";
    cout << "Average Milliseconds per element: " << (double)average_milliseconds / NUM_ELEMENTS << " ms/element" << endl;
    
    return 0;
}
