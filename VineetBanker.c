// Libraries Used
#include<stdio.h>
#include<stdlib.h>
#include<pthread.h>
#include<unistd.h>
#include<stdbool.h>
#include<string.h>

// Defining number of Customers
#define NUM_CUSTOMERS 5
// Defining number of resources
#define NUM_RESOURCES 3

// Data structures required for Banker's Algorithm
int available[NUM_RESOURCES];
int tresources[NUM_RESOURCES];
int maximum[NUM_CUSTOMERS][NUM_RESOURCES];
int allocation[NUM_CUSTOMERS][NUM_RESOURCES];
int need[NUM_CUSTOMERS][NUM_RESOURCES];
int rank[NUM_CUSTOMERS];
int sequence[NUM_CUSTOMERS];
bool completed[NUM_CUSTOMERS];
int run=0;

int checked[NUM_CUSTOMERS];
// Pthread mutex
pthread_mutex_t lockResources;
pthread_cond_t waiting;
// Functions used in the code
void print();
void* function(void* );
void computeavailable();
bool request_res(int n_customer, int request[]);
void uncovered();
bool release_res(int n_customer, int release[]);


// Main function: this is the function that will execute first
int main(int argc, char**argv){
// checking if the required resources are passed in terminal
if( argc < NUM_RESOURCES ) {
printf("usage: %s config_file\n", argv[0]);
return -1;
}
// Converting the passed agguments to int type 
for(int i=1;i<argc;i++){
tresources[i-1]=atoi(argv[i]);
}

// Defining the threads array
pthread_t id[NUM_CUSTOMERS];

// Taking input
for(int i=0;i<NUM_CUSTOMERS;i++){
printf("Enter Max resources (r1 r2 r3) for P%d\n",i);
int maxinput=0;
while(maxinput<NUM_RESOURCES){
scanf(" %d",&maximum[i][maxinput]);
maxinput++;
}
}

for(int i=0;i<NUM_CUSTOMERS;i++){
int a[NUM_RESOURCES];
int allocinput=0;
printf("Enter Allocated resources (r1 r2 r3) for P%d\n",i);
while(allocinput<NUM_RESOURCES){
scanf("%d",&a[allocinput]);
if(a[allocinput]>maximum[i][allocinput]){
printf("Wrong Input!! Allocated resources can't be greater than maximum required. Please Enter a digits less than %d respectively\n",maximum[i][allocinput]);
scanf("%d",&a[allocinput]);
if(a[allocinput]>maximum[i][allocinput]){
printf("Wrong input again!! Try next time\n");
exit(-1);
}
}
allocation[i][allocinput]=a[allocinput];
need[i][allocinput]=maximum[i][allocinput]-allocation[i][allocinput];
allocinput++;
}
}


//This function will compute available resources based on allocated resources
computeavailable();
printf("\n");
for(int i=0;i<NUM_CUSTOMERS;i++){
completed[i]=false;
rank[i]=i;
checked[i]=3;
}
// This for loop is creating the threads

for(int i=0;i<NUM_CUSTOMERS;i++){
pthread_create(&id[i],NULL,function,(void*) &rank[i]);
}
// This while loop will join all the threads after completion
/*int tdjoin=0;
while(tdjoin<NUM_CUSTOMERS){
 pthread_join(id[tdjoin],NULL);
 tdjoin++;
}*/

for(int i=0;i<NUM_CUSTOMERS;i++){
pthread_join(id[i],NULL);
}
if(run==NUM_CUSTOMERS){
printf("All THE CUSTOMERS WERE ALLOCATED RESOURCES\n The safe sequence is : ");
for(int i=0;i<NUM_CUSTOMERS-1;i++){
printf("%d -> ",sequence[i]);
}
printf("%d\nHENCE SYSTEM IS SAFE\n",sequence[NUM_CUSTOMERS-1]);
}
// To exit from the code
exit(-1);
} 



