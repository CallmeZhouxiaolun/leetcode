/*
Given n non-negative integers representing the histogram's bar height where the width of each bar is 1, find the area of largest rectangle in the histogram.
Above is a histogram where width of each bar is 1, given height = [2,1,5,6,2,3].
The largest rectangle is shown in the shaded area, which has area = 10 unit.

Example:

Input: [2,1,5,6,2,3]
Output: 10
*/

/*
多种解法可以参照： https://leetcode.wang/leetCode-84-Largest-Rectangle-in-Histogram.html

又开始c++刷题啦，虽然这道题还是不会做，抄评论咯
*/ 
//1. 暴力法，我们分别以所有的高度来进行计算，这里的高度就是数组元素值


class Solution {
public:
    int largestRectangleArea(vector<int>& heights) {
        if(heights.empty())
            return 0;
        set<int> st(heights.begin(), heights.end());
        int res=0;
        for(int height : st){
            int width=0, max_width=0;
            for(int i=0; i<heights.size(); i++){
                if(heights[i] >= height){
                    width++;
                    max_width = max(width, max_width);
                }
                else
                    width=0;
                    
            }
            res = max(res, max_width * height);
        }
        return res;
    }
};
/*
TLE了，我们来分析一下
这里主要的耗时在于针对每一个长度，我们的需要重新遍历对应的最大宽度，想一想能不能保存一下最大的宽度呢？
1. 首先遍历数组，以每一个元素值为高度，那他的最大宽度是什么呢？ 
    左右边界应该为最后一个>=该元素的索引位置（即下一个位置元素值<该元素值）
2. 我们能不能通过一次遍历来获取所有元素对应的左右边界？
    我们去找第一个小于当前元素的索引位置
*/    
class Solution {
public:
    int largestRectangleArea(vector<int>& heights) {
        if(heights.empty())
            return 0;
        vector<int> left(heights.size());
        left[0] = -1;
        for(int i=1; i<heights.size(); i++){
            int p = i-1;
            while(p >=0 && heights[p] >= heights[i])
                p = left[p];  //注意heights[p] >= heights[i] 则p可以直接跳到left[p]，因为left[p] - p之间的元素都大于heights[p]，肯定也大于heights[i]；
            left[i] = p;
        }
        vector<int> right(heights.size());
        right[heights.size()-1] = heights.size();
        for(int i=heights.size()-2; i>=0; i--){
            int p = i+1;
            while(p<heights.size() && heights[p] >= heights[i])
                p = right[p];
            right[i] = p;
        }
        
        int res =0;
        for(int i=0; i<heights.size(); i++){
            res = max(res, heights[i] *(right[i]- left[i] -1));
        }
        return res;
    }
};

/*
3. 通过2的解法，我们知道其实质在于寻找左右两边第一个小于当前元素的索引位置，这里我们可以使用栈来解决；
    1. 栈为空：push(i)，将当前元素索引推入
    2. 若heights[i] >= heights[st.top()] 我们不断将当前元素推入，这里就可以发现栈中存储元素其实是递增的
        也就是说前一个元素就是后一个元素对应的“第一个小于当前元素的索引位置”  这样我们其实以及找到了左边界
    3. heights[i] < heights[st.top()],此时我们找到了右边界，而左边界已经在栈中，我们可以开始处理以heights[st.top()]为高的矩形面积

    4. 最后，我们遍历完所有元素后，栈中可能还有元素，此时代表右边没有元素比heights[st.top()]更小，即右边界为heights.size()
*/
class Solution {
public:
    int largestRectangleArea(vector<int>& heights) {
        if(heights.empty())
            return 0;
        stack<int> st;
        int res=0;
        for(int i=0; i<heights.size(); i++){
            while(!st.empty() && heights[i] < heights[st.top()]){
                int height = heights[st.top()];
                st.pop();
                int left = st.empty() ? -1:st.top();
                res = max(res, height * (i-left-1));
            }
            st.push(i);
        }
        cout<<res<<endl;
        while(!st.empty()){
            int height = heights[st.top()];
            st.pop();
            int left = st.empty() ? -1 : st.top();
            int right = heights.size();
            res = max(res, height *(right-left-1));
        }
        return res;
    }
};

