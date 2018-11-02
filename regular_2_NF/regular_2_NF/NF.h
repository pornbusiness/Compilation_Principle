#pragma once

#pragma warning(disable:4996)

#include<stdio.h>
#include<stdlib.h>
#include <stdbool.h>
#include<string.h>

#define TS_SIZE 10
#define CLST_SIZE 10
#define S_SZIE 100
#define E_SIZE 20
#define F_SIZE 10
#define TRLST_SIZE 1000

int nr_state = 0;

struct Character {
	char c;
	int nr_state;
	int toState[TS_SIZE];
};
typedef struct transfer {
	int num;
	int nr_charct;
	struct Character cList[CLST_SIZE];
}Transfer;

typedef struct state {
	int nr_s;
	int s[S_SZIE];
}State;

typedef struct e {
	int nr_e;
	char _e[E_SIZE];
}E;

typedef struct ff {
	int nr_f;
	int _f[F_SIZE];
}FF;

typedef struct fa {
	State S;
	E ee;
	Transfer transferList[TRLST_SIZE];
	int q;
	FF fs;
}FA;


void newState(Transfer *list, State *s, char c) {
	
	//初态
	list[0].num = nr_state++;				//分配状态号
	s->s[s->nr_s] = list[0].num;
	s->nr_s++;
	list[0].nr_charct = 0;				
	
	//终态
	list[1].num = nr_state++;
	s->s[s->nr_s] = list[1].num;
	s->nr_s++;
	list[1].nr_charct = 0;

	//连接状态
	list[0].cList[list[0].nr_charct].c = c;
	list[0].cList[list[0].nr_charct].nr_state = 0;
	list[0].cList[list[0].nr_charct++].toState[list[0].cList[list[0].nr_charct].nr_state++] = list[1].num;
	
}

void initFA(FA *f) {
	f->S.nr_s = 0;
	f->ee.nr_e = 0;
	f->fs.nr_f = 0;

	for (int i = 0; i < TRLST_SIZE; i++) {
		f->transferList[i].nr_charct = 0;
		for (int j = 0; j < CLST_SIZE; j++) {
			f->transferList[i].cList[j].nr_state = 0;
		}
	}
	
}

FA *newFA(char c) {
	FA *f = (FA *)malloc(sizeof(FA));

	initFA(f);
	newState(f->transferList, &(f->S), c);
	f->ee._e[f->ee.nr_e++] = c;
	f->q = f->S.s[0];
	f->fs._f[f->fs.nr_f++] = f->S.s[1];

	return f;
}

void printFA(FA *f){
	printf("S:{");
	for (int i = 0; i < f->S.nr_s; i++) {
		printf("%d ", f->S.s[i]);
	}
	printf("}\n\n");

	printf("E:{");
	for (int i = 0; i < f->ee.nr_e; i++) {
		printf("%c ", f->ee._e[i]);
	}
	printf("}\n\n");

	printf("transferList:\n");
	for (int i = 0; i < f->S.nr_s; i++) {
		for (int j = 0; j < f->transferList[i].nr_charct; j++) {
			for (int k = 0; k < f->transferList[i].cList[j].nr_state; k++) {
				printf("%d->%c->%d  ", f->transferList[i].num, f->transferList[i].cList[j].c, f->transferList[i].cList[j].toState[k]);
			}
		}
		printf("\n");
	}
	

	printf("q:%d\n\n", f->q);

	printf("f:{");
	for (int i = 0; i < f->fs.nr_f; i++) {
		printf("%d ", f->fs._f[i]);
	}
	printf("}\n");
	
}

void test_newFA() {
	char a = 'a';
	FA *f = newFA(a);

	printFA(f);
}



enum {
	TK_OR = 255,
	TK_CONNECT = 256,
	TK_CLOSURE = 257,
	TK_C = 300
};


char re_str[1000];

void getRegularStr() {
	printf("(regular string):");
	scanf("%s", re_str);
}


typedef struct token {
	int type;
	char ctk;
}Token;

Token tokens[1000];
int nr_token;

bool is_character(char e) {
	return ((e >= 48 && e <= 57) || (e >= 65 && e <= 90) || (e >= 97 && e <= 122));
}

bool make_token(char *e) {
	int position = 0;
	nr_token = 0;

	while (e[position] != '\0') {
		if ( is_character(e[position]) ) {
			tokens[nr_token].type = TK_C;
			tokens[nr_token++].ctk = e[position];

			if (is_character(e[position + 1]) || e[position + 1] == '(' ){
				tokens[nr_token++].type = TK_CONNECT;
			}
		}

		else if (e[position] == '|') {
			tokens[nr_token++].type = TK_OR;
		}

		else if (e[position] == '*') {
			tokens[nr_token++].type = TK_CLOSURE;

			if (e[position + 1] == '(' || is_character(e[position + 1])) {
				tokens[nr_token++].type = TK_CONNECT;
			}
		}

		else if (e[position] == '(') {
			tokens[nr_token++].type = '(';
		}

		else if (e[position] == ')') {
			tokens[nr_token++].type = ')';

			if (is_character(e[position + 1]) || e[position + 1] == '(') {
				tokens[nr_token++].type = TK_CONNECT;
			}
		}

		else {
			return false;
		}

		++position;

	}

	return true;
}

