#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#define SIZE 5

clock_t start, stop, linear_bg, linear_ed, cubic_ed;
double duration, duration_linear, duration_cubic;
//用于记忆化的数组
//double memoization[SIZE][SIZE] = { 0 };

//布置数组
void Deploy(int origin[][50], int arr[][491]);

//线性插值
void linearInterpolation(int img[][491]);

//牛顿插值
int Newton(int x[], int y[], int value);

//行操作
void rowOperation(int img[][491]);

//列操作
void colOperation(int img[][491]);

//插值放大图片
void cubicInterpolation(int img[][491]);

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
	Deploy(img, linear);
	linearInterpolation(linear);
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
	Deploy(img, cubic);
	cubicInterpolation(cubic);
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
	printf("牛顿线性插值所用时间：%f\n", duration_linear);
	printf("牛顿三次插值所用时间：%f\n", duration_cubic);
	printf("程序运行所用时间：%f\n", duration);

	system("pause");
	return 0;
}

void Deploy(int origin[][50], int arr[][491]) {
	for (int i = 0; i < 50; ++i) {
		int ii = i * 10;
		for (int j = 0; j < 50; j++) {
			arr[ii][j * 10] = origin[i][j];
		}
	}
}

void linearInterpolation(int img[][491]) {
	//先插行
	for (int i = 0; i < 50; ++i) {
		int ii = i * 10;
		for (int j = 0; j < 49; ++j) {
			int jj = j * 10;
			//f[x0,x1]
			double coeff = 1.0 * (img[ii][jj + 10] - img[ii][jj]) / 10;
			for (int k = 1; k < 10; ++k) {
				//f(x0) + f[x0,x1](x-x0)
				double res = img[ii][jj] + coeff * k;
				img[ii][jj + k] = (int)res;
			}
		}
	}
	//再插列
	for (int j = 0; j < 491; ++j) {
		for (int i = 0; i < 49; ++i) {
			int ii = i * 10;
			double coeff = 1.0 * (img[ii + 10][j] - img[ii][j]) / 10;
			for (int k = 1; k < 10; ++k) {
				double res = img[ii][j] + coeff * (k);
				img[ii + k][j] = (int)res;
			}
		}
	}
}

int Newton(int x[], int y[], int value) {
	double sheet[SIZE][SIZE] = { 0 };	//差商表
	double sum = 0.0;
	double product = 1.0;
	//构造差商表，矩阵主对角线以下的元素为0
	for (int i = 0; i < 4; ++i) {
		for (int j = i; j < 4; ++j) {
			if (i == 0) {
				//矩阵第一行的元素为f(x)，即已知点的函数值
				sheet[i][j] = (double)y[j];
			}
			else {
				//递归求差商，虽然没有用到递归函数（其实是在这里没必要用递归函数）
				sheet[i][j] = (sheet[i - 1][j] - sheet[i - 1][j - 1]) / ((double)x[j] - (double)x[j - i]);
			}
		}
	}
		for (int i = 0; i < 4; ++i) {
			product = 1.0;
			for (int j = 0; j < i; ++j) {
				//(x-x0)(x-x1)(x-x2)...
				product *= (value -	(double)x[j]);
			}
			//f(x0) + f[x0,x1](x-x0) + f[x0,x1,x2](x-x0)(x-x1) +...
			sum += product * sheet[i][i];
		}
	//四舍五入
	return (int)sum ? (int)sum + 1 : (sum - (int)sum) < 0.5 ;
}

void rowOperation(int img[][491]) {
	/*注意：x，y数组元素的顺序不可变*/
	for (int i = 0; i < 50; ++i) {
		int ii = i * 10;
		for (int j = 0; j < 49; ++j) {
			int jj = j * 10;
			
			//0-1之间的间隔
			if (j == 0) {
				int x[] = { jj, jj + 10, jj + 20, jj + 30 };
				int y[] = { img[ii][jj], img[ii][jj + 10], img[ii][jj + 20], img[ii][jj + 30] };
				for (int k = 1; k < 10; ++k) {
					img[ii][jj + k] = Newton(x, y, jj + k);
				}
			}
			//48-49之间的间隔
			else if (j == 48) {
				int x[] = { jj - 10, jj - 20, jj, jj + 10, };
				int y[] = { img[ii][jj - 10], img[ii][jj - 20], img[ii][jj], img[ii][jj + 10]};
				for (int k = 1; k < 10; ++k) {
					img[ii][jj + k] = Newton(x, y, jj + k);
				}
			}
			//2-48每两个的间隔
			else {
				int x[] = { jj - 10, jj, jj + 10, jj + 20 };
				int y[] = { img[ii][jj - 10], img[ii][jj], img[ii][jj + 10], img[ii][jj + 20] };
				for (int k = 1; k < 10; ++k) {
					img[ii][jj + k] = Newton(x, y, jj + k);
				}
			}
		}
	}
}

void colOperation(int img[][491]) {
	//和行操作类似
	for (int j = 0; j < 491; ++j) {
		for (int i = 0; i < 49; ++i) {
			int ii = i * 10;
			if (i == 0) {
				int x[] = { ii, ii + 10, ii + 20, ii + 30 };
				int y[] = { img[ii][j], img[ii + 10][j], img[ii + 20][j], img[ii + 30][j] };
				for (int k = 1; k < 10; ++k) {
					img[ii + k][j] = Newton(x, y, ii + k);
				}
			}
			else if (i == 48) {
				int x[] = { ii - 20, ii - 10, ii, ii + 10 };
				int y[] = { img[ii - 20][j], img[ii - 10][j], img[ii][j], img[ii + 10][j] };
				for (int k = 1; k < 10; ++k) {
					img[ii + k][j] = Newton(x, y, ii + k);
				}
			}
			else {
				int x[] = { ii - 10, ii, ii + 10, ii + 20 };
				int y[] = { img[ii - 10][j], img[ii][j], img[ii + 10][j], img[ii + 20][j] };
				for (int k = 1; k < 10; ++k) {
					img[ii + k][j] = Newton(x, y, ii + k);
				}
			}
		}
	}
}

void cubicInterpolation(int img[][491]) {
	rowOperation(img);
	colOperation(img);
}