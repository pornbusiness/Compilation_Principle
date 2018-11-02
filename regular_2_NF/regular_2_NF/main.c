#include "DF.h"

int main() {
	/*getRegularStr();

	make_token(re_str);

	printToken();*/

	//test_newFA();

	getRegularStr();
	make_token(re_str);

	printToken();
	FA *f = eval(0, nr_token - 1);

	printFA(f);

	/*IX *temp = buildIX(0);

	eClosure(f, f->q, temp->ces);
	temp->nr_ces++;

	printIX(temp);
	IX *ii = buildIX(0);
	IX *iix = buildIX(&f->ee);

	fillList(f, ii, iix);
	printList(ii, iix, f->ee.nr_e);

	free(ii);
	free(iix);
	free(f);*/

	FA *df = NF_2_DF(f);

	printFA(df);
	
	system("pause");

	return 0;
}