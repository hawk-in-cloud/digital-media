# digital-media
### 基于拉普拉斯模板的图像锐化处理

## 图片读写操作
 > 程序设计过程
  > 
  >介绍文件头相关的读取
  >
  >介绍算法逻辑和优化过程
  >
  >展示图片处理效果

  # 介绍文件头和信息头读取
程序基于C语言自带读写函数，主要使用fopen,fread,fwrite等函数实现.bmp图片数据的读入以及更改后的输出。

首先利用C语言的struct封装文件的文件头和信息头数据

```
typedef struct {
	unsigned char bfType[2];
	unsigned int bfSize;
	unsigned short bfReserved1;
	unsigned short bfReserved2;
	unsigned int bfOffBits;
}bitmapFileHeader;//文件头

typedef struct {
	unsigned int biSize;
	unsigned int biWidth;
	unsigned int biHeight;
	unsigned short biPlanes;
	unsigned short biBitCount;
	unsigned int biCompression;
	unsigned int biSizeImage;
	unsigned int biXPixPerMeter;
	unsigned int biYPixPerMeter;
	unsigned int biClrUsed;
	unsigned int biClrImportant;
}bitmapInfoHeader;//信息头


typedef struct {
	unsigned char rgbBlue;
	unsigned char rgbGreen;
	unsigned char rgbRed;
	unsigned char rgbReserved;
}rgbQUAD;//取色板

typedef struct {
	bitmapFileHeader bfHeader;
	bitmapInfoHeader biHeader;
	rgbQUAD palette[256];
	unsigned char* imgData;
}bmp;//图片的数据结构
```

此处有必要着重对bisizeimage进行解释，在msdn社区有人给出这样的说法：

biSizeImage
Specifies the size, in bytes, of the image. This may be set to zero for BI_RGB bitmaps.
Windows 98, Windows 2000: If biCompression is BI_JPEG or BI_PNG, biSizeImage indicates the size of the JPEG or PNG image buffer, respectively.

<font color=blue>

在biCompression（即压缩方式）为BI_RGB时（即真彩图片），biSizeImage表示图片的大小（以byte为单位），然而特定情况下，当图片为24位深度即真彩图片时，biSizeImage有时被设定为0.该情况在我进行图片测试时也经常遇到，尤其是在浏览器上下载的bmp文件，biSizeImage通常为0.

由此我推断，该设置方式可能是微软在下载时对真彩图片文件做的安全处理操作，使得biSizeImage强制为0.

</font>

解决方法亦很简单，biSizeImage的大小无非是计算问题，可以利用biSizeImage=linebytes*height（关于linebytes值的计算在下面会有所介绍）来进行手动计算，对图片重新赋值

当然更简单的解决方法：打开ps或画图，将bmp图片重新保存一遍。

关于rgb的保存方式和顺序

则由于 Blue 对应数字最大，因此顺序为蓝、绿、红、透明。

数据按照像素行进行包装（真彩图片中蓝绿红各占一个字节），便于读取。但是这并不是全部，因为其中还可能会有补零（zero-padding）。这涉及到计算机的数据结构对齐（data structure alignment）的问题。

主流的 CPU 每次从内存中读取并处理数据块（chunk），且通常为 32 bit（4 bytes）。因此，为了提升读取效率，位图每行的数据（字节）都需要是 4 的倍数。不可避免地，有些时候每行的结尾就会出现补零（其实补其他任意数字也是可以的，但常见都是补 0）。

对于每行补零的个数，是有计算方法的。公式如下：

