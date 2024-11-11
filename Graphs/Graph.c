//Graphs Functions
#include <stdlib.h>
#include <stdio.h>

typedef struct e{
	int dest;
	float w;
	struct e * next;
} Edge;

typedef struct {
	int val;
	Edge* adjList;
} Node;

typedef struct {
	int N;
	int E;
	Node* nodes;
} Graph;

typedef struct {
	int n1, n2;
	float w;
} CompleteEdge;

typedef struct n{
	int index;
	struct n * next;
} StackNode;

typedef StackNode* Stack;


void push(Stack* s, int n){
	StackNode* node=(StackNode*)malloc(sizeof(StackNode));
	if(node==NULL) exit(1);
	node->index=n;
	node->next=*s;
	*s=node;
}


int pop(Stack* s){
	if(*s==NULL)
		return -1;
	int res=(*s)->index;
	Stack temp=*s;
	*s=(*s)->next;
	free(temp);
	return res;
}


void insert(Edge** adjList,int dest,float weight){
	Edge* e=(Edge*)malloc(sizeof(Edge));
	e->dest=dest;
	e->w=weight;
	e->next=*adjList;
	*adjList=e;
}


Graph load(char* file){
	FILE* in = fopen(file,"r");
	if(in){
		Graph g;

		fscanf(in,"%d",&g.N);
		g.nodes=(Node*)malloc(g.N*sizeof(Node));

		for(int i=0;i<g.N;i++){
			fscanf(in,"%d",&g.nodes[i].val);
			g.nodes[i].adjList=NULL;
		}
		int n1,n2;
		float w;
		g.E=0;
		while(fscanf(in, "%d %d %f",&n1,&n2,&w)==3){
			insert(&g.nodes[n1].adjList,n2,w);
			insert(&g.nodes[n2].adjList,n1,w);
			g.E++;
		}
		return g;

	}
	else{
		perror("Open file: ");
		exit(1);
	}
}


void save(Graph g, char* file){
	FILE* out = fopen(file,"w");
	if(out){
		fprintf(out,"%d\n",g.N);
		for(int i=0;i<g.N;i++){
			fprintf(out,"%d ",g.nodes[i].val);
		}
		for(int i=0;i<g.N;i++){
			Edge* temp=g.nodes[i].adjList;
			while(temp){
				if(temp->dest>i)
					fprintf(out,"\n%d %d %f",i,temp->dest, temp->w);
				temp=temp->next;
			}
		}
		fclose(out);	
	}
    else{
		perror("Open file: ");
		exit(1);
	}
}


void visit(int n,int p,int* visited, int* result, int* visitedN){
	visited[n]=p;
	result[*visitedN]=n;
	*visitedN=*visitedN+1;
}


Graph bfs(Graph g, int start, int* nConComp){
	Graph result_forest = create_graph(g.N);
	int parent[g.N]; 
	int visitedN=0;
	
	for(int i=0;i<g.N;i++)
		if(i==start)
			parent[i]=i;
		else
			parent[i]=-1;
	
	Queue q;
	initQueue(&q);
	enqueue(&q,start);
	visitedN = visit(NULL, &result_forest, parent, visitedN);
	
	if(nConComp) 
		*nConComp=1;
	
	while(!isEmpty(q)){
		int v=dequeue(&q);
		Edge* e=g.nodes[v].adjList;
		while(e!=NULL){
			if(parent[e->dest]==-1){
				visitedN = visit(e,&result_forest,parent,visitedN);
				enqueue(&q,e->dest);
			}
			e=e->next;
		}
		if(isEmpty(q) && visitedN<g.N){
			int i=0;
			while(parent[i]!=-1)
				i++;
			visitedN = visit(NULL,&result_forest,parent,visitedN);
			enqueue(&q,i);
			if(nConComp) 
				(*nConComp)++;
		}
	}
	return result_forest;
}


int* dfs(Graph g, int start, int* hasCycles, int* nConComp){
	int prev[g.N];
	
	int* result=(int*)malloc(g.N*sizeof(int));
	int visitedN=0;

	for(int i=0;i<g.N;i++){
		prev[i]=-1;
	}
	
	Stack s=NULL;

	push(&s,start);
	visit(start, -2,prev,result,&visitedN);
	if(nConComp) *nConComp=1;
	
	if(hasCycles)
		*hasCycles=0;

	while(visitedN<g.N){
		int next=pop(&s);
		
		Edge* e=g.nodes[next].adjList;
		while(e!=NULL){
			if(prev[e->dest]==-1){
				visit(e->dest,next,prev,result,&visitedN);
				push(&s,e->dest);
			} 
            else{
				if(prev[next]!=e->dest)
					if(hasCycles) *hasCycles=1;
			}
			e=e->next;
		}
		if(s==NULL && visitedN<g.N){
			int i=0;
			while(prev[i]!=-1)
				i++;
			visit(i,-2,prev,result,&visitedN);
			push(&s,i);
			if(nConComp) (*nConComp)++;
		}
	}
	return result;
}


