#include <iostream>
#include <vector>
#include <cstdlib> // For rand()
#include <ctime>   // For time()
using namespace std;

// Function to swap two elements
void swap(int& a, int& b) {
    int temp = a;
    a = b;
    b = temp;
}

// Deterministic Partition: Using last element as pivot
int deterministicPartition(vector<int>& arr, int low, int high) {
    int pivot = arr[high]; // Fixed pivot
    int i = low - 1;
    for (int j = low; j < high; j++) {
        if (arr[j] < pivot) {
            i++;
            swap(arr[i], arr[j]);
        }
    }
    swap(arr[i + 1], arr[high]);
    return i + 1;
}

// Randomized Partition: Choosing random pivot
int randomizedPartition(vector<int>& arr, int low, int high) {
    int randomIndex = low + rand() % (high - low + 1); // Random index between low and high
    swap(arr[randomIndex], arr[high]); // Swap random element with last element
    return deterministicPartition(arr, low, high);
}

// Deterministic Quick Sort
void deterministicQuickSort(vector<int>& arr, int low, int high) {
    if (low < high) {
        int pi = deterministicPartition(arr, low, high);
        deterministicQuickSort(arr, low, pi - 1);
        deterministicQuickSort(arr, pi + 1, high);
    }
}

// Randomized Quick Sort
void randomizedQuickSort(vector<int>& arr, int low, int high) {
    if (low < high) {
        int pi = randomizedPartition(arr, low, high);
        randomizedQuickSort(arr, low, pi - 1);
        randomizedQuickSort(arr, pi + 1, high);
    }
}

// Utility function to print the array
void printArray(const vector<int>& arr) {
    for (int x : arr) {
        cout << x << " ";
    }
    cout << endl;
}

int main() {
    srand(time(0)); // Seed for random number generation

    vector<int> arr1 = {10, 7, 8, 9, 1, 5};
    vector<int> arr2 = arr1; // Copy for randomized variant

    cout << "Original array:\n";
    printArray(arr1);

    // Deterministic Quick Sort
    cout << "\nDeterministic Quick Sort:\n";
    deterministicQuickSort(arr1, 0, arr1.size() - 1);
    printArray(arr1);

    // Randomized Quick Sort
    cout << "\nRandomized Quick Sort:\n";
    randomizedQuickSort(arr2, 0, arr2.size() - 1);
    printArray(arr2);

    return 0;
}
