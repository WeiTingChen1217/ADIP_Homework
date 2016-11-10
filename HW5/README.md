Note

## OpenCV-API
* [copyMakeBorder()](#API_copyMakeBorder)



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
## Reference
- [**傅立葉轉換(Fourier Transform) - 阿洲的程式教學**](http://monkeycoding.com/?p=864)
