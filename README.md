# 2Dimensional_Linear_Programming
Solve 2D linear programming problem by prune and search

## Description
在二維平面上，使用 Prune and Search 技巧，實作線性規劃。

## Input
第一行包含一個正整數 ![](https://latex.codecogs.com/gif.latex?n\leq10^5)，代表限制條件的個數，下一行開始每行三個整數代表 
![](https://latex.codecogs.com/gif.latex?ax+by\leq) ![](https://latex.codecogs.com/gif.latex?c)，其中 ![](https://latex.codecogs.com/gif.latex?a^2+b^2>0)，
 ![](https://latex.codecogs.com/gif.latex?-300)  ![](https://latex.codecogs.com/gif.latex?\leq) ![](https://latex.codecogs.com/gif.latex?a,b,c\leq300)。

## Output
請輸出滿足所有限制條件的最小 ![](https://latex.codecogs.com/gif.latex?y) 值(四捨五入至整數)；若無解，請輸出 ![](https://latex.codecogs.com/gif.latex?NA)；若為負無限大，請輸出  
![](https://latex.codecogs.com/gif.latex?INF)。