/*
4. 线段树
具体思路可以参考 ：https://leetcode.wang/leetCode-84-Largest-Rectangle-in-Histogram.html
目的转换为最小元素索引位置，这类最值、区间问题常常考虑线段树
线段树介绍可以参照： https://zhuanlan.zhihu.com/p/34150142
*/
class SegTreeNode{
public:
    int start;
    int end;
    int min_index;
    SegTreeNode* left;
    SegTreeNode* right;
    
    SegTreeNode(int start_, int end_){
        this->start = start_;
        this->end = end_;
        left = right = NULL;
    }
};


class Solution {
public:
    int largestRectangleArea(vector<int>& heights) {
        if(heights.empty())
            return 0;
        SegTreeNode* root = buildSegTree(heights, 0, heights.size()-1);
        return getMaxArea(root, heights, 0, heights.size()-1);
    }
    
    int getMaxArea(SegTreeNode* root, vector<int>& heights, int start, int end){
        if(start > end)
            return -1;
        if(start == end)
            return heights[start];
        int min_index = query(heights, root, start, end);
        int left_max = getMaxArea(root, heights, start, min_index-1);
        int right_max = getMaxArea(root, heights, min_index+1, end);
        return max(heights[min_index] *(end-start+1), max(left_max, right_max));
    }
    
    SegTreeNode* buildSegTree(vector<int>& heights, int start , int end){
        if(start > end)
            return NULL;
        SegTreeNode* root = new SegTreeNode(start, end);
        if(start == end){
            root->min_index = start;
            return root;
        }else{
            int mid = (start+ end)/2;
            root->left = buildSegTree(heights, start, mid);
            root->right = buildSegTree(heights, mid+1, end);
            root->min_index = heights[root->left->min_index] > heights[root->right->min_index] ? 
                root->right->min_index : root->left->min_index;
            return root;
        }
    } 
    
    int query(vector<int>& heights, SegTreeNode* root,  int start , int end){
        if(root ==NULL || start > root->end || end < root->start)
            return -1;
        if(start <= root->start && end>= root->end)
            return root->min_index;
        
        int left_min = query(heights, root->left, start, end);
        int right_min = query(heights, root->right, start, end);
        if(left_min ==-1)
            return right_min;
        if(right_min == -1)
            return left_min;
        return heights[left_min] > heights[right_min] ? right_min: left_min;
    }
};


//5. 这个我累了，直接参照上面的连接把，，，人傻了
class Solution {
public:
    int largestRectangleArea(vector<int>& heights) {
        if(heights.empty())
            return 0;
        return getMaxArea(heights, 0, heights.size()-1);
    }
    
    int getMaxArea(vector<int>& heights, int left, int right){
        if(left > right)
            return -1;
        if(left == right)
            return heights[left];
        int mid = left + (right-left)/2;
        int left_area = getMaxArea(heights, left, mid);
        int right_area = getMaxArea(heights, mid+1, right);
        int mid_area = getMidArea(heights, left, mid, right);
        return max(mid_area, max(left_area,right_area));
    }
    
    int getMidArea(vector<int>& heights, int left, int mid, int right){
        int i=mid, j=mid+1;
        int minHeight = min(heights[i], heights[j]);
        int area = minHeight * 2;
        while(i>=left && j<=right){
            minHeight = min(min(heights[i], heights[j]), minHeight);
            area = max(area, minHeight*(j-i+1));
            if(i==left)
                j++;
            else if(j==right)
                i--;
            else if(heights[i-1] > heights[j+1])
                i--;
            else
                j++;
        }
        return area;
    }
    
};




