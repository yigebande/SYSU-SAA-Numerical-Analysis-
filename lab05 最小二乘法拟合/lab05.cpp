#include<stdio.h>
#include<stdlib.h>
#include<math.h>
#include<time.h>

// xyz的A矩阵
typedef struct _matrixA {
    double X[5][5];
    double Y[5][5];
    double Z[5][5];
}matrixA;

// xyz的B矩阵
typedef struct _matrixB {
    double X[5];
    double Y[5];
    double Z[5];
}matrixB;
//换个名字，方程的解是double类型数据
typedef double solution;

//xyz坐标
double Xaxis[] = { -386976.786, -397199.228, -403002.333,
                   -404301.874, -401058.65, -393279.751,
                   -381019.561, -364380.489, -343513.4};

double Yaxis[] = { 106369.212, 70584.989, 34008.818,
                  -2946.251, -39859.487, -76305.83,
                  -111859.429, -146097.317, -178603.222};

double Zaxis[] = { 61240.44, 41725.648, 21741.252,
                   1512.866, -18729.308, -38752.308,
                   -58322.344, -77206.812, -95176.378 };

double tau[] = { -1, -0.75, -0.5, -0.25, 0, 0.25, 0.5, 0.75, 1 };

//复制数组A
void copyMatA(double mat[][5], double matA[][5]);
//Tn(x)
double Tn(int n, double x);
//内积
double Inner(const int i, const int j);
double InnerForB(const double axis[], const int j);
//创建系数矩阵
void createMatA(double mat[][5]);
//创建矩阵B，传入需要修改的数组，并指明坐标xyz
void createMatB(double mat[], double axis[]);
//打印矩阵A
void PrintA(double mat[][5], double matB[]);
//打印矩阵B
void PrintB(double mat[]);
//分割线
void barrier();
//Doolittle分解
void doolittle(double mat[][5]);
//L
void getL(double matL[][5], double LU[][5]);
//U
void getU(double matU[][5], double LU[][5]);
//Ly=X, y
void getY(double y[], double L[][5], double B[]);
//Ux=y, x
void getX(double x[], double U[][5], double y[]);
//解线性方程组
void solve(double axis[], double matA[][5], double matB[]);

//clock_t为clock()函数返回的变量类型
clock_t start, stop;
double duration;

int main()
{
    start = clock();
    matrixA matA;
    matrixB matB;

    double disp_matA[5][5] = { 0 };
    //xyz求出的矩阵A是完全一样的，用复制的方法比重新创建更快
    createMatA(matA.X);
    copyMatA(matA.Y, matA.X);
    copyMatA(matA.Z, matA.X);
    copyMatA(disp_matA, matA.X);
    //创建矩阵B
    createMatB(matB.X, Xaxis);
    createMatB(matB.Y, Yaxis);
    createMatB(matB.Z, Zaxis);
    //待求未知数
    solution x[5] = { 0 };
    solution y[5] = { 0 };
    solution z[5] = { 0 };
    //解方程
    solve(x, matA.X, matB.X);
    solve(y, matA.Y, matB.Y);
    solve(z, matA.Z, matB.Z);
    printf("=========== X坐标拟合结果 ===========\n");
    PrintA(disp_matA, matB.X);
    PrintB(x);
    printf("=========== Y坐标拟合结果 ===========\n");
    PrintA(disp_matA, matB.Y);
    PrintB(y);
    printf("=========== Z坐标拟合结果 ===========\n");
    PrintA(disp_matA, matB.Z);
    PrintB(z);

    


    stop = clock();
    //CLK_TCK为clock()函数的时间单位，即时钟打点
    duration = (double)(stop - start) / CLK_TCK;
    printf("解方程所用时间：%f\n", duration);
    system("pause");
}

void copyMatA(double mat[][5], double matA[][5]) {
    for (int i = 0; i < 5; ++i) {
        for (int j = 0; j < 5; ++j) {
            mat[i][j] = matA[i][j];
        }
    }
}

double Tn(int n, double x) {
    if (n == 0) {
        return 1;
    }
    else if (n == 1) {
        return x;
    }
    else if (n == 2) {
        return 2 * x * x - 1;
    }
    else if (n == 3) {
        return 4 * x * x * x - 3 * x;
    }
    else if (n == 4) {
        return 8 * x * x * x * x - 8 * x * x + 1;
    }
    else {
        return cos(n * acos(x));
    }
}

