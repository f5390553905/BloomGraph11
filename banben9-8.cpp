#include <bits/stdc++.h>
#include <sys/time.h>
//#include <cmph.h>
using namespace std;

#define BITPERWORD 32     //表示int所占的字节数  
#define SHIFT 5           //表示2^5=32位的int  
#define MASK 0x1f         //5位的掩码，表示二进制数1111

class Graph
{
    int V;    // No. of vertices
    int k; //mitmap  length 
	int array_len;  
    list<int> *t_adj;
 	int *reach_size;   
	int **bit_reach;   
	int *node;
	int *unumb;                                              
public:
	int UV;//No.unreach node
	int Est_UV;//No.unreach node
	double Plity;
	list<int> *adj;
	list<int> *adj1;
	vector<int> my_array;
    vector<int> indegree;// Vector to store indegree of vertices  
    vector<int> indegree1;
    
	Graph();
    void init(int V);
    void bit_set(int i, int j);
    void calcu(int *res);
    void addEdge(int v, int w);
    void beforeMycal();
    void topologicalSort(int location, int t_num, int *res, stack<int> &mystack);
    void topologicalSort1(int location, int t_num, int *res, stack<int> &mystack);
    void write_results();
    bool Ifreach(int i, int j, bool visited[]);    
    bool beforereach(int i, int j);  
};

Graph::Graph(){
}

void Graph::init(int V)
{
    this->V = V;
    this->UV = 0;
    array_len = 0;
    k =  (V-1) / BITPERWORD + 1; 
    adj = new list<int>[V];
    node = new int[V];
    unumb = new int[V];
    adj1 = new list<int>[V];
    t_adj = new list<int>[V];
    bit_reach = new int*[V];
	reach_size = new int[V];
	for (int i = 0; i < V; i++){
		reach_size[i] = 1;
		node[i] = 0;
		unumb[i] = 0;
		indegree.push_back(0);
		indegree1.push_back(0);
		bit_reach[i] = new int[k];
		for (int j = 0; j < k; j++){
			bit_reach[i][j] = 0;
		}
	}
        
}

void Graph::addEdge(int v, int w)
{
    adj[v].push_back(w); // Add w to v's list.
    adj1[v].push_back(w);
    // increasing inner degree of w by 1
    indegree[w]++;
    indegree1[w]++;
}

void Graph::bit_set(int i, int j){
	bit_reach[i][j >> SHIFT] |= (1 << (j&MASK));
}

bool Graph::Ifreach(int i, int j, bool visited[]){
	visited[i] = true;
	bool panduan = false;
	if (i == j){
		return true;
	}
	//judge according to the hash function
    //cout<<"this is phash"<<endl;
    int t_len = 0;
	for(int t = 1; t <= array_len; t++){
//		printf("%d  %d\n", my_array[i-1+t_len], my_array[j-1+t_len]);
		if(my_array[i-1+t_len] > my_array[j-1+t_len]) return false;
		t_len += (V-1);
	}

	list<int>:: iterator temp;
    	for (temp = adj[i].begin(); temp != adj[i].end(); temp++){
    		if(visited[*temp] ==  false)
    			panduan = (Ifreach(*temp, j, visited)||panduan);
		}
	return panduan;
}

bool Graph::beforereach(int i, int j){
	bool *vs = new bool[V];
	for(int i = 1; i < V; i++) vs[i] = false;
	return Ifreach(i, j, vs);
}

void Graph::topologicalSort1(int location, int t_num, int *res, stack<int> &mystack)
{
	int mynum = t_num;
	list<int>:: reverse_iterator j;
	res[location++] = mynum; 
	for(j = adj1[mynum].rbegin();  j != adj1[mynum].rend(); j++){
			indegree1[*j]--;
			if (indegree1[*j] == 0){
				node[*j] = 1;
				mystack.push(*j);
		}
		
	}
	while (!mystack.empty()) {	
		    int first = mystack.top();			
		//	printf("%d ",first);
			mystack.pop(); 
			res[location++] = first; 
			for(j = adj1[first].rbegin();  j != adj1[first].rend(); j++){
				indegree1[*j]--;
				if (indegree1[*j] == 0){
					mystack.push(*j);
			}
			node[*j] = node[*j] + node[first];
		}
	}	
}

