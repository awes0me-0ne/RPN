/*
  逆ポーランド記法 関数電卓プログラム
  作成日:2018/01/25
  作成者:B18B031
  氏  名:佐藤将嗣
  
*/
#include<stdio.h>
#include<stdlib.h> /* malloc用 */
#include<ctype.h>
#include<string.h>

#define DUMMY -1     /* ダミーデータ */
#define NODATA 0     /* データなし */
#define EXISTDATA 1  /* データあり */
#define EPS 1.0e-5   /* 微小量 ニュートン法の収束条件に使用 */
#define LOOPMAX 1000 /* 繰り返しの最大回数 */
#define MAX 12
#define NUM '0'

/*
double型データを保持するセル
スタックで用いる
自己参照構造体
*/
struct doubleCell {
	double data;             /* データ部 */
	struct doubleCell *next; /* ポインタ部 */
};

typedef struct doubleCell DoubleCell;
typedef DoubleCell* DoubleStack; /* スタック型 */
typedef DoubleCell* Cell_P;      /* セル型(型としてはスタック型と同一) */

								 /* プロトタイプ宣言 */
DoubleStack makeStack();                           /* スタックの作成 */
void push(DoubleStack stack, double data);         /* データ挿入 */
double pop(DoubleStack stack);                     /* データ取り出し */
double isEmpty(DoubleStack stack);                 /* 空であるかのテスト */
void printStack(DoubleStack stack);                /* リスト表示 */
void fscanToStack(FILE * fp, DoubleStack stack);   /* ファイルからのデータ入力 */
void reverse_fprint(FILE * fp, DoubleStack stack); /* ファイルへ逆順に出力 */
Cell_P makeCell(double data);                      /* セル作成 */

												   /* 基本演算 */
double absolute(double a);                         /* 絶対値 */

												   /* 標準演算 */
double Root(double input_a);                       /* 平方根 */
double Sine(double input_a);                       /* サイン */
double Cose(double input_a);                       /* コサイン */
double Power(double input_a, double input_b);      /* べき乗 */
double fact(double t);


int main()
{
	FILE *input;
	FILE *output;
	double ans;

	/* スタックの動作 */
	DoubleStack stack;

	stack = makeStack(); /* スタック生成 */
	printf("ファイルから計算式を読み込みます。\n");
	input = fopen("Data", "r");

	fscanToStack(input, stack);

	fclose(input);

	output = fopen("Output", "w");

	ans = pop(stack);
	fprintf(output, "Ans=%12.4f\n", ans);
	fclose(output);


	return 0;
}


DoubleStack makeStack()
{/* スタックの作成 */
	DoubleStack stack = (DoubleStack)makeCell(DUMMY); /* ダミーのセル */
	return stack;
}
void push(DoubleStack stack, double data)
{/* データ挿入 */
	Cell_P new_cell = makeCell(data);
	new_cell->next = stack->next;
	stack->next = new_cell;
	return;
}
double pop(DoubleStack stack)
{/* データ取り出し */
	Cell_P top = stack->next;
	double data;
	if (top != NULL) {
		data = top->data;
		stack->next = top->next;
		free(top);
		return data;
	}
	else {
		printf("No data\n");
		return -1;
	}
}
double isEmpty(DoubleStack stack)
{/* 空であるかのテスト */
	if (stack->next == NULL)
	{
		return NODATA;
	}
	else
	{
		return EXISTDATA;
	}
}
void printStack(DoubleStack stack)
{/* リスト表示 */
	Cell_P p = stack->next;
	printf("(");
	while (p != NULL)
	{
		printf("%12.4f\n", p->data);
		p = p->next;
	}
	printf("]\n");
	return;
}

void reverse_fprint(FILE * fp, DoubleStack stack)
{/* ファイルへ逆順に出力 */
	double data;
	while (isEmpty(stack) == EXISTDATA) {
		data = pop(stack);
		fprintf(fp, "%12.4f\n", data);
	}
	return;
}
Cell_P makeCell(double data)
{/* セル作成 */
	Cell_P new = (Cell_P)malloc(sizeof(DoubleCell));
	new->data = data;
	return new;
}

/* 基本演算 */
double absolute(double a)
{/* 絶対値 */
	if (a >= 0) {
		return a;
	}
	else {
		return -1 * a;
	}
}