void printToken() {
	for (int i = 0; i < nr_token; i++) {
		printf("%d ", tokens[i].type);
	}

	printf("\n");
}

/* 检查括号，用栈实现 */
static bool check_parentheses(int p, int q) {
	//printf ("enter check");
	if (tokens[p].type == '(' && tokens[q].type == ')') {
		int stack[50]; int i;
		int stack_pos = 0; int end_pos;
		for (i = p + 1, end_pos = q - 1; i <= end_pos; i++) {
			//printf ("i:%d\n", i);
			if (tokens[i].type == '(') { stack[++stack_pos] = tokens[i].type; /*printf ("1 ");*/ }
			else if (tokens[i].type == ')' && stack_pos > 0 && stack[stack_pos] == '(') { --stack_pos; /*printf ("2 ");*/ }
		}
		//printf ("%d\n",stack_pos);
		return stack_pos == 0;
	}

	return false;
}

bool is_operator(int token_type) {
	return token_type == TK_CLOSURE || token_type == TK_CONNECT || token_type == TK_OR;
}

int cmp_priority(int l, int r) {
	if (l > r) {
		return 1;
	}

	else {
		return -1;
	}
}

int find_the_position_of_dominant_operator(int p, int q) {
	int dom_op = -1; int level = 0;

	for (int i = p; i <= q; i++) {
		if (tokens[i].type == '(') { ++level; }
		else if (tokens[i].type == ')') { --level; }
		else if (level == 0 && is_operator(tokens[i].type)) {
			if (dom_op == -1) {
				dom_op = i;
			}
			else {
				int cmp = cmp_priority(tokens[i].type, tokens[dom_op].type);
				if (cmp <= 0) { dom_op = i; }
			}
		}
	}

	return dom_op;
}

FA *closureMerge(FA *f) {
	FA *rf = (FA *)malloc(sizeof(FA));

	initFA(rf);

	/*添加初态*/
	rf->transferList[0].num = nr_state++;
	rf->q = rf->transferList[0].num;					//
	rf->S.s[rf->S.nr_s++] = rf->transferList[0].num;

	for (int i = 0; i < f->S.nr_s; i++) {
		rf->transferList[rf->S.nr_s] = f->transferList[i];
		rf->S.s[rf->S.nr_s++] = f->S.s[i];
	}

	/*添加终态*/
	rf->transferList[rf->S.nr_s].num = nr_state++;
	rf->S.s[rf->S.nr_s++] = rf->transferList[rf->S.nr_s].num;
	rf->fs._f[rf->fs.nr_f++] = rf->S.s[rf->S.nr_s - 1];
	
	
	//添加转换关系
	rf->transferList[0].cList[rf->transferList[0].nr_charct].c = '#';
	rf->transferList[0].cList[rf->transferList[0].nr_charct].toState[0] = rf->S.s[1];
	rf->transferList[0].cList[rf->transferList[0].nr_charct].toState[1] = rf->S.s[rf->S.nr_s - 1];
	rf->transferList[0].cList[rf->transferList[0].nr_charct++].nr_state += 2;

	rf->transferList[rf->S.nr_s - 2].cList[rf->transferList[rf->S.nr_s - 2].nr_charct].c = '#';
	rf->transferList[rf->S.nr_s - 2].cList[rf->transferList[rf->S.nr_s - 2].nr_charct].toState[0] = rf->S.s[1];
	rf->transferList[rf->S.nr_s - 2].cList[rf->transferList[rf->S.nr_s - 2].nr_charct].toState[1] = rf->S.s[rf->S.nr_s - 1];
	rf->transferList[rf->S.nr_s - 2].cList[rf->transferList[rf->S.nr_s - 2].nr_charct++].nr_state += 2;

	//合并字符集
	for (int i = 0; i < f->ee.nr_e; i++) {
		rf->ee._e[rf->ee.nr_e++] = f->ee._e[i];
	}

	free(f);

	return rf;

}

bool notBelongTo(char a, char *c) {
	for (int i = 0; i < strlen(c); i++) {
		if (c[i] == a) {
			return 0;		//a belong to c[]
		}
	}

	return 1;		//a not belong to c[]
}

