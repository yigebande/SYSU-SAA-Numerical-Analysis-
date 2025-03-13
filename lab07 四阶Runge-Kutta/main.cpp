#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <math.h>
// 高度表有10个元素
const int nums = 10;
// 高度表
double height[] = { 30, 40, 50, 60, 80, 90, 100, 110, 120, 130 };

// 对应高度的空气密度表，接下来要构建一个密度的对数表
double rho[] = { 7394136673.0, 1858858651.0, 467310199.6, 117480058.3, 7424751.8,
				 1866555.2, 497400.3, 117966.5, 24900.2, 8377.1 };

// ln(rho)
double ln_rho[10] = { 0 };
// 构造ln(rho)的函数
void makeLnRho(double rho[], double make[]);
// 对x*y的求和函数
double sumOfXY(double x[], double y[]);
// 对单个x（或y）求和的函数
double sumOfSingle(double z[]);
// 对单个x（或y）的平方的求和函数
double sumOfSquare(double z[]);
// 最小二乘法拟合直线求截距a的函数
double findA(double x[], double y[]);
// 最小二乘法拟合直线球斜率b的函数
double findB(double x[], double y[], double A);
// ln(rho_0)
double rhoZero(double b);
// -1 / H_0 
double reciprocalHZero_minus(double a);
// 大气密度公式
double funcOfRho(double h, double rhoo, double reciprocal);
// Runge-Kutta法主函数
double rk(double y, double t, double h);
// k系数
void coK(double y, double h, double arrK[]);
// 速度导数
double dVelocity(double v);

clock_t start, stop;
double duration;

int main()
{
	start = clock();


	makeLnRho(rho, ln_rho);
	double linearA = findA(height, ln_rho);
	double linearB = findB(height, ln_rho, linearA);
	//printf("A=%lf\nB=%lf\n", linearA, linearB);
	double rhoSeventy = funcOfRho(70.0, rhoZero(linearB), linearA);
	printf("H0=%lf\n", -1.0 / linearA);
	printf("r0=%lf\n", rhoZero(linearB));
	printf("r70=%lf\n", rhoSeventy);
	printf("------------------------\n");
	printf("步长为 10.000000 四阶Runge-Kutta法计算结果：%lf\n", rk(11.0, 0.0, 10.0));
	printf("步长为 5.000000 四阶Runge-Kutta法计算结果：%lf\n", rk(11.0, 0.0, 5.0));
	printf("步长为 2.000000 四阶Runge-Kutta法计算结果：%lf\n", rk(11.0, 0.0, 2.0));
	printf("步长为 1.000000 四阶Runge-Kutta法计算结果：%lf\n", rk(11.0, 0.0, 1.0));
	printf("步长为 0.100000 四阶Runge-Kutta法计算结果：%lf\n", rk(11.0, 0.0, 0.1));
	stop = clock();
	duration = (double)(stop - start) / CLK_TCK;
	printf("程序运行所用时间：%f\n", duration);

	system("pause");
	return 0;
}


void makeLnRho(double rho[], double make[]) {
	for (int i = 0; i < nums; ++i) {
		make[i] = log(rho[i]);
	}
}

double sumOfXY(double x[], double y[]) {
	double res = 0.0;
	for (int i = 0; i < nums; ++i) {
		res += x[i] * y[i];
	}
	return res;
}

double sumOfSingle(double z[]) {
	double sum = 0.0;
	for (int i = 0; i < nums; ++i) {
		sum += z[i];
	}
	return sum;
}

double sumOfSquare(double z[]) {
	double sum = 0.0;
	for (int i = 0; i < nums; ++i) {
		sum += z[i] * z[i];
	}
	return sum;
}

double findA(double x[], double y[]) {
	double sumOfXSquare = sumOfSingle(x);
	return ((nums * sumOfXY(x, y) - sumOfSingle(x) * sumOfSingle(y)) /
		(nums * sumOfSquare(x) - sumOfXSquare * sumOfXSquare));
}

double findB(double x[], double y[], double A) {
	// 浮点运算不要搞int类型的数进去，1和1.0有天壤之别
	// return (1 / nums * sumOfSingle(y) - A / nums * sumOfSingle(x));
	return (1.0 / nums * sumOfSingle(y) - A / nums * sumOfSingle(x));
}


double rhoZero(double b) {
	return exp(b);
}

double reciprocalHZero_minus(double a) {
	return a;
}

double funcOfRho(double h, double rhoo, double reciprocal) {
	return rhoo * exp(h * reciprocal);
}

double rk(double y, double t, double h) {
	double arrK[5] = { 0.0 };
	coK(y, h, arrK);
	int times = (int)180.0 / h;
	for (int i = 1; i <= times; ++i) {
		y = y + h * (arrK[1] + 2.0 * arrK[2] + 2.0 * arrK[3] + arrK[4]) / 6;
		// 不要忘记更新系数K
		coK(y, h, arrK);
	}
	return y;
}

void coK(double y, double h, double arrK[]) {
	arrK[1] = dVelocity(y);
	arrK[2] = dVelocity(y + h * 0.5 * arrK[1]);
	arrK[3] = dVelocity(y + h * 0.5 * arrK[2]);
	arrK[4] = dVelocity(y + h * arrK[3]);
}

double dVelocity(double v) {
	return -0.01 * pow(10.0, -9) * 29534086.785989 * v * v;

}