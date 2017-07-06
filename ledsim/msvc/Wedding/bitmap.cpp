#include "bitmap.h"
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

bitmap::bitmap()
{
}


bitmap::~bitmap()
{
}

bool bitmap::read_bmp(char *filename)
{
	FILE *fp;
	unsigned char header_buf[54];	//ヘッダ情報を取り込む
	int real_width;
	unsigned int color;
	unsigned char *bmp_line_data;

	/* ファイル読み込み */
	if ((fp = fopen(filename, "rb")) == NULL){
		fprintf(stderr, "Error: %s could not read.", filename);
		return false;
	}

	fread(header_buf, sizeof(unsigned char), sizeof(header_buf), fp); //ヘッダ部分全てを取り込む

	//最初の2バイトがBM(Bitmapファイルの印)であるか
	if (strncmp((const char *)header_buf, "BM", 2)){
		fprintf(stderr, "Error: %s is not Bitmap file.", filename);
		return false;
	}
	memcpy(&width, header_buf + 18, sizeof(width)); //画像の見た目上の幅を取得
	memcpy(&height, header_buf + 22, sizeof(height)); //画像の高さを取得
	memcpy(&color, header_buf + 28, sizeof(unsigned int)); //何bitのBitmapであるかを取得
	//24bitで無ければ終了
	if (color != 24){
		fprintf(stderr, "Error: %s is not 24bit color image", filename);
		return false;
	}

	//RGB情報は画像の1行分が4byteの倍数で無ければならないためそれに合わせている
	real_width = width * 3 + width % 4;

	//画像の1行分のRGB情報を取ってくるためのバッファを動的に取得
	if ((bmp_line_data = (unsigned char *)malloc(sizeof(char) * real_width)) == NULL){
		fprintf(stderr, "Error: Allocation error.\n");
		return false;
	}

	if ((Rdata = (unsigned char*)malloc(sizeof(char) * width * height)) == NULL){
		fprintf(stderr, "Allocation error\n");
		free(Rdata);
		return false;
	}
	if ((Gdata = (unsigned char*)malloc(sizeof(char) * width * height)) == NULL){
		fprintf(stderr, "Allocation error\n");
		free(Gdata);
		return false;
	}
	if ((Bdata= (unsigned char*)malloc(sizeof(char) * width * height)) == NULL){
		fprintf(stderr, "Allocation error\n");
		free(Bdata);
		return false;
	}

	//BitmapファイルのRGB情報は左下から右へ、下から上に並んでいる
	for (int i = 0; i < height; i++){
		fread(bmp_line_data, 1, real_width, fp);
		for (int j = 0; j < width; j++){
			Bdata[(height - i - 1)*width + j] = bmp_line_data[j * 3];
			Gdata[(height - i - 1)*width + j] = bmp_line_data[j * 3 + 1];
			Rdata[(height - i - 1)*width + j] = bmp_line_data[j * 3 + 2];
		}
	}

	return true;
}