int hasCycles(Graph g){
	int cycles;
	int* temp=dfs(g,0,&cycles,NULL);
	free(temp);
	return cycles;
}


int countConnectedComponents(Graph g){
	int ncc;
	int* temp=dfs(g,0,NULL,&ncc);
	free(temp);
	return ncc;
}


int isTree(Graph g){
	int cycles,ncc;
	int* temp=dfs(g,0,&cycles,&ncc);
	free(temp);
	return (ncc==1 && !cycles);
}


void insertEdge(Graph g, CompleteEdge e){
	insert(&(g.nodes[e.n1].adjList), e.n2,e.w);
	insert(&(g.nodes[e.n2].adjList), e.n1,e.w);
}


void _remove(Edge** adjList, int n){
	if((*adjList)!=NULL){
		if((*adjList)->dest==n){
			Edge* temp=(*adjList);
			(*adjList)=temp->next;
			free(temp);
		} 
		else{
			_remove(&((*adjList)->next),n);
		}
	}
}


void removeEdge(Graph g, CompleteEdge e){
	_remove(&(g.nodes[e.n1].adjList),e.n2);
	_remove(&(g.nodes[e.n2].adjList),e.n1);	
}


int compareEdge(const void* e1,const void* e2){
	return (int)((CompleteEdge*)e1)->w-((CompleteEdge*)e2)->w;
}


Graph Kruskal(Graph g){
	Graph tree;
	tree.N=g.N;
	tree.E=0;
	tree.nodes=(Node*)malloc(g.N*sizeof(Node));
	if(tree.nodes==NULL){
		printf("Memory error");
		exit(1);
	}

	for(int i=0;i<tree.N;i++){
		tree.nodes[i].val=g.nodes[i].val;
		tree.nodes[i].adjList=NULL;
	}

	CompleteEdge edges[g.E];
	int dim=0;
	
	for(int i=0;i<g.N;i++){
		Edge* al=g.nodes[i].adjList;
		while(al!=NULL){
			if(i<al->dest){
				CompleteEdge e;
				e.n1=i;
				e.n2=al->dest;
				e.w=al->w;
				edges[dim]=e;
				dim++;
			}
			al=al->next;
		}
	}
	qsort(edges,g.E,sizeof(CompleteEdge),compareEdge);

	int e=0;
	while(tree.E<tree.N-1){
		insertEdge(tree,edges[e]);
		if (hasCycles(tree)){
			removeEdge(tree,edges[e]);
		} else {
			tree.E++;
		}
		e++;
	}
	return tree;	
}


Graph Dijkstra(Graph g, int source, int orientato){
	Graph result_ssp = create_graph(g.N);

	int Q[g.N];
	int num_q_element = 1;
	int parent[g.N];
	float path_w[g.N];
	float w[g.N];
	for(int i=0;i<result_ssp.N;i++){
		if(i==source){
			parent[source] = source;
			Q[source] = 1;
			path_w[source] = 0;
		}
		else{
			path_w[i] = FLT_MAX;
			parent[i] = -1;
			Q[i] = 0;
		}
	}

	while(num_q_element>0){
		int minimo_v = -1;
		float min_path_w = FLT_MAX;

		for (int i = 0; i < g.N; i++)
			if (Q[i] && (path_w[i] < min_path_w)){
				minimo_v = i;
				min_path_w = path_w[i];
			}

		Q[minimo_v] = 0;
		num_q_element--;

		if(parent[minimo_v]!=minimo_v){
			insert(&result_ssp, minimo_v, parent[minimo_v], w[minimo_v]);
			if(!orientato)
				insert(&result_ssp, parent[minimo_v], minimo_v, w[minimo_v]);
		}
		
		Edge* e=g.nodes[minimo_v].adjList;

		while(e!=NULL){
			int v_dest = e->dest;
			int new_path_w = path_w[minimo_v] + e->w;
			if (path_w[v_dest] > new_path_w)
			{
				parent[v_dest] = minimo_v;
				path_w[v_dest] = new_path_w;
				w[v_dest] =  e->w;

				if(Q[v_dest] == 0)
				{
					Q[v_dest] = 1;
					num_q_element++;
				}
			}
			e=e->next;
		}
	}
	return result_ssp;
}
