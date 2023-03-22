#include<stdio.h>
#include<stdlib.h>
#include<math.h>
#include<string.h>
#include<time.h>
#define _POSIX_C_SOURCE 199309L

typedef struct SLL {
	int data;
	struct SLL *next;
}node;

node *start = NULL;
int count = 0;
long tri[5][10];
const char *names[5] = {"Linear Search", "Sentinel Search", "Binary Search", "Jump Search", "Exponential Srch"};
const char *line = "______|_________________|_________________|_________________|_________________|_________________|";

void insert(int value) {
	node* temp = (node*)malloc(sizeof(node));
	temp->data = value;
    temp->next = NULL;
    count++;
    if (start == NULL) {
        start = temp;
		return;
    }
    if(start->data >= temp->data) {
        temp->next = start;
        start = temp;
        return;
    }
	node* cur = start;
    while (cur->next != NULL && (cur->next)->data < temp->data) 
        cur = cur->next;
    temp->next = cur->next;
    cur->next = temp;
    return;
}

void display() {
    if(start == NULL) {
        printf("\n\nList empty\n");
        return;
    }
    node *temp = start;
    printf("\n\nSLL is: ");
    while(temp != NULL) {
        printf("%d -> ",temp->data);
        temp = temp->next;
    }
    printf("NULL");
}

node* linSrch(int value) {
    if(start == NULL)
        return NULL;
    node *temp = start;
    while(temp != NULL) {
        if(temp->data == value)
            return temp;
        temp = temp->next;
    }
    return NULL;
}

node *sntSrch(int value) {
    if(start == NULL)
        return NULL;
    node* last = start;
    while(last->next != NULL)
        last = last->next;
    int t = last->data;
    last->data = value;
    node *temp = start;
    while(temp->data != value)
        temp = temp->next;
    if(temp != last) {
        last->data = t;
        return temp;
    }
    else {
        if(t == value)
            return temp;
        else
            last->data = t;
        return NULL;
    }
}

node* binSrch(int value) {
    if (start==NULL)
        return NULL;
    node* beg = start;
    node* end = NULL;
    do {
        node* slow = beg;
        node* fast = beg->next;
        while (fast != end) {
            fast = fast->next;
            if (fast != end) {
                slow = slow->next;
                fast = fast->next;
            }
        }
        node* mid = slow;
        if (mid == NULL)
            return NULL;
        if (mid->data == value)
            return mid;
        else if (mid->data < value)
            beg = mid -> next;
        else
            end = mid;
    } while (end == NULL || end != beg);
    return NULL;
}

node* jmpSrch(int value) {
    if(start == NULL)
        return NULL;
    int size = sqrt(count);
    node *cur = start;
    node *prev = NULL;
    while(cur != NULL && cur->data <= value) {
        prev = cur;
        for(int i = 0; i < size && cur != NULL; i++)
            cur = cur->next;
    }
    while(prev != NULL || prev != cur) {
        if(prev->data == value)
            return prev;
        prev = prev->next;
    }
    return NULL;
}

node* expSrch(int value) {
    if(start == NULL)
        return NULL;
    int i = 1;
    node* temp = start;
    while(i < count && temp->data < value) {
        for(int j = 0; j < i && temp!=NULL; j++)
            temp = temp->next;
        i *= 2;
    }
    int l = i/2;
    int h = i;
    node* beg = start;
    for(int j = 0; i < i/2 && beg !=NULL; j++)
        beg = beg->next;
    node* end = beg->next;
    for(int j = i/2 ; j < i && end !=NULL; j++)
        end = end->next;
    do {
        node* slow = beg;
        node* fast = beg->next;
        while (fast != end) {
            fast = fast->next;
            if (fast != end) {
                slow = slow->next;
                fast = fast->next;
            }
        }
        node* mid = slow;
        if (mid == NULL)
            return NULL;
        if (mid->data == value)
            return mid;
        else if (mid->data < value)
            beg = mid -> next;
        else
            end = mid;
    } while (end == NULL || end != beg);
    return NULL;
}

node* (*func[5])(int) = {linSrch, sntSrch, binSrch, jmpSrch, expSrch};

void check(int x, int y) {
    struct timespec start, end;
    long time_elapsed_ns;
    printf("\nTo find %d:\n", x);
    for(int i=0; i<5; i++) {
        clock_gettime(CLOCK_REALTIME, &start);
        func[i](x);
        clock_gettime(CLOCK_REALTIME, &end);
        time_elapsed_ns = (end.tv_sec - start.tv_sec) * 1000000000 + (end.tv_nsec - start.tv_nsec);
        tri[y][i] = time_elapsed_ns;
        printf("Time taken by %s: %lld nanoseconds\n",names[i], time_elapsed_ns);
    }
}

int main() {
    long ar[100] = {0}, sum[5] = {0};
    int n = 1, a[10], p, ch1, ch2, t=0;
    FILE *f = fopen("SLL.txt", "a");
    time_t now;
    time(&now);
    fprintf(f, "\n%s\nTrial\t|", ctime(&now));
    for(int i = 0; i < 5; i++)
        fprintf(f, "%s\t|",names[i]);
    fprintf(f, "\n%s\n", line);
    while(n==1) {
        printf("\n\n1. Start Trial\n2. Exit\n#? ");
        scanf("%d", &ch1);
        switch(ch1) {
            case 1: srand(time(0));
                    printf("\nInserting: ");
                    for(int i=0; i<10; i++) {
                        p= rand()%100 + 1; //[rand%(max + 1 - min)]+min
                        ar[p]++;
                        if(ar[p]==1)
                            a[i] =p;
                        else {
                            i--;
                            continue;
                        }
                        printf("%d ",a[i]);
                        insert(a[i]);
                    }
                    display();
                    for(int i=0; i<10; i++)
                        check(a[i], i);
                    for(int i=0; i<5;i++) {;
                        printf("\nTotal time taken by %s: ", names[i]);
                        for(int j=0; j<10;j++)
                            sum[i]+= tri[j][i];
                        printf("%4d nanoseconds", sum[i]);
                    }
                    t++;
                    printf("\n1. Save\n2. Discard\n#? ");
                    scanf("%d", &ch2);
                    if(ch2==1){
                        fprintf(f, "  %d.\t|", t);
                        for(int i=0; i<5; i++)
                            fprintf(f, "\t%dns\t|", sum[i]);
                        fprintf(f, "\n%s\n", line);
                    }
                    node *cur = start;
                    node *prev = NULL;
                    while(cur != NULL) {
                        prev = cur;
                        cur = cur->next;
                        free(prev);
                    }
                    start = NULL;
                    count = 0;
                    memset(tri, 0, 50*sizeof(long));
                    memset(ar, 0, 100*sizeof(long));
                    memset(sum, 0, 5*sizeof(long));
                    break;
            case 2: n = 2;
                    break;
            default: break;
        }
    }
    fclose(f);
    return 0;
}