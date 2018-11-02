#pragma once
#include "NF.h"

#define LIST_SIZE 100

typedef struct es {
	int num;
	State st;
}ES;

typedef struct ix {
	char x;
	int nr_ces;
	ES ces[LIST_SIZE];
}IX;

/*typedef struct _I {
	int nr_IS;
	ES IS[LIST_SIZE];
}I;

I *newI() {
	I *ni = (I *)malloc(sizeof(I));
	ni->nr_IS = 0;

	for (int i = 0; i < LIST_SIZE; i++) {
		ni->IS[i].st.nr_s = 0;
	}
	
	return ni;
}*/


IX *buildIX(E *ee) {
	if (!ee) {
		IX *ret = (IX *)malloc(sizeof(IX));
		ret->x = 0;
		ret->nr_ces = 0;
		for (int i = 0; i < LIST_SIZE; i++) {
			ret->ces[i].st.nr_s = 0;
		}
		return ret;
	}
	IX *ret = (IX *)malloc(sizeof(IX) * ee->nr_e);
	for (int i = 0; i < ee->nr_e; i++) {
		ret[i].x = ee->_e[i];
		ret[i].nr_ces = 0;
		for (int j = 0; j < LIST_SIZE; j++) {
			ret[i].ces[j].st.nr_s = 0;
		}
	}

	return ret;
}

void printIX(IX *pi) {
	printf("I%c\n", pi->x);
	for (int i = 0; i < pi->nr_ces; i++) {
		for (int j = 0; j < pi->ces[i].st.nr_s; j++) {
			printf("%d ", pi->ces[i].st.s[j]);
		}
		printf("----%d\n", pi->ces[i].num);
	}
}



int eClosure(FA *f, int state, ES *eSet) {
	int position = 0; int i;
	//´ÓfÖÐÕÒµ½×´Ì¬state
	for (i = 0; i < f->S.nr_s; i++) {
		if (f->S.s[i] == state) {
			break;
		}
	}
	if (i == f->S.nr_s) {
		printf("eClosure--Exit!!!!!!!\n");
		exit(0);
	}
	position = i;

	for (i = 0; i < eSet->st.nr_s; i++) {
		if (eSet->st.s[i] == f->transferList[position].num) {
			return 0;
		}
	}
	eSet->st.s[eSet->st.nr_s++] = f->transferList[position].num;
	for (i = 0; i < f->transferList[position].nr_charct; i++) {
		if (f->transferList[position].cList[i].c == '#') {
			for (int j = 0; j < f->transferList[position].cList[i].nr_state; j++) {
				eClosure(f, f->transferList[position].cList[i].toState[j], eSet);
			}
		}
	}

	return 0;
}

int J(FA *f, ES *a, char c, int *b) {
	int position = 0; int i, j;
	int count = 0;
	for (i = 0; i < a->st.nr_s; i++) {
		for (j = 0; j < f->S.nr_s; j++) {
			if (f->S.s[j] == a->st.s[i]) {
				break;
			}
		}
		if (j == f->S.nr_s) {
			printf("J--Exit!!!!!!!\n");
			exit(0);
		}
		position = j;

		for (int k = 0; k < f->transferList[position].nr_charct; k++) {
			if (f->transferList[position].cList[k].c == c) {
				for (int m = 0; m < f->transferList[position].cList[k].nr_state; m++) {
					b[count++] = f->transferList[position].cList[k].toState[m];
				}
			}
		}
	}

	return count;
}

bool stQual(State *a, State *b) {
	if (a->nr_s != b->nr_s) {
		return false;
	}
	for (int i = 0; i < a->nr_s; i++) {
		if (a->s[i] != b->s[i]) {
			return false;
		}
	}

	return true;
}

bool has_same_cstate(IX *ii, ES *es) {
	for (int i = 0; i < ii->nr_ces; i++) {
		if (stQual(&ii->ces[i].st, &es->st)) {
			es->num = ii->ces[i].num;
			return true;
		}
	}
	es->num = nr_state++;
	return false;
}

void fillList(FA *f, IX *ii, IX *iix) {
	int ii_ptr = 0;  
	int arr[100];
	int n_arr;
	eClosure(f, f->q, &ii->ces[ii->nr_ces]);
	ii->ces[ii->nr_ces++].num = nr_state++;
	
	for(ii_ptr = 0; ii_ptr < ii->nr_ces; ii_ptr++){
		for (int i = 0; i < f->ee.nr_e; i++) {
			n_arr = J(f, &ii->ces[ii_ptr], iix[i].x, arr);
			for (int j = 0; j < n_arr; j++) {
				eClosure(f, arr[j], &iix[i].ces[iix[i].nr_ces]);
			}
			if (!has_same_cstate(ii, &iix[i].ces[iix[i].nr_ces])) {
				ii->ces[ii->nr_ces++] = iix[i].ces[iix[i].nr_ces];
			}
			iix[i].nr_ces++;
		}
	}
	
}

void printList(IX *ii, IX *iix, int iix_n) {
	printIX(ii);
	printf("\n");
	for (int i = 0; i < iix_n; i++) {
		printIX(iix + i);
		putchar('\n');
	}
	
}

FA *NF_2_DF(FA *f) {
	IX *ii = buildIX(0);
	IX *iix = buildIX(&f->ee);

	fillList(f, ii, iix);
	printList(ii, iix, f->ee.nr_e);

	FA *df = (FA *)malloc(sizeof(FA));
	initFA(df);

	for (int i = 0; i < ii->nr_ces; i++) {
		df->S.s[df->S.nr_s++] = ii->ces[i].num;
	}
	for (int i = 0; i < f->ee.nr_e; i++) {
		df->ee._e[df->ee.nr_e++] = f->ee._e[i];
	}
	for (int i = 0; i < df->S.nr_s; i++) {
		df->transferList[i].num = df->S.s[i];
		for (int j = 0; j < df->ee.nr_e; j++) {
			df->transferList[i].cList[df->transferList[i].nr_charct].c = df->ee._e[j];
			df->transferList[i].cList[df->transferList[i].nr_charct].toState[df->transferList[i].cList[df->transferList[i].nr_charct++].nr_state++] = iix[j].ces[i].num;
		}
	}

	df->q = ii->ces[0].num;

	for (int i = 0; i < ii->nr_ces; i++) {
		for (int j = 0; j < ii->ces[i].st.nr_s; j++) {
			if (ii->ces[i].st.s[j] == f->fs._f[0]) {
				df->fs._f[df->fs.nr_f++] = ii->ces[i].num;
				break;
			}
		}
	}

	free(ii); free(iix);

	return df;
}