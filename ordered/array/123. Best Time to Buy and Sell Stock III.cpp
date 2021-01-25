/*
Say you have an array for which the ith element is the price of a given stock on day i.

Design an algorithm to find the maximum profit. You may complete at most two transactions.

Note: You may not engage in multiple transactions at the same time (i.e., you must sell the stock before you buy again).

Example 1:

Input: [3,3,5,0,0,3,1,4]
Output: 6
Explanation: Buy on day 4 (price = 0) and sell on day 6 (price = 3), profit = 3-0 = 3.
             Then buy on day 7 (price = 1) and sell on day 8 (price = 4), profit = 4-1 = 3.
*/


/*
我们的目标是前n天最多交易两次的情况下的最大收益，这里有两个量化参数：n天、2次（定义为k天）
也就是说我们的目标在于前n天最多交易k次的情况下的最大收益
因此，我们定义dp[i][k] 为前i天最多交易k次时最大收益
针对第i天，我们进行一下操作
1. 买入/不操作，则：dp[i]][k] = dp[i-1][k];
2. 卖出操作，则代表0-（i-1）天必须有一天买入，假设j天买入，则dp[i][k] = prices[i] - prices[j] + dp[j-1][k-1] (因为j-i进行了一次交易，所以变成k-1)
则 dp[i][k] = max(dp[i-1][k],  prices[i] - prices[j] + dp[j-1][k-1]);
综上所述，我们已经完成了最终目标的解决方案，这里我们进一步细化 prices[i] - prices[j] + dp[j-1][k-1]
我们需要计算 j=0 - j=（i-1）中最大的 prices[i] - prices[j] + dp[j-1][k-1]；
又  prices[i] - prices[j] + dp[j-1][k-1] =  prices[i] - （prices[j] - dp[j-1][k-1]）
因此我们需要遍历j=0 - j=（i-1）获得最小的 prices[j] - dp[j-1][k-1]

其实仔细想一下: 假设我们第j天买入，则j-1天只能进行卖出/不操作，则dp[j]][k-1] = dp[j-1][k-1]
则上述表达式变为： dp[i][k] = max(dp[i-1][k],  prices[i] - prices[j] + dp[j][k-1]);

按照dp转换方程，我们可以写代码如下：
*/
class Solution {
public:
    int maxProfit(vector<int>& prices) {
        if(prices.empty())
            return 0;
        int K= 2, n=prices.size();
        vector<vector<int>> dp(n, vector<int>(K+1,0));
        for(int k= 1; k<=K; k++){
            for(int i=1; i<n; i++){
                int min_= INT_MAX;
                for(int j=0; j<=i; j++){ //这里=i不影响结果，因为第i天买入，第i天卖出结果为0，不影响最大值。
                    min_ = min(min_, prices[j]-dp[j][k-1]);
                }
                dp[i][k] = max(dp[i-1][k], prices[i]-min_);
            }
        }
        return dp[n-1][K];
    }
};

/*
结果TLE了！！！！，我们想想怎么进一步优化
针对第二个for循环，我们发现对于每一天i，我们都会重新计算0 -（i-1）天的prices[j]-dp[j][k-1]，
也就是说类似于prices[0]-dp[0][k-1]将会被重复计算，我们其实可以不断记录前i天最小的prices[j]-dp[j][k-1]
*/
class Solution {
public:
    int maxProfit(vector<int>& prices) {
        if(prices.empty())
            return 0;
        int K= 2, n=prices.size();
        vector<vector<int>> dp(n, vector<int>(K+1,0));
        for(int k= 1; k<=K; k++){
            int min_ = prices[0] - dp[0][k-1];
            for(int i=1; i<n; i++){
                min_ = min(min_, prices[i]-dp[i][k-1]);
                dp[i][k] = max(dp[i-1][k], prices[i]-min_);
            }
        }
        return dp[n-1][K];
    }
};

