#include <bits/stdc++.h>
#include <sys/time.h>
//#include <cmph.h>
using namespace std;

#define BITPERWORD 32     //表示int所占的字节数  
#define SHIFT 5           //表示2^5=32位的int  
#define MASK 0x1f         //5位的掩码，表示二进制数1111
FILE *ffin ;
FILE *mytt;//记录每次拓扑的过滤数 
int jishu = 0;
int temp_jishu = -1;
int n = 0;
int tuopucishu = 1;
bool Phashtemp = true;

class Graph
{
    int V;    // No. of vertices
    int RV;//No.reach node
    int k; //mitmap  length 
    int phash;//NO.perfacthash function whith tuopu number 
	int array_len;  
    list<int> *t_adj;
 	int *reach_size; 
	int *unreach_size;
	int **bit_reach;  
	int vis;
	int **judge;
	int *node_vis;
	int *final;
       
    void alltopologicalSortUtil(int location, int  *res, bool visited[], bool& myflag);                                    
								   
	void My_alltopological(int *res, bool visited[],  int &mynum, int &flag);                                                    
public:
	int UV;//No.unreach node
	int Est_UV;//No.unreach node
	double Plity;
    int number_edges;
	list<int> *adj;
    vector<int> indegree;// Vector to store indegree of vertices  
    vector<int> my_array;
    Graph(int V);   // Constructor
    void bit_clear(int i, int j); 
    void bit_intersection(int i, int j);
    int bit_len(int n);
    void bit_set(int i, int j);

    // function to add an edge to graph
    void addEdge(int v, int w);

    // Prints all Topological Sorts
    void alltopologicalSort();
    //calculate unreach nodes
    //author by fanzhijun 2018.3.6
    void mycalculate(int node,  bool visited[], int &location, int *res);

    void beforeMycal();
    //calculate if Reached the stop condition
    //author by fanzhijun 2018.3.7
    bool ifstop();

    //generator a OMPHF file
    //author by fanzhijun 2018.3.7
    void Gphash(vector<int>& res);
       
    void BeforeGIlabel();
     //calculate if Reach
    //author by fanzhijun 2018.3.9
    bool Ifreach(int i, int j, bool visited[]);
    
    bool beforereach(int i, int j);   
     //calculate if Reach by using  prefect hash function
    //author by fanzhijun 2018.3.9
    bool Ifphase(int i, int j);   
    //calculate if Reach by using i-label
    //author by fanzhijun 2018.3.9
    bool Ifilabel(int i, int j);   
    //calculate if Reach by using i-label equal
    //author by fanzhijun 2018.3.9
    bool Ifilabelequal(int i, int j);
    
    void my_print(int *arr);
    void topologicalSort(int &location, int t_num, int *res);
    bool my_Pretreat(stack<int> &mystack, int t_num, int C_location);
    void my_PreRestore(int t_num);
    void topologicalSort(int location, int t_num, int *res, stack<int> &mystack);
};

Graph::Graph(int V)
{
    this->phash = 0;
    this->V = V;
    this->UV = 0;
    this->number_edges = 0;
    this->RV = 0;
    this->vis = 0;
    array_len = 0;
    k =  (V-1) / BITPERWORD + 1; 
    adj = new list<int>[V];
    t_adj = new list<int>[V];
    bit_reach = new int*[V];
    judge = new int*[V];
    node_vis = new int[V];
    final = new int[k];
    unreach_size = new int[V];
	reach_size = new int[V];
	for (int i = 0; i < V; i++){
		reach_size[i] = 1;
		indegree.push_back(0);
		node_vis[i] = 0;
		bit_reach[i] = new int[k];
		judge[i] = new int[k];
		for (int j = 0; j < k; j++){
			bit_reach[i][j] = 0;
			judge[i][j] = 0;
			final[j] = 0;
		}
	}
        
}

void Graph::addEdge(int v, int w)
{
    adj[v].push_back(w); // Add w to v's list.
    // increasing inner degree of w by 1
    indegree[w]++;
    number_edges++;
}

void Graph::bit_clear(int i, int j){
	bit_reach[i][j >> SHIFT] &= ~(1 << (j&MASK)); 
} 

