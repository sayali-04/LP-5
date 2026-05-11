#include <iostream>
#include <vector>
#include <omp.h>
#include <ctime>

using namespace std;

// ======================================================
// BUBBLE SORT
// ======================================================

void sequentialBubbleSort(vector<int> arr) {
    int n = arr.size();

    for (int i = 0; i < n - 1; i++) {
        for (int j = 0; j < n - i - 1; j++) {
            if (arr[j] > arr[j + 1]) {
                swap(arr[j], arr[j + 1]);
            }
        }
    }

    cout << "\nSequential Bubble Sorted Array: ";
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
            for (int j = 1; j < n - 1; j += 2) {
                if (arr[j] > arr[j + 1]) {
                    swap(arr[j], arr[j + 1]);
                }
            }

        } else {

            #pragma omp parallel for
            for (int j = 0; j < n - 1; j += 2) {
                if (arr[j] > arr[j + 1]) {
                    swap(arr[j], arr[j + 1]);
                }
            }
        }
    }

    cout << "\nParallel Bubble Sorted Array: ";
    for (int i = 0; i < n; i++) {
        cout << arr[i] << " ";
    }
    cout << endl;
}

// ======================================================
// MERGE SORT
// ======================================================

void merge(vector<int>& arr, int left, int mid, int right) {

    int n1 = mid - left + 1;
    int n2 = right - mid;

    vector<int> L(n1), R(n2);

    for (int i = 0; i < n1; i++)
        L[i] = arr[left + i];

    for (int j = 0; j < n2; j++)
        R[j] = arr[mid + 1 + j];

    int i = 0, j = 0, k = left;

    while (i < n1 && j < n2) {

        if (L[i] <= R[j]) {
            arr[k] = L[i];
            i++;
        } else {
            arr[k] = R[j];
            j++;
        }
        k++;
    }

    while (i < n1) {
        arr[k] = L[i];
        i++;
        k++;
    }

    while (j < n2) {
        arr[k] = R[j];
        j++;
        k++;
    }
}

void sequentialMergeSort(vector<int>& arr, int left, int right) {

    if (left < right) {

        int mid = (left + right) / 2;

        sequentialMergeSort(arr, left, mid);

        sequentialMergeSort(arr, mid + 1, right);

        merge(arr, left, mid, right);
    }
}

void parallelMergeSort(vector<int>& arr, int left, int right, int depth) {

    if (left < right) {

        int mid = (left + right) / 2;

        if (depth <= 3) {

            #pragma omp parallel sections
            {

                #pragma omp section
                {
                    parallelMergeSort(arr, left, mid, depth + 1);
                }

                #pragma omp section
                {
                    parallelMergeSort(arr, mid + 1, right, depth + 1);
                }
            }

        } else {

            sequentialMergeSort(arr, left, mid);
            sequentialMergeSort(arr, mid + 1, right);
        }

        merge(arr, left, mid, right);
    }
}

// ======================================================
// MAIN
// ======================================================

int main() {

    cout << "=== Parallel Bubble Sort and Merge Sort using OpenMP ===" << endl;

    int n;

    cout << "\nEnter number of elements: ";
    cin >> n;

    vector<int> arr(n);

    cout << "Enter " << n << " elements: ";

    for (int i = 0; i < n; i++) {
        cin >> arr[i];
    }

    cout << "\nOriginal Array: ";
    for (int i = 0; i < n; i++) {
        cout << arr[i] << " ";
    }
    cout << endl;

    // ======================================================
    // BUBBLE SORT
    // ======================================================

    cout << "\n========== BUBBLE SORT ==========" << endl;

    clock_t seqStart1 = clock();

    sequentialBubbleSort(arr);

    clock_t seqEnd1 = clock();

    double seqTime1 = double(seqEnd1 - seqStart1) / CLOCKS_PER_SEC;

    clock_t parStart1 = clock();

    parallelBubbleSort(arr);

    clock_t parEnd1 = clock();

    double parTime1 = double(parEnd1 - parStart1) / CLOCKS_PER_SEC;

    cout << "Sequential Time : " << seqTime1 << " seconds" << endl;
    cout << "Parallel Time   : " << parTime1 << " seconds" << endl;

    if (parTime1 > 0)
        cout << "Speedup         : " << seqTime1 / parTime1 << "x faster" << endl;

    // ======================================================
    // MERGE SORT
    // ======================================================

    vector<int> arr1 = arr;
    vector<int> arr2 = arr;

    cout << "\n========== MERGE SORT ==========" << endl;

    clock_t seqStart2 = clock();

    sequentialMergeSort(arr1, 0, n - 1);

    clock_t seqEnd2 = clock();

    double seqTime2 = double(seqEnd2 - seqStart2) / CLOCKS_PER_SEC;

    cout << "\nSequential Merge Sorted Array: ";

    for (int i = 0; i < n; i++) {
        cout << arr1[i] << " ";
    }

    cout << endl;

    cout << "Sequential Time : " << seqTime2 << " seconds" << endl;

    clock_t parStart2 = clock();

    parallelMergeSort(arr2, 0, n - 1, 0);

    clock_t parEnd2 = clock();

    double parTime2 = double(parEnd2 - parStart2) / CLOCKS_PER_SEC;

    cout << "\nParallel Merge Sorted Array: ";

    for (int i = 0; i < n; i++) {
        cout << arr2[i] << " ";
    }

    cout << endl;

    cout << "Parallel Time   : " << parTime2 << " seconds" << endl;

    if (parTime2 > 0)
        cout << "Speedup         : " << seqTime2 / parTime2 << "x faster" << endl;

    return 0;

}

//command to run :g++  Pr2_Merge&bubblesort.cpp -fopenmp -o Pr2_Merge&bubblesort
//                Pr2_MergeBubblesort.exe  

/* OUTPUT
=== Parallel Bubble Sort and Merge Sort using OpenMP ===

Enter number of elements: 6
Enter 6 elements: 64 34 25 12 22 11

Original Array: 64 34 25 12 22 11 

========== BUBBLE SORT ==========

Sequential Bubble Sorted Array: 11 12 22 25 34 64 

Parallel Bubble Sorted Array: 11 12 22 25 34 64 
Sequential Time : 0.001 seconds
Parallel Time   : 0.011 seconds
Speedup         : 0.0909091x faster

========== MERGE SORT ==========

Sequential Merge Sorted Array: 11 12 22 25 34 64 
Sequential Time : 0 seconds

Parallel Merge Sorted Array: 11 12 22 25 34 64 
Parallel Time   : 0.001 seconds
Speedup         : 0x faster
*/

