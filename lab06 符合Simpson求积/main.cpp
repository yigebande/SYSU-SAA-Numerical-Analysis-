#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <math.h>
// 区间长度
const double interval = 0.08;
// 节点间隔
const double nodeLength = 0.04;

// 复合Simpson公式求面积
void areaSimpson(double surface[][101], double zx[]);
// 面积的奇数节点函数值求和
double sumOfOddArea(double surface[][101], int i);
// 面积的偶数节点函数值求和
double sumOfEvenArea(double surface[][101], int i);
// 打印S_i
// 测试用
void printYArea(double zx[]);
// 复合Simpson公式求体积
void VSimpson(double area[]);
// 体积的奇数节点函数值求和
double sumOfOddV(double area[101]);
// 体积的偶数节点函数值求和
double sumOfEvenV(double area[101]);

clock_t start, stop;
double duration;

int main()
{
	start = clock();

	FILE* read = NULL;
	//打开曲面数据的文件
	fopen_s(&read, "surface_data.txt", "r");
	if (!read)
	{
		perror("fail to read !");
		exit(-1);
	}
	//读入.txt文件数据
	double surface[101][101];
	for (int i = 0; i < 101; i++)
	{
		for (int j = 0; j < 101; j++)
		{
			fscanf(read, "%lf", &surface[i][j]);
		}
	}
	fclose(read);

	//提示：曲面数据为surface[101][101]；
	//***********************************
	//*这里写下程序实现Simpson求积*
	//***********************************
	double yArea[101] = { 0 };
	areaSimpson(surface, yArea);
	VSimpson(yArea);


	stop = clock();
	duration = (double)(stop - start) / CLK_TCK;
	printf("程序运行所用时间：%f\n", duration);

	system("pause");
	return 0;
}

void areaSimpson(double surface[][101], double zx[]) {
	for (int i = 0; i < 101; ++i) {
		zx[i] = 4.0 / (6 * 50) * (surface[i][0] + sumOfOddArea(surface, i) + sumOfEvenArea(surface, i) + surface[i][100]);
	}
}

double sumOfOddArea(double surface[][101], int i) {
	/* i是S_i里的i
	 */
	double res = 0;
	for (int k = 1; k < 101; k += 2) {
		res += surface[i][k];
	}
	return 4 * res;
}

double sumOfEvenArea(double surface[][101], int i) {
	/* i是S_i里的i
	 */
	double res = 0;
	// 小于100是因为偶数节点只加到第98个
	for (int k = 2; k < 100; k += 2) {
		res += surface[i][k];
	}
	return 2 * res;
}

void printYArea(double zx[]) {
	for (int i = 0; i < 101; ++i) {
		printf("%10.6lf\n", zx[i]);
	}
}

void VSimpson(double area[]) {
	double res = 0;
	res += 4.0 / (6 * 50) * (area[0] + sumOfOddV(area) + sumOfEvenV(area) + area[100]);
	printf("体积为：%lf\n", res);
	
}

double sumOfOddV(double area[101]) {
	double res = 0;
	for (int k = 1; k < 101; k += 2) {
		res += area[k];
	}
	return 4 * res;
}

double sumOfEvenV(double area[101]) {
	double res = 0;
	for (int k = 2; k < 100; k += 2) {
		res += area[k];
	}
	return 2 * res;
}