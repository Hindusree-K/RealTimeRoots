#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/init.h>
#include <linux/kthread.h>
#include <linux/smp.h>
#include <linux/delay.h>

#define THREAD_COUNT 2

static struct task_struct *threads[THREAD_COUNT];

int thread_fn(void *data) {
    int cpu = smp_processor_id();
    pr_info("Thread running on CPU %d\n", cpu);

    while (!kthread_should_stop()) {
        ssleep(1); // Simulate some work
    }

    pr_info("Thread on CPU %d stopping\n", cpu);
    return 0;
}

static int __init multicore_init(void) {
    int i;
    for (i = 0; i < THREAD_COUNT; i++) {
        threads[i] = kthread_create(thread_fn, NULL, "cpu_thread_%d", i);
        if (IS_ERR(threads[i])) {
            pr_err("Failed to create thread %d\n", i);
            threads[i] = NULL;
        } else {
            kthread_bind(threads[i], i); // Bind to specific CPU
            wake_up_process(threads[i]);
        }
    }

    pr_info("Multicore thread module loaded\n");
    return 0;
}

static void __exit multicore_exit(void) {
    int i;
    for (i = 0; i < THREAD_COUNT; i++) {
        if (threads[i])
            kthread_stop(threads[i]);
    }
    pr_info("Multicore thread module unloaded\n");
}

module_init(multicore_init);
module_exit(multicore_exit);

MODULE_LICENSE("GPL");
MODULE_AUTHOR("Mohan + Copilot");
MODULE_DESCRIPTION("Kernel module to run threads on different CPUs");

