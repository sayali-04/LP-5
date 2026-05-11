#include <iostream>
#include <vector>
#include <climits>
#include <ctime>
#include <omp.h>
using namespace std;

int main() {
    cout << "=== Parallel Reduction using OpenMP ===" << endl;

    int n;
    cout << "\nEnter number of elements: ";
    cin >> n;

    vector<int> arr(n);
    cout << "Enter " << n << " elements: ";
    for (int i = 0; i < n; i++) {
        cin >> arr[i];
    }

    
    cout << "\nArray: ";
    for (int i = 0; i < n; i++) {
        cout << arr[i] << " ";
    }
    cout << endl;

 
    cout << "\n--- Sequential Operations ---" << endl;

    clock_t seqStart = clock();

    int seqMin = arr[0];
    for (int i = 1; i < n; i++) {
        if (arr[i] < seqMin)
            seqMin = arr[i];
    }

   
    int seqMax = arr[0];
    for (int i = 1; i < n; i++) {
        if (arr[i] > seqMax)
            seqMax = arr[i];
    }

    long long seqSum = 0;
    for (int i = 0; i < n; i++) {
        seqSum += arr[i];
    }

    double seqAvg = (double)seqSum / n;

    clock_t seqEnd = clock();
    double seqTime = double(seqEnd - seqStart) / CLOCKS_PER_SEC;

    cout << "Min     : " << seqMin << endl;
    cout << "Max     : " << seqMax << endl;
    cout << "Sum     : " << seqSum << endl;
    cout << "Average : " << seqAvg << endl;
    cout << "Time    : " << seqTime << " seconds" << endl;

   
    cout << "\n--- Parallel Operations ---" << endl;

    clock_t parStart = clock();

    int parMin = arr[0];
    #pragma omp parallel for reduction(min: parMin)
    for (int i = 0; i < n; i++) {
        if (arr[i] < parMin)
            parMin = arr[i];
    }

    int parMax = arr[0];
    #pragma omp parallel for reduction(max: parMax)
    for (int i = 0; i < n; i++) {
        if (arr[i] > parMax)
            parMax = arr[i];
    }

    long long parSum = 0;
    #pragma omp parallel for reduction(+: parSum)
    for (int i = 0; i < n; i++) {
        parSum += arr[i];
    }

    double parAvg = (double)parSum / n;

    clock_t parEnd = clock();
    double parTime = double(parEnd - parStart) / CLOCKS_PER_SEC;

    cout << "Min     : " << parMin << endl;
    cout << "Max     : " << parMax << endl;
    cout << "Sum     : " << parSum << endl;
    cout << "Average : " << parAvg << endl;
    cout << "Time    : " << parTime << " seconds" << endl;

    cout << "\n=== Performance Comparison ===" << endl;
    cout << "Sequential Time : " << seqTime << " seconds" << endl;
    cout << "Parallel Time   : " << parTime << " seconds" << endl;
    if (parTime > 0)
        cout << "Speedup         : " << seqTime / parTime << "x faster" << endl;
    else
        cout << "Speedup         : Parallel was too fast to measure!" << endl;

    return 0;
}

//Command to run:  g++ Pr3_MinMax.cpp -fopenmp -o Pr3_MinMax
//                 Pr3_MinMax.exe

/*
Enter number of elements: 6
Enter 6 elements: 10 20 30 40 50 60

=== Parallel Reduction using OpenMP ===

Array: 10 20 30 40 50 60

--- Sequential Operations ---
Min     : 10
Max     : 60
Sum     : 210
Average : 35
Time    : 0 seconds

--- Parallel Operations ---
Min     : 10
Max     : 60
Sum     : 210
Average : 35
Time    : 0.001 seconds

=== Performance Comparison ===
Sequential Time : 0 seconds
Parallel Time   : 0.001 seconds
Speedup         : 0x faster

*/