/*
我们发现最外层的for循环为K次固定次数，为了更好的访问prices数组，我们将内外for循环颠倒，
首先我们查看整个n*K的数组，这里我画出一个5*3的示例（k=0列所有的元素为0，i=0行所有元素为0）
0 0 0
0 _ _
0 _ _
0 _ _
0 _ _

 根据转换方程：dp[i][k] = max(dp[i-1][k],  prices[i] - (prices[j] - dp[j][k-1]));
 我们针对二维数组某个元素，只会应用其左上角的元素，这也给了我们调换循环的可能
 那么问题来了，调换之后我们怎么更好的记录prices[j] - dp[j][k-1]

 我们可以定义 min[K], 当i=0时 min[k] = prices[0];
 在遍历第i行时不断与前i行的最小值比较，更新最小值
 min_[k] = min(prices[i] - dp[i][k-1], min_[k]);

*/
class Solution {
public:
    int maxProfit(vector<int>& prices) {
        if(prices.empty())
            return 0;
        int K= 2, n=prices.size();
        vector<vector<int>> dp(n, vector<int>(K+1,0));
        vector<int> min_(K+1, prices[0]);
        for(int i=1; i<n; i++){
            for(int k=1; k<=K; k++){
                min_[k] = min(prices[i] - dp[i][k-1], min_[k]);
                dp[i][k] = max(dp[i-1][k], prices[i]-min_[k]);
            }
        }
        return dp[n-1][K];
    }
};

/*
仔细观察上面的代码，我们发现更新二维数组dp时，更新第i行只用到i-1行对应位置的元素和同一行的前一个元素
因此，我们可以将二维数组缩减为一维数组
*/
class Solution {
public:
    int maxProfit(vector<int>& prices) {
        if(prices.empty())
            return 0;
        int K= 2, n=prices.size();
        vector<int> dp(K+1, 0);
        vector<int> min_(K+1, prices[0]);
        for(int i=1; i<n; i++){
            for(int k=1; k<=K; k++){
                min_[k] = min(prices[i] - dp[k-1], min_[k]);
                dp[k] = max(dp[k], prices[i]-min_[k]);
            }
        }
        return dp[K];
    }
};

/*
这里k为常数，可以直接定义变量
*/
class Solution {
public:
    int maxProfit(vector<int>& prices) {
        if(prices.empty())
            return 0;
  
        int dp1=0, dp2=0;
        int min1=prices[0], min2=prices[0];
        for(int i=1; i<prices.size(); i++){
            min1 = min(prices[i] -0, min1);
            dp1 = max(dp1, prices[i]-min1);
            
            min2 = min(prices[i]-dp1, min2);
            dp2 = max(dp2, prices[i]-min2);
        }
        
        return dp2;
    }
};











