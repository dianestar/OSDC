#include "sched.h"

#define MYPRIORITY_TIME_SLICE 4

void init_mypriority_rq(struct mypriority_rq *mypriority_rq)
{
	printk(KERN_INFO "***[MYPRIORITY] Mypriorityclass is online \n");
	mypriority_rq->nr_running=0;
	INIT_LIST_HEAD(&mypriority_rq->queue);
}

static void update_curr_mypriority(struct rq *rq)
{
	struct task_struct *curr=rq->curr;
	struct sched_mypriority_entity *mypriority_se=&curr->mypriority;

	mypriority_se->update_num++;
	printk(KERN_INFO "***[MYPRIORITY] update_curr_mypriority\tpid=%d update_num=%u\n", curr->pid, mypriority_se->update_num);

	//if time slice is over
	if (mypriority_se->update_num > MYPRIORITY_TIME_SLICE) {
		struct list_head *ptr;
		struct sched_mypriority_entity *temp;
		struct list_head *q=&rq->mypriority.queue;
		//traverse runqueue and calculate prionum_avg
		int prionum_sum=0;
		list_for_each(ptr, q) {
			temp=list_entry(ptr, struct sched_mypriority_entity, run_list);
			prionum_sum+=temp->prionum;
		}
		int prionum_avg=prionum_sum/rq->mypriority.nr_running;
		//traverse runqueue and update priority (aging)
		list_for_each(ptr, q) {
			temp=list_entry(ptr, struct sched_mypriority_entity, run_list);
			if (temp->prionum > prionum_avg)
				temp->prionum--;
		}
		//time slice initialization
		mypriority_se->update_num=0;
		//call prio_changed_mypriority function
		//to compare newly updated prionum of runqueue first node and current task
		struct sched_mypriority_entity *first_entity=container_of(q->next, struct sched_mypriority_entity, run_list);
		struct task_struct *first_strut=container_of(first_entity, struct task_struct, mypriority);
		prio_changed_mypriority(rq, first_struct, 0);
	}
}

static void enqueue_task_mypriority(struct rq *rq, struct task_struct *p, int flags) 
{
	//if runqueue is empty, just enqueue
	if (rq->mypriority.nr_running==0) {
		list_add_tail(&p->mypriority.run_list, &rq->mypriority.queue);
		rq->mypriority.nr_running++;
	}

	//if runqueue is not empty, compare priority and enqueue
	else {
		struct list_head *q=&rq->mypriority.queue;
		struct list_head *tail=container_of(q->prev, struct sched_mypriority_entity, run_list);
		struct list_head *ptr;
		struct sched_mypriority_entity *mypriority_se;

		if (tail->prio < p->mypriority.prio)
			list_add_tail(&p->mypriority.run_ist, &rq->mypriority.queue);
		else {
			list_for_each(ptr, &rq->mypriority.queue) {
			mypriority_se=list_entry(ptr, struct sched_mypriority_entity, run_list);
			if (mypriority_se->prio > p->my_priority.prio)
				list_add(&p->mypriority.run_list, mypriority_se);
		}

		rq->mypriority.nr_running++;
	}
	printk(KERN_INFO "***[MYPRIORITY] enqueue: success cpu=%d,nr_running=%u,p->pid=%d\n", p->on_cpu, rq->mypriority.nr_running, p->pid);
	//call check_preempt_curr_mypriority function
	struct list_head *q=&rq->mypriority.queue;
	struct sched_mypriority_entity *first_entity=container_of(q->next, struct sched_mypriority_entity, run_list);
	struct task_struct *first_struct=container_of(first_entity, struct task_struct, mypriority);
	check_preempt_curr_mypriority(rq, first_struct, flags);
}

static void dequeue_task_mypriority(struct rq *rq, struct task_struct *p, int flags)
{
	list_del(&p->mypriority.run_list);
	rq->mypriority.nr_running--;
	printk(KERN_INFO "***[MYPRIORITY] dequeue: success cpu=%d,nr_running=%u,p->pid=%d\n", p->on_cpu, p->state, p->pid, rq->curr->pid);
}

static void check_preempt_curr_mypriority(struct rq *rq, struct task_struct *p, int flags) 
{ 
	struct task_struct *curr=rq->curr;
	struct sched_mypriority_entity *curr_entity=&curr->mypriority;
	struct sched_mypriority_entity *compare_entity=&p->mypriority;
	if (curr != p) {
		// if prionum of compare_entity is higher, reschedule
		if (compare_entity->prionum < curr_entity->prionum) {
			resched_curr(rq);
		}
	}
	printk(KERN_INFO "***[MYPRIORITY] check_preempt_curr_mypriority: success cpu=%d, nr_running=%u, p->pid=%d\n ", p->on_cpu, p->state, p->pid, rq->curr->pid);
}

struct task_struct *pick_next_task_mypriority(struct rq *rq, struct task_struct *prev)
{
	if (rq->mypriority.nr_running!=0) {
		struct list_head *q;
		struct sched_mypriority_entity *next_entity;
		struct task_struct *next_struct;
		
		put_prev_task(rq, prev);

		q=&rq->mypriority.queue;
		next_entity=container_of(q->next, struct sched_mypriority_entity, run_list);
		next_struct=container_of(next_entity, struct task_struct, mypriority);
		printk(KERN_INFO "***[MYPRIORITY] pick_next_task: cpu=%d,prev->pid=%d,next_p->pid=%d,nr_running=%u\n", prev->on_cpu, prev->pid, next_struct->pid, rq->mypriority.nr_running);

		return next_struct;
	}
	return NULL;
}

static void put_prev_task_mypriority(struct rq *rq, struct task_struct *p) 
{
	printk(KERN_INFO "***[MYPRIORITY] put_prev_task: do nothing, p->pid=%d\n", p->pid);
}
static int select_task_rq_mypriority(struct task_struct *p, int cpu, int sd_flag, int flags)
{
	return task_cpu(p);
}
static void set_curr_task_mypriority(struct rq *rq) 
{ 
	printk(KERN_INFO "***[MYPRIORITY] set_curr_task_mypriority\n");
}
static void task_tick_mypriority(struct rq *rq, struct task_struct *p, int queued) 
{ 
	update_curr_mypriority(rq);
}
static void prio_changed_mypriority(struct rq *rq, struct task_struct *p, int oldprio) 
{
	struct task_struct *curr=rq->curr;
	struct sched_mypriority_entity *curr_entity=&curr->mypriority;
	struct sched_mypriority_entity *compare_entity=&p->mypriority;

	if (curr != p) {
		//if prionum of compare_entity is higher, reschedule
		if (compare_entity->prionum < curr_entity->prionum) {
			resched_curr(rq);
		}
	}
}
/* This routine is called when a task migrates between classes */
static void switched_to_mypriority(struct rq *rq, struct task_struct *p)
{
	resched_curr(rq);
}
const struct sched_class mypriority_sched_class = {
	.next			= &fair_sched_class,
	.enqueue_task		= enqueue_task_mypriority,
	.dequeue_task		= dequeue_task_mypriority,
	.check_preempt_curr	= check_preempt_curr_mypriority,
	.pick_next_task		= pick_next_task_mypriority,
	.put_prev_task		= put_prev_task_mypriority,

#ifdef CONFIG_SMP
	.select_task_rq		= select_task_rq_mypriority,
#endif
	.set_curr_task		= set_curr_task_mypriority,
	.task_tick		= task_tick_mypriority,
	.prio_changed		= prio_changed_mypriority,
	.switched_to		= switched_to_mypriority,
	.update_curr		= update_curr_mypriority,
};
