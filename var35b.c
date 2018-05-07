#include "malloc.h"
#include "string.h"
#include "stdio.h"
int size = 3;
const char *menu[] = { "1.Add to Parent Table","2.Add to Child Table", "3.Delete", "4.Show", "5.Exit" };
const int lenmenu = sizeof(menu) / sizeof(menu[0]);
FILE *fileP, *fileC, *filePI, *fileCI;

typedef struct Parent {
	int key, offset, len;
	Parent *next;
}Parent;

typedef struct Child {
	int key, pkey, offset, len;
	Child *next;
}Child;



int getInt(int *x)
{
	int k;
	do {
		k = scanf_s("%d", x, sizeof(int));
		if (k < 0) {
			printf("** EoF **\n");
			return 0;
		}
		if (!k)
		{
			printf("Error, repeat:  ");
			scanf_s("%*[^\n]");
		}
	} while (!k);
	scanf_s("%*[^\n]");
	scanf_s("%*c");
	return 1;
}

int addMemStr(char **p, int col, int flag)
{
	*p = flag ? (char *)calloc(col, sizeof(char)) : (char *)realloc(*p, col*sizeof(char));
	if (!*p)
	{
		printf("Not enough member!\n");
		return 0;
	}
	return 1;
}

int addElem(char **p, char *ch)
{
	int b = strlen(*p) + strlen(ch) + 1;
	if (!addMemStr(p, b, 0)) return 0;
	strcat_s(*p, b*sizeof(char), ch);
	return 1;
}

int getStr(char **sp)
{
	int k, fl = 1;
	char s[51], *s1 = NULL;
	if (!addMemStr(&s1, 1, 1)) return 0;
	do {
		k = scanf_s("%50[^\n]", s, 51);
		if (k > 0)
		{
			if (!addElem(&s1, s))
				return 0;
			continue;
		}
		if (k < 0)
		{
			free(s1);
			s1 = NULL;
			fl = 0;
			printf("** EoF **\n");
		}
		else scanf_s("%*c");
	} while (k > 0);
	*sp = s1;
	return fl;
}

int dialog(const char *menu[], int lenmenu)
{
	int num, i = 0;
	char *err = "";
	printf("\n");
	for (; i < lenmenu; i++)
		puts(menu[i]);
	printf("\nChoose task:  ");
	do {
		puts(err);
		if (!getInt(&num)) return 0;
		err = "Error, repeat:  ";
	} while (num<1 || num>lenmenu);
	return num;
}


