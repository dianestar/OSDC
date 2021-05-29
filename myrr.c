#include "sched.h"
static void put_prev_task_myrr(struct rq *rq, struct task_struct *p);
static int select_task_rq_myrr(struct task_struct *p, int cpu, int sd_flag, int flags);
static void set_curr_task_myrr(struct rq *rq);
static void task_tick_myrr(struct rq *rq,struct task_struct *p, int oldprio);
static void switched_to_myrr(struct rq *rq, struct task_struct *p);
void init_myrr_rq(struct myrr_rq *myrr_rq);
static void update_curr_myrr(struct rq *rq);
static void enqueue_task_myrr(struct rq *rq, struct task_struct *p, int flags);
static void dequeue_task_myrr(struct rq *rq, struct task_struct *p, int flags);
static void check_preempt_curr_myrr(struct rq *rq, struct task_struct *p,int flags);
struct task_struct *pick_next_task_myrr(struct rq *rq, struct task_struct *prev);
static void prio_changed_myrr(struct rq *rq, struct task_struct *p, int oldprio);



const struct sched_class myrr_sched_class={
	.next=&fair_sched_class,
	.enqueue_task=&enqueue_task_myrr,
	.dequeue_task=dequeue_task_myrr,
	.check_preempt_curr=check_preempt_curr_myrr,
	.pick_next_task=pick_next_task_myrr,
	.put_prev_task=put_prev_task_myrr,
#ifdef CONFIG_SMP
	.select_task_rq=select_task_rq_myrr,
#endif
	.set_curr_task=set_curr_task_myrr,
	.task_tick=task_tick_myrr,
	.prio_changed=prio_changed_myrr,
	.switched_to=switched_to_myrr,
	.update_curr=update_curr_myrr,
};


void init_myrr_rq (struct myrr_rq *myrr_rq)
{
	printk(KERN_INFO "***[MYRR] Mysched class is online \n");
	myrr_rq->nr_running=0;
	INIT_LIST_HEAD(&myrr_rq->queue);

}
static void update_curr_myrr(struct rq *rq) {
	struct task_struct *curr=rq->curr;
	struct sched_myrr_entity *myrr_se=&curr->myrr;
	
	myrr_se->update_num++;

	printk(KERN_INFO "***[MYRR] update_curr_myrr\tpid=%d update_num=%u\n", curr->pid, myrr_se->update_num);

	if (myrr_se->update_num > 4) {
		list_move_tail(&myrr_se->run_list, &rq->myrr.queue);
		myrr_se->update_num=0;
		resched_curr(rq);
	}
}

static void enqueue_task_myrr(struct rq *rq, struct task_struct *p, int flags) {
	list_add_tail(&p->myrr.run_list, &rq->myrr.queue);
	rq->myrr.nr_running++;

	printk(KERN_INFO "***[MYRR] enqueue: success cpu=%d, nr_running=%u, pid=%d\n", p->on_cpu, rq->myrr.nr_running, p->pid);
}
static void dequeue_task_myrr(struct rq *rq, struct task_struct *p, int flags) 
{
	list_del(&p->myrr.run_list);
	rq->myrr.nr_running--;

	printk(KERN_INFO "\t***[MYRR] dequeue: success cpu=%d, nr_running=%u, pid=%d\n", p->on_cpu, rq->myrr.nr_running, p->pid);
}
void check_preempt_curr_myrr(struct rq *rq, struct task_struct *p, int flags) {
	printk("***[MYRR] check_preempt_curr_myrr\n");
}
struct task_struct *pick_next_task_myrr(struct rq *rq, struct task_struct *prev)
{
	if(rq->myrr.nr_running==0 ){
		return NULL;
	}
	else {
		struct list_head *q;
		struct sched_myrr_entity *next_entity;
		struct task_struct *next_struct;

		put_prev_task(rq, prev);
		q=&rq->myrr.queue;
		next_entity=container_of(q->next, struct sched_myrr_entity, run_list);
		next_struct=container_of(next_entity, struct task_struct, myrr);

		printk(KERN_INFO "\t***[MYRR] pick_next_task: cpu=%d, prev->pid=%d, next_p->pid=%d, nr_running=%d", prev->on_cpu, prev->pid, next_struct->pid, rq->myrr.nr_running);

		return next_struct;
	}
}
void put_prev_task_myrr(struct rq *rq, struct task_struct *p) {
	printk(KERN_INFO "\t***[MYRR] put_prev_task: do_nothing, p->pid=%d\n",p->pid);
}
int select_task_rq_myrr(struct task_struct *p, int cpu, int sd_flag, int flags){return task_cpu(p);}
void set_curr_task_myrr(struct rq *rq){
	printk(KERN_INFO"***[MYRR] set_curr_task_myrr\n");
}
void task_tick_myrr(struct rq *rq, struct task_struct *p, int queued) {
	update_curr_myrr(rq);
}
void prio_changed_myrr(struct rq *rq, struct task_struct *p, int oldprio) { }
/*This routine is called when a task migrates between classes*/
void switched_to_myrr(struct rq *rq, struct task_struct *p)
{
	resched_curr(rq);
}
