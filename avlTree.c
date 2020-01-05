#include <stdio.h>
#include <stdlib.h>

typedef struct node
{
	int key;
	int lvl;
	struct node *lft;
	struct node *rgt;
	struct node *fth;
}
node;

//nodo para balancear e pai do nodo
node *nblc = NULL;
node *nblcfth = NULL;

node *createNode(int key , int lvl , node *fth)
{
	node *n = malloc(sizeof(node));
	n->key = key;
	n->lvl = lvl;
	n->lft = NULL;
	n->rgt = NULL;
	n->fth = fth;

	nblc = n;
	nblcfth = fth;

	return n;
}

node *insert(node *n , int key , int lvl , node *fth)
{
	if(n == NULL)
		return createNode(key,lvl,fth);

	if(key < n->key)
		n->lft = insert(n->lft,key,lvl+1,n);
	else
		n->rgt = insert(n->rgt,key,lvl+1,n);
	
	return n;
}

//proximo nodo para excluir nodo com 2 filhos
node *nextNode(node *n)
{
	while(n->lft != NULL)
		n = n->lft;
	return n;
}

//diminui altura do nodo e subnodos
node *lowerLvl(node *n)
{
	if(n == NULL)
		return n;

	n->lvl--;
	n->lft = lowerLvl(n->lft);
	n->rgt = lowerLvl(n->rgt);
	return n;
}

//aumenta altura do nodo e subnodos
node *higherLvl(node *n)
{
	if(n == NULL)
		return n;

	n->lvl++;
	n->lft = higherLvl(n->lft);
	n->rgt = higherLvl(n->rgt);
	return n;
}

//remove nodo n
node *delete(node *n , int key)
{
	if(n == NULL)
		return n;

	if(key < n->key)
		n->lft = delete(n->lft,key);
	else if(key > n->key)
		n->rgt = delete(n->rgt,key);

	//achou chave
	else
	{
		//n tem 2 filhos
		if(n->lft != NULL && n->rgt != NULL)
		{
			node *next = nextNode(n->rgt);
			next->rgt = lowerLvl(next->rgt);

			//arrumar ponteiro do pai do next
			if(next == next->fth->lft)
				next->fth->lft = next->rgt;
			else
				next->fth->rgt = next->rgt;

			//arrumar ponteiro do filho do next
			if(next->rgt != NULL)
				next->rgt->fth = next->fth;

			nblc = next->rgt;
			nblcfth = next->fth;

			n->key = next->key;
			free(next);
			return n;
		}

		//n = folha
		if(n->lft == NULL && n->rgt == NULL)
		{
			nblc = NULL;
			nblcfth = n->fth;

			free(n);
			return NULL;
		}

		//nfth = pai do nodo a remover
		node *nfth = n->fth;

		//n so tem filho esquerdo
		if(n->lft != NULL)
		{
			n->lft = lowerLvl(n->lft);

			//n = raiz
			if(nfth == NULL)
			{
				nfth = n->lft;
				nfth->fth = NULL;

				nblc = nfth;
				nblcfth = NULL;

				free(n);
				return nfth;
			}
			else
			{
				//n = filho esquerdo
				if(n == nfth->lft)
				{
					nfth->lft = n->lft;
					nfth->lft->fth = nfth;

					nblc = nfth->lft;
					nblcfth = nfth;

					free(n);
					return nfth->lft;
				}
				//n = filho direito
				else
				{
					nfth->rgt = n->lft;
					nfth->rgt->fth = nfth;

					nblc = nfth->rgt;
					nblcfth = nfth;

					free(n);
					return nfth->rgt;
				}
			}
		}

		//n so tem filho direito
		n->rgt = lowerLvl(n->rgt);

		if(nfth == NULL)
		{
			nfth = n->rgt;
			nfth->fth = NULL;

			nblc = nfth;
			nblcfth = NULL;

			free(n);
			return nfth;
		}
		else
		{
			if(n == nfth->lft)
			{
				nfth->lft = n->rgt;
				nfth->lft->fth = nfth;

				nblc = nfth->lft;
				nblcfth = nfth;

				free(n);
				return nfth->lft;
			}
			else
			{
				nfth->rgt = n->rgt;
				nfth->rgt->fth = nfth;

				nblc = nfth->rgt;
				nblcfth = nfth;

				free(n);
				return nfth->rgt;
			}
		}
	}

	return n;
}

