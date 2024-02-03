#include <iostream>
#include <bits/stdc++.h>
#include <utility>
#include <vector>

using namespace std;

typedef pair<int,int> ii;
/*
This function generates all the intervals for merge sort iteratively, given the 
range of indices to sort. Algorithm runs in O(n).

Parameters:
start : int - start of range
end : int - end of range (inclusive)

Returns a list of integer pairs indicating the ranges for merge sort.
*/
vector<ii> generate_intervals(int start, int end);

/*
This function performs the merge operation of merge sort.

Parameters:
array : vector<int> - array to sort
s     : int         - start index of merge
e     : int         - end index (inclusive) of merge
*/
void merge(vector<int> &array, int s, int e);

int main(){
    // TODO: Seed your randomizer
	int seed = 1001;
    // TODO: Get array size and thread count from user
    int n, nthread, start, end;
    
	std::cout << "Enter array size: " <<std::endl;
	cin >> n;
	do{
		std::cout << "Enter number of Threads: " <<std::endl;
		cin >> nthread;
		if (nthread < 1)
		{
			std::cout << "Threads cannot be less than 1 " <<std::endl;
		}
	} while (nthread < 1);
    // TODO: Generate a random array of given size
	int array[n];
	for (int j=0;j<n;j++)
	{
		array[j] = j+1;
	} //fill with 1 to n
	//shuffle 
	shuffle(array, array+n,default_random_engine(seed));
	/*for (int k=0;k<n;k++)
	{
		std::cout << array[k] <<std::endl;
	}*/	
    // TODO: Call the generate_intervals method to generate the merge sequence
	vector<ii> intervals = generate_intervals(1,n);
    // TODO: Call merge on each interval in sequence
    // Once you get the single-threaded version to work, it's time to implement 
    // the concurrent version. Good luck :)
}

vector<ii> generate_intervals(int start, int end) {
    vector<ii> frontier;
    frontier.push_back(ii(start,end));
    int i = 0;
    while(i < (int)frontier.size()){
        int s = frontier[i].first;
        int e = frontier[i].second;

        i++;

        // if base case
        if(s == e){
            continue;
        }

        // compute midpoint
        int m = s + (e - s) / 2;

        // add prerequisite intervals
        frontier.push_back(ii(m + 1,e));
        frontier.push_back(ii(s,m));
    }

    vector<ii> retval;
    for(int i = (int)frontier.size() - 1; i >= 0; i--) {
        retval.push_back(frontier[i]);
    }
    return retval;
}

void merge(vector<int> &array, int s, int e) {
    int m = s + (e - s) / 2;
    vector<int> left;
    vector<int> right;
    for(int i = s; i <= e; i++) {
        if(i <= m) {
            left.push_back(array[i]);
        } else {
            right.push_back(array[i]);
        }
    }
    int l_ptr = 0, r_ptr = 0;

    for(int i = s; i <= e; i++) {
        // no more elements on left half
        if(l_ptr == (int)left.size()) {
            array[i] = right[r_ptr];
            r_ptr++;

        // no more elements on right half or left element comes first
        } else if(r_ptr == (int)right.size() || left[l_ptr] <= right[r_ptr]) {
            array[i] = left[l_ptr];
            l_ptr++;
        } else {
            array[i] = right[r_ptr];
            r_ptr++;
        }
    }
}