int CreateTables(Parent ***ppt, Child ***tt)
{
	char *fnameP, *fnameC, *pr = "CHCHCH", *fnamePI, *fnameCI = NULL;
	Parent *last1 = NULL;
	Child *last2 = NULL;
	printf("Enter name of file for Table: ");
	if (!getStr(&fnameP)) return 0;
	int l = strlen(pr), l1 = strlen(fnameP);
	fnameC = (char*)malloc(l + l1 + 1);
	*fnameC = '\0';
	fnamePI = (char*)malloc(l1 + 2);
	*fnamePI = '\0';
	strncat_s(fnamePI, (l1 + 1)*sizeof(char), fnameP, l1 + 1);
	strncat_s(fnamePI, (strlen(fnamePI) + 2)*sizeof(char), "I", 1);
	strncat_s(fnameC, (l1 + 1)*sizeof(char), fnameP, l1);
	strncat_s(fnameC, (strlen(fnameC) + l + 1)*sizeof(char), pr, l);
	int l2 = strlen(fnameC);
	fnameCI = (char*)malloc(l2 + 2);
	*fnameCI = '\0';
	strncat_s(fnameCI, (l2 + 1)*sizeof(char), fnameC, l2);
	strncat_s(fnameCI, (strlen(fnameCI) + 2)*sizeof(char), "I", 1);
	fopen_s(&fileP, fnameP, "r+b");
	fopen_s(&fileC, fnameC, "r+b");
	if ((fileP) && (fileC))
	{
		printf("Using and opened old file\n");
		fread(&size, sizeof(int), 1, fileP);
		*ppt = (Parent**)calloc(1, sizeof(Parent));
		if (!*ppt)
		{
			printf("Not enough member\n");
			return 0;
		}
		int k = size;
		int i = 0, n = 0;
		while (--k >= 0)
		{
			fread(&n, sizeof(int), 1, fileP);
			*(*ppt + i) = NULL;
			while (--n >= 0)
			{
				Parent *dop = (Parent *)calloc(1, sizeof(Parent));
				if (!dop)
				{
					printf("Not enough member\n");
					return 0;
				}
				fread(&dop->key, sizeof(int), 1, fileP);
				fread(&dop->len, sizeof(int), 1, fileP);
				fread(&dop->offset, sizeof(int), 1, fileP);
				dop->next = NULL;
				if (!(*(*ppt + i)))*(*ppt + i) = dop;
				else last1->next = dop;
				last1 = dop;
			}
			i++;
		}
	}
	else
	{
		printf("Creating and opened new file\n");
		fopen_s(&fileP, fnameP, "w+b");
		*ppt = (Parent **)calloc(size, sizeof(Parent *));
		if (!(*ppt))
		{
			printf("Not enough member!\n");
			return 0;
		}
		for (int i = 0; i < size; i++)
			*(*ppt + i) = NULL;
	}
	if ((fileP) && (fileC))
	{
		int k1 = 0, n1 = 0, i1 = 0;
		fread(&size, sizeof(int), 1, fileC);
		*tt = (Child**)calloc(size, sizeof(Child*));
		if (!(*tt))
		{
			printf("Not enough member\n");
			return 0;
		}
		k1 = size;
		while (--k1 >= 0)
		{
			fread(&n1, sizeof(int), 1, fileC);
			*(*tt + i1) = NULL;
			while (--n1 >= 0)
			{
				Child *dop1 = (Child*)calloc(1, sizeof(Child));
				if (!dop1)
				{
					printf("Not enough member\n");
					return 0;
				}
				fread(&dop1->key, sizeof(int), 1, fileC);
				fread(&dop1->len, sizeof(int), 1, fileC);
				fread(&dop1->offset, sizeof(int), 1, fileC);
				fread(&dop1->pkey, sizeof(int), 1, fileC);
				dop1->next = NULL;
				if (!(*(*tt + i1))) *(*tt + i1) = dop1;
				else last2->next = dop1;
				last2 = dop1;
			}
			i1++;
		}
	}
	else
	{
		fopen_s(&fileC, fnameC, "w+b");
		*tt = (Child**)calloc(size, sizeof(Child*));
		if (!(*tt))
		{
			printf("Not enough member\n");
			return 0;
		}
		for (int i1 = 0; i1 < size; i1++)
			*(*tt + i1) = NULL;
	}
	fopen_s(&filePI, fnamePI, "r+b");
	if (!filePI) fopen_s(&filePI, fnamePI, "w+b");
	fopen_s(&fileCI, fnameCI, "r+b");
	if (!fileCI) fopen_s(&fileCI, fnameCI, "w+b");
	return 1;
}

int hashFun1(int k) {
	k = k>0 ? k : -k;
	return k%size;
}

int SearchP(Parent **pt, int f, int flag)
{
	char *str;
	Parent *list = pt[hashFun1(f)];
	while (list && list->key != f)
		list = list->next;
	if (!list) return 0;
	if (flag)
	{
		if (!addMemStr(&str, list->len, 1)) return 0;
		fseek(fileP, list->offset, SEEK_SET);
		fread(str, sizeof(char), list->len, fileP);
		printf("%5d		%s\n", list->key, str);
		free(str);
	}
	return 1;
}

int SearchC(Child **pt, int f, int f1, int flag)
{
	char *str;
	Child *list = pt[hashFun1(f)];
	while (list && list->key != f1)
		list = list->next;
	if (!list) return 0;
	return 1;
}

