#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <math.h>

#define MIN_PER_RANK 3 /* Nodes/Rank: How 'fat' the DAG should be.  */
#define MAX_PER_RANK 7
#define MIN_RANKS 2    /* Ranks: How 'tall' the DAG should be.  */
#define MAX_RANKS 6
#define PERCENT 30     /* Chance of having an Edge.  */

int main (void)
{
  int i, j, k,nodes = 0, edg = 0;
  srand (time (NULL));
  FILE *fp = fopen("MyDAG7.txt","w+");
  int ranks = MIN_RANKS
              + (rand () % (MAX_RANKS - MIN_RANKS + 1));//取【MIN_RANKS，MAX_RANKS】之间的数 

  printf ("digraph {\n");
  for (i = 0; i < ranks; i++)
    {
      /* New nodes of 'higher' rank than all nodes generated till now.  */
      int new_nodes = MIN_PER_RANK
                      + (rand () % (MAX_PER_RANK - MIN_PER_RANK + 1));

      /* Edges from old nodes ('nodes') to new ones ('new_nodes').  */
      for (j = 0; j < nodes; j++){ 
        for (k = 0; k < new_nodes; k++){ 
          if ( (rand () % 100) < PERCENT){ 
           // printf ("  %d -> %d;\n", j, k + nodes); /* An Edge.  */
            edg++;
            fprintf(fp, "%d %d\n", j+1, k + nodes+1); 
		  }
		}
	  } 
      nodes += new_nodes; /* Accumulate into old node set.  */
    }
  fprintf(fp, "节点个数：%d 边个数：%d", nodes, edg); 
  printf ("}\n");
  printf("节点个数：%d 边个数：%d\n",nodes,edg);
//  printf("Card:%f\n",nodes*nodes*(1-exp(1-((double)edg/nodes))));
  fclose(fp);
  return 0;
}
