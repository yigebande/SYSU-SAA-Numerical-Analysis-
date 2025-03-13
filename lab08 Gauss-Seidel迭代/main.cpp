#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <math.h>
// 矩阵A的行数和列数
const int size = 9;
// 矩阵A
double originMatrixA[][size] = { {31, -13, 0, 0, 0, -10, 0, 0, 0},
								 {-13, 35, -9, 0, -11, 0, 0, 0, 0},
								 {0, -9, 31, -10, 0, 0, 0, 0, 0},
								 {0, 0, -10, 79, -30, 0, 0, 0, -9},
								 {0, 0, 0, -30, 57, -7, 0, -5, 0},
								 {0, 0, 0, 0, 7, 47, -30, 0, 0},
								 {0, 0, 0, 0, 0, -30, 41, 0, 0},
								 {0, 0, 0, 0, -5, 0, 0, 27, -2},
								 {0, 0, 0, -9, 0, 0, 0, -2, 29} };
// 矩阵B
double originMatrixB[] = { -15, 27, -23, 0, -20, 12, -7, 7, -10 };

// 复制向量
void copyVector(double newVec[], double oldVec[]);

// 对矩阵X第i行进行操作
double rowOperation(int index, const double matA[][size], const double matB[], double x[]);

// Jacobi迭代法求解，返回迭代次数
int Jacobi(const double matA[][size], const double matB[], double x[]);

// Gauss-Seidel迭代法求解，返回迭代次数
int Seidel(const double matA[][size], const double matB[], double x[]);

// 返回向量1范数
double normOfVector(double x[]);

// 打印矩阵
void PrintMat(double x[]);

clock_t start, stop;
double duration;

int main()
{
	start = clock();



	//***********************************
	//*这里写下程序实现*
	
	double x[9] = { 0.0 };
	double xx[9] = { 0.0 };
	int timesOfJacobi = Jacobi(originMatrixA, originMatrixB, x);
	int timesOfSeidel = Seidel(originMatrixA, originMatrixB, xx);
	printf("Jacobi迭代次数为: %d\n", timesOfJacobi);
	printf("Jacobi求解结果为: \n");
	PrintMat(x);
	printf("\n");
	printf("Seidel迭代次数为: %d\n", timesOfSeidel);
	printf("Seidel求解结果为: \n");
	PrintMat(xx);
	printf("\n");

	//***********************************


	stop = clock();
	duration = (double)(stop - start) / CLK_TCK;
	printf("程序运行所用时间：%f\n", duration);

	system("pause");
	return 0;
}

void copyVector(double newVec[], double oldVec[]) {
	for (int i = 0; i < size; ++i) {
		newVec[i] = oldVec[i];
	}
}

double rowOperation(int index, const double matA[][size], const double matB[], double x[]) {
	double res = 0.0;
	for (int k = 0; k < size; ++k) {
		if (k == index) {		// 如果下标k与index相等，则不作任何处理
			continue;
		}
		else {
			res -= matA[index][k] * x[k];
		}
	}
	res = (res + matB[index]) / matA[index][index];
	return res;
}

int Jacobi(const double matA[][size], const double matB[], double x[]) {
	int counter = 0;	// 计数器
	double gap = 1.0;	// 预设gap=1, 使循环正常进行
	double tVec[size] = { 0 };	// 每次计算新的x_i都是用前一个向量的x_i计算的, 先用tVec储存前一个向量
	double normOfCurr = normOfVector(x);	
	// 10E-5 != 0.00001, 0.00001更好能迭代22次, 而10E-5只能迭代20次
	while (gap >= 0.00001) {				// 如果范数的误差大于等于0.00001, 继续迭代
		double normOfTemp = normOfCurr;		// 储存上一次向量的范数
		copyVector(tVec, x);				// 储存前一个向量
		for (int i = 0; i < size; ++i) {
			x[i] = rowOperation(i, originMatrixA, originMatrixB, tVec);		// 计算新的向量
		}
		normOfCurr = normOfVector(x);			// 计算得到的新向量的范数
		gap = fabs(normOfCurr - normOfTemp);	// 新旧范数之间差的绝对值
		counter++;								// 计数器自增
	}
	return counter;
}

int Seidel(const double matA[][size], const double matB[], double x[]) {
	// 思路与Jacobi法一致, 只是计算新向量时有所变化
	int counter = 0;
	double gap = 1.0;
	// double tVec[size] = { 0 };
	double normOfCurr = normOfVector(x);
	// 10E-5 != 0.00001
	while (gap >= 0.00001) {
		double normOfTemp = normOfCurr;
		// copyVector(tVec, x);
		for (int i = 0; i < size; ++i) {
			x[i] = rowOperation(i, originMatrixA, originMatrixB, x);
		}
		normOfCurr = normOfVector(x);
		gap = fabs(normOfCurr - normOfTemp);
		counter++;
	}
	return counter;
}

double normOfVector(double x[]) {
	double res = 0.0;
	for (int i = 0; i < size; ++i) {
		res += fabs(x[i]);
	}
	return res;
}

void PrintMat(double x[]) {
	for (int i = 0; i < size; ++i) {
		printf("%10.6lf", x[i]);
	}
	printf("\n");
}