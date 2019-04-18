#include <stdlib.h>
#include <stdio.h>
#include <assert.h>

// This template is only a guide 
// You need to include additional fields, data structures and functions  

// data type for heap nodes
typedef struct HeapNode { 
	// each node stores the priority (key), name, execution time,
	//  release time and deadline of one task
	int key; //key of this task
	int TaskName;  //task name 
	int Etime; //execution time of this task
	int Rtime; // release time of this task
	int Dline; // deadline of this task
	int degree;
	struct HeapNode *sibling;
	struct HeapNode *child;
} HeapNode;

//data type for a priority queue (heap) 
typedef struct BinomialHeap{ //this is heap header
	int  size;      // count of items in the heap
	HeapNode *head;
} BinomialHeap;

// create a new heap node to store an item (task) 
HeapNode *newHeapNode(int k, int n, int c, int r, int d)
{ 
	HeapNode *new;
	new = malloc(sizeof(HeapNode));
	assert(new != NULL);
	new->key = k;
	new->TaskName = n;
	new->Etime = c;
	new->Rtime = r; 
	new->Dline = d;
	new->degree = 0;
	new->child = NULL;
	new->sibling = NULL;
	return new;
}

// create a new empty heap-based priority queue
BinomialHeap *newHeap()
{ // this function creates an empty binomial heap-based priority queue
	BinomialHeap *T;
	T = malloc(sizeof(BinomialHeap));
	assert (T != NULL);
	T->size = 0;
	T->head = NULL;
	return T;
}

// Reverses the connection in the heap
// Used after deletion, as the child is always the node with highest degree
// Complexity: O(log n)
HeapNode *ReverseHeap(HeapNode *start){
	if(!start) return start;
	HeapNode *prev = NULL,*sibling = start->sibling;
	while(start && sibling){
		start->sibling = prev;
		prev = start;
		start = sibling;
		sibling = start->sibling;
	}
	start->sibling = prev;
	return start;
}

// Merges all the nodes to one heap
// Output may consist heap with nodes having same degree
//Complexity: O(log n + log m)
HeapNode *MergeHeap(HeapNode *h1, HeapNode *h2){
	if(!h1) return h2;
	if(!h2) return h1;
	HeapNode *head, *tail, *h1n = h1, *h2n = h2;
	if( h1->degree < h2->degree){
		head = h1;
		h1n = h1->sibling;
	} else {
		head = h2;
		h2n = h2 -> sibling;
	} 
	tail = head;
	while(h1n&&h2n){
		if(h1n->degree<h2n->degree || ((h1n->degree==h2n->degree)&&(h1n->TaskName<h2n->TaskName))){
			tail->sibling = h1n;
			h1n = h1n->sibling;
		} else {
			tail->sibling = h2n;
			h2n = h2n->sibling;
		}
		tail = tail-> sibling;
	}
	if(h1n)	tail->sibling = h1n;
	if(h2n)	tail->sibling = h2n;
	return head;
}

// MergeTree merges two Heapnodes following the min heap property 
// Complexity: Constant
HeapNode *MergeTree(HeapNode *h1, HeapNode *h2){
	if(!h1) return h2;
	if(!h2) return h1;
	if(h1->key<h2->key || ((h1->key==h2->key)&&(h1->TaskName<h2->TaskName))){
		h1->sibling = h2->sibling;
		h2->sibling = h1->child;
		h1->child = h2;
		h1->degree++;
		return h1;
	}
	h1->sibling = h2->child;
	h2->child = h1;
	h2->degree++;
	return h2;
}

// Finds and Merges nodes with same degree
// Complexity: O(log n)
void MakeValidHeap(BinomialHeap *T){
	HeapNode *root = T->head;
	HeapNode *sibling = root->sibling, *prev=NULL;
	while(root && sibling){
		if(root->degree == sibling->degree){
			root = MergeTree(root,sibling);
			if (prev == NULL) T->head = root;			
			sibling = root->sibling;
		} else {
			prev = root;
			root = sibling;
			sibling = root->sibling;
		}
		
	}
}

// Complexity: O(log n), Complexity of MakeValidHeap
void Insert(BinomialHeap *T, int k, int n, int c, int r, int d)
{ 
  //insert as B0 node 
  HeapNode *new = newHeapNode(k,n,c,r,d);
  new->sibling = T->head;
  T->head = new;
	//check whether other B0 node there
  //if yes merge these if no return
  MakeValidHeap(T);
	T->size++;
}

// Complexity: O(log n), as complexity of
// Finding Minimum: O(log n),
// Reverse Heap: O(log n),
// Merge Heap: O(log n),
// Make Valid Heap: O(log n)
HeapNode *RemoveMin(BinomialHeap *T)
{
	if(T->size<=0) return NULL;
	HeapNode *root = T->head, *sibling = root->sibling, *minNode = root, *prev=NULL;
	int minKey = root->key;
	while(root && sibling){
		if(sibling->key<minKey){
			minNode = sibling;
			prev = root;
			minKey = sibling->key;
		}
		root = sibling;
		sibling = root->sibling;
	}
	if (prev==NULL){
		T->head = minNode->sibling;
		BinomialHeap *temp = newHeap();
		temp->head = ReverseHeap(minNode->child);
		T->head = MergeHeap(T->head, temp->head);
		T->size--;
		return minNode;
	}
	prev->sibling = minNode->sibling;
	BinomialHeap *temp = newHeap();
	temp->head = ReverseHeap(minNode->child);
	T->head = MergeHeap(T->head, temp->head);
	MakeValidHeap(T);
	T->size--;
	return minNode;
}

