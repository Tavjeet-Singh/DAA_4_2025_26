#include <bits/stdc++.h>
#define MAX 100
using namespace std;

int heap[MAX];
int heapSize = 0;

void heapifyDown(int i)
{

    int smallest = i;
    int left = 2 * i + 1;
    int right = 2 * i + 2;
    if (left < heapSize && heap[left] < heap[smallest])
        smallest = left;
    if (right < heapSize && heap[right] < heap[smallest])
        smallest = right;
    if (smallest != i)
    {
        swap(heap[smallest], heap[i]);
        heapifyDown(smallest);
    }
}

void heapifyUp(int i)
{

    while (i > 0 && heap[(i - 1) / 2] > heap[i])
    {

        swap(heap[i], heap[(i - 1) / 2]);
        i = (i - 1) / 2;
    }
}

void insert(int val)
{
    if (heapSize == MAX)
    {
        cout << "heap full";
        return;
    }
    heap[heapSize] = val;
    heapSize++;

    heapifyUp(heapSize - 1);
}

void deleteMin()
{
    if (heapSize == 0)
    {
        cout << "no element";
        return;
    }
    heap[0] = heap[heapSize - 1];
    heapSize--;
    heapifyDown(0);
}

void deleteValue(int val)
{
    if(heapSize == 0 )
    {
        cout<< " no element";
        return;
    }
    int index = -1;
    for ( int i = 0; i< heapSize; i++ )
    {
        if ( heap[i] == val)
        {
            index = i;
            break;
        }
    }

    if ( index == -1 )
    {
        cout<< " Value not found";
        return;
    }

    heap[index] = heap[heapSize - 1];
    heapSize--;

    if( index >= heapSize) return;

    int parent = (index-1)/2;
    if ( index > 0 && heap[index] < heap[parent]) 
    {
        heapifyUp(index);
    }
    else 
    {
        heapifyDown(index);
    }
}

void disp() 
{
    for (int i = 0; i < heapSize; i++)
    {
        cout << heap[i] << " ";
    }
    cout<< "\n";
}

int main()
{
    insert(2);
    insert(4);
    insert(6);
    insert(1);
    disp();
    deleteMin();
    disp();
    deleteValue(4);
    disp();
}
/*
    Question: Why is the complexity O(n) instead of O(n log n) in deleteValue?

    Answer:
        Because Search Operation takes O(n) time, Replacement takes O(1) and Heapify takes O(log n) time in the worst case.
        So Total complexity - O(n) + O(1) + O(log n) = O(n)
*/
