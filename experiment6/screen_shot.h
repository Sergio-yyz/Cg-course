#pragma once

#include <glad/glad.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
//#include <windef.h>

typedef unsigned long       DWORD;
typedef int                 BOOL;
typedef unsigned char       BYTE;
typedef unsigned short      WORD;
typedef long LONG;

BOOL WriteBitmapFile(char* filename, int width, int height, unsigned char* bitmapData)
{
	

	//
	FILE* filePtr;			//连接要保存的bitmap文件用
	FILE* headerPtr;
	unsigned char tempRGB;	//临时色素
	int imageIdx;

	GLubyte BMP_Header[54] = {0};

	//交换R、B的像素位置,bitmap的文件放置的是BGR,内存的是RGB
	for (imageIdx = 0; imageIdx < width * abs(height) * 3; imageIdx += 3)
	{
		tempRGB = bitmapData[imageIdx];
		bitmapData[imageIdx] = bitmapData[imageIdx + 2];
		bitmapData[imageIdx + 2] = tempRGB;
	}

	fopen_s(&headerPtr, "header.bmp", "rb");
	if (NULL == headerPtr)
	{
		return false;
	}

	fopen_s(&filePtr, filename, "wb");
	if (NULL == filePtr)
	{
		return false;
	}

	fread(BMP_Header, sizeof(BMP_Header), 1, headerPtr);  //读取文件头和信息头，占据54字节
	fwrite(BMP_Header, sizeof(BMP_Header), 1, filePtr);

	fwrite(bitmapData, width*height*3, 1, filePtr);

	fclose(filePtr);
	return true;
}


void screen_shot(void)
{
	int clnWidth = 1280, clnHeight=720;	//client width and height
	static void* screenData;
	//RECT rc;
	int len = clnWidth * clnHeight * 3;
	screenData = malloc(len);
	memset(screenData, 0, len);
	glReadPixels(0, 0, clnWidth, clnHeight, GL_RGB, GL_UNSIGNED_BYTE, screenData);

	//生成文件名字符串，以时间命名
	time_t tm = 0;
	tm = time(NULL);
	char lpstrFilename[256] = { 0 };
	sprintf_s(lpstrFilename, sizeof(lpstrFilename), "%d.bmp", tm);

	WriteBitmapFile(lpstrFilename, clnWidth, clnHeight, (unsigned char*)screenData);
	//释放内存
	free(screenData);
}