double Inner(const int i, const int j) {
    double sum = 0;
    for (int k = 0; k < 9; ++k) {
        sum += Tn(i, tau[k]) * Tn(j, tau[k]);
    }
    return sum;
}

double InnerForB(const double axis[], const int j) {
    double sum = 0;
    for (int k = 0; k < 9; ++k) {
        sum += axis[k] * Tn(j, tau[k]);
    }
    return sum;
}

void createMatA(double mat[][5]) {
    for (int i = 0; i <= 4; ++i) {
        for (int j = 0; j <= 4; ++j) {
            mat[i][j] = Inner(i, j);
        }
    }
}

void createMatB(double mat[], double axis[]) {
    for (int i = 0; i <= 4; ++i) {
        mat[i] = InnerForB(axis, i);
    }
}

void PrintA(double mat[][5], double matB[]) {
    printf("A=\n");
    for (int i = 0; i < 5; ++i) {
        for (int j = 0; j < 5; ++j) {
            printf("%10.6lf", mat[i][j]);
        }
        printf(" % 10.6lf", matB[i]);
        printf("\n");
    }
    //barrier();
}


void PrintB(double mat[]) {
    printf("Result=\n");
    for (int i = 0; i < 5; ++i) {
        printf("%10.6lf ", mat[i]);
    }
    printf("\n\n");
    //barrier();
}

void barrier() {
    printf("-------------------------------------------------------------------\n");
}

void doolittle(double mat[][5]) {
    int i, j, k, l, m;
    double up = 0.0;
    double low = 0.0;
    //5x5矩阵共处理5次行，5次列，第1行不用处理
    for (i = 1; i < 5; ++i) {
        //第j行，第i-1列
        for (j = i; j < 5; ++j) {
            if (i == 1) {
                mat[j][i - 1] /= mat[0][0];
            }
            else {
                low = 0.0;
                for (l = 0; l < i - 1; ++l) {
                    low += mat[j][l] * mat[l][i - 1];
                }
                mat[j][i - 1] = (mat[j][i - 1] - low) / mat[i - 1][i - 1];
            }
        }
        //第i行，第k列
        for (k = i; k < 5; ++k) {
            up = 0.0;
            for (m = 0; m < i; ++m) {
                up += mat[m][k] * mat[i][m];
            }
            mat[i][k] -= up;
        }
    }
}

void getL(double matL[][5], double LU[][5]) {
    int i, j;
    for (i = 0; i < 5; ++i) {
        for (j = 0; j <= i; ++j) {
            if (j == i) {
                matL[i][j] = 1;
            }

            else {
                matL[i][j] = LU[i][j];
            }
        }
    }
}

void getU(double matU[][5], double LU[][5]) {
    int i, j;
    for (i = 0; i < 5; ++i) {
        for (j = i; j < 5; ++j) {
            matU[i][j] = LU[i][j];
        }
    }
}

void getY(double y[], double L[][5], double B[]) {
    int i, j;
    double co = 0;
    for (i = 0; i < 5; ++i) {
        if (i == 0) {
            y[0] = B[0];
        }
        else {
            co = 0;
            for (j = 0; j < i; ++j) {
                co += y[j] * L[i][j];
            }
            y[i] = B[i] - co;
        }
    }
}

void getX(double x[], double U[][5], double y[]) {
    int i, j;
    double co = 0;
    for (i = 4; i >= 0; --i) {
        if (i == 4) {
            x[i] = y[i] / U[i][i];
        }
        else {
            co = 0;
            for (j = 4; j > i; --j) {
                co += x[j] * U[i][j];
            }
            x[i] = (y[i] - co) / U[i][i];
        }
    }
}

void solve(double axis[], double matA[][5], double matB[]) {
    double L[5][5] = { 0 };
    double U[5][5] = { 0 };
    //double x[5] = { 0 };
    double y[5] = { 0 };
    doolittle(matA);
    getL(L, matA);
    getU(U, matA);
    getY(y, L, matB);
    getX(axis, U, y);
}