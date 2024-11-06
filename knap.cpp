#include <iostream>
#include <vector>
#include <algorithm>

using namespace std;

// Structure for an item which stores weight and value
struct Item {
    int value, weight;

    // Constructor
    Item(int value, int weight) : value(value), weight(weight) {}
};

// Comparator function to sort items by value-to-weight ratio
bool compare(Item a, Item b) {
    double r1 = (double)a.value / a.weight;
    double r2 = (double)b.value / b.weight;
    return r1 > r2; // Sort in descending order of value-to-weight ratio
}

// Function to calculate the maximum value for the given weight capacity
double fractionalKnapsack(int W, vector<Item>& items) {
    // Sort items by their value-to-weight ratio
    sort(items.begin(), items.end(), compare);

    double maxValue = 0.0; // Maximum value in knapsack

    // Iterate through sorted items
    for (const auto& item : items) {
        if (W >= item.weight) {
            // If the item can fit in the knapsack, take it fully
            W -= item.weight;
            maxValue += item.value;
        } else {
            // Take the fraction of the remaining item
            maxValue += item.value * ((double)W / item.weight);
            break; // Knapsack is full
        }
    }
    return maxValue;
}

int main() {
    int W; // Knapsack weight capacity
    cout << "Enter the maximum weight capacity of the knapsack: ";
    cin >> W;

    int n;
    cout << "Enter the number of items: ";
    cin >> n;

    vector<Item> items;
    cout << "Enter value and weight for each item:" << endl;
    for (int i = 0; i < n; i++) {
        int value, weight;
        cin >> value >> weight;
        items.emplace_back(value, weight);
    }

    double maxValue = fractionalKnapsack(W, items);
    cout << "The maximum value for the given knapsack capacity is: " << maxValue << endl;

    return 0;
}
