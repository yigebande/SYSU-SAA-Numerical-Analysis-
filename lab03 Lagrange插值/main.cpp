#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

clock_t start, stop, linear_bg, linear_ed, cubic_ed;
double duration, duration_linear, duration_cubic;
//线性插值
int linearInterpolation(int x1, int y1, int x0, int y0, int x);
//三次插值
double cubicInterpolation(int x[], int y[], int point);

int main()
{
	start = clock();

	FILE* read = NULL;
	FILE* write1 = NULL;
	FILE* write2 = NULL;
	//打开.txt文件
	//打开原始数据的文件
	fopen_s(&read, "image_data/1.txt", "r");
	if (!read)
	{
		perror("fail to read !");
		exit(-1);
	}
	//创建保存线性插值结果的文件
	fopen_s(&write1, "image_data/1_linear.txt", "w");
	if (!write1)
	{
		perror("fail to write1 !");
		exit(-2);
	}
	//创建保存三次插值结果的文件
	fopen_s(&write2, "image_data/1_cubic.txt", "w");
	if (!write2)
	{
		perror("fail to write1 !");
		exit(-3);
	}
	//读入.txt文件数据
	int img[50][50];
	for (int i = 0; i < 50; i++)
	{
		for (int j = 0; j < 50; j++)
		{
			fscanf(read, "%d", &img[i][j]);
		}
	}
	fclose(read);
	linear_bg = clock();
	// ==========================线性插值==========================
	//提示：待处理图像数据为img[50][50]；
	int linear[491][491] = { 0 }; //线性插值最终结果保存到这个数组

	//***********************************
	 //*这里写下程序实现拉格朗日线性插值*
	//***********************************

	//把img的点位布置到linear中
	for (int i = 0; i < 50; ++i) {
		for (int j = 0; j < 50; ++j) {
			linear[i * 10][j * 10] = img[i][j];
		}
	}
	//对每行进行插值
	for (int i = 0; i < 50; ++i) {
		int ii = i * 10;
		for (int j = 0; j < 49; ++j) {
			int jj = j * 10;
			for (int k = 1; k <= 9; ++k) {
				linear[ii][jj + k] = linearInterpolation(jj, linear[ii][jj], jj + 10, linear[ii][jj + 10], jj + k);
			}
		}
	}
	//对每列进行插值
	for (int j = 0; j < 491; ++j) {
		//int jj = j;
		for (int i = 0; i < 49; ++i) {
			int ii = i * 10;
			for (int k = 1; k <= 9; ++k) {
				linear[ii + k][j] = linearInterpolation(ii, linear[ii][j], ii + 10, linear[ii + 10][j], ii + k);
			}
		}
	}
//线性插值结果写入.txt
	for (int i = 0; i < 491; i++)
	{
		for (int j = 0; j < 491; j++)
		{
			fprintf(write1, "%3d  ", linear[i][j]);
		}
		fprintf(write1, "\n");
	}
	linear_ed = clock();


	// ==========================三次插值==========================
	//提示：待处理图像数据为img[50][50]
	int cubic[491][491] = { 0 }; //三次插值最终结果保存到这个数组

	//***********************************
	 //*这里写下程序实现拉格朗日三次插值*
	//***********************************

	//把img的点位布置到cubic中
	for (int i = 0; i < 50; ++i) {
		for (int j = 0; j < 50; ++j) {
			cubic[i * 10][j * 10] = img[i][j];
		}
	}
	//先插行
	for (int i = 0; i < 50; ++i) {
		int ii = i * 10;
		for (int j = 0; j < 49; ++j) {
			int jj = j * 10;
			if (j == 0) {
				int x[] = { jj, jj + 10, jj + 20, jj + 30 };
				int y[] = { img[i][j], img[i][j + 1], img[i][j + 2], img[i][j + 3] };
				//int y[] = { cubic[ii][jj], cubic[ii][jj + 10], cubic[ii][jj + 20], cubic[ii][jj + 30] };
				for (int k = 1; k < 10; ++k) {
					cubic[ii][jj + k] = (int)cubicInterpolation(x, y, jj + k);
				}
			}
			else if (j == 48) {
				int x[] = { jj, jj + 10, jj - 20, jj - 10 };
				int y[] = { img[i][j], img[i][j + 1], img[i][j - 2], img[i][j - 1] };
				//int y[] = { cubic[ii][jj], cubic[ii][jj + 10], cubic[ii][jj + 20], cubic[ii][jj - 10] };
				for (int k = 1; k < 10; ++k) {
					cubic[ii][jj + k] = (int)cubicInterpolation(x, y, jj + k);
				}
			}
			else {
				int x[] = { jj - 10, jj + 10, jj + 20, jj };
				int y[] = { img[i][j - 1], img[i][j + 1], img[i][j + 2], img[i][j] };
				//int y[] = { img[ii][jj - 10], img[ii][jj + 10], img[ii][jj + 20], img[ii][jj] };
				for (int k = 1; k < 10; ++k) {
					cubic[ii][jj + k] = (int)cubicInterpolation(x, y, jj + k);
				}
			}
		}
	}
	//再插列
	for (int j = 0; j < 491; ++j) {
		for (int i = 0; i < 49; ++i) {
			int ii = i * 10;
			if (i == 0) {
				int x[] = { ii, i + 10, ii + 20, ii + 30 };
				//int y[] = { img[i][j], img[i + 1][j], img[i + 2][j], img[i + 3][j] };
				int y[] = { cubic[ii][j], cubic[ii + 10][j], cubic[ii + 20][j], cubic[ii + 30][j] };
				for (int k = 1; k < 10; ++k) {
					cubic[ii + k][j] = (int)cubicInterpolation(x, y, ii + k);
				}
			}
			else if (i == 48) {
				int x[] = { ii, ii + 10, ii - 20, ii - 10 };
				//int y[] = { img[i][j], img[i + 1][j], img[i - 2][j], img[i - 1][j] };
				int y[] = { cubic[ii][j], cubic[ii + 10][j], cubic[ii - 20][j], cubic[ii - 10][j] };
				for (int k = 1; k < 10; ++k) {
					cubic[ii + k][j] = (int)cubicInterpolation(x, y, ii + k);
				}
			}
			else {
				int x[] = { ii - 10, ii + 10, ii + 20, ii };
				//int y[] = { img[i - 1][j], img[i + 1][j], img[i + 2][j], img[i][j] };
				int y[] = { cubic[ii - 10][j], cubic[ii + 10][j], cubic[ii + 20][j], cubic[ii][j] };
				for (int k = 1; k < 10; ++k) {
					cubic[ii + k][j] = (int)cubicInterpolation(x, y, ii + k);
				}
			}
		}
	}

	//三次插值结果写入.txt
	for (int i = 0; i < 491; i++)
	{
		for (int j = 0; j < 491; j++)
		{
			fprintf(write2, "%3d  ", cubic[i][j]);
		}
		fprintf(write2, "\n");
	}
	cubic_ed = clock();
	//关闭插值结果文件
	fclose(write1);
	fclose(write2);

	stop = clock();
	duration_linear = (double)(linear_ed - linear_bg) / CLK_TCK;
	duration_cubic = (double)(cubic_ed - linear_ed) / CLK_TCK;
	duration = (double)(stop - start) / CLK_TCK;
	printf("拉格朗日线性插值所用时间：%f\n", duration_linear);
	printf("拉格朗日三次插值所用时间：%f\n", duration_cubic);
	printf("程序运行所用时间：%f\n", duration);

	system("pause");
	return 0;
}

int linearInterpolation(int x1, int y1, int x0, int y0, int x){
	return y0 * (x - x1) / (x0 - x1) + y1 * (x - x0) / (x1 - x0);
}

double cubicInterpolation(int x[], int y[], int point) {
	double coeff = 1.0;
	double sum = 0;
	for (int i = 0; i < 4; ++i) {
		coeff = 1.0;
		for(int j = 0; j < 4; ++j) {
			if(i != j) {
				coeff *= (point - x[j]);
				//sum += coeff * y[i];
			}
		}
		for (int j = 0; j < 4; ++j) {
			if (i != j) {
				coeff /= (x[i] - x[j]);
				//sum += coeff * y[i];
			}
		}
		sum += coeff * y[i];
	}
	return sum;
}