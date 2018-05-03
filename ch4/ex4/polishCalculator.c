#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>		/* for atof() */

#define MAXOP 100
#define NUMBER '0'		/* Signal that a number was found */

#define MAXVAL 100
int sp = 0;				/* next free stack position */
double val[MAXVAL];		/* value stack */

#define BUFSIZE 100
char buf[BUFSIZE];	/* buffer for ungetch */
int bufp = 0;		/* next free position in buf */

int getch(void);
void ungetch(int);
int getop(char s[]);
void push(double f);
double pop(void);

int main()
{
	int type;
	double op2;
	char s[MAXOP];

	while ((type = getop(s)) != EOF) {
		switch (type) {
		case NUMBER:
			push(atof(s));
			break;
		case '+':
			push(pop() + pop());
			break;
		case '*':
			push(pop() * pop());
			break;
		case '-':
			op2 = pop();
			push(pop() - op2);
			break;
		case '/':
			op2 = pop();
			if(op2 != 0) {
				push(pop() / op2);
			} else {
				printf("error: zero divisor\n");
			}
			break;
		case '\n':
			printf("\t%.8g\n", pop());
			break;
		default:
			printf("error: unknown command %s\n", s);
			break;
		}
	}
	return 0;
}

/* getop: get the next operator or numeric operand */
int getop(char s[])
{
	int i, c;

	while((s[0] = c = getch()) == ' ' || c == '\t') {
		;
	}
	s[1] = '\0';
	if(!isdigit(c) && c != '.') {
		return c;	/* not a number */
	}

	i = 0;
	if (isdigit(c)) {	/* collect integer part */
		while (isdigit(s[++i] = c = getch())) {
			;
		}
	}

	if (c == '.') {		/* collect fraction part */
		while (isdigit(s[++i] = c = getch())){
			;
		}
	}

	s[i] = '\0';
	if (c != EOF){
		ungetch(c);
	}

	return NUMBER;
}

/* getch: get a (possibly pushed back) character */
int getch(void)
{
	return (bufp > 0) ? buf[--bufp] : getchar();
}

/* push character back on input */
void ungetch(int c)
{
	if (bufp >= BUFSIZE) {
		printf("ungetch: too many characters\n");
	} else {
		buf[bufp++] = c;
	}
}

/* push: push f onto the value stack */
void push(double f)
{
	if (sp < MAXVAL) {
		val[sp++] = f;
	} else {
		printf("error: stack full, cant push %g\n", f);
	}
}

/* pop: pop and return top value from stack */
double pop(void)
{
	if (sp > 0) {
		return val[--sp];
	} else {
		printf("error: stack empty\n");
		return 0.0;
	}
}

/* printTopElement: prints the top element without popping */
void printTopElement(void) {
	if (sp > 0) {
		printf ("%f", val[sp - 1]);
	} else {
		printf("error: stack is empty");
	}
}

void duplicate(void) {
	if (sp > 0) {
		double top = pop();
		push(top);
		push(top);
	} else {
		printf("error: stack is empty");
	}
}

void swapTop(void) {
	if (sp > 1) {
		double first = pop();
		double second = pop();
		push(first);
		push(second);
	} else {
		printf("error: not enough elements to swap");
	}
}

void clearStack (void) {
	sp = 0;
}