int SearchC1(Child **pt, int f)
{
	int t = 0;
	Child *list = pt[hashFun1(f)];
	while (list)
	{
		if (list->pkey == f)
			t++;
		list = list->next;
	}
	return t;
}

void insert1(Parent **pt, int f1, char *str1)
{
	Parent *dop = (Parent *)calloc(1, sizeof(Parent));
	if (!dop)
	{
		printf("Not enough member!\n");
		return;
	}
	int pos = hashFun1(f1);
	dop->len = strlen(str1) + 1;
	fseek(filePI, 0, SEEK_END);
	dop->offset = ftell(filePI);
	fwrite(str1, sizeof(char), dop->len, filePI);
	dop->key = f1;
	dop->next = pt[pos];
	pt[pos] = dop;
}

void insert2(Child **tt, int f1, int f2, char *str2)
{
	Child *dop1 = (Child *)calloc(1, sizeof(Child));
	if (!dop1)
	{
		printf("Not enough member!\n");
		return;
	}
	int pos1 = hashFun1(f1);
	dop1->len = strlen(str2) + 1;
	fseek(fileCI, 0, SEEK_END);
	dop1->offset = ftell(fileCI);
	fwrite(str2, sizeof(char), dop1->len, fileCI);
	dop1->key = f2;
	dop1->pkey = f1;
	dop1->next = tt[pos1];
	tt[pos1] = dop1;
}
int dAdd1(Parent **pt, Child **tt)
{
	int f1;
	char *str1 = NULL;
	printf("Enter key Parent: ");
	if (!getInt(&f1)) return 0;
	if (SearchP(pt, f1, 0))
		puts("Error, double keys");
	else {
		puts("Enter info Parent:");
		if (!getStr(&str1)) return 0;
		insert1(pt, f1, str1);
		free(str1);
	}
	return 1;
}

int dAdd2(Parent **pt, Child **tt)
{
	int f1, f2;
	char *str2;
	printf("Enter key Parent: ");
	if (!getInt(&f1)) return 0;
	if (SearchP(pt, f1, 0))
	{
		printf("Enter key Child:");
		if (!getInt(&f2)) return 0;
		if (SearchC(tt, f1, f2, 1)) {
			puts("Error, double keys");
		}
		else {
			printf("Enter info Child\n");
			if (!getStr(&str2)) return 0;
			insert2(tt, f1, f2, str2);
			free(str2);
		}
	}
	else printf("Key wasn't found in Parent");
	return 1;
}

void delete1(Child **pt, int f)
{
	Child *prev = pt[hashFun1(f)], *list = NULL;
	if (prev->pkey == f) {
		list = prev;
		pt[hashFun1(f)] = prev->next;
		free(list);
		return;
	}
	list = prev->next;
	while (list && list->pkey != f) {
		prev = list;
		list = list->next;
	}
	if (!list) {
		puts("Key not find");
		return;
	}
	prev->next = list->next;
	free(list);
}

void Delete(Parent **pt, Child**tt, int f)
{
	Parent *prev = pt[hashFun1(f)], *list = NULL;
	if (!prev) {
		puts("Key not find");
		return;
	}
	while (SearchC1(tt, f))
	{
		delete1(tt, f);
	}
	if (prev->key == f) {
		list = prev;
		pt[hashFun1(f)] = prev->next;
		free(list);
		return;
	}
	list = prev->next;
	while (list && list->key != f) {
		prev = list;
		list = list->next;
	}
	if (!list) {
		puts("Key not find");
		return;
	}
	prev->next = list->next;
	free(list);
}

int dDelete(Parent **pt, Child **tt)
{
	int f = 0;
	printf("Enter key: ");
	if (!getInt(&f)) return 0;
	Delete(pt, tt, f);
	return 1;
}

