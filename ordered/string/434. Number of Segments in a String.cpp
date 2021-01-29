/*

You are given a string s, return the number of segments in the string. 

A segment is defined to be a contiguous sequence of non-space characters.

 

Example 1:

Input: s = "Hello, my name is John"
Output: 5
Explanation: The five segments are ["Hello,", "my", "name", "is", "John"]
Example 2:

Input: s = "Hello"
Output: 1
Example 3:

Input: s = "love live! mu'sic forever"
Output: 4
Example 4:

Input: s = ""
Output: 0
 

Constraints:

0 <= s.length <= 300
s consists of lower-case and upper-case English letters, digits or one of the following characters "!@#$%^&*()_+-=',.:".
The only space character in s is ' '.
*/

/*
主要是处理连接空格、首尾空格的问题
*/
class Solution {
public:
    int countSegments(string s) {
        int res=0, i=0;
        while(i<s.size()){
            int j = s.find_first_of(' ',i);
            if(j==string::npos){
                res++;
                return res;
            }
            if(j>i){
                i=j+1;
                res++;
            }else
                i++;
            
        }
        return res;
    }
};

/*再简单一点, 抓住每段的末尾进行判断*/
class Solution {
public:
    int countSegments(string s) {
        int res=0;
        s +=' ';
        for(int i=1; i<s.size(); i++){
            if(s[i]==' ' && s[i-1] !=' ')
                res++;
        }
        return res;
    }
};
