# include <stdio.h>
# include <stdlib.h>
# include <time.h>



typedef struct {
	int id;
	int arrival_time;
	int service_time;
} element;			

#define MAX_QUEUE_SIZE 15000
typedef struct { 
	element  data[MAX_QUEUE_SIZE];
	int  front, rear;
} QueueType;


void error(char *message)
{
	fprintf(stderr, "%s\n", message);
	exit(1);
}


void init_queue(QueueType *q)
{
	q->front = q->rear = 0;
}

 
int is_empty(QueueType *q)
{
	return (q->front == q->rear);
}

 
int is_full(QueueType *q)
{
	return ((q->rear + 1) % MAX_QUEUE_SIZE == q->front);
}


void queue_print(QueueType *q)
{
	printf("QUEUE(front=%d rear=%d) = ", q->front, q->rear);
	if (!is_empty(q)) {
			int i = q->front;
			do {
				i = (i + 1) % (MAX_QUEUE_SIZE);
				printf("%d | ", q->data[i]);
				if (i == q->rear)
					break;
			} while (i != q->front);
		}
	printf("\n");
}


void enqueue(QueueType *q, element item)
{
	if (is_full(q))
		error("큐가 포화상태입니다.");
	q->rear = (q->rear + 1) % MAX_QUEUE_SIZE;
	q->data[q->rear] = item;
}


element dequeue(QueueType *q)
{
	if (is_empty(q))
		error("큐가 공백상태입니다.");
	q->front = (q->front + 1) % MAX_QUEUE_SIZE;
	return q->data[q->front];
}


element peek(QueueType *q)
{
	if (is_empty(q))
		error("큐가 공백상태입니다.");
	return q->data[(q->front + 1) % MAX_QUEUE_SIZE];
}


int main(void) 
{
	int minutes = 50000;
	int service_time = 0;
	int service_customer;

	int cnt_nom = 0;
	int nom_total_wait = 0;
	int nom_total_customers = 0;
	QueueType normal;
	init_queue(&normal);
	
	int cnt_vip = 0;
	int vip_total_wait = 0;
	int vip_total_customers = 0;
	QueueType vip;
	init_queue(&vip);

	srand(time(NULL));
	for (int clock = 0; clock < minutes; clock++) {
		printf("현재시간=%d\n", clock);
		if ((rand()%10) > 1 && (rand()%10) < 4) {
			element normal_cst;
			normal_cst.id = nom_total_customers++;
			normal_cst.arrival_time = clock;
			normal_cst.service_time = rand() % 3+1;
			enqueue(&normal, normal_cst);
			printf("일반고객 %d님이 %d분에 들어옵니다. 업무처리시간= %d분\n", 
			normal_cst.id, normal_cst.arrival_time, normal_cst.service_time);
			cnt_nom += 1;
		}
		else if ((rand()%10) < 1 ) {
			element vip_cst;
			vip_cst.id = vip_total_customers++;
			vip_cst.arrival_time = clock;
			vip_cst.service_time = rand() % 3+1;
			enqueue(&vip, vip_cst);
			printf("vip고객 %d님이 %d분에 들어옵니다. 업무처리시간= %d분\n", 
			vip_cst.id, vip_cst.arrival_time, vip_cst.service_time);
			cnt_vip += 1;
		}
		
		if (service_time > 0) {
			printf("고객 %d 업무처리중입니다. \n\n", service_customer);
			service_time--;
		}
		else {
			if (!is_empty(&normal) && is_empty(&vip)) {
				element normal_cst = dequeue(&normal);
				service_customer = normal_cst.id;
				service_time = normal_cst.service_time;
				printf("고객 %d이 %d분에 업무를 시작합니다. 대기시간은 %d분이었습니다.\n\n",
					normal_cst.id, clock, clock - normal_cst.arrival_time);
				nom_total_wait += clock - normal_cst.arrival_time;
			}

			if ((!is_empty(&vip) && is_empty(&normal)) || (!is_empty(&vip) && !is_empty(&normal))) {
				element vip_cst = dequeue(&vip);
				service_customer = vip_cst.id;
				service_time = vip_cst.service_time;
				printf("고객 %d이 %d분에 업무를 시작합니다. 대기시간은 %d분이었습니다.\n\n",
					vip_cst.id, clock, clock - vip_cst.arrival_time);
				vip_total_wait += clock - vip_cst.arrival_time;
			}
		}
	}

	printf("vip고객 전체 대기 시간 : %d \nvip고객 방문 수 : %d \nvip고객 평균 대기 시간 : %d \n\n", vip_total_wait, cnt_vip, vip_total_wait/cnt_vip);
	printf("일반고객 전체 대기 시간 : %d \n일반고객 방문 수 : %d \n일반고객 평균 대기 시간 : %d \n\n", nom_total_wait, cnt_nom, nom_total_wait/cnt_nom);
	return 0;
}