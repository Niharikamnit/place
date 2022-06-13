class Solution {
public:
    void sortColors(vector<int>& a) {
    // coode here 
        int begin=0,mid=0,end=a.size()-1;
        while(mid<=end) {
            if(a[mid]==0) {
                swap(a[begin],a[mid]);
                begin++;
                mid++;
            }
            else if(a[mid]==1){
                mid++;
            }
            else if(a[mid]==2){
                swap(a[end],a[mid]);
                end--;
            }
        }
    }
};