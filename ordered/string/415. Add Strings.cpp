/*
Given two non-negative integers num1 and num2 represented as string, return the sum of num1 and num2.

Note:

The length of both num1 and num2 is < 5100.
Both num1 and num2 contains only digits 0-9.
Both num1 and num2 does not contain any leading zero.
You must not use any built-in BigInteger library or convert the inputs to integer directly.
*/


/*
比较简单，直接从后往前遍历
*/
class Solution {
public:
    string addStrings(string num1, string num2) {
        int m=num1.size(), n=num2.size();
        int i=m-1, j=n-1, c =0;
        string res="";
        while(i>=0 || j>=0 || c >0){
            int v1 = i>=0 ? num1[i] -'0' : 0;
            int v2 = j>=0 ? num2[j] -'0' : 0;
            int sum = v1 +v2+c;
            c = sum /10;
            sum %=10;
            res += (char)(sum +'0');
            i--; j--;
        }
        reverse(res.begin(), res.end());
        return res;
    }
};