int dShow(Parent **pt, Child **tt)
{
	Parent *list = NULL;
	Child*list1 = NULL;
	char *str1, *str2;
	printf("\n");
	for (int i = 0; i < size; i++) {
		list = pt[i];
		while (list) {
			if (!addMemStr(&str1, list->len, 1)) return 0;
			fseek(filePI, list->offset, SEEK_SET);
			fread(str1, sizeof(char), list->len, filePI);
			printf("%5d   %s\n", list->key, str1);
			free(str1);
			list = list->next;
		}
	}
	for (int j = 0; j < size; j++)
	{
		list1 = tt[j];
		while (list1) {
			if (!addMemStr(&str2, list1->len, 1)) return 0;
			fseek(fileCI, list1->offset, SEEK_SET);
			fread(str2, sizeof(char), list1->len, fileCI);
			printf("%5d		%5d   %s\n", list1->pkey, list1->key, str2);
			free(str2);
			list1 = list1->next;
		}
	}
	return 1;
}

int NUL(Parent **y, Child **n) {
	return 0;
}
void writeTabInFile(Parent **pt, Child **tt)
{
	Parent *list = NULL;
	int n = 0, i = 0, pos = 0, posL = 0;
	fclose(filePI);
	fseek(fileP, 0, SEEK_SET);
	fwrite(&size, sizeof(int), 1, fileP);
	for (; i<size; i++, n = 0) {
		list = pt[i];
		pos = ftell(fileP);
		fseek(fileP, sizeof(int), SEEK_CUR);
		while (list) {
			n++;
			fwrite(&list->key, sizeof(int), 1, fileP);
			fwrite(&list->len, sizeof(int), 1, fileP);
			fwrite(&list->offset, sizeof(int), 1, fileP);
			list = list->next;
		}
		posL = ftell(fileP);
		fseek(fileP, pos, SEEK_SET);
		fwrite(&n, sizeof(int), 1, fileP);
		fseek(fileP, posL, SEEK_SET);
	}
	fclose(fileP);
	fclose(fileCI);
	Child *list1 = NULL;
	int n1 = 0, i1 = 0, pos1 = 0, posL1 = 0;
	fseek(fileC, 0, SEEK_SET);
	fwrite(&size, sizeof(int), 1, fileC);
	for (; i1<size; i1++, n1 = 0) {
		list1 = tt[i1];
		pos1 = ftell(fileC);
		fseek(fileC, sizeof(int), SEEK_CUR);
		while (list1) {
			n1++;
			fwrite(&list1->key, sizeof(int), 1, fileC);
			fwrite(&list1->len, sizeof(int), 1, fileC);
			fwrite(&list1->offset, sizeof(int), 1, fileC);
			fwrite(&list1->pkey, sizeof(int), 1, fileC);
			list1 = list1->next;
		}
		posL1 = ftell(fileC);
		fseek(fileC, pos1, SEEK_SET);
		fwrite(&n1, sizeof(int), 1, fileC);
		fseek(fileC, posL1, SEEK_SET);
	}
	fclose(fileC);
}
void vanishTable(Parent **pt, Child **tt)
{
	Parent *list = NULL;
	Child *list1 = NULL;
	for (int i = 0; i < size; i++) {
		while (pt[i]) {
			list = pt[i];
			pt[i] = list->next;
			free(list);
		}
		while (tt[i]) {
			list1 = tt[i];
			tt[i] = list1->next;
			free(list1);
		}
	}

	free(pt);
	free(tt);
}

int(*fun[])(Parent **pt, Child **tt) = { dAdd1, dAdd2, dDelete, dShow, NUL };

int main()
{
	int com;
	Parent **table1;
	Child **table2;
	if (!CreateTables(&table1, &table2)) return 0;
	while (com = dialog(menu, lenmenu))
		if (!fun[com - 1](table1, table2))
			break;
	puts("That is all");
	writeTabInFile(table1, table2);
	vanishTable(table1, table2);
	return 0;
}
