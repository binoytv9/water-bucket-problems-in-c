#include<stdio.h>
#include<stdlib.h>

struct queue{
	int newState[2];
	struct queue *next;
};

struct seen{
	int newState[2];
	int oldState[2];
	struct seen *next;
};

int min(int a,int b);
int max(int a,int b);
void printSol(struct seen *node);
int hasState(struct seen *sHead,int *newState);
void getParent(struct seen *sHead,int *newState);
void lastState(struct queue *qHead,int *newState);
struct seen *playGame(int aMax,int bMax,int goal);
void getState(struct queue **qHeadref,int *state);
void appendState(struct queue **qHeadref,int *newState);
struct seen *getSolution(struct seen *sHead,struct queue *qHead);
void pushState(struct seen **sHeadref,int *oldState,int *newState);
void addState(struct seen **sHeadref,struct queue **qHeadref,int *oldState,int *newState);
int test(struct seen **sHeadref,struct queue **qHeadref,int *oldState,int *newState,int goal);

main()
{
	int goal;
	int aMax,bMax;
	struct seen *solHead = NULL;

	printf("\n\nenter capacity of first bucket : ");
	scanf("%d",&aMax);
	printf("\nenter capacity of second bucket : ");
	scanf("%d",&bMax);
	printf("\nenter how much capacity u want to fill : ");
	scanf("%d",&goal);

	if(goal > max(aMax,bMax)){
		printf("\n\nerror : ur wanted capacity is much greater than the maximum capacity of the two bucket!!!\nexiting...\n\n");
		exit(0);
	}

	solHead = playGame(aMax,bMax,goal);

	printf("\n\nsolution is :\n");
	printSol(solHead);
}

int max(int a,int b)
{
	return a >= b ? a : b;
}

struct seen *playGame(int aMax,int bMax,int goal)
{
	int howmuch;
	int aHas,bHas;
	int newState[2] = {0,0};
	int oldState[2] = {-1,-1};
	struct seen *sHead = NULL;
	struct queue *qHead = NULL;

	addState(&sHead,&qHead,oldState,newState);
	while(1){
		getState(&qHead,oldState);
		aHas = oldState[0]; bHas = oldState[1];

		newState[0] = aMax; newState[1] = bHas;
		if(test(&sHead,&qHead,oldState,newState,goal))
			break;//fill A from well

		newState[0] = 0; newState[1] = bHas;
		if(test(&sHead,&qHead,oldState,newState,goal))
			break;//empty A to well

		newState[0] = aHas; newState[1] = bMax;
		if(test(&sHead,&qHead,oldState,newState,goal))
			break;//fill B from well

		newState[0] = aHas; newState[1] = 0;
		if(test(&sHead,&qHead,oldState,newState,goal))
			break;//empty B to well

		howmuch = min(aHas,bMax-bHas);
		newState[0] = aHas-howmuch; newState[1] = bHas+howmuch;
		if(test(&sHead,&qHead,oldState,newState,goal))
			break;//pour A to B
		
		howmuch = min(bHas,aMax-aHas);
		newState[0] = aHas+howmuch; newState[1] = bHas-howmuch;
		if(test(&sHead,&qHead,oldState,newState,goal))
			break;//pour B to A
	}
	return getSolution(sHead,qHead);
}

void addState(struct seen **sHeadref,struct queue **qHeadref,int *oldState,int *newState)
{
	if(hasState(*sHeadref,newState))
		return;
	pushState(sHeadref,oldState,newState);
	appendState(qHeadref,newState);
	//printf("--- Adding [%d,%d]\n", newState[0],newState[1]);
}

int hasState(struct seen *sHead,int *newState)
{
	struct seen *node = sHead;

	while(node != NULL){
		if( (node->newState[0] ==  newState[0]) && (node->newState[1] ==  newState[1]) )
			return 1;
		node = node->next;
	}
	return 0;
}

void pushState(struct seen **sHeadref,int *oldState,int *newState)
{
	struct seen *new = (struct seen *)malloc(sizeof(struct seen));

	new->newState[0] = newState[0];
	new->newState[1] = newState[1];

	new->oldState[0] = oldState[0];
	new->oldState[1] = oldState[1];

	new->next = *sHeadref;
	*sHeadref = new;
}

void appendState(struct queue **qHeadref,int *newState)
{
	struct queue *node = *qHeadref;
	struct queue *new = (struct queue *)malloc(sizeof(struct queue));

	new->newState[0] = newState[0];
	new->newState[1] = newState[1];
	new->next = NULL;

	if(node == NULL){
		*qHeadref = new;
		return;
	}
	while(node->next != NULL)
		node = node->next;
	node->next = new;
}

void getState(struct queue **qHeadref,int *state)
{
	struct queue *node = *qHeadref;

	if(node == NULL){
		printf("\n\nqueue should not be empty!!!\nexiting...\n\n");
		exit(0);
	}
	state[0] = node->newState[0];
	state[1] = node->newState[1];
	*qHeadref = node->next;
}

int test(struct seen **sHeadref,struct queue **qHeadref,int *oldState,int *newState,int goal)
{
	int won;

	won = (newState[0] == goal) || (newState[1] == goal);
	addState(sHeadref,qHeadref,oldState,newState);
	return won;
}

int min(int a,int b)
{
	return a >= b ? b : a;
}

struct seen *getSolution(struct seen *sHead,struct queue *qHead)
{
	struct seen *solHead = NULL;
	struct seen *node = sHead;
	int oldState[2] = {-1,-1};
	int newState[2] = {-1,-1};

	lastState(qHead,newState);

	while( (newState[0] != -1) && (newState[1] != -1) ){
		pushState(&solHead,oldState,newState);
		getParent(sHead,newState);
	}
	return solHead;
}

void lastState(struct queue *qHead,int *newState)
{
	while(qHead->next != NULL)
		qHead = qHead->next;

	newState[0] = qHead->newState[0];
	newState[1] = qHead->newState[1];
}

void getParent(struct seen *sHead,int *newState)
{
	struct seen *node = sHead;

	while(node != NULL){
		if( (node->newState[0] ==  newState[0]) && (node->newState[1] ==  newState[1]) )
			break;
		node = node->next;
	}
	newState[0] = node->oldState[0];
	newState[1] = node->oldState[1];
}

void printSol(struct seen *node)
{
	while(node != NULL){
		printf("\n\t[%d,%d]",node->newState[0],node->newState[1]);
		node = node->next;
	}
	printf("\n\n");
}