//retorna maior altura entre filho da esquerda e filho da direita
int maxHeight(int hlft , int hrgt)
{
	if(hlft > hrgt)
		return hlft;
	else
		return hrgt;
}

//retorna altura da subarvore
int height(node *n,int nfthlvl)
{
	if(n == NULL)
		return nfthlvl;

	return maxHeight(height(n->lft,n->lvl) , height(n->rgt,n->lvl));
}

//rotacao para a esquerda
node *rotateLft(node *n , node *nfth)
{
	//arrumando filho de n
	nfth->rgt = n->lft;
	if(nfth->rgt != NULL)
		nfth->rgt->fth = nfth;

	//arrumando n
	n->lft = nfth;
	n->fth = nfth->fth;
	nfth->fth = n;

	//arrumando pai
	if(n->fth != NULL)
	{
		if(nfth == n->fth->lft)
			n->fth->lft = n;
		else
			n->fth->rgt = n;
	}

	//diminuir lvl de n e filho direito de n
	n->lvl--;
	n->rgt = lowerLvl(n->rgt);

	//aumentar lvl de nfth e filho esquerdo de nfth
	nfth->lvl++;
	nfth->lft = higherLvl(nfth->lft);

	return n;
}

//rotacao para a direita
node *rotateRgt(node *n , node *nfth)
{
	nfth->lft = n->rgt;
	if(nfth->lft != NULL)
		nfth->lft->fth = nfth;

	n->rgt = nfth;
	n->fth = nfth->fth;
	nfth->fth = n;

	if(n->fth != NULL)
	{
		if(nfth == n->fth->lft)
			n->fth->lft = n;
		else
			n->fth->rgt = n;
	}

	n->lvl--;
	n->lft = lowerLvl(n->lft);

	nfth->lvl++;
	nfth->rgt = higherLvl(nfth->rgt);

	return n;
}

//equilibra a arvore
node *balance(node *n , node *nfth)
{

	//raiz
	if(nfth == NULL)
		return n;

	//altura dos filhos de nfth
	int hlft = height(nfth->lft,nfth->lvl);
	int hrgt = height(nfth->rgt,nfth->lvl);

	//checa se as duas subarvores estao balanceadas
	if(abs(hlft - hrgt) > 1)
	{
		//lado esquerdo maior
		if(hlft > hrgt)
		{
			n = nfth->lft;

			//altura dos filhos de n
			hlft = height(n->lft,n->lvl);
			hrgt = height(n->rgt,n->lvl);

			//zigzag
			if(hrgt > hlft)
				n = rotateLft(n->rgt,n);

			//zigzig
			nfth = rotateRgt(n,nfth);
		}
		//lado direito maior
		else
		{
			n = nfth->rgt;

			hlft = height(n->lft,n->lvl);
			hrgt = height(n->rgt,n->lvl);

			if(hlft > hrgt)
				n = rotateRgt(n->lft,n);

			nfth = rotateLft(n,nfth);
		}
	}
	
	//subarvore ja esta balanceada, manda o problema pra cima
	return balance(nfth , nfth->fth);
}

int start = 1;
//print da arvore
void inOrder(node *n)
{
	if(n == NULL)
		return;

	inOrder(n->lft);
	if(start)
	{
		printf("%d,%d" , n->key,n->lvl);
		start = 0;
	}
	else
	{
		puts("");
		printf("%d,%d" , n->key,n->lvl);
	}
	inOrder(n->rgt);
}

int main()
{
	node *tree = NULL;
	char option;
	int key;

	//le todos os comandos e poe na arvore
	while(scanf("%c %d" , &option,&key) != EOF)
	{
		switch(option)
		{
			case('i'):
				tree = insert(tree,key,0,NULL);
				break;

			case('r'):
				tree = delete(tree,key);
		}

		//balanceia avl apos cada operacao
		tree = balance(nblc,nblcfth);
	}

	inOrder(tree);
}