/*

java做法


首先做法相当于暴力法，遍历数组，以当前元素为中介点分别计算两边的最大利润，然后计算利润和的最大值
class Solution {
    public int maxProfit(int[] prices) {
        if(prices.length ==0)
            return 0;
        int n=prices.length;
        int[] max= new int[n];
        int[] second = new int[n];
        int maxProfit =0;
        for(int i=0; i< prices.length ;i++){
            max[i] = segmentProfit(prices, 0,i);
            second[i] =segmentProfit(prices, i,prices.length);
            maxProfit = Math.max(maxProfit, max[i]+ second[i]);
        }
        
        return maxProfit;
        
        
    }
    public int segmentProfit(int[] prices, int left, int right){
        if(left >= right)
            return 0;
        int maxProfit =0, min = prices[left];
        for(int i= left+1; i< right; i++){
            if(prices[i] >=min)
                maxProfit = Math.max(maxProfit, prices[i]-min);
            else
                min =prices[i];
        }
        return maxProfit;
    }
}
看到动态规划的方法 解决这一类问题：https://leetcode-cn.com/problems/best-time-to-buy-and-sell-stock-iii/solution/yi-ge-tong-yong-fang-fa-tuan-mie-6-dao-gu-piao-wen/
首先写了一下通用做法
class Solution {
    public int maxProfit(int[] prices) {
        if(prices.length ==0)
            return 0;
        int maxK=2;
        if (maxK > prices.length / 2) 
            return maxProfit_k_inf(prices);
        int[][][] dp= new int[prices.length][maxK+1][2];
        for(int i=0; i<prices.length; i++){
            for(int k =maxK; k>=1; k--){
                if(i-1 ==-1){
                    dp[i][k][0] = 0;
                    dp[i][k][1] = -prices[i];
                    continue;
                }
                dp[i][k][0] = Math.max(dp[i-1][k][0], dp[i-1][k][1] +prices[i]);
                dp[i][k][1] = Math.max(dp[i-1][k][1], dp[i-1][k-1][0] -prices[i]);
            }
        }
        return dp[prices.length -1][maxK][0];
    }
    
    public int maxProfit_k_inf(int[] prices) {
        if(prices.length ==0)
            return 0;
        int maxProfit =0;
        for(int i=1; i<prices.length ;i++){
            if(prices[i] >prices[i-1])
                maxProfit += prices[i] - prices[i-1];
        }
        return maxProfit;
    }
}

其实k=2比较小 可以直接计算

class Solution {
    public int maxProfit(int[] prices) {
        if(prices.length ==0)
            return 0;
        int dp_i10 = 0, dp_i11=Integer.MIN_VALUE;
        int dp_i20 =0, dp_i21 =Integer.MIN_VALUE;
        for(int i=0; i<prices.length; i++){
            dp_i20 = Math.max(dp_i20, dp_i21+prices[i]);
            dp_i21 = Math.max(dp_i21, dp_i10-prices[i]);
            dp_i10 = Math.max(dp_i10, dp_i11+prices[i]);
            dp_i11 = Math.max(dp_i11, -prices[i]);
        }
        return dp_i20;
    }
}

上面动态规划的方法都是采用状态机的思想来写的,下面我们直接使用动态规划求出数组和状态转移方程:
https://leetcode-cn.com/problems/best-time-to-buy-and-sell-stock-iii/solution/xiang-xi-tong-su-de-si-lu-fen-xi-duo-jie-fa-by--29/

class Solution {
    public int maxProfit(int[] prices) {
        if(prices.length ==0)
            return 0;
        int n= prices.length;
        int maxK=2;
        int[][] dp = new int[n][maxK+1];
        for(int k= 1; k<=maxK;k++){
            for(int i=1; i<n; i++){
                int min =Integer.MAX_VALUE;
                for(int j=0; j<i; j++){
                    min = Math.min(min, prices[j]- dp[j][k-1]);
                }
                dp[i][k] = Math.max(dp[i-1][k], prices[i] - min);
            }
        }
        return dp[n-1][maxK];
    }
}

上述方法每次都需要遍历求取前面的最小值,其实可以直接使用min记录
class Solution {
    public int maxProfit(int[] prices) {
        if(prices.length ==0)
            return 0;
        int n= prices.length;
        int maxK=2, min;
        int[][] dp = new int[n][maxK+1];
        for(int k=1; k<=maxK; k++){
            min = prices[0];
            for(int i=1; i<n; i++){
                min = Math.min(min, prices[i] -dp[i][k-1]);
                dp[i][k] = Math.max(dp[i-1][k], prices[i]-min);
            }
        }
        return dp[n-1][maxK];
    }
}

优化优化
class Solution {
    public int maxProfit(int[] prices) {
        if(prices.length ==0)
            return 0;
        int n= prices.length;
        int K=2;
        int[] dp = new int[K+1];
        int[] min = new int[K+1];
        Arrays.fill(min,prices[0]);
        for(int i=1; i<n; i++){
            for(int k=1; k<=K; k++){
                min[k] = Math.min(prices[i]- dp[k-1], min[k]);
                dp[k] = Math.max(dp[k],prices[i]- min[k]);
            }
        }
        return dp[K];
    }
}
*/
