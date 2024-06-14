#include<stdio.h>
#include<stdlib.h>
#define Heap_Size 2048

char HeapMemory[Heap_Size];//heap memory

typedef struct Meta_data_tag
    {
    int size;
    int free;/* used to know if a memory is free or allocated 1 if free 0 if allocated*/
    struct Meta_data_tag * next;
} Meta_Data;


Meta_Data *hdr = (void *) HeapMemory;//pointer to heap memory


//initialising the metadata of the heap pointer
void initialize( Meta_Data *hdr)
 {
    hdr->size=Heap_Size-sizeof(Meta_Data);
    hdr->free=1;
    hdr->next=NULL;
}



void* Allocate(int bytes){
    Meta_Data*curr;
    void * result;
    curr=hdr;

    while(((curr->size<bytes)|| (curr->free==0 ))&& curr !=NULL){
      
        curr=curr->next;

    }
    if(curr->size==bytes){
        curr->free=0;
        result=(void *)curr + sizeof(Meta_Data);
    }
    else if(curr->size> bytes){

        if(curr->size > bytes +sizeof(Meta_Data)){
        
        Meta_Data *new;
        new=( void *)((void *)(curr) + sizeof(Meta_Data) + bytes);
       
        new->free=1;
        new->size=curr->size-bytes-sizeof(Meta_Data);
       
        curr->free=0;       
        curr->size=bytes;
        curr->next=new;
        result=(void *)curr +sizeof(Meta_Data);
    }
    }
    else if(curr==NULL){
        result=NULL;
        printf("Memory shortage can't allocate memory:\n");
    }

    return  result;
}

void merge(){
    Meta_Data*curr,*Next;
    int flag;
    curr=hdr;
    Next=curr->next;
    flag=0;

    while(flag==0 && Next!=NULL){

        if(curr->free==1&&Next->free==1){

            if(Next->next!=NULL && Next->next->free==1){

                curr->size=curr->size+2*sizeof(Meta_Data)+Next->size+(Next->next->size);
                curr->next=Next->next->next;
            }
            else{
                curr->size=curr->size + sizeof(Meta_Data) + Next->size;
                curr->next=Next->next;
            }
            flag=1;
        }
        else{
            curr=Next;
            Next=Next->next;
        }
    }

}
void freee(void * ptr){
    if(ptr>=(void *) hdr &&ptr <= ((void*)hdr+ Heap_Size)){
    Meta_Data *nptr = ptr - sizeof(Meta_Data);
    nptr->free=1;
    merge();
    printf("memory is now freed\n");
    }
    else{
        printf("Free called on unallocated memory\n ");
    }
}

void showHeap()
{
    
	Meta_Data *ptr;
    ptr = hdr;
	while(ptr != NULL)
	{
        if(ptr->free == 0)
        {
            printf("%16s\n", "ALLOCATED");
            printf("----------------------\n");
            printf("%10s %7d\n","Size: ",ptr->size);
            printf("----------------------\n");	
        }
        else
        {
            printf("%12s\n", "FREE");
            printf("----------------------\n");
            printf("%10s %7d\n","Size: ",ptr->size);
            printf("----------------------\n");
        }
        ptr = ptr->next;
	}
}
int main(){
    printf("%d",sizeof(Meta_Data));
 initialize(hdr);
 showHeap();
printf("_________________\n");
 void *z=Allocate(1);
 showHeap();
printf("_________________\n");

 void *y=Allocate(100);
 showHeap();
printf("_________________\n");

 freee(z);
 showHeap();
printf("_________________\n");

 freee(y);
 showHeap();
 return 0;
}