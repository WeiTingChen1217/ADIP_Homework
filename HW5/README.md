Note

## OpenCV-API
* [copyMakeBorder()](#API_copyMakeBorder)
* [merge()](#API_merge)
* [convertTo()](#API_convertTo)



*************************************************
<a name="API_copyMakeBorder"></a>  
### copyMakeBorder()
進行卷積處理影像時，在影像的邊界，核心沒有足夠元素納入計算，OpenCV裡用`copyMakeBorder()`函式將原圖稍微放大，再開始進行卷積，OpenCV在空間濾波的相關函式內部已包含`copyMakeBorder()`了，所以實際上使用空間濾波時，不需要呼叫`copyMakeBorder()`。

在OpenCV裡進行卷積時，處理邊界問題通常分以下3個步驟：
1. 先將原始影像的資料，複製到一個稍大的影像裡。
2. 將邊界進行外推。
3. 處理完後將外推的部分切掉，使得輸出圖和輸入圖大小相同。

**C++:**
`void copyMakeBorder(InputArray src, OutputArray dst, int top, int bottom, int left, int right, int borderType, const Scalar&value=Scalar())`

**Parameters**
- **src** - Source image.
- **dst** - 輸出圖，深度和輸入圖相同，尺寸會依輸入參數而變得稍大，`Size(src.cols+left+right, src.rows+top+bottom)`。
- **top、bottom、left、right** - 上下左右分別外推多少像素。
- **borderType** - 邊緣型態。
    - **BORDER_CONSTANT：** 外推的值為常數，常在仿射變換、透視變換中使用。
    - **BORDER_REPLICATE：** 外推的值和邊緣值相同，OpenCV的medianBlur默認的處理方式。
    - **BORDER_REFLECT_101：** 外推像素和影像邊界成鏡像映射，OpenCV的 filter2D、blur、GaussianBlur、bilateralFilter默認的處理方式。
- **value** - 當`borderType`為`BORDER_CONSTANT`，此為外推的值。


**Reference:** 
[卷積邊界處理(copyMakeBorder) - 阿洲的程式教學](http://monkeycoding.com/?tag=copymakeborder)


*************************************************
<a name="API_merge"></a>  
### merge()
`merge()`函数是把多个但通道数组连接成1个多通道数组。而`split()`函数则相反，把1个多通道函数分解成多个但通道函数。

**C++:** `void merge(const Mat* mv, size_t count, OutputArray dst)`

**Parameters**
- 

**Reference:** 
[Blog-tornadomeet](http://www.cnblogs.com/tornadomeet/archive/2012/07/26/2610414.html)


*************************************************
<a name="API_convertTo"></a>  
### convertTo()
將源`Mat`的格式轉換為另一種數據格式存儲到m中,如果`m`的格式或大小不正確則會重新創建.

**C++:** `void convertTo( OutputArray m, int rtype, double alpha=1, double beta=0 ) const;`

**Parameters**
- **m** - 輸出矩陣
- **rtype** - 為轉換后的類型,如果為負數,則與源Mat相同.
- **alpha,beta** - 這個函數可以對其中的值進行縮放和偏移,公式為 m(x,y)=newtype((alpha*src(x,y)+beta));

**Reference:** 
[C博客-OpenCV學習心得(11) -- Mat類型與Arraya](http://blog.csdn.net/jianguo_cui/article/details/7387169)


*************************************************
## Reference
- [**傅立葉轉換(Fourier Transform) - 阿洲的程式教學**](http://monkeycoding.com/?p=864)
