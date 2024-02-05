#include <iostream>
#include <bits/stdc++.h>
#include <utility>
#include <vector>
#include <chrono>
#include <thread>
#include <mutex>
#include <condition_variable>

using namespace std;
using namespace std::chrono;

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

class ThreadPool {
public:
    ThreadPool(int n) : num_threads(n) {}

    void start_threads(vector<ii> &intervals, vector<int> &arr) {
        for (int i = 0; i < num_threads; ++i) {
            threads.emplace_back(&ThreadPool::thread_worker, this, ref(intervals), ref(arr));
        }
    }

    void wait_for_threads() {
        for (auto &thread : threads) {
            thread.join();
        }
    }

private:
    int num_threads;
    vector<thread> threads;
    mutex mtx;
    condition_variable cv;

    void thread_worker(vector<ii> &intervals, vector<int> &arr) {
        unique_lock<mutex> lock(mtx);

        while (!intervals.empty()) {
            ii interval = intervals.front();
            intervals.erase(intervals.begin());
            
            //cout << "Thread working on interval [" << interval.first << ", " << interval.second << "]" << endl;

            lock.unlock();

            merge(arr, interval.first-1, interval.second-1);

            lock.lock();
        }

        cv.notify_all();
    }
};

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
	std::vector<int> arr(n);
	for (int j=0;j<n;j++)
	{
		arr[j] = j+1;
	} //fill with 1 to n
	//shuffle 
    std::shuffle(arr.begin(), arr.end(), std::default_random_engine(seed));
	//start timer when we start mergesorting
	auto begin = std::chrono::high_resolution_clock::now();
    // TODO: Call the generate_intervals method to generate the merge sequence
    vector<ii> intervals = generate_intervals(1,n);
	
    // TODO: Call merge on each interval in sequence
    //for (const auto& interval : intervals) {
    //        merge(arr, interval.first-1, interval.second-1);
    //    }

    /* for (int k=0;k<n;k++)
	{
		std::cout << arr.at(k) <<std::endl;
	}
    for (const auto &interval : intervals) {
        cout << "[" << interval.first << ", " << interval.second << "] ";
    } */

    ThreadPool thread_pool(nthread);
    thread_pool.start_threads(intervals, arr);
    thread_pool.wait_for_threads();

    // Once you get the single-threaded version to work, it's time to implement 
    // the concurrent version. Good luck :)
    //end timer when we finished mergesorting
    auto finish = std::chrono::high_resolution_clock::now();
	auto duration = duration_cast<microseconds>(finish - begin);
    cout << "Duration: " << duration.count() << endl; //print time it took to merge sort
    cout << endl;
    /* for (int k=0;k<n;k++)
	{
		std::cout << arr.at(k) <<std::endl;
	} */
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
