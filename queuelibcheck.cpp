#include <iostream>
#include <queue>

using namespace std;


int main(){

    //Declare a queue of integers
    queue<int> myQueue;

    //Push elements onto the queue
    myQueue.push(10);
    myQueue.push(20);
    myQueue.push(30);
    myQueue.push(40);

    //Checks if the queue is empty
    cout << "Is the queue empty? " << (myQueue.empty() ? "Yes":"No") << endl;


    //Print the size of the queue
    cout << "Queue size: "<< myQueue.size() << endl;

    //Access and remove elements from the queue
    while(!myQueue.empty()){
        cout <<  "Front element: " << myQueue.front() << endl;
        myQueue.pop();
    }

    cout << "Is the queue empty now? " << (myQueue.empty() ? "Yes" : "No") << endl;



    return 0;
}