class Solution {
public:
    int findDuplicate(vector<int>& nums) {
        int n=nums.size();
        int k=nums[0];
        //or sum method
        
        for(int i=0;i<n;i++) {
            if (nums[abs(nums[i]) - 1] > 0)
            nums[abs(nums[i]) - 1] = -nums[abs(nums[i]) - 1];
           else
            k=abs(nums[i]);
        }
        return k;
    }
};



I ask this problem in interviews as it has so many approaches (but with the repeated number appearing exacctly twice). I usually give 5 to 15 minutes to the candidate to solve this problem, trying to get the candidate to a O(n) time O(1) space not modifying the input solution.

NOTE: I removed the sum based solution as it works when duplicate number appears only twice, which is not exactly the constraints of this problem.

Summary solutions that I found:

With extra space - O(n) time, O(n) space
using an additional vector
using an unordered_set
Without extra space, modifying the input - O(1) space
marking visted value - O(n) time
sorting input array - O(n log(n)) time
index sort - O(n) time
Without extra space, non modifying the input - O(1) space
brute force - O(n^2)
cycle detection - O(n) time
Solution 1 : Using an additional vector - O(n) time, O(n) space

That is generally the approach that comes to mind immediately, it does not fully take advantage of the problem.

int findDuplicate(vector<int>& nums) {
	vector<int> seen(nums.size()+1);
	for(auto n: nums){
		if(seen[n]) 
			return n;
		seen[n] = true;
	}
	return 0; // return whatever here
}
Note 1: Some could suggest to use vector<bool> to consume less memory. True, but to do that, vector<bool> is a specialized implementation of vector that comes with weird issues (returned element is a proxy, not a reference to a bool) and is quite slow.

Note 2: Some extra effort could be made on this solution to have a single return statement.

Solution 2 : Using an unordered_set - O(n) time, O(n) space

More or less the same as solution 1.

int findDuplicate(vector<int>& nums) {
	unordered_set<int> seen(nums.size()+1);
	for(auto n: nums){
		if(seen.count(n)) 
			return n;
		seen.insert(n);
	}
	return 0; // return whatever here
}
Solution 3: Marking visited value within the array - O(n) time, O(1) space

This approach is similar to solution 1, but a bit more clever: the idea here is to encode the fact that a value has been seen within the input array.
It relies on the observation that the values within the array fit the indexing space, all values are in between [1..n] and the array size is n+1.
A simple way to encode this information is to flip the sign of nums[n] to indicate that value n has been observed. It works beccause it's guaranteed that there is no negative value in the input.

int findDuplicate(vector<int>& nums) {
	for(auto n: nums) {
		int idx = abs(n);
		if(nums[idx] < 0)
			return idx;
		nums[idx] = -nums[idx];
	}
	return 0;
}
Solution 4: Sorting the array then finding adjacent duplicate - O(n log(n)) time, O(1) space

This is very natural approach to solve the problem, however it has some complexity penalty and it does not take advantage of the constraints of the problem. It's implementation in C++ is quite elegant in just 2 lines (when using STL):

int findDuplicate(vector<int>& nums) {
	sort(nums.begin(), nums.end());
	return *adjacent_find(nums.cbegin(), nums.cend());
}
Solution 5: Index sort - O(n) time, O(1) space

Generally after 5 minutes looking at the problem, we notice that once sorted, each value v is stored at index v-1 (i.e. 1 is at index 0, 5 at index 4, etc..). Therefore we can sort the input array in linear time:

The algorithm starts at index 0 and finishes at index n
if the value is already sorted (nums[i] == i+1), then skip to the next value
else, check if there is alreay the same value at the correct index (nums[i] == nums[nums[i]-1]):
if there is one, that's our duplicate
else exchange the value with the one at it's place
int findDuplicate(vector<int>& nums) {
	for(int i = 0; i < nums.size();) {
		if(nums[i] == i+1)
			++i;
		else if(int &n = nums[i]; nums[n-1] == n)
			return n;
		else
			swap(n, nums[n-1]);
	}
	return 0;
}
Solutions not modifying the input and without extra space

Solution 6: Brute force - O(n^2) time, O(1) space - TLE

Surprisingly (or not), this is not the approach that comes first to the mind to candidates (they suggest it when I ask to solve the problem in O(1) space and without modifying the input array). However I consider it fine as a starter solution during an interview.

int findDuplicate(const vector<int>& nums) {
	for(int i=0; i < nums.size()-1; ++i)
		for(int j=i+1; j < nums.size(); ++j)
			if(nums[i] == nums[j])
				return nums[i];
	return 0;
}
Solution 7: Cycle detection using slow fast pointers - O(n) time, O(1) space

The idea here is to apply the slow fast pointer approach to find a cycle in a linked list.
Since the values space [1..n] fits the index space [0..n], the input array can represent an underlying linked list where the next element of nums[i] is nums[nums[i]].
The existence of a duplicate in the array will therefore create a cycle in the underlying linked list as 2 nodes of the linked list will point to the same next element.
So now, the problem is the same as 142. Linked List Cycle II, please refer to this solution for the explanation of the slow fast pointer approach to solve this problem.

