#ifndef MYTBF_H__
#define MYTBF_H__
typedef void mytbf_s;
#define MYTBF_MAX 1024

typedef struct mytbf_st 
{
 	int cps;
	int burst;
	int token;
	int position;
}mytbf_st;


mytbf_st * mytbf_init(int cps, int burs);

int mytbf_fetchtoken(mytbf_st *, int);

int mytbf_returntoken(mytbf_st *,int );

int mytbf_desory(mytbf_st *);

void module_destory(void);
#endif