//Complexity: O(log n), as we over only the root nodes of the trees
int Min(BinomialHeap *T)
{
  int minKey;
  HeapNode *root = T->head;
  minKey = root->key;
  while (root&&root->sibling){
	  minKey = (minKey <= root->sibling->key) ? minKey : root->sibling->key;
	  root = root->sibling;
  }
  return minKey;
}

// Auxilary function only for testing purpose
void Display(BinomialHeap *T){
	if(T==NULL) printf("EmptyHeap\n");
	HeapNode *node = T->head;
	printf("Heap:\t");
	while(node!=NULL){
		printf("%d,v%d:%d\t",node->key,node->TaskName,node->degree);
		node = node->sibling;
	}
	printf(":end\nsize = %d\n",T->size);
}

void freeNodes(HeapNode *n){
	if(n){
		freeNodes(n->child);
		freeNodes(n->sibling);
		free(n);
	}
}

void freeHeap(BinomialHeap *T){
	if(T->head){
		freeNodes(T->head);
	}
	free(T);
}


// Complexity: O(nlog n),
// We are operating on n tasks, and every operation takes 
// in worst case senario: constant * log n time.  
int TaskScheduler(char *f1, char *f2, int m )
{
	BinomialHeap *rel=newHeap(),
	 *de=newHeap(),
	 *sch=newHeap();
 	FILE *txf;
	int n, et, rt, d, i;
	txf = fopen(f1, "r");
	if(txf==NULL){
		printf("%s does not found.", f1);
		exit(EXIT_FAILURE);
	}
	while ((i=fscanf(txf,"%d%d%d%d",&n,&et,&rt,&d))!=EOF){
		if(i!=4){
			printf("input error when reading the attribute of task %d.",n);
			exit(EXIT_FAILURE);
		}
		Insert(rel,rt,n,et,rt,d);
	}
	fclose(txf);
	int leastReleaseTime = Min(rel);
	for (int i=m; i>=1; i--){//inserting nodes into the schedule point heap for each core
		Insert(sch,leastReleaseTime,i,0,0,0);
	}
	txf = fopen(f2,"w+");
	assert(txf!=NULL); 
	while(rel->size>0){
		while(rel->size>0 && Min(rel)<=leastReleaseTime){
			HeapNode *nextTask = RemoveMin(rel);
			if (nextTask) 
				Insert(de,nextTask->Dline,nextTask->TaskName,nextTask->Etime,nextTask->Rtime,nextTask->Dline);
		}
		if(de->size==0){
			leastReleaseTime = Min(rel);
			while(Min(sch)<leastReleaseTime){
				HeapNode *t = RemoveMin(sch);
				Insert(sch,leastReleaseTime,t->TaskName, 0,0,0);
			}
		}
		while(de->size>0){
			HeapNode *nextTask = RemoveMin(de), *nextavailablecore = RemoveMin(sch);
			if (nextTask && nextavailablecore){
				int taskends = nextavailablecore->key + nextTask->Etime;
				if(taskends > nextTask->Dline) {
					fclose(txf);
					freeHeap(rel);
					freeHeap(sch);
					freeHeap(de);
					return 0;
				}
				fprintf(txf,"%d Core%d %d ", nextTask->TaskName, nextavailablecore->TaskName, nextavailablecore->key);
				Insert(sch,taskends,nextavailablecore->TaskName, 0,0,0);
			}
			leastReleaseTime = Min(sch);
			while(rel->size>0 && Min(rel)<=leastReleaseTime){
			HeapNode *nextTask = RemoveMin(rel);
			if (nextTask) 
				Insert(de,nextTask->Dline,nextTask->TaskName,nextTask->Etime,nextTask->Rtime,nextTask->Dline);
			}
		}	
		leastReleaseTime = Min(sch);
	}
	fclose(txf);
	freeHeap(rel);
	freeHeap(sch);
	freeHeap(de);
	return 1;
}


int main() //sample main for testing 
{ int i;
  i=TaskScheduler("samplefile1.txt", "feasibleschedule1.txt", 4);
  if (i==0) printf("No feasible schedule!\n");
  /* There is a feasible schedule on 4 cores */
  i=TaskScheduler("samplefile1.txt", "feasibleschedule2.txt", 3);
  if (i==0) printf("No feasible schedule!\n");
  /* There is no feasible schedule on 3 cores */
  i=TaskScheduler("samplefile2.txt", "feasibleschedule3.txt", 5);
  if (i==0) printf("No feasible schedule!\n");
  /* There is a feasible schedule on 5 cores */
  i=TaskScheduler("samplefile2.txt", "feasibleschedule4.txt", 4);
  if (i==0) printf("No feasible schedule!\n");
  /* There is no feasible schedule on 4 cores */
  i=TaskScheduler("samplefile3.txt", "feasibleschedule5.txt", 2);
  if (i==0) printf("No feasible schedule!\n");
  /* There is no feasible schedule on 2 cores */
  i=TaskScheduler("samplefile4.txt", "feasibleschedule6.txt", 2);
  if (i==0) printf("No feasible schedule!\n");
  /* There is a feasible schedule on 2 cores */
 return 0; 
}
