#include <iostream>
#include <vector> 

using namespace std;

int main(){

    //Creates a vector of integers
    vector<int> numbers;

    //Add elements to the vector using push_back()
    numbers.push_back(10);
    numbers.push_back(20);
    numbers.push_back(30);


    cout << "First element: " << numbers[0] << endl;
    cout << "Second element: " << numbers[1] << endl;


    cout << "Size of the vector: " << numbers.size() << endl;
    cout << "All elements in the vector: ";

    for(int i = 0; i < numbers.size(); i++){
        cout << numbers[i] << " ";
    }    
    cout << endl;

    numbers.pop_back();
    cout << "After pop_back(), size of the vector: " << numbers.size() << endl;

    cout << "Remaining elements: ";
    for(int num : numbers) {
        cout << num << " ";
    }
    cout << endl;


    cout << "First element: " << numbers.front() << endl;
    cout << "Last element: " << numbers.back() << endl;


    numbers.clear();
    cout << "After clearing, size of the vector: "<< numbers.size() << endl;


    return 0;

}