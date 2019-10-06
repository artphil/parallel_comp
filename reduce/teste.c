// https://gist.github.com/pajayrao/166bbeaf029012701f790b6943b31bb2

#include<mpi.h>
#include<stdio.h>
#include<stdlib.h>
void allocate(int a[],int n)
{
	int i;
	for(i=0;i<n;i++)
		a[i]=rand()%50;

}

int main(int argc, char**argv)
{
int rank,size,n,count,*a=NULL,*b=NULL,*c=NULL,*d=NULL,*e=NULL,*f=NULL,i;

MPI_Init(&argc,&argv);
MPI_Comm_size(MPI_COMM_WORLD,&size);
MPI_Comm_rank(MPI_COMM_WORLD,&rank);
n=atoi(argv[1]);
printf("Rank =%d n= %d",rank,n);
count=n/size;

d=(int*)malloc(count*sizeof(int));
e=(int*)malloc(count*sizeof(int));
f=(int*)malloc(count*sizeof(int));

if(rank ==0)
	{
	a=(int*)malloc(n*sizeof(int));
	b=(int*)malloc(n*sizeof(int));
	c=(int*)malloc(n*sizeof(int));
	allocate(a,n);
	allocate(b,n);	

}

MPI_Scatter(a,count,MPI_INT,d,count,MPI_INT,0,MPI_COMM_WORLD);
MPI_Scatter(b,count,MPI_INT,e,count,MPI_INT,0,MPI_COMM_WORLD);

for(i=0;i<count;i++)
	f[i]=d[i]+e[i];

MPI_Gather(f,count,MPI_INT,c,count,MPI_INT,0,MPI_COMM_WORLD);
if(rank ==0)
{
for(i=0;i<n;i++)
	printf("%d + %d = %d \n",a[i],b[i],c[i]);
}
MPI_Finalize();


}