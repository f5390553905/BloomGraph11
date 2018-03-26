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
    int UV;//No.unreach node
    int RV;//No.reach node
    int k; //mitmap  length 
    int phash;//NO.perfacthash function whith tuopu number  
    list<int> *adj;// Pointer to an array containing adjacency list 
	int *reach_size; 
	int *unreach_size;
	int **bit_reach;  
    vector<int> indegree;// Vector to store indegree of vertices      

    void alltopologicalSortUtil(int location, int  *res,
                                bool visited[], bool& myflag);
                                     
								   
	void My_alltopological(int *res,
                                   bool visited[],  int &mynum, int &flag);                         
                                

	 

public:
    
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
    void mycalculate(int node,  bool visited[], set<int> *dp);

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
};

Graph::Graph(int V)
{
    this->phash = 0;
    this->V = V;
    this->UV = 0;
    this->RV = 0;
    k =  V / BITPERWORD + 1; 
    adj = new list<int>[V];
    bit_reach = new int*[V];
    unreach_size = new int[V];
	reach_size = new int[V];
	for (int i = 0; i < V; i++){
		reach_size[i] = 1;
		indegree.push_back(0);
		bit_reach[i] = new int[k];
		for (int j = 0; j < k; j++){
			bit_reach[i][j] = 0;
		}
	}
        
}

void Graph::addEdge(int v, int w)
{
    adj[v].push_back(w); // Add w to v's list.

    // increasing inner degree of w by 1
    indegree[w]++;
}


void Graph::alltopologicalSortUtil(int location, int *res,
                                   bool visited[],  bool& myflag)
{
	  if (location == V)
    {    	  	
		myflag = true;
        for (int i = 0; i < V; i++){
    		int temp_i = res[i];
            for (int j = i+1; j < V; j++){
                    if ((bit_reach[res[j]][temp_i>>SHIFT] & (1 << (temp_i&MASK))) == 0){
                    		bit_set(res[j], res[i]);
							unreach_size[j]--;
                    }
            }
	} 
        if(V%2==0) jishu = V/2*(V-1);
        else jishu = (V-1)/2*V;
		return ;
    }
    for (int i = 0; i < V; i++)
    {

        if (indegree[i] == 0 && !visited[i])
        {
            list<int>:: iterator j;
            for (j = adj[i].begin(); j != adj[i].end(); j++)
                indegree[*j]--;
            res[location] = i;
			visited[i] = true;
            alltopologicalSortUtil(location+1, res, visited,  myflag);
            visited[i] = false;
            for (j = adj[i].begin(); j != adj[i].end(); j++)
                indegree[*j]++;
			if (myflag) return;
        }
    } 
}

void Graph::My_alltopological(int *res,
                                   bool visited[],  int &mynum, int &flag){
                                   	
	int first;
	int location = 0;
	bool myflag = false, myflag1 = false;
	list<int>:: iterator j;
	stack<int> mystack;
	mystack.push(flag);
	visited[flag] = true;
	while(!mystack.empty()){
		first = mystack.top(); 
	//	cout<<"++++++"<<first;
		res[location++] = first; 
    	mystack.pop(); 
    	myflag = myflag1 = false; 
		for (j = adj[first].begin(); j != adj[first].end(); j++){
			indegree[*j]--;
		//	cout<<"*****"<<*j<<" ";
			if (indegree[*j] == 0 && !visited[*j] && *j != mynum){
				myflag = true;
				mystack.push(*j);
				visited[*j] = true;
			}
		}	
	if (!myflag){
		if(!mystack.empty())continue;
		for (int i = 0; i < V; i++){
			if (indegree[i] == 0 && !visited[i] && i != mynum){
				myflag1 = true;
				mystack.push(i);
				visited[i] = true;
				break;
		}
		}
		if(location == V)break;
//		cout<<"size:"<<res.size()<<endl;
		if(!myflag1){
			mystack.push(mynum);
			visited[mynum] = true;
		} 
		
		}
	}
//	cout<<"-------------------------------------------------"<<endl;
//	printf("第%d次拓扑相关信息：\n", tuopucishu++);
//	fprintf(mytt, "----------\n第%d次拓扑相关信息：\n", tuopucishu++); 
 //   for (int i = 0; i < V; i++)
  //      cout << res[i] << " ";
 //   cout << endl;
    int temp = 0;     
    for (int i = 0; i < V; i++){
    		int temp_i = res[i];
            for (int j = i+1; j < V; j++){
            	
         //   	cout<<bit_reach[res[i]][temp_j>>SHIFT]<<endl;
          //  	cout<<(1 << (temp_j&MASK))<<endl;
         //   	cout<<(bit_reach[res[i]][temp_j>>SHIFT] & (1 << (temp_j&MASK)))<<endl;
                    if ((bit_reach[res[j]][temp_i>>SHIFT] & (1 << (temp_i&MASK))) == 0){
                    		bit_set(res[j], res[i]);
                    //		cout<<res[j]<<"的大小为"<<unreach[res[j]].size()<<endl; 
							unreach_size[res[j]]--;
                            jishu++;
                            temp++;
                    }
            }
	}        
//    if (temp_jishu !=  jishu){
//    	temp_jishu = jishu;
//    	fprintf(mytt, "过滤了%d个点\n", temp); 
//    	fprintf(mytt, "jishu:%d\n", jishu); 
//    	fprintf(mytt, "UV:%d\n", UV); 
//    	fprintf(mytt, "过滤的百分比为:%f\n--------------", (double)jishu/UV); 
 //       cout<<"过滤了"<<temp<<"个点"<<endl;
//        cout<<"jishu:"<<jishu<<endl;
//        cout<<"UV:"<<UV<<endl;
//        cout<<"过滤的百分比为:"<<(double)jishu/UV<<endl;
//	}
//	cout<<"--------------------------------------------------"<<endl;
	return ;
		
}


