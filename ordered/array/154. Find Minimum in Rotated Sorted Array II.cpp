/*
Suppose an array sorted in ascending order is rotated at some pivot unknown to you beforehand.

(i.e.,  [0,1,2,4,5,6,7] might become  [4,5,6,7,0,1,2]).

Find the minimum element.

The array may contain duplicates.

Example 1:

Input: [1,3,5]
Output: 1
Example 2:

Input: [2,2,2,0,1]
Output: 0
*/

/*
其实和[153. Find Minimum in Rotated Sorted Array]类似，唯一的不同在于多了重复元素，
因此我们采用153题一样的二分解法，唯一的不同在于对重复元素进行处理及记录
*/

class Solution {
public:
    int findMin(vector<int>& nums) {
        if(nums.empty())
            return 0;
        int left=0, right=nums.size()-1;
        int res = INT_MAX;
        while(left < right){
            if(nums[left] == nums[right]){
                res = min(res, nums[left]);
                left++; right--;
            }else{
                int mid = left+(right-left)/2;
                if(nums[mid] > nums[right])
                    left = mid+1;
                else
                    right = mid;
            }
        }
        return min(res, nums[left]);
    }
};

/*
评论区还有一种思路：
1. nums[mid] > nums[right] , min_value在[mid+1, right];
2. nums[mid] < nums[right], min_value 在[left, mid];
3. nums[mid] = nums[right], 此时肯定是nums[left] = nums[right],这里让right--，排除两端相等的状况
*/

class Solution {
public:
    int findMin(vector<int>& nums) {
        if(nums.empty())
            return 0;
        int left=0, right=nums.size()-1;
        while(left < right){
            int mid= left+(right-left)/2;
            if(nums[mid] > nums[right])
                left = mid+1;
            else if(nums[mid] < nums[right])
                right = mid;
            else
                right--;
        }
        return nums[left];
    }
};
