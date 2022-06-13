class Solution {
public:
    
    int removeDuplicates(vector<int>& nums) {
        if(nums.size()==0) return 0;
        if(nums.size()==1) return nums[0];
        int k=1;
        int i=1;
        int t=nums[0];
        
        while(i<nums.size()){
            if(t==nums[i]) i++;
            
            else {
                t=nums[i];
                nums[k]=nums[i];
                k++;
                i++;
            }
        }
        return k;
    }
};