void Graph::alltopologicalSort()
{
    // Mark all the vertices as not visited
    
    bool *visited = new bool[V];
    int *res = new int[V];
    for (int i = 0; i < V; i++)
        visited[i] = false;
	
  
    int mynum;
    bool flag = false;
    int xx = 2;
    printf("start tuopu!\n");
    timeval start_at, end_at;
    gettimeofday(&start_at, 0);
    alltopologicalSortUtil(0, res, visited, flag); 
  	flag = false;
    while(xx++<=6){
    	
    	if (xx == 1){
    		mynum = -1;
    		xx++;
    	}else{
    	int mmax = 0;
	    for (int i = 0; i < V; i++){
	    	if (unreach_size[i]*reach_size[i] > mmax){
	    		mmax = unreach_size[i]*reach_size[i];
//	    		printf("第%d个:%d\n",i,unreach_size[i]);
				mynum = i;
	   	    }
	    }		
		}
//	    cout<<mynum<<"***"<<endl;
	    
	 
	for (int i = 0; i < V; i++){
        if (indegree[i] == 0 && !visited[i] && i != mynum){
        	My_alltopological(res, visited, mynum, i);
        	break;
		}
	}
	    
        for (int i = 0; i < V; i++){        
            visited[i] = false;
    	}
    	for (int i = 0; i < V; i++){
    		list<int>:: iterator j;
    		for (j = adj[i].begin(); j != adj[i].end(); j++)
                indegree[*j]++;
    	}
        flag = false;
    	if (jishu == UV)break;
	}
	gettimeofday(&end_at, 0);
  	printf("index time: %.3fs\n",
         end_at.tv_sec - start_at.tv_sec +
             double(end_at.tv_usec - start_at.tv_usec) / 1000000);
    delete(visited);
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

int Graph::bit_len(int n){
	int count = 0;
	while (n != 0)
    {
        count++;
		n = n&(n-1); 
    }
    return count;
}
void Graph::mycalculate(int node,  bool visited[], set<int> *dp){
        if (visited[node]){
                return ;
        }
        else{
        visited[node] = true;
        if(adj[node].empty()){ 
			RV += 1;  
			unreach_size[node] = V - reach_size[node]; 	
            return ;
        }
        list<int>:: iterator j;
        for (j = adj[node].begin(); j != adj[node].end(); j++){   
			bit_set(node, *j);   			
        	mycalculate(*j, visited, dp);
        	bit_intersection(node , *j);
    	}
    	for (int i = 0; i < k; i++){
    		reach_size[node] += bit_len(bit_reach[node][i]);
    	   // printf("第%d个中第%d个:%d bit:%d\n",node, i, bit_reach[node][i],bit_len(bit_reach[node][i]));
		}
		RV += reach_size[node];
	//	printf("第%d个中第%d个\n", node, reach_size[node]);
		unreach_size[node] = V - reach_size[node]; 
    }
    return ;
}


void Graph::beforeMycal(){
	set<int> a[V];
    bool *visited = new bool[V];
    for (int i = 0; i < V; i++)
        visited[i] = false;
    int *dp = new int[V];
    for (int i = 0; i < V; i++)
    {
        if (indegree[i] == 0){
            mycalculate(i, visited, a);
        }
    }	
	UV = V*V - RV;
 //   cout<<"所有可达节点数："<<RV<<endl;
   // cout<<"非可达结点数为："<<UV<<endl;
    delete(visited);
    delete(dp);

}

bool Graph::ifstop(){
	n++;
	if(n == 7){
		Phashtemp = false;
    	return false;
	}
	return true;
}




bool Graph::Ifreach(int i, int j, bool visited[]){
	visited[i] = true;
	bool panduan = false;
	if (i == j){
		return true;
	}
	//judge according to the hash function
        cout<<"this is phash"<<endl;
/*	if (!Ifphase(i, j)){
		return false;
	}
	*/
	/*
	if ((ilabel[i].size() == ilabel[j].size())&&Ifilabelequal(i, j)){
		return true;
	}
	if (!Ifilabel(i, j)){
		return false;
	}
	*/
	list<int>:: iterator temp;
    	for (temp = adj[i].begin(); temp != adj[i].end(); temp++){
    		if(visited[*temp] ==  false)
    			panduan = (Ifreach(*temp, j, visited)||panduan);
		}
	return panduan;
}

int main()
{
    
 	mytt = fopen("rizhituopu4.txt","w+");
	timeval start_at, end_at;
	gettimeofday(&start_at, 0);
	FILE *file = fopen("MyDAG1.txt", "r");
	
	int n;
	fscanf(file, "%d", &n);
	cout<<"节点个数："<<n<<endl;
	Graph *g = new Graph(n);
	int u, v;
	fgetc(file);
	int i=0;
	while (!feof(file)){ 
	fscanf(file, "%d%d",&u, &v);
	g->addEdge(u, v); 
	}
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
	g->alltopologicalSort();
    //ffin = fopen("final.txt","w");
 //   cout<<"begin"<<endl;
//    cout<<g.beforereach(2,5)<<endl;
 	fclose(mytt);
    return 0;
}