/*

java解法！！！！

1. 首先采用暴力法，计算每个高度的最大面积，也就是说计算每个高度的最大宽度，最后求取最大面积
暴力法，获取每个高度的最大面积
class Solution {
    public int largestRectangleArea(int[] heights) {
        if(heights.length ==0)
            return 0;
        Set<Integer> heightSet = new HashSet<>();
        for(int i=0; i< heights.length; i++){
            heightSet.add(heights[i]);
        }
        
        int max_area =0 ;
        for(int height :heightSet){
            int width =0, max_width=1;
            for(int i=0; i< heights.length; i++){
                if(heights[i] >= height){
                    width++;
                }else{
                    max_width = Math.max(max_width, width);
                    width =0;
                }
            }
            max_width = Math.max(max_width, width);
            max_area = Math.max(max_area, max_width* height);
        }
        return max_area;
    }
}


//2. 认识了一种新的数据结构：线段树，这里可以参照讲解:https://zhuanlan.zhihu.com/p/34150142

class Solution {
    public int largestRectangleArea(int[] heights) {
        if(heights.length==0)
            return 0;
        SegmentTree tree = new SegmentTree(heights);
        tree.build(0);
        return getMaxArea(tree, 0, heights.length-1, heights);
    }
    
    public  int getMaxArea(SegmentTree tree, int start, int end, int[] heights){
        if(start > end)
            return Integer.MIN_VALUE;
        if(start == end)
            return heights[start];
        int min_index = tree.query(0, start, end).min_index;
        int area1 = heights[min_index] * (end - start+1);
        int area2 = getMaxArea(tree, start, min_index-1, heights);
        int area3 = getMaxArea(tree, min_index+1, end, heights);
        return Math.max(Math.max(area1, area2), area3);
    }
}

class Node{
    int start; //区间起点
    int end; //区间终点
    int data; //区间最小值
    int min_index; //区间最小值对应的数组中的索引

    public Node(int start, int end){
        this.start = start;
        this.end = end;
    }
}

class SegmentTree
{
    private int[] base;
    private Node[] nodes;

    public SegmentTree(int[] nums){
        base = new int[nums.length];
        for(int i=0; i<nums.length; i++){
            base[i] = nums[i];
        }

        nodes = new Node[4* nums.length];
    }

    public void build(int index){
        Node node = nodes[index];
        if(node == null){ //等于null表示根节点，需要进行创建
            nodes[index] = new Node(0, base.length-1);
            node = nodes[index];
        }

        if(node.start == node.end){
            node.data = base[node.start];
            node.min_index = node.start;
        }else{
//            int mid= node.start + (node.end - node.start)>>1;
            int mid= (node.start+node.end)>>1;
            nodes[(index<<1) +1] = new Node(node.start, mid);
            nodes[(index<<1) +2] = new Node(mid+1, node.end);
            build((index<<1)+1);
            build((index<<1)+2);
            if(nodes[(index<<1)+1].data > nodes[(index<<1)+2].data){
                node.data = nodes[(index<<1)+2].data;
                node.min_index = nodes[(index<<1)+2].min_index;
            }else{
                node.data = nodes[(index<<1)+1].data;
                node.min_index = nodes[(index<<1)+1].min_index;
            }
        }
    }

    public Node query(int index, int start, int end){
        Node node = nodes[index];
        if(node.start > end || node.end < start)
            return null;
        if(node.start >=start && node.end <= end){
            return node;
        }
        Node left = query((index<<1)+1, start, end);
        int data_left = left ==null? Integer.MAX_VALUE: left.data;
        Node right = query((index<<1)+2, start, end);
        int data_right = right ==null? Integer.MAX_VALUE : right.data;

        return data_left> data_right? right :left;
    }
}

3. 相对于2方法的调整找到最小值位置时间复杂度使整体达到nlog(n)，这里我们强制中分，对于第三区域的计算：
采用贪婪的思想，每次选两边较高的柱子扩展柱子。然后其实就是求出了 2 个柱子，3 个柱子，4 个柱子，5 个柱子...每种情况的最大值，然后选最大的就可以了。
①初始的时候是两个柱子，记录此时的面积。②然后加 1 个柱子，选取两边较高的柱子，然后计算此时的面积，更新最大区域面积。
③不停的重复过程 ②，直到所有柱子遍历完

class Solution {
    public int largestRectangleArea(int[] heights) {
        if(heights.length ==0)
            return 0;
        return getMaxArea(heights, 0, heights.length-1);
    }
    
    public  int getMaxArea(int[] heights, int left, int right){
        if(left == right)
            return heights[left];
        int mid = left+ ((right-left)>>1);
        int area1 = getMaxArea(heights, left, mid);
        int area2 = getMaxArea(heights, mid+1, right);
        int area3 = getMaxArea(heights, left, mid, right);
        return Math.max(Math.max(area1, area2), area3);
    }
    public  int getMaxArea(int[] heights, int left,int mid, int right){
        int i= mid, j= mid+1;
        int min =Math.min(heights[i], heights[j]);
        int max_area = min*2;
        while(i>=left && j<=right){
            min = Math.min(min, Math.min(heights[i], heights[j]));
            max_area= Math.max(max_area, min*(j-i+1));
            if(i==left)
                j++;
            else if(j==right)
                i--;
            else{
                if(heights[i-1] > heights[j+1])
                    i--;
                else
                    j++;
            }
        }
        return max_area;
    }
    
}

4. 使用动态规划的算法，我们分别计算包含每个bar的最大矩形的面积，然后求取最大。其中也就是从当前点向两边扩散，分别找到第一个比当前值小的值的位置
主要是找的过程可以优化为O（n）
class Solution {
    public int largestRectangleArea(int[] heights) {
        if(heights.length ==0)
            return 0;
        int[] leftMin = new int[heights.length];
        int[] rightMin = new int[heights.length];
        leftMin[0] =-1; 
        rightMin[heights.length-1] = heights.length;
        for(int i=1; i< heights.length; i++){
            int p=i-1;
            while(p>=0 && heights[p] >= heights[i])
                p= leftMin[p]; //略过中间元素，直接从比p位置小的第一个元素开始即可，因为比p大的肯定比i大
            leftMin[i] = p;
        }
        for(int i= heights.length-2; i>=0; i--){
            int p= i+1;
            while(p<heights.length && heights[p] >= heights[i])
                p = rightMin[p];
            rightMin[i] =p;
        }
        
        int maxArea =0;
        for(int i=0; i<heights.length; i++){
            maxArea = Math.max(maxArea, (rightMin[i]- leftMin[i]-1)*heights[i]);
        }
        return maxArea;
    }
}

5. 具体讲解见链接，其实就是大于栈顶当前元素就进栈，一直遇到比栈顶元素小的，即栈顶元素右边第一个小于他的值，栈中相邻元素为左边第一个小于它的值；
其实第四种方法类似，只是该方法不是按顺序计算每个柱子的面积，而实先计算大柱子的面积，等pop之后计算小柱子的面积

class Solution {
    public int largestRectangleArea(int[] heights) {
        if(heights.length ==0)
            return 0;
        int p=0 ,maxArea =0;
        Stack<Integer> stack = new Stack<>();
        while(p< heights.length){
            if(stack.empty()){
                stack.push(p);
                p++;
            }else{
                int top =stack.peek();
                if(heights[p] >= heights[top]){
                    stack.push(p);
                    p++;
                }else{
                    int height = heights[stack.pop()];
                    int leftMin = stack.empty()? -1: stack.peek();
                    int rightMin = p;
                    maxArea = Math.max(maxArea, height *(rightMin-leftMin-1));
                }
            }
        }
        while(!stack.empty()){
            int height = heights[stack.pop()];
            int leftMin = stack.empty()? -1: stack.peek();
            int rightMin = heights.length;
            maxArea = Math.max(maxArea, height*(rightMin- leftMin-1));
        }
        return maxArea;
    }
}


动态规划的时间效率最高


*/
