﻿#define _CRT_SECURE_NO_WARNINGS

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

	fread(imageData, imSize * sizeof(unsigned char), 1, fp);//读取所有像素的数据
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
		for (j = 3; j < 3 * (width - 1); j++) { //1~width-2
			temp = imageData[lineBytes * (i + 1) + j - 3] * sharpen[0];//temp越大，越白
			temp += imageData[lineBytes * (i + 1) + j] * sharpen[1];
			temp += imageData[lineBytes * (i + 1) + j + 3] * sharpen[2];
			temp += imageData[lineBytes * i + j - 3] * sharpen[3];
			temp += imageData[lineBytes * i + j] * sharpen[4];//中心所求点
			temp += imageData[lineBytes * i + j + 3] * sharpen[5];
			temp += imageData[lineBytes * (i - 1) + j - 3] * sharpen[6];
			temp += imageData[lineBytes * (i - 1) + j] * sharpen[7];
			temp += imageData[lineBytes * (i - 1) + j + 3] * sharpen[8];
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