FA *connectMerge(FA *f1, FA *f2) {
	FA *rf = (FA *)malloc(sizeof(FA));

	initFA(rf);
	
	//拷贝f1状态和转换关系
	for (int i = 0; i < f1->S.nr_s; i++) {
		rf->transferList[rf->S.nr_s] = f1->transferList[i];			//状态转换表中添加该状态
		rf->S.s[rf->S.nr_s++] = f1->S.s[i];							//状态集中添加该状态
	}

	//拷贝f1=2状态和转换关系
	for (int i = 0; i < f2->S.nr_s; i++) {
		rf->transferList[rf->S.nr_s] = f2->transferList[i];			//状态转换表中添加该状态
		rf->S.s[rf->S.nr_s++] = f2->S.s[i];
	}

	//添加转换关系
	rf->transferList[f1->S.nr_s - 1].nr_charct = 0;													
	rf->transferList[f1->S.nr_s - 1].cList[rf->transferList[f1->S.nr_s - 1].nr_charct].nr_state = 0;	//rf状态表中的f1状态原转换关系清空(此处f1不代表FA f1)
	rf->transferList[f1->S.nr_s - 1].cList[rf->transferList[f1->S.nr_s - 1].nr_charct].c = '#';
	rf->transferList[f1->S.nr_s - 1].cList[rf->transferList[f1->S.nr_s - 1].nr_charct].toState[0] = f2->S.s[0];
	rf->transferList[f1->S.nr_s - 1].cList[rf->transferList[f1->S.nr_s - 1].nr_charct++].nr_state++;

	//设置初态和终态
	rf->q = f1->q;
	for (int i = 0; i < f2->fs.nr_f; i++) {
		rf->fs._f[rf->fs.nr_f++] = f2->fs._f[i];
	}

	//合并字符集
	for (int i = 0; i < f1->ee.nr_e; i++) {
		if (notBelongTo(f1->ee._e[i], f2->ee._e)) {
			rf->ee._e[rf->ee.nr_e++] = f1->ee._e[i];
		}
	}
	for (int i = 0; i < f2->ee.nr_e; i++) {
		rf->ee._e[rf->ee.nr_e++] = f2->ee._e[i];
	}

	free(f1); free(f2);
	
	return rf;
}

FA *orMerge(FA *f1, FA *f2) {
	FA *rf = (FA *)malloc(sizeof(FA));

	initFA(rf);

	/*添加初态*/
	rf->transferList[0].num = nr_state++;
	rf->q = rf->transferList[0].num;					//
	rf->S.s[rf->S.nr_s++] = rf->transferList[0].num;

	//拷贝中间态
	for (int i = 0; i < f1->S.nr_s; i++) {
		rf->transferList[rf->S.nr_s] = f1->transferList[i];
		rf->S.s[rf->S.nr_s++] = f1->S.s[i];
	}
	for (int i = 0; i < f2->S.nr_s; i++) {
		rf->transferList[rf->S.nr_s] = f2->transferList[i];
		rf->S.s[rf->S.nr_s++] = f2->S.s[i];
	}

	//添加终态
	rf->transferList[rf->S.nr_s].num = nr_state++;
	rf->S.s[rf->S.nr_s++] = rf->transferList[rf->S.nr_s].num;
	rf->fs._f[rf->fs.nr_f++] = rf->S.s[rf->S.nr_s - 1];

	//添加转换关系
	rf->transferList[0].cList[rf->transferList[0].nr_charct].c = '#';
	rf->transferList[0].cList[rf->transferList[0].nr_charct].toState[0] = f1->S.s[0];
	rf->transferList[0].cList[rf->transferList[0].nr_charct].toState[1] = f2->S.s[0];
	rf->transferList[0].cList[rf->transferList[0].nr_charct++].nr_state += 2;

	rf->transferList[f1->S.nr_s].cList[rf->transferList[f1->S.nr_s].nr_charct].c = '#';
	rf->transferList[f1->S.nr_s].cList[rf->transferList[f1->S.nr_s].nr_charct].toState[0] = rf->fs._f[rf->fs.nr_f - 1];
	rf->transferList[f1->S.nr_s].cList[rf->transferList[f1->S.nr_s].nr_charct++].nr_state++;

	rf->transferList[rf->S.nr_s - 2].cList[rf->transferList[rf->S.nr_s - 2].nr_charct].c = '#';
	rf->transferList[rf->S.nr_s - 2].cList[rf->transferList[rf->S.nr_s - 2].nr_charct].toState[0] = rf->fs._f[rf->fs.nr_f - 1];
	rf->transferList[rf->S.nr_s - 2].cList[rf->transferList[rf->S.nr_s - 2].nr_charct++].nr_state++;

	//合并字符集
	for (int i = 0; i < f1->ee.nr_e; i++) {
		if (notBelongTo(f1->ee._e[i], f2->ee._e)) {
			rf->ee._e[rf->ee.nr_e++] = f1->ee._e[i];
		}
	}
	for (int i = 0; i < f2->ee.nr_e; i++) {
		rf->ee._e[rf->ee.nr_e++] = f2->ee._e[i];
	}

	free(f1); free(f2);

	return rf;
}

FA *eval(int p, int q) {
	if (p > q) {
		exit(0);
	}

	else if (p == q) {
		if (tokens[p].type != TK_C) {
			exit(0);
		}
		return newFA(tokens[p].ctk);
	}

	else if (check_parentheses(p, q) == true) {
		return eval(p + 1, q - 1);
	}

	else {
		int op = find_the_position_of_dominant_operator(p, q);

		FA *f1 = eval(p, op - 1);
		if (tokens[op].type == TK_CLOSURE) {
			return closureMerge(f1);
		}

		FA *f2 = eval(op + 1, q);
		if (tokens[op].type == TK_OR) {
			return orMerge(f1, f2);
		}

		else if (tokens[op].type == TK_CONNECT) {
			return connectMerge(f1, f2);
		}


	}
}