/* 標準演算 */
double Root(double input_a)
{/* 平方根 */
	double approx;
	int count;
	int input_k = 2;
	if (input_a <= 0) {
		printf("0より大きな値を入力してください\n");
		return 0;
	}
	else {
		approx = input_a; /* ニュートン法の初期値を入力値に設定 */
		count = 0;
		/* ニュートン法の繰り返し処理 */
		while (absolute(Power(approx, input_k) - input_a) > EPS)
		{ /* 差がEPSより大きい間は繰り返す */
			if (count >= LOOPMAX)
			{
				printf("ニュートン法の繰り返し回数が、規定の繰返し(1000回)に達しました。\n");
				printf("求めた近似解は精度要求を満たしていません。\n");
				break;
			}
			/* ニュートン法の途中経過の表示 */
			printf("x%d = %15.8f \n", count, approx);
			/* ニュートン法の漸化式の計算 */
			approx = (approx*(input_k - 1) + input_a / Power(approx, input_k - 1)) / input_k;

			count++; /* 漸化式の計算を行った回数のカウント */
		}

	}
	return approx;
}
double fact(double t)
{
	int i;
	double tmp = 1;
	for (i = 1; i <= t; i++) {
		tmp = tmp*i;
	}
	return tmp;
}
double Sine(double input_a)
{/* サイン */
	int i = 0; //カウンタ
	int j = 0;
	double tmp = 0;
	while ((1 / fact(i)) > EPS) {
		j = 2 * i + 1;
		if (i % 2 == 0) {
			tmp = tmp + Power(input_a, j) / fact(j);
		}
		else {
			tmp = tmp - 1 * Power(input_a, j) / fact(j);
		}
		i++;
	}
	return tmp;
}
double Cose(double input_a)
{/* コサイン */
	int i = 0; //カウンタ
	int j = 0;
	double tmp = 0;
	while ((1 / fact(i)) > EPS) {
		j = 2 * i;
		if (i % 2 == 0) {
			tmp = tmp + Power(input_a, j) / fact(j);
		}
		else {
			tmp = tmp - 1 * Power(input_a, j) / fact(j);
		}
		i++;
	}
	return tmp;
}
double Power(double input_a, double input_b)
{/* べき乗 */
	int i; //カウンタ
	double tmp = 1;
	if (input_b > 0) {
		for (i = 1; i <= input_b; i++) {
			tmp = tmp*input_a;
		}
		return tmp;
	}
	else {
		input_b = -1 * input_b;
		for (i = 1; i <= input_b; i++) {
			tmp = tmp / input_a;
		}
		return tmp;
	}
}
void fscanToStack(FILE * fp, DoubleStack stack)
{/* ファイルからのデータ入力 */
	char cdata[MAX];
	double op1;
	double op2;
	double ans;

	while (fscanf(fp, "%s", cdata) != EOF)
	{
		printStack(stack);

		if (strcmp(cdata, "+") == 0)
		{
			printf("Called add\n");
			op1 = pop(stack);             /*第1項*/
			op2 = pop(stack);            /*第2項*/
			ans = op1 + op2;          /*足し算*/
			push(stack, ans);            /*ansをstackにpush*/
		}
		else  if (strcmp(cdata, "-") == 0)
		{
			op1 = pop(stack);
			op2 = pop(stack);
			ans = op2 - op1;
			push(stack, ans);
		}
		else  if (strcmp(cdata, "*") == 0)
		{
			op1 = pop(stack);
			op2 = pop(stack);
			ans = op1*op2;
			push(stack, ans);
		}
		else  if (strcmp(cdata, "/") == 0)
		{
			op1 = pop(stack);
			op2 = pop(stack);
			ans = op2 / op1;
			push(stack, ans);
		}
		else  if (strcmp(cdata, "M") == 0)
		{
			op1 = pop(stack);
			ans = -op1;
			push(stack, ans);
		}
		else  if (strcmp(cdata, "ABS") == 0)
		{
			op1 = pop(stack);
			ans = absolute(op1);
			push(stack, ans);
		}
		else  if (strcmp(cdata, "POW") == 0)
		{
			op1 = pop(stack);
			op2 = pop(stack);
			if (op1 == 0)
			{
				printf("0以外の実数値を入力してください\n");
			}
			else
			{
				ans = Power(op2, op1);
				push(stack, ans);
			}
		}
		else  if (strcmp(cdata, "RT") == 0)
		{
			op1 = pop(stack);
			ans = Root(op1);
			push(stack, ans);
		}
		else if (strcmp(cdata, "SIN") == 0)
		{
			op1 = pop(stack);
			ans = Sine(op1);
			push(stack, ans);
		}
		else if (strcmp(cdata, "COS") == 0)
		{
			op1 = pop(stack);
			ans = Cose(op1);
			push(stack, ans);
		}
		else
		{
			/*演算子でなくて、数値が入力された*/

			ans = atof(cdata);
			push(stack, ans);
		}

	}
	printStack(stack);
	return;
}