![Paste_Image.png](https://i.328888.xyz/2023/04/14/ixY4Ha.png)

即，每行的字节数等于：每像素比特数乘以图片宽度加 31 的和除以 32，并向下取整(由此可保证所求字节数一定是4的倍数，满足C语言读写原则)，最后乘以 4。（为什么不直接除以8？直接除以8则得到linebytes=width*3，不一定为4的倍数，计算机会在行尾补0，位数错误一定会影响之后对于数据位置的计算）

得到了每行的字节数，进而就能够得到原始位图数据，或者说存储图片的所有像素的色彩信息的数据，的大小了：

即，原始位图数据大小等于：每行的字节数（linebytes）乘以图像高度（也就是总行数，height）

# 介绍算法逻辑

介绍了C语言对于数据读取，下面介绍锐化的核心算法：

本文件实现的算法是基于拉普拉斯模板的图像锐化处理算法，借助卷积的思想，通过像素边缘信息更改像素本身信息。

在原算法中，由于调色板的加入（如下），我们不难发现，原算法作者巧妙地将256个颜色（灰度色系）以rgb（真彩）形式存了起来，这样每个像素的颜色可视为“从真彩退化为黑白”，直接利用各像素颜色值代入拉普拉斯模板即可实现图像锐化。

```
bmp b;
	memcpy(&(b.bfHeader), &bfHeader, sizeof(bfHeader));
	memcpy(&(b.biHeader), &biHeader, sizeof(biHeader));
	b.imgData = (unsigned char*)malloc(sizeof(unsigned char) * imSize);
	memset(b.imgData, 0, sizeof(unsigned char) * imSize);
	for (int i = 0; i < 256; i++) {
		b.palette[i].rgbBlue = i;
		b.palette[i].rgbGreen = i;
		b.palette[i].rgbRed = i;
	}

	int i, j, temp;
	int sharpen[9] = { -1,-1,-1,-1,9,-1,-1,-1,-1 };
	for (i = 1; i < height - 1; i++) {  //1~height-2
		for (j = 1; j < width - 1; j++) { //1~width-2
			temp = imageData[lineBytes * (i + 1) + j - 1] * sharpen[0];
			temp += imageData[lineBytes * (i + 1) + j] * sharpen[1];
			temp += imageData[lineBytes * (i + 1) + j + 1] * sharpen[2];
			temp += imageData[lineBytes * i + j - 1] * sharpen[3];
			temp += imageData[lineBytes * i + j] * sharpen[4];
			temp += imageData[lineBytes * i + j + 1] * sharpen[5];
			temp += imageData[lineBytes * (i - 1) + j - 1] * sharpen[6];
			temp += imageData[lineBytes * (i - 1) + j] * sharpen[7];
			temp += imageData[lineBytes * (i - 1) + j] * sharpen[8];
			if (temp > 255)b.imgData[lineBytes * i + j] = 255;
			else if (temp < 0&&temp>=-255)b.imgData[lineBytes * i + j] = abs(temp);
			else if(temp < 0 && temp < -255)b.imgData[lineBytes * i + j] = 0;
			else b.imgData[lineBytes * i + j] = temp;
		}
	}
```
# 介绍优化逻辑
<font color=red>

但是此算法有很明显的弊端，既然颜色已经“退化”又何必存储为24位深度的图片呢？（浪费空间且数据丢失）此处我写了一个24位深度转8位的小程序（如下）：

</font>

```
// 图像数据data灰度化
VOID Gray(BitmapData *data)
{
PARGBQuad p = (PARGBQuad)data->Scan0;
INT offset = data->Stride - data->Width * sizeof(ARGBQuad);

for (UINT y = 0; y < data->Height; y ++, (BYTE*)p += offset)
{
for (UINT x = 0; x < data->Width; x ++, p ++)
p->Blue = p->Green = p->Red =
(UINT)(p->Blue * 29 + p->Green * 150 + p->Red * 77 + 128) >> 8;

}
}
// 图像数据data灰度同时二值化，threshold阀值
VOID GrayAnd2Values(BitmapData *data, BYTE threshold)
{
PARGBQuad p = (PARGBQuad)data->Scan0;
INT offset = data->Stride - data->Width * sizeof(ARGBQuad);

for (UINT y = 0; y < data->Height; y ++, (BYTE*)p += offset)
{
for (UINT x = 0; x < data->Width; x ++, p ++)
{
if (((p->Blue * 29 + p->Green * 150 + p->Red * 77 + 128) >> 8) < threshold)
p->Color &= 0xff000000;
else
p->Color |= 0x00ffffff;

}
}
}

```
由此，我们可以对文件进行压缩操作，丢失2/3不必要的数据，将黑白图像锐化，若图像本身为8位深度，则不用转换，直接对像素数据进行处理（事实上bmp文件极少有8位深度，毕竟无压缩方式的图片正常情况都会追求像素颜色的完整保留）

但是对于一名接触图像处理并深感兴趣的大学生，自然不会只局限于单一的黑白图像锐化，于是我决心将黑白色板删除，最后存储仍为真彩图像，不丢失图像色彩数据。

因而有了以下程序（虽然类似算法估计早有前辈做过，但自主改进的过程依然富有成就感）
```
int i, j, temp;
	int sharpen[9] = { -1,-1,-1,-1,9,-1,-1,-1,-1 };//拉普拉斯锐化模板
	for (i = 1; i < height - 1; i++) {  //1~height-2
		for (j = 3; j < 3*(width - 1); j++) { //1~width-2
			temp = imageData[lineBytes * (i + 1) + j - 3] * sharpen[0];//temp越大，越白
			temp += imageData[lineBytes * (i + 1) + j] * sharpen[1];
			temp += imageData[lineBytes * (i + 1) + j + 3] * sharpen[2];
			temp += imageData[lineBytes * i + j - 3] * sharpen[3];
			temp += imageData[lineBytes * i + j] * sharpen[4];//中心所求点
			temp += imageData[lineBytes * i + j + 3] * sharpen[5];
			temp += imageData[lineBytes * (i - 1) + j - 3] * sharpen[6];
			temp += imageData[lineBytes * (i - 1) + j] * sharpen[7];
			temp += imageData[lineBytes * (i - 1) + j+3] * sharpen[8];
			if (temp > 255)b.imgData[lineBytes * i + j] = 255;//四周黑，则点为白
			else if (temp < 0)b.imgData[lineBytes * i + j] = abs(temp);//四周很亮，则中间变亮
			else if (temp < 0 && temp < -255)b.imgData[lineBytes * i + j] = 0;
			else b.imgData[lineBytes * i + j] = temp;//过渡域颜色保持稳定
		}
	}
```
其中有必要稍微解释程序逻辑，程序将以每个字节为单位（不是像素），即以每个像素的每个rgb值（blue，red，grreen）为单位，对其进行处理。



处理方法：
以像素的第i个（i<=3）rgb值的9倍减去周围像素的第i个rgb值。

<font color=green>

若该像素某个rgb值附近8个像素的对应rgb值过小（即附近越暗）且自身很亮，则该像素的该rgb值设置为255（设为最亮）

若该像素某个rgb值附近8个像素的对应rgb值很大（即附近越亮）且自身不够亮，则该像素的该rgb值设置为矩阵计算后的绝对值（设为更亮）

</font>

由此不难看出，当一片暗区出现一个亮点，亮点会更亮；一片亮区中有黑点，黑点会被消除。（增加图像边缘锐化和噪声）


因为图象中的边缘就是那些灰度发生跳变的区域，所以锐化模板在边缘检测中很有用。


下面贴出图像处理效果：
==

lena原图像：
==========
![Lena.png](https://s2.loli.net/2023/08/08/7jA3u4eEHYxFsz1.png)

lena修改后的图像：
===
![Lena_test02.png](https://s2.loli.net/2023/08/08/5EwhaJL6FybpWPd.png)

lena原图像（彩色）：
==========
![LenaRGB——test01.png](https://s2.loli.net/2023/08/08/lCq8KsY1IAWS3LM.png)

lena修改后的图像（彩色）：
===
![LenaRGB——test1.png](https://s2.loli.net/2023/08/08/PZYIfOV5DECTyov.png)

我的桌面壁纸（）：
==========
![test1.png](https://s2.loli.net/2023/08/08/njUwEvy5rGRY8Cd.png)

修改后的壁纸：
===
![test11.jpg](https://s2.loli.net/2023/08/08/WUEViyk2db67BNH.jpg)

由上述图片不难看出，当图像分辨率较低时（lena图），算法对边缘有较高敏感度，但当分辨率升高，算法对边缘感知范围并未增大，因而边缘锐化敏感度不高（如最后两张图，此时可能需要提高范围，搜索更“远”的像素数据

下附完整程序
```
#define _CRT_SECURE_NO_WARNINGS

#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<math.h>
#pragma pack(1)   //全紧凑模式
//强制结构体元素以1字节对齐

typedef struct {
	unsigned char bfType[2];
	unsigned int bfSize;
	unsigned short bfReserved1;
	unsigned short bfReserved2;
	unsigned int bfOffBits;
}bitmapFileHeader;

typedef struct {
	unsigned int biSize;
	unsigned int biWidth;
	unsigned int biHeight;
	unsigned short biPlanes;
	unsigned short biBitCount;
	unsigned int biCompression;
	unsigned int biSizeImage;
	unsigned int biXPixPerMeter;
	unsigned int biYPixPerMeter;
	unsigned int biClrUsed;
	unsigned int biClrImportant;
}bitmapInfoHeader;


typedef struct {
	unsigned char rgbBlue;
	unsigned char rgbGreen;
	unsigned char rgbRed;
	unsigned char rgbReserved;

}rgbQUAD;

typedef struct {
	bitmapFileHeader bfHeader;
	bitmapInfoHeader biHeader;
	rgbQUAD palette[256];
	unsigned char* imgData;
}bmp;

int main() {

	FILE* fp;
	if ((fp = fopen("D:\\uestc_work\\digital media\\LenaRGB——test001.bmp", "rb")) == NULL) {
		perror("can not open file!");
		return -1;
	}

	//读入彩色bmp图像文件头，信息头和图像数据
	bitmapFileHeader bfHeader;//文件头
	fread(&bfHeader, 14, 1, fp);
	bitmapInfoHeader biHeader;//数据头
	fread(&biHeader, 40, 1, fp);
	int imSize = biHeader.biSizeImage;
	int width = biHeader.biWidth;
	int height = biHeader.biHeight;
	int bitCount = biHeader.biBitCount;

	int lineBytes = (width * bitCount + 31) / 32 * 4;//计算每行实际的字节数
	int imSize1 = lineBytes * biHeader.biHeight;//原始位图数据大小，即lineBytes*biHeight
	fseek(fp, bfHeader.bfOffBits, SEEK_SET);//文件读取返回文件头
	unsigned char* imageData = (unsigned char*)malloc(imSize * sizeof(unsigned char));

	fread(imageData,imSize * sizeof(unsigned char),1, fp);//读取所有像素的数据
	fclose(fp);

	bmp b;
	
	
	memcpy(&(b.bfHeader), &bfHeader, sizeof(bfHeader));
	memcpy(&(b.biHeader), &biHeader, sizeof(biHeader));
	b.imgData = (unsigned char*)malloc(sizeof(unsigned char) * imSize);
	memset(b.imgData, 0, sizeof(unsigned char) * imSize);
	for (int i = 0; i < 256; i++) {
		b.palette[i].rgbBlue = i;
		b.palette[i].rgbGreen = i;
		b.palette[i].rgbRed = i;
	}//设置调色板,从黑到白

	int i, j, temp;
	int sharpen[9] = { -1,-1,-1,-1,9,-1,-1,-1,-1 };//拉普拉斯锐化模板
	for (i = 1; i < height - 1; i++) {  //1~height-2
		for (j = 3; j < 3*(width - 1); j++) { //1~width-2
			temp = imageData[lineBytes * (i + 1) + j - 3] * sharpen[0];//temp越大，越白
			temp += imageData[lineBytes * (i + 1) + j] * sharpen[1];
			temp += imageData[lineBytes * (i + 1) + j + 3] * sharpen[2];
			temp += imageData[lineBytes * i + j - 3] * sharpen[3];
			temp += imageData[lineBytes * i + j] * sharpen[4];//中心所求点
			temp += imageData[lineBytes * i + j + 3] * sharpen[5];
			temp += imageData[lineBytes * (i - 1) + j - 3] * sharpen[6];
			temp += imageData[lineBytes * (i - 1) + j] * sharpen[7];
			temp += imageData[lineBytes * (i - 1) + j+3] * sharpen[8];
			if (temp > 255)b.imgData[lineBytes * i + j] = 255;//四周黑，则点为白
			else if (temp < 0)b.imgData[lineBytes * i + j] = abs(temp);//四周很亮，则中间变亮
			else if (temp < 0 && temp < -255)b.imgData[lineBytes * i + j] = 0;
			else b.imgData[lineBytes * i + j] = temp;//四周黑，则中间更亮
		}
	}

	char savePath[] = "D:\\uestc_work\\digital media\\LenaRGB——test1.bmp";
	FILE* f_save = fopen(savePath, "wb");
	if (f_save == NULL) {
		perror("can not open file!\n");
		return -2;
	}

	fwrite(&b.bfHeader, sizeof(bitmapFileHeader), 1, f_save);
	fwrite(&b.biHeader, sizeof(bitmapInfoHeader), 1, f_save);
	//fwrite(&b.palette, 1024, 1, f_save);//写入调色板
	fwrite(b.imgData, sizeof(unsigned char) * b.biHeader.biSizeImage, 1, f_save);
	fclose(f_save);

	free(imageData);
	free(b.imgData);
	getchar();
	return 0;
}

```
