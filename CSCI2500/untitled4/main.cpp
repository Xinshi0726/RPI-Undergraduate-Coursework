// you can use includes, for example:
// #include <algorithm>

// you can write to stdout for debugging purposes, e.g.
// cout << "this is a debug message" << endl;
#include <vector>
using namespace std;
int solution(vector<int> &A) {
    // write your code in C++14 (g++ 6.2.0)
    //We use an array to record the time of an integer appeared in A. Trading space with time
    vector<int> table(0);
    //we reserve the capacity of the vector in case the expansion of the vector causes extra time.
    int largest_x = 0;
    table.reserve(100);
    for (int i = 0; i<A.size();i++)
    {
        table[A.at(i)] += 1;
    }

    for (int j = 0; j<table.size();j++)
    {
        if (table[j] == j && j>largest_x)
        {
            largest_x = j;
        }
    }
    return largest_x;
}

int main()
{
    vector<int> temp;
    temp.push_back(7);
    temp.push_back(1);
    temp.push_back(2);
    temp.push_back(8);
    temp.push_back(2);
    solution(temp);
}