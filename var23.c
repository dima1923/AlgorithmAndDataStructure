// ConsoleApplication2.cpp: определяет точку входа для консольного приложения.
//

#include "stdafx.h"
#include "stdafx.h"
#include "string.h"
#include "locale.h"
#include "malloc.h"

struct item {
	char c;
	item * next;
};

item *dellist(item *);
void outlist(item * );
int getList(item **);
item * skipspace(item * );
item * add(item *, item *);
item *delWord(item *);
item * word(item ** );
int kol(item * );
int prover(item*, item *);
item * preobr(item *, item*);

int main(void)
{
	setlocale(LC_ALL, "");
	item *p = NULL, *r = NULL, *f = NULL;
	while (puts("Введите строку..."), getList(&p) && getList(&r)) {
		puts("Исходная строка:");
		outlist(p);
		outlist(r);
		p = preobr(p, r);
		puts("Конечная строка:");
		outlist(p);
		p = dellist(p);
		r = dellist(r);
	}
	puts("Закончено выполнение программы\n");
	return 0;
}

item *dellist(item *p)
{
	item *tmp = NULL;
	while (p != NULL) {
		tmp = p;
		p = p->next;
		free(tmp);
	}
	return p;
}

void outlist(item * p)
{
	printf("\"");
	while (p) {
		printf("%c", p->c);
		p = p->next;
	}
	printf("\"\n");
}

int getList(item **pptr)
{
	char buf[81], *str;
	item head = { '*', NULL };
	item *last = &head;
	int n, rc = 1;
	do {
		n = scanf_s("%80[^\n]", buf, 81);
		if (n < 0) {
			dellist(head.next);
			head.next = NULL;
			rc = 0;
			continue;
		}
		if (n > 0) {
			for (str = buf; *str != '\0'; ++str) {
				last->next = (item *)malloc(sizeof(item));
				last = last->next;
				last->c = *str;
			}
			last->next = NULL;
		}
		else
			scanf_s("%*c");
	} while (n > 0);
	*pptr = head.next;
	return rc;
}

item * skipspace(item * p)
{
	if (p) {
		while (p->next && (p->c == ' ' || p->c == '\t'))
			p = p->next;
	}
	if (p && (p->c == ' ' || p->c == '\t')) {
		item *tmp = p;
		p = NULL;
		free(tmp);
	}
	return p;
}

item * add(item * tto, item * ffrom)
{
	item * to = tto, *from = ffrom;
	if (to) {
		while (to->next)
			to = to->next;
		to->next = (item *)malloc(sizeof(item));
		to = to->next;
		to->c = ' ';
		to->next = from;
		return tto;
	}
	else
		return ffrom;
}

item *delWord(item *p)
{
	item *tmp;
	if (p) {
		while (p->next && p->c != ' ' && p->c != '\t') {
			tmp = p;
			p = p->next;
			free(tmp);
		}
	}
	if (p && p->c != ' ' && p->c != '\t') {
		item *tmp = p;
		p = NULL;
		free(tmp);
	}
	return p;
}

item * word(item ** p)
{
	item head = { '*',NULL };
	item * r = &head;
	item *b = *p;
	while (b->next && (b->next)->c != ' ' && (b->next)->c != '\t') {
		r->next = (item *)malloc(sizeof(item));
		r = r->next;
		r->c = b->c;
		b = b->next;
		r->next = NULL;
	}
	r->next = (item *)malloc(sizeof(item));
	r = r->next;
	r->c = b->c;
	r->next = NULL;
	b = b->next;
	*p = b;
	return head.next;
}

int kol(item * p)
{
	int k = 0;
	while (p->next && (p->c != ' ') && (p->c != '\t')) {
		k++;
		p = p->next;
	}
	if (p && (p->c != ' ') && (p->c != '\t'))
		k++;
	return k;
}

int prover(item*p, item *l)
{
	item z = { '\0',l };
	int t = 0;
	item *g = &z;
	g = skipspace(g->next);
	while (g = skipspace(g))
	{
		item *m = word(&g);
		int i = 0, k = kol(m);
		if (kol(m) == kol(p))
		{
			item *rt = p;
			while ((rt) && (m))
			{
				if (rt->c == m->c)
					++i;
				rt = rt->next;
				m = m->next;
			}
			if (i == k)
				t++;
		}
		g = delWord(g);
	}
	return t;
}

item * preobr(item * p, item*l)
{
	int k;
	item head = { '\0',p };
	item * tmp = &head, *r, *f = NULL;
	tmp = skipspace(tmp->next);
	while (tmp = skipspace(tmp))
	{
		r = word(&tmp);
		if (prover(r, l)) {
			f = add(f, r);
		}
		else
			tmp = delWord(tmp);
	}
	return f;
}