void Graph::topologicalSort(int location, int t_num, int *res, stack<int> &mystack)
{
	int mynum = t_num;
	list<int>:: iterator j;
	res[location++] = mynum; 
	for (j = adj[mynum].begin(); j != adj[mynum].end(); j++){
			indegree[*j]--;
			if (indegree[*j] == 0){
				mystack.push(*j);
		}
	}

	while (!mystack.empty()) {	
	    int first = mystack.top();			
//		printf("%d ",first);
		mystack.pop(); 
		res[location++] = first; 
		for (j = adj[first].begin(); j != adj[first].end(); j++){
		//	cout<<*j<<endl;
			indegree[*j]--;
			if (indegree[*j] == 0){
				mystack.push(*j);
			}
		}	
	}	
}

void Graph::beforeMycal(){
	int location = 0;
	int *res = new int[V];
	stack<int> mystack;
	timeval start_at, end_at;
	gettimeofday(&start_at, 0);
	topologicalSort1(location, 0, res, mystack);
	topologicalSort(location, 0, res, mystack);
	gettimeofday(&end_at, 0);
	printf("index time(graph): %.8fs\n",
	     end_at.tv_sec - start_at.tv_sec +
	         double(end_at.tv_usec - start_at.tv_usec) / 1000000);
}

void read_graph(Graph *g){
	timeval start_at, end_at;
	gettimeofday(&start_at, 0);
	FILE *file = fopen("MyDAG.txt", "r");	
	int n;
	fscanf(file, "%d", &n);
	cout<<"节点个数："<<n<<endl;
	g->init(n+1);	
	int u, v;
	fgetc(file);
	int i = 0;
	while (!feof(file)){ 
	fscanf(file, "%d%d",&u, &v);
	g->addEdge(u, v); 
	}
	for (int i = 1; i < n+1; i++){
        if (g->indegree[i] == 0){
        	g->addEdge(0, i);
		}
	}
	g->Plity = 1;
//	printf("错误率：%f\n",	g->Plity);
	fclose(file);
	gettimeofday(&end_at, 0);
	printf("read time(graph): %.8fs\n",
	     end_at.tv_sec - start_at.tv_sec +
	         double(end_at.tv_usec - start_at.tv_usec) / 1000000);
}

void Graph::calcu(int *res){
	//	for(int i = 1; i < V; i++)	 printf("%d ",res[i]);
//	printf("\n");
	for (int i = 1; i < V; i++){
			int temp_i = res[i];
            for (int j = i+1; j < V; j++){          		
                    if ((bit_reach[res[j]][temp_i>>SHIFT] & (1 << (temp_i&MASK))) == 0){
                    		bit_set(res[j], res[i]);
                    	//	printf("*****%d,%d\n",arr[j],arr[i]);
							unumb[res[j]] = unumb[res[j]]+1;
							UV++;
                    }
            }
	}
}
void Graph::write_results(){
	list<int>:: iterator j;
	int location = 0;
	int *res = new int[V];
	stack<int> mystack;
	for (int i = 0; i < V; i++){
		node[i] = 0;
	}
	for(int i = 0; i <V ;i++){
		for (j = adj1[i].begin(); j != adj1[i].end(); j++){
				indegree1[*j]++;			
		}
	}
	for(int i = 0; i <V ;i++){
		for (j = adj[i].begin(); j != adj[i].end(); j++){
				indegree[*j]++;			
		}
	}
	topologicalSort1(location, 0, res, mystack);
	calcu(res);
	topologicalSort(location, 0, res, mystack);
	calcu(res);
	int num = 0;
	for (int i = 1; i < V; i++){
		if(node[i] == 1){
			num += (V-1-unumb[i]);
		} 
	}
	printf("answered only by label --> unreached:%d + reached:%d = %d\n",UV,num, num+UV);
	printf("rate:%f\n",1.0*(num+UV)/((V-1)*(V-1)));
}

int main()
{
  	Graph *g = new Graph();
  	
    read_graph(g);
	
	g->beforeMycal();
    
    g->write_results();
    
    return 0;
}








