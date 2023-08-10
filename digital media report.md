### ����������˹ģ���ͼ���񻯴���

## ͼƬ��д����
 > ������ƹ���
  > 
  >�����ļ�ͷ��صĶ�ȡ
  >
  >�����㷨�߼����Ż�����
  >
  >չʾͼƬ����Ч��

  # �����ļ�ͷ����Ϣͷ��ȡ
�������C�����Դ���д��������Ҫʹ��fopen,fread,fwrite�Ⱥ���ʵ��.bmpͼƬ���ݵĶ����Լ����ĺ�������

��������C���Ե�struct��װ�ļ����ļ�ͷ����Ϣͷ����

```
typedef struct {
	unsigned char bfType[2];
	unsigned int bfSize;
	unsigned short bfReserved1;
	unsigned short bfReserved2;
	unsigned int bfOffBits;
}bitmapFileHeader;//�ļ�ͷ

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
}bitmapInfoHeader;//��Ϣͷ


typedef struct {
	unsigned char rgbBlue;
	unsigned char rgbGreen;
	unsigned char rgbRed;
	unsigned char rgbReserved;
}rgbQUAD;//ȡɫ��

typedef struct {
	bitmapFileHeader bfHeader;
	bitmapInfoHeader biHeader;
	rgbQUAD palette[256];
	unsigned char* imgData;
}bmp;//ͼƬ�����ݽṹ
```

�˴��б�Ҫ���ض�bisizeimage���н��ͣ���msdn�������˸���������˵����

biSizeImage
Specifies the size, in bytes, of the image. This may be set to zero for BI_RGB bitmaps.
Windows 98, Windows 2000: If biCompression is BI_JPEG or BI_PNG, biSizeImage indicates the size of the JPEG or PNG image buffer, respectively.

<font color=blue>

��biCompression����ѹ����ʽ��ΪBI_RGBʱ�������ͼƬ����biSizeImage��ʾͼƬ�Ĵ�С����byteΪ��λ����Ȼ���ض�����£���ͼƬΪ24λ��ȼ����ͼƬʱ��biSizeImage��ʱ���趨Ϊ0.��������ҽ���ͼƬ����ʱҲ����������������������������ص�bmp�ļ���biSizeImageͨ��Ϊ0.

�ɴ����ƶϣ������÷�ʽ������΢��������ʱ�����ͼƬ�ļ����İ�ȫ���������ʹ��biSizeImageǿ��Ϊ0.

</font>

���������ܼ򵥣�biSizeImage�Ĵ�С�޷��Ǽ������⣬��������biSizeImage=linebytes*height������linebytesֵ�ļ�����������������ܣ��������ֶ����㣬��ͼƬ���¸�ֵ

��Ȼ���򵥵Ľ����������ps��ͼ����bmpͼƬ���±���һ�顣

����rgb�ı��淽ʽ��˳��

������ Blue ��Ӧ����������˳��Ϊ�����̡��졢͸����

���ݰ��������н��а�װ�����ͼƬ�����̺��ռһ���ֽڣ������ڶ�ȡ�������Ⲣ����ȫ������Ϊ���л����ܻ��в��㣨zero-padding�������漰������������ݽṹ���루data structure alignment�������⡣

������ CPU ÿ�δ��ڴ��ж�ȡ���������ݿ飨chunk������ͨ��Ϊ 32 bit��4 bytes������ˣ�Ϊ��������ȡЧ�ʣ�λͼÿ�е����ݣ��ֽڣ�����Ҫ�� 4 �ı��������ɱ���أ���Щʱ��ÿ�еĽ�β�ͻ���ֲ��㣨��ʵ��������������Ҳ�ǿ��Եģ����������ǲ� 0����

����ÿ�в���ĸ��������м��㷽���ġ���ʽ���£�

![Paste_Image.png](https://i.328888.xyz/2023/04/14/ixY4Ha.png)

����ÿ�е��ֽ������ڣ�ÿ���ر���������ͼƬ��ȼ� 31 �ĺͳ��� 32��������ȡ��(�ɴ˿ɱ�֤�����ֽ���һ����4�ı���������C���Զ�дԭ��)�������� 4����Ϊʲô��ֱ�ӳ���8��ֱ�ӳ���8��õ�linebytes=width*3����һ��Ϊ4�ı����������������β��0��λ������һ����Ӱ��֮���������λ�õļ��㣩

�õ���ÿ�е��ֽ������������ܹ��õ�ԭʼλͼ���ݣ�����˵�洢ͼƬ���������ص�ɫ����Ϣ�����ݣ��Ĵ�С�ˣ�

����ԭʼλͼ���ݴ�С���ڣ�ÿ�е��ֽ�����linebytes������ͼ��߶ȣ�Ҳ������������height��

# �����㷨�߼�

������C���Զ������ݶ�ȡ����������񻯵ĺ����㷨��

���ļ�ʵ�ֵ��㷨�ǻ���������˹ģ���ͼ���񻯴����㷨�����������˼�룬ͨ�����ر�Ե��Ϣ�������ر�����Ϣ��

��ԭ�㷨�У����ڵ�ɫ��ļ��루���£������ǲ��ѷ��֣�ԭ�㷨��������ؽ�256����ɫ���Ҷ�ɫϵ����rgb����ʣ���ʽ��������������ÿ�����ص���ɫ����Ϊ��������˻�Ϊ�ڰס���ֱ�����ø�������ɫֵ����������˹ģ�弴��ʵ��ͼ���񻯡�

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
# �����Ż��߼�
<font color=red>

���Ǵ��㷨�к����Եı׶ˣ���Ȼ��ɫ�Ѿ����˻����ֺαش洢Ϊ24λ��ȵ�ͼƬ�أ����˷ѿռ������ݶ�ʧ���˴���д��һ��24λ���ת8λ��С�������£���

</font>

```
// ͼ������data�ҶȻ�
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
// ͼ������data�Ҷ�ͬʱ��ֵ����threshold��ֵ
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
�ɴˣ����ǿ��Զ��ļ�����ѹ����������ʧ2/3����Ҫ�����ݣ����ڰ�ͼ���񻯣���ͼ����Ϊ8λ��ȣ�����ת����ֱ�Ӷ��������ݽ��д�����ʵ��bmp�ļ�������8λ��ȣ��Ͼ���ѹ����ʽ��ͼƬ�����������׷��������ɫ������������

���Ƕ���һ���Ӵ�ͼ���������Ȥ�Ĵ�ѧ������Ȼ����ֻ�����ڵ�һ�ĺڰ�ͼ���񻯣������Ҿ��Ľ��ڰ�ɫ��ɾ�������洢��Ϊ���ͼ�񣬲���ʧͼ��ɫ�����ݡ�

����������³�����Ȼ�����㷨��������ǰ���������������Ľ��Ĺ�����Ȼ���гɾ͸У�
```
int i, j, temp;
	int sharpen[9] = { -1,-1,-1,-1,9,-1,-1,-1,-1 };//������˹��ģ��
	for (i = 1; i < height - 1; i++) {  //1~height-2
		for (j = 3; j < 3*(width - 1); j++) { //1~width-2
			temp = imageData[lineBytes * (i + 1) + j - 3] * sharpen[0];//tempԽ��Խ��
			temp += imageData[lineBytes * (i + 1) + j] * sharpen[1];
			temp += imageData[lineBytes * (i + 1) + j + 3] * sharpen[2];
			temp += imageData[lineBytes * i + j - 3] * sharpen[3];
			temp += imageData[lineBytes * i + j] * sharpen[4];//���������
			temp += imageData[lineBytes * i + j + 3] * sharpen[5];
			temp += imageData[lineBytes * (i - 1) + j - 3] * sharpen[6];
			temp += imageData[lineBytes * (i - 1) + j] * sharpen[7];
			temp += imageData[lineBytes * (i - 1) + j+3] * sharpen[8];
			if (temp > 255)b.imgData[lineBytes * i + j] = 255;//���ܺڣ����Ϊ��
			else if (temp < 0)b.imgData[lineBytes * i + j] = abs(temp);//���ܺ��������м����
			else if (temp < 0 && temp < -255)b.imgData[lineBytes * i + j] = 0;
			else b.imgData[lineBytes * i + j] = temp;//��������ɫ�����ȶ�
		}
	}
```
�����б�Ҫ��΢���ͳ����߼���������ÿ���ֽ�Ϊ��λ���������أ�������ÿ�����ص�ÿ��rgbֵ��blue��red��grreen��Ϊ��λ��������д���



��������
�����صĵ�i����i<=3��rgbֵ��9����ȥ��Χ���صĵ�i��rgbֵ��

<font color=green>

��������ĳ��rgbֵ����8�����صĶ�Ӧrgbֵ��С��������Խ���������������������صĸ�rgbֵ����Ϊ255����Ϊ������

��������ĳ��rgbֵ����8�����صĶ�Ӧrgbֵ�ܴ󣨼�����Խ����������������������صĸ�rgbֵ����Ϊ��������ľ���ֵ����Ϊ������

</font>

�ɴ˲��ѿ�������һƬ��������һ�����㣬����������һƬ�������кڵ㣬�ڵ�ᱻ������������ͼ���Ե�񻯺�������


��Ϊͼ���еı�Ե������Щ�Ҷȷ������������������ģ���ڱ�Ե����к����á�


��������ͼ����Ч����
==

lenaԭͼ��
==========
![Lena.png](https://s2.loli.net/2023/08/08/7jA3u4eEHYxFsz1.png)

lena�޸ĺ��ͼ��
===
![Lena_test02.png](https://s2.loli.net/2023/08/08/5EwhaJL6FybpWPd.png)

lenaԭͼ�񣨲�ɫ����
==========
![LenaRGB����test01.png](https://s2.loli.net/2023/08/08/lCq8KsY1IAWS3LM.png)

lena�޸ĺ��ͼ�񣨲�ɫ����
===
![LenaRGB����test1.png](https://s2.loli.net/2023/08/08/PZYIfOV5DECTyov.png)

�ҵ������ֽ������
==========
![test1.png](https://s2.loli.net/2023/08/08/njUwEvy5rGRY8Cd.png)

�޸ĺ�ı�ֽ��
===
![test11.jpg](https://s2.loli.net/2023/08/08/WUEViyk2db67BNH.jpg)

������ͼƬ���ѿ�������ͼ��ֱ��ʽϵ�ʱ��lenaͼ�����㷨�Ա�Ե�нϸ����жȣ������ֱ������ߣ��㷨�Ա�Ե��֪��Χ��δ���������Ե�����жȲ��ߣ����������ͼ����ʱ������Ҫ��߷�Χ����������Զ������������

�¸���������
```
#define _CRT_SECURE_NO_WARNINGS

#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<math.h>
#pragma pack(1)   //ȫ����ģʽ
//ǿ�ƽṹ��Ԫ����1�ֽڶ���

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
	if ((fp = fopen("D:\\uestc_work\\digital media\\LenaRGB����test001.bmp", "rb")) == NULL) {
		perror("can not open file!");
		return -1;
	}

	//�����ɫbmpͼ���ļ�ͷ����Ϣͷ��ͼ������
	bitmapFileHeader bfHeader;//�ļ�ͷ
	fread(&bfHeader, 14, 1, fp);
	bitmapInfoHeader biHeader;//����ͷ
	fread(&biHeader, 40, 1, fp);
	int imSize = biHeader.biSizeImage;
	int width = biHeader.biWidth;
	int height = biHeader.biHeight;
	int bitCount = biHeader.biBitCount;

	int lineBytes = (width * bitCount + 31) / 32 * 4;//����ÿ��ʵ�ʵ��ֽ���
	int imSize1 = lineBytes * biHeader.biHeight;//ԭʼλͼ���ݴ�С����lineBytes*biHeight
	fseek(fp, bfHeader.bfOffBits, SEEK_SET);//�ļ���ȡ�����ļ�ͷ
	unsigned char* imageData = (unsigned char*)malloc(imSize * sizeof(unsigned char));

	fread(imageData,imSize * sizeof(unsigned char),1, fp);//��ȡ�������ص�����
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
	}//���õ�ɫ��,�Ӻڵ���

	int i, j, temp;
	int sharpen[9] = { -1,-1,-1,-1,9,-1,-1,-1,-1 };//������˹��ģ��
	for (i = 1; i < height - 1; i++) {  //1~height-2
		for (j = 3; j < 3*(width - 1); j++) { //1~width-2
			temp = imageData[lineBytes * (i + 1) + j - 3] * sharpen[0];//tempԽ��Խ��
			temp += imageData[lineBytes * (i + 1) + j] * sharpen[1];
			temp += imageData[lineBytes * (i + 1) + j + 3] * sharpen[2];
			temp += imageData[lineBytes * i + j - 3] * sharpen[3];
			temp += imageData[lineBytes * i + j] * sharpen[4];//���������
			temp += imageData[lineBytes * i + j + 3] * sharpen[5];
			temp += imageData[lineBytes * (i - 1) + j - 3] * sharpen[6];
			temp += imageData[lineBytes * (i - 1) + j] * sharpen[7];
			temp += imageData[lineBytes * (i - 1) + j+3] * sharpen[8];
			if (temp > 255)b.imgData[lineBytes * i + j] = 255;//���ܺڣ����Ϊ��
			else if (temp < 0)b.imgData[lineBytes * i + j] = abs(temp);//���ܺ��������м����
			else if (temp < 0 && temp < -255)b.imgData[lineBytes * i + j] = 0;
			else b.imgData[lineBytes * i + j] = temp;//���ܺڣ����м����
		}
	}

	char savePath[] = "D:\\uestc_work\\digital media\\LenaRGB����test1.bmp";
	FILE* f_save = fopen(savePath, "wb");
	if (f_save == NULL) {
		perror("can not open file!\n");
		return -2;
	}

	fwrite(&b.bfHeader, sizeof(bitmapFileHeader), 1, f_save);
	fwrite(&b.biHeader, sizeof(bitmapInfoHeader), 1, f_save);
	//fwrite(&b.palette, 1024, 1, f_save);//д���ɫ��
	fwrite(b.imgData, sizeof(unsigned char) * b.biHeader.biSizeImage, 1, f_save);
	fclose(f_save);

	free(imageData);
	free(b.imgData);
	getchar();
	return 0;
}

```