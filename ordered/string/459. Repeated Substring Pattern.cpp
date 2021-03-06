/*
Given a non-empty string check if it can be constructed by taking a substring of it and appending multiple copies of the substring together. You may assume the given string consists of lowercase English letters only and its length will not exceed 10000.

 

Example 1:

Input: "abab"
Output: True
Explanation: It's the substring "ab" twice.
Example 2:

Input: "aba"
Output: False
Example 3:

Input: "abcabcabcabc"
Output: True
Explanation: It's the substring "abc" four times. (And the substring "abcabc" twice.)
*/
/*
首先的想法比较简单，直接采用固定长度拼接
*/
class Solution {
public:
    bool repeatedSubstringPattern(string s) {
        if(s.empty())
            return true;
        int n= s.size();
        for(int i=1; i<=n/2; i++){
            int tmp = n%i;
            if(tmp)
                continue;
            string str = "";
            for(int j=0; j< n/i; j++)
                str += s.substr(0, i);
            if(str == s)
                return true;
        }
        return false;
    }
};


/*
 将字符串double，去除所生成的字符串的首位和最后一位(破坏两个s字符串)，如果存在某种模式，那么可以从上半部分取一部分，
 下半部分去一部分组成一个s，
 当然我们也可以通过find找到的index != s.size()来排除第二个s带来的影响，从而起到去除最后一位的目的
*/
class Solution {
public:
    bool repeatedSubstringPattern(string s) {
        // return (s+s).find(s, 1) != s.size();
        return (s+s).substr(1, 2*s.size()-2).find(s) !=-1;
    }
};



/*
本题其实也可以使用kmp算法来做，因为本质上就是从s+s的字符串找s，排除第一个和最后一个字符
KMP算法可以参考28题
    https://github.com/CallmeZhouxiaolun/leetcode/blob/master/ordered/21to30/28.%20Implement%20strStr().md
*/

class Solution {
public:
    bool repeatedSubstringPattern(string s) {
        string query = s+s;
        return kmp(query, s);
    }
    
    bool kmp(string& query, string& pattern){
        int m= query.size(), n= pattern.size();
        vector<int> next(n, -1);
        int i=0, j=-1;
        while(i< n-1){
            if(j==-1 || pattern[i] == pattern[j]){
                i++; j++;
                next[i] = j;
            }else{
                j = next[j];
            }
        }
        
        i=1,  j=0;
        while(i< m-1 && j<n){
            if(j==-1 || query[i] == pattern[j]){
                i++; j++;
            }else{
                j = next[j];
            }
        }
        if(j ==n)
            return true;
        return false;
    }
};