int findDuplicate(const vector<int>& nums) {
	int slow = nums[0], fast = nums[slow];
	while(slow != fast) {
		slow = nums[slow];
		fast = nums[nums[fast]];
	}
	slow = 0;
	while(slow != fast) {
		slow = nums[slow];
		fast = nums[fast];
	}
	return fast;
}
This can be code golfed in 4 unreadable lines:

int findDuplicate(const vector<int>& nums) {
	int n1, n2;
	for(n1=nums[0], n2=nums[n1]; n1!=n2; n1=nums[n1], n2=nums[nums[n2]]);
	for(n1=0; n1 != n2; n1=nums[n1], n2=nums[n2]);
	return n2;
}



Binary Search
Note that the key is to find an integer in the array [1, 2,.., n] instead of finding an integer in the input array.

We can use the binary search algorithm, each round we guess one number, then scan the input array, narrow the search range, and finally get the answer.

According to the Pigeonhole Principle, n + 1 integers, placed in an array of length n, at least 1 integer will be repeated.

So guess a number first(the number mid in the valid range [left, right]), count the elements of the array which is less than or equal to mid in the array.

If cnt is strictly greater than mid. According to the Pigeonhole Principle, repeated elements are in the interval [left, mid];
Otherwise, the repeated element is in the interval [mid + 1, right].
With extra O(1) space, without modifying the input.

    public static int findDuplicate_bs(int[] nums) {
        int len = nums.length;
        int low = 1;
        int high = len - 1;
        while (low < high) {
            int mid = low + (high - low) / 2;
            int cnt = 0;
            for (int i = 0; i < len; i++) {
                if (nums[i] <= mid) {
                    cnt++;
                }
            }

            if (cnt <= mid) {
                low = mid + 1;
            } else {
                high = mid;
            }
        }

        return low;
    }
Analysis
Time Complexity: O(nlogn)
Space Complexity: O(1)

Bit
This method is convert all the numbers to binary numbers. If we can get each digit of the repeated number in binary, we can rebuild the repeated number.

Count all the bits of [1, n] and array numbers as 1 respectively, and then restore them bit by bit to get this repeated number.

For example, the ith digit, note that in the nums array, the sum of all the elements whose ith digit is 1 is x as convert the number to binary.

As the range [1, n], we can also count the sum of the number whose ith digit is 1, we denoted it y.

We can easily get that x > y.

The following table lists whether each bit in the binary of each number is 1 or 0 and what the x and y of the corresponding bit are:

1	3	4	2	2	x	y
Bit 0	1	1	0	0	0	2	2
Bit 1	1	0	1	1	1	3	2
Bit 2	0	0	1	0	0	1	1
From the table, we found that only the 1th bit x > y, so after bitwise restoration target=(010)_2=(2)_10, which is the answer.

The proof of correctness is actually similar to method 1. We can consider the change of the number x of the i-th in different example arrays.

If target appears twice in the test case array, the rest of the numbers appear once, and the ith bit of target is 1, then the nums array x, is exactly one greater than y. If bit i of target is 0, then both are equal.

If target appears three or more times in the array of test cases, then there must be some numbers that are not in the nums array. At this time, it is equivalent to replacing these with target, we consider the impact on x when replacing:

If the i-th bit of the number to be replaced is 1, and the i-th bit of target is 1: x remains unchanged, x > y.

If the i-th bit of the number being replaced is 0, and the i-th bit of target is 1: x plus one, x > y.

If the i-th bit of the number to be replaced is 1, and the i-th bit of target is 0: x minus one, x <= y.

If the i-th bit of the number to be replaced is 0, and the i-th bit of target is 0: x remains unchanged, satisfying x <= y.

Therefore, if the ith bit of target is 1, then after each replacement, only x will be unchanged or increased. If it is 0, only x will be unchanged or decreased.

When x > y, the ith bit of target is 1, otherwise it is 0. We only need to restore this repeated number bitwise.

With extra O(1) space, without modifying the input.

    public static int findDuplicate_bit(int[] nums) {
        int n = nums.length;
        int ans = 0;
        int bit_max = 31;
        while (((n - 1) >> bit_max) == 0) {
            bit_max -= 1;
        }

        for (int bit = 0; bit <= bit_max; ++bit) {
            int x = 0, y = 0;
            for (int i = 0; i < n; ++i) {
                if ((nums[i] & (1 << bit)) != 0) {
                    x += 1;
                }
                if (i >= 1 && ((i & (1 << bit)) != 0)) {
                    y += 1;
                }
            }
            if (x > y) {
                ans |= 1 << bit;
            }
        }

        return ans;
    }
Analysis
Time Complexity: O(nlogn)
Space Complexity: O(1)
