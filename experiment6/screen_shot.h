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
	FILE* filePtr;			//����Ҫ�����bitmap�ļ���
	FILE* headerPtr;
	unsigned char tempRGB;	//��ʱɫ��
	int imageIdx;

	GLubyte BMP_Header[54] = {0};

	//����R��B������λ��,bitmap���ļ����õ���BGR,�ڴ����RGB
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

	fread(BMP_Header, sizeof(BMP_Header), 1, headerPtr);  //��ȡ�ļ�ͷ����Ϣͷ��ռ��54�ֽ�
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

	//�����ļ����ַ�������ʱ������
	time_t tm = 0;
	tm = time(NULL);
	char lpstrFilename[256] = { 0 };
	sprintf_s(lpstrFilename, sizeof(lpstrFilename), "%d.bmp", tm);

	WriteBitmapFile(lpstrFilename, clnWidth, clnHeight, (unsigned char*)screenData);
	//�ͷ��ڴ�
	free(screenData);
}