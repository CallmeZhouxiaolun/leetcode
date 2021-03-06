/*
Given a 2D binary matrix filled with 0's and 1's, find the largest rectangle containing only 1's and return its area.

Example:

Input:
[
  ["1","0","1","0","0"],
  ["1","0","1","1","1"],
  ["1","1","1","1","1"],
  ["1","0","0","1","0"]
]
Output: 6
*/


/*
首先采用的是暴力方法，我们以每一个"1"作为矩形的右下角，我们从该点向上遍历，获取所有矩形的面积
1. 计算以每个元素为右下角时，同行边的长度，即当前元素为第几个"1";
    "1" : matrix[i][j] = j>0 ? (char)(matrix[i][j-1] +1) : matrix[i][j];
    "0" : continue;
2. 之后我们从该点不断的向上遍历，此时需要注意，随着向上遍历，矩形高度变大，但是宽可能变小，因此需要不断的更新
  width = min(width, matrix[k][j]-'0');
*/
  
class Solution {
public:
    int maximalRectangle(vector<vector<char>>& matrix) {
        if(matrix.empty() || matrix[0].empty())
            return 0;
        int m= matrix.size(), n= matrix[0].size();
        int res=0;
        for(int i=0; i<m; i++){
            for(int j=0; j<n; j++){
                if(matrix[i][j] == '1'){
                    matrix[i][j] = j>0 ? (char)(matrix[i][j-1] +1) : matrix[i][j];
                }else{
                    continue;
                }
                int width = matrix[i][j] -'0';
                for(int k=i; k>= 0; k--){
                    if(matrix[k][j] == '0')
                        break;
                    width = min(width, matrix[k][j]-'0');
                    res = max(res, width * (i-k+1));
                }
            }
        }
        return res;
    }
};
  
/* 
2. 我们可以将本题切分为84题，以图例为例：
[
  ["1","0","1","0","0"],
  ["1","0","1","1","1"],
  ["1","1","1","1","1"],
  ["1","0","0","1","0"]
]
我们分别以每一行为底，可以构成84题的矩形

例如,前三行可以构成[3,1,3,2,2]的heights
["1"," ","1"," "," "],
["1"," ","1","1","1"],
["1","1","1","1","1"],
因此可以调用84题的函数，这里我们也复习一下
①首先是动态规划
*/

class Solution {
public:
    int maximalRectangle(vector<vector<char>>& matrix) {
        if(matrix.empty() || matrix[0].empty())
            return 0;
        int m=matrix.size(), n =matrix[0].size();
        int res=0;
        vector<int> heights(n,0);
        for(int i=0; i<m; i++){
            for(int j=0; j<n; j++){
                if(matrix[i][j] == '1')
                    heights[j] +=1;
                else
                    heights[j]=0;
            }
            res = max(res, largestRectangle(heights));
        }
        return res;
    }
    int largestRectangle(vector<int>& heights){
        if(heights.empty())
            return 0;
        vector<int> left(heights.size());
        left[0] =-1;
        for(int i=1; i<heights.size(); i++){
            int p = i-1;
            while(p >=0 && heights[p] >= heights[i])
                p = left[p];
            left[i] = p;
        }
        vector<int> right(heights.size());
        right[heights.size()-1] = heights.size();
        for(int i=heights.size()-2; i>=0; i--){
            int p= i+1;
            while(p <heights.size() && heights[p] >= heights[i])
                p = right[p];
            right[i] = p;
        }
        int res=0;
        for(int i=0; i<heights.size(); i++){
            int width = right[i] - left[i]-1;
            res = max(res, heights[i]* width);
        }
        return res;
    }
};

//② 栈的使用

int largestRectangle(vector<int>& heights){
    if(heights.empty())
        return 0;
    int res=0;
    stack<int> st;
    for(int i=0 ;i<heights.size(); i++){
        while(!st.empty() && heights[i] < heights[st.top()]){
            int height = heights[st.top()];
            st.pop();
            int left = st.empty()? -1: st.top();
            res = max(res, height * (i-left-1));
        }
        st.push(i);
    }
    while(!st.empty()){
        int height = heights[st.top()];
        st.pop();
        int left = st.empty() ? -1: st.top();
        int right = heights.size();
        res = max(res, height*(right-left-1));
    }
    return res;
}
    
//③ 线段树

class SegTreeNode{
public:
    int start;
    int end;
    int min_;
    SegTreeNode* left;
    SegTreeNode* right;
    SegTreeNode(int start_, int end_){
        start = start_;
        end = end_;
        left=right = NULL;
    }
};

class Solution {
public:
    int maximalRectangle(vector<vector<char>>& matrix) {
        if(matrix.empty() || matrix[0].empty())
            return 0;
        int m=matrix.size(), n =matrix[0].size();
        int res=0;
        vector<int> heights(n,0);
        for(int i=0; i<m; i++){
            for(int j=0; j<n; j++){
                if(matrix[i][j] == '1')
                    heights[j] +=1;
                else
                    heights[j]=0;
            }
            res = max(res, largestRectangle(heights));
        }
        return res;
    }
    int largestRectangle(vector<int>& heights){
        if(heights.empty())
            return 0;
        SegTreeNode* root = buildSegTree(heights, 0, heights.size()-1);
        return getMaxArea(root, heights, 0, heights.size()-1);
    }
    int getMaxArea(SegTreeNode* root, vector<int>& heights, int start, int end){
        if(start > end){
            return -1;
        }
        int min_ = query(root, heights, start, end);
        int left_area = getMaxArea(root, heights, start, min_-1);
        int right_area = getMaxArea(root, heights, min_+1, end);
        return max(max(left_area, right_area), heights[min_]*(end-start+1));
    }
    SegTreeNode* buildSegTree(vector<int>& heights, int start, int end){
        if(start > end)
            return NULL;
        SegTreeNode* root = new SegTreeNode(start , end);
        if(start == end){
            root->min_ = start;
        }else{
            int mid = start + (end- start)/2;
            root->left = buildSegTree(heights, start, mid);
            root->right = buildSegTree(heights, mid+1, end);
            root->min_ = heights[root->left->min_] < heights[root->right->min_] ? root->left->min_ :root->right->min_; 
        }
        return root;
    }
    int query(SegTreeNode* root, vector<int>& heights, int start, int end){
        if(root==NULL || start > root->end || end < root->start)
            return -1;
        if(start <= root->start && end >= root->end)
            return root->min_;
        int left_min = query(root->left, heights, start, end);
        int right_min = query(root->right, heights, start, end);
        if(left_min == -1)
            return right_min;
        if(right_min==-1)
            return left_min;
        return heights[left_min] < heights[right_min] ? left_min : right_min;
    }
};

