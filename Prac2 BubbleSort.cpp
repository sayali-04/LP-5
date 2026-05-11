#include <iostream>
#include <vector>
#include <omp.h>
#include <ctime>
using namespace std;

void sequentialBubbleSort(vector<int> arr) {
    int n = arr.size();
    for (int i = 0; i < n-1; i++) {
        for (int j = 0; j < n-i-1; j++) {
            if (arr[j] > arr[j+1]) {
                swap(arr[j], arr[j+1]);
            }
        }
    }
    cout << "\nSequential Sorted Array: ";
    for (int i = 0; i < n; i++) {
        cout << arr[i] << " ";
    }
    cout << endl;
}

void parallelBubbleSort(vector<int> arr) {
    int n = arr.size();
    for (int i = 0; i < n; i++) {
        if (i % 2 == 1) {
            #pragma omp parallel for
            for (int j = 1; j < n-1; j += 2) {
                if (arr[j] > arr[j+1]) {
                    swap(arr[j], arr[j+1]);
                }
            }
        }
        else {
            #pragma omp parallel for
            for (int j = 0; j < n-1; j += 2) {
                if (arr[j] > arr[j+1]) {
                    swap(arr[j], arr[j+1]);
                }
            }
        }
    }
    cout << "\nParallel Sorted Array:   ";
    for (int i = 0; i < n; i++) {
        cout << arr[i] << " ";
    }
    cout << endl;
}

int main() {
    cout << "=== Parallel Bubble Sort using OpenMP ===" << endl;

    int n;
    cout << "\nEnter number of elements: ";
    cin >> n;

    vector<int> arr(n);
    cout << "Enter " << n << " elements: ";
    for (int i = 0; i < n; i++) {
        cin >> arr[i];
    }

    // print original array
    cout << "\nOriginal Array: ";
    for (int i = 0; i < n; i++) {
        cout << arr[i] << " ";
    }
    cout << endl;

    cout << "\n--- Sequential Bubble Sort ---";
    clock_t seqStart = clock();     
    sequentialBubbleSort(arr);
    clock_t seqEnd = clock();
    double seqTime = double(seqEnd - seqStart) / CLOCKS_PER_SEC;
    cout << "Time taken: " << seqTime << " seconds" << endl;

    cout << "\n--- Parallel Bubble Sort ---";
    clock_t parStart = clock();     
    parallelBubbleSort(arr);
    clock_t parEnd = clock();
    double parTime = double(parEnd - parStart) / CLOCKS_PER_SEC;
    cout << "Time taken: " << parTime << " seconds" << endl;

    cout << "\n=== Performance Comparison ===" << endl;
    cout << "Sequential Time : " << seqTime << " seconds" << endl;
    cout << "Parallel Time   : " << parTime << " seconds" << endl;
    if(parTime > 0)
        cout << "Speedup         : " << seqTime/parTime << "x faster" << endl;
    else
        cout << "Speedup         : Parallel was too fast to measure!" << endl;

    return 0;
}

/*
Enter number of elements: 6
Enter 6 elements: 64 34 25 12 22 11

=== Parallel Bubble Sort using OpenMP ===

Original Array: 64 34 25 12 22 11

--- Sequential Bubble Sort ---
Sequential Sorted Array: 11 12 22 25 34 64
Time taken: 0.000015 seconds

--- Parallel Bubble Sort ---
Parallel Sorted Array:   11 12 22 25 34 64
Time taken: 0.000008 seconds

=== Performance Comparison ===
Sequential Time : 0.000015 seconds
Parallel Time   : 0.000008 seconds
Speedup         : 1.875x faster

*/