//This function is for calculation of available resources
void computeavailable(){
int ra[NUM_RESOURCES];
for(int i=0;i<NUM_RESOURCES;i++){
ra[i]=0;

}
// Loop to calculate the available resources
for(int i=0;i<NUM_CUSTOMERS;i++){
int avail=0;
while(avail<NUM_RESOURCES){
ra[avail]+=allocation[i][avail];
avail++;
}
}

// Ckecking if the available resources are less than maximum resources
int avail=0;
while(avail<NUM_RESOURCES){
ra[avail]=tresources[avail]-ra[avail];

if(ra[avail]>=0){
available[avail]=ra[avail];
}
else{
printf("Total number of resources allocated for resource r%d is greater than total given resources. Please try again.\n",avail+1); 
}
avail++;
}
return;
}

// This is a predefined function from assingment instructions to check if the system will be in safe state or not after allocating resources
bool request_res(int n_customer, int request[]){
bool possible=true;
for(int i=0;i<NUM_RESOURCES;i++){
if(request[i]>available[i]){
possible=false;
}
}
return possible;
}



//This is also a predefined function from assingment instructions to release the resources
bool release_res(int n_customer, int release[]){
for(int i=0;i<NUM_RESOURCES;i++){
//printf("N_customer : %d , allocation %d , Avaialble : %d\n",n_customer,allocation[n_customer][i],available[i]);
available[i] += release[i];//allocation[n_customer][i];
allocation[n_customer][i]=0;
}
return true;

}



//After main this function is executed which is accessed by each thread directly
void* function(void *tid){
// mutex lock
pthread_mutex_lock(&lockResources);
int p = *((int *) tid);

// Checking safe or unsafe statement 
while(!request_res(p, need[p])){
printf("Requesting resources for %d\n",p);
int rs=0;
printf("Resources requested by %d: ",p);
while(rs<NUM_RESOURCES){
printf(" %d",need[p][rs]); 
rs++;
}
printf("\n");
printf("Available resources");
rs=0;
while(rs<NUM_RESOURCES){
printf(" %d",available[rs]); 
rs++;
}
printf("\n");
printf("Unsafe Mode!!\n");
printf("Request denied\n\n\n");

// Deadlock checking is done here
checked[p]=0;
bool deadlock=true;
for(int i=0;i<NUM_CUSTOMERS;i++){
if(checked[i]==1 || checked[i]==3){
deadlock=false;
}
}
if(deadlock){
printf("DEADLOCK\n");
exit(-1);
}
// unlocking mutex
pthread_mutex_unlock(&lockResources);
// making thread to wait until the desirable resources are available and there is a signal
pthread_cond_wait(&waiting, &lockResources);
}

// This is the main part of the banker algorithm
printf("Requesting resources for %d\n",p);
int rs=0;
printf("Resources requested by %d: ",p);
while(rs<NUM_RESOURCES){
printf(" %d",need[p][rs]); 
rs++;
}
printf("\n");
printf("Available resources");
rs=0;
// Printing available resources
while(rs<NUM_RESOURCES){
printf(" %d",available[rs]); 
rs++;
}
printf("\n");
printf("Safe Mode!!\n");
printf("Request accepted\n");
printf("Resources allocated to %d\n",p);

printf("Resources released by %d :",p);
int rl=0;
while(rl<NUM_RESOURCES){
int x=need[p][rl]+allocation[p][rl];
printf(" %d",x);
rl++;
}
printf("\n");
// Releasing resources
release_res(p, allocation[p]);
printf("Available resources after release");
rs=0;
while(rs<NUM_RESOURCES){
printf(" %d",available[rs]); 
rs++;
}
printf("\n\n\n");

// Updating values that will be used by other functions
rank[p]=-1;
completed[p]=true;
checked[p]=2;
sequence[run]=p;
run+=1;

for(int i=0;i<NUM_CUSTOMERS;i++){
if(!(checked[i]==2)){
checked[i]=3;
}
}

sleep(1);
// Broadcasting condition
pthread_cond_broadcast(&waiting);
//unlocking mutex
pthread_mutex_unlock(&lockResources);
}