void Graph::bit_intersection(int i, int j){
	for (int kk = 0; kk < k; kk++){
		bit_reach[i][kk] |= bit_reach[j][kk];
	}
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
	if((final[i >> SHIFT] & (1 << (i&MASK))) != 0 || (final[j >> SHIFT] & (1 << (j&MASK))) != 0){
		return true;
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
void Graph::my_print(int *arr){
//	for(int i = 1; i < V; i++){
//		printf("%d ",arr[i]);
//	}
	int tt = 0;
	array_len++;
	for (int i = 1; i < V; i++)	my_array.push_back(0);
	int lenth = (array_len-1)*(V-1);
	for (int i = 1; i < V; i++){
			int temp_i = arr[i];
			my_array[lenth+temp_i-1] = i;
            for (int j = i+1; j < V; j++){          		
                    if ((bit_reach[arr[j]][temp_i>>SHIFT] & (1 << (temp_i&MASK))) == 0){
                    		bit_set(arr[j], arr[i]);
                    	//	printf("*****%d,%d\n",arr[j],arr[i]);
                    		tt++;
							UV++;
							unreach_size[arr[j]]--;
                    }
            }
	}
	if(UV/(double)Est_UV >= Plity){
		Phashtemp = false;
	}
//	printf("过滤了：%d，总共过滤了：%d\n",tt, UV);
//	printf("******\n");
}

bool Graph::my_Pretreat(stack<int> &mystack, int t_num, int C_location){
	list<int>:: iterator j;
    bool flag = false;
    int t_temp = 0;
    for (j = adj[t_num].begin(); j != adj[t_num].end(); ){
    	int temp_num = *j;
    	indegree[temp_num]--;
    	if (indegree[*j] == 0){   					
    		if((final[temp_num >> SHIFT] & (1 << (temp_num&MASK))) != 0){
				t_adj[t_num].push_back(temp_num);
				adj[t_num].erase(j++);	
				indegree[temp_num]++;
				t_temp++;
    			continue;
			} 
			flag = true;
			mystack.push(temp_num);			
    	}
    	++j;
    }  
    if(!flag){
    	if(t_temp != 1){
	//		printf("%d全部过滤完！\n", t_num);
			if (t_num != 0){
				Est_UV = Est_UV - V + C_location + t_adj[t_num].size() + adj[t_num].size();
			}
			return false;   
		}else{
			for (j = t_adj[t_num].begin(); j != t_adj[t_num].end(); j++){
			int temp_num = *j;
	    	indegree[temp_num]--;
	    	if (indegree[temp_num] == 0){
				mystack.push(temp_num);
    	}
    }
    	return true;
		}
    }
	for (j = t_adj[t_num].begin(); j != t_adj[t_num].end(); j++){
		int temp_num = *j;
    	indegree[temp_num]--;
    	if (indegree[temp_num] == 0){
			mystack.push(temp_num);
    	}
    }
    t_adj[t_num].push_back(adj[t_num].front());
    adj[t_num].pop_front();
    return true;
}

void Graph::my_PreRestore(int t_num){
	list<int>:: iterator j;
	for (j = adj[t_num].begin(); j != adj[t_num].end(); j++){
        	indegree[*j]++;
	}
	for (j = t_adj[t_num].begin(); j != t_adj[t_num].end(); j++){
        	indegree[*j]++;
	}
}
void Graph::topologicalSort(int location, int t_num, int *res, stack<int> &mystack)
{
	int mynum = t_num;
	int t_location = location;
	final[mynum >> SHIFT] |= (1 << (mynum&MASK));
	res[location++] = mynum; 
 	if (location == V){
 		my_print(res);
 		return ;
 	}
 	while(true){
 		location = t_location + 1;
	 	if (mystack.empty()){
	 		if(!my_Pretreat(mystack, mynum, location)){
	 			break;
	 		}
	 	}
		while (!mystack.empty()) {	
		    int first = mystack.top();			
	//		printf("%d ",first);
			mystack.pop(); 
			if(mystack.empty()){	
				topologicalSort(location, first, res, mystack);
				if (!Phashtemp)return ;
				break;
			}
			res[location++] = first; 
			list<int>:: iterator j;
			for (j = adj[first].begin(); j != adj[first].end(); j++){
			//	cout<<*j<<endl;
				indegree[*j]--;
				if (indegree[*j] == 0){
					mystack.push(*j);
				}
			}
			for (j = t_adj[first].begin(); j != t_adj[first].end(); j++){
		//		cout<<*j<<endl;
				indegree[*j]--;
				if (indegree[*j] == 0){
					mystack.push(*j);
				}
			}
		}	
		for(int i = t_location ; i < location; i++){
					my_PreRestore(res[i]);
		}
	}
	 
}

void Graph::beforeMycal(){
	int location = 0;
	int *res = new int[V];
	stack<int> mystack;
	topologicalSort(location, 0, res, mystack);
	int number = my_array.size();
	int k = number / (V-1);
//	printf("%d**8\n",Est_UV);
//	printf("%d**8\n",k);
//	for (int i = 0; i < my_array.size(); i++){
//		printf("%d-",my_array[i]);
//	}
}

int main()
{
 //	mytt = fopen("rizhituopu4.txt","w+");
	timeval start_at, end_at;
	gettimeofday(&start_at, 0);
	FILE *file = fopen("MyDAG1.txt", "r");	
	int n;
	fscanf(file, "%d", &n);
	cout<<"节点个数："<<n<<endl;
	Graph *g = new Graph(n+1);
	int u, v;
	fgetc(file);
	int i = 0;
	while (!feof(file)){ 
	fscanf(file, "%d%d",&u, &v);
	g->addEdge(u, v); 
	}
	g->Est_UV = n*(n-1) - g->number_edges;
	for (int i = 1; i < n+1; i++){
        if (g->indegree[i] == 0){
        	g->addEdge(0, i);
		}
	}
	g->Plity = 0.6;
	printf("错误率：%f\n",	g->Plity);
	fclose(file);
	gettimeofday(&end_at, 0);
	printf("read time(graph): %.3fs\n",
	     end_at.tv_sec - start_at.tv_sec +
	         double(end_at.tv_usec - start_at.tv_usec) / 1000000);

    gettimeofday(&start_at, 0);
	g->beforeMycal();
	gettimeofday(&end_at, 0);
    printf("index time(graph): %.3fs\n",
         end_at.tv_sec - start_at.tv_sec +
             double(end_at.tv_usec - start_at.tv_usec) / 1000000);
	gettimeofday(&start_at, 0);
	printf("结果:%d\n",g->beforereach(23,1300));
	gettimeofday(&end_at, 0);
    printf("reach time(graph): %.3fms\n",
         end_at.tv_sec - start_at.tv_sec +
             double(end_at.tv_usec - start_at.tv_usec) / 1000000);
 	fclose(mytt);
    return 0;
}








