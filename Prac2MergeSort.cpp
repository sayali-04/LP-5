#include <iostream>
#include <vector>
#include <omp.h>
#include <ctime>
using namespace std;

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
                    // sort left half - Thread 1
                    parallelMergeSort(arr, left, mid, depth + 1);
                }

                #pragma omp section
                {
                    // sort right half - Thread 2
                    parallelMergeSort(arr, mid + 1, right, depth + 1);
                }
            }
        } 
        else {
        
            sequentialMergeSort(arr, left, mid);
            sequentialMergeSort(arr, mid + 1, right);
        }

        merge(arr, left, mid, right);
    }
}

int main() 
{
    cout << "=== Parallel Merge Sort using OpenMP ===" << endl;

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

 
    vector<int> arr1 = arr; 
    vector<int> arr2 = arr;

    cout << "\n--- Sequential Merge Sort ---" << endl;
    clock_t seqStart = clock();
    sequentialMergeSort(arr1, 0, n - 1);
    clock_t seqEnd = clock();
    double seqTime = double(seqEnd - seqStart) / CLOCKS_PER_SEC;

    cout << "Sorted Array: ";
    for (int i = 0; i < n; i++) {
        cout << arr1[i] << " ";
    }
    cout << endl;
    cout << "Time taken: " << seqTime << " seconds" << endl;

  
    cout << "\n--- Parallel Merge Sort ---" << endl;
    clock_t parStart = clock();
    parallelMergeSort(arr2, 0, n - 1, 0);
    clock_t parEnd = clock();
    double parTime = double(parEnd - parStart) / CLOCKS_PER_SEC;

    cout << "Sorted Array: ";
    for (int i = 0; i < n; i++) {
        cout << arr2[i] << " ";
    }
    cout << endl;
    cout << "Time taken: " << parTime << " seconds" << endl;

    cout << "\n=== Performance Comparison ===" << endl;
    cout << "Sequential Time : " << seqTime << " seconds" << endl;
    cout << "Parallel Time   : " << parTime << " seconds" << endl;
    if (parTime > 0)
        cout << "Speedup         : " << seqTime / parTime << "x faster" << endl;
    else
        cout << "Speedup         : Parallel was too fast to measure!" << endl;

    return 0;
}


/*
Enter number of elements: 8
Enter 8 elements: 38 27 43 3 9 82 10 1

=== Parallel Merge Sort using OpenMP ===

Original Array: 38 27 43 3 9 82 10 1

--- Sequential Merge Sort ---
Sorted Array: 1 3 9 10 27 38 43 82
Time taken: 0.000021 seconds

--- Parallel Merge Sort ---
Sorted Array: 1 3 9 10 27 38 43 82
Time taken: 0.000010 seconds

=== Performance Comparison ===
Sequential Time : 0.000021 seconds
Parallel Time   : 0.000010 seconds
Speedup         : 2.1x faster

*/