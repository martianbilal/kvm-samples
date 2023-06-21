/*
 * QEMU KVM support
 *
 * Copyright IBM, Corp. 2008
 *           Red Hat, Inc. 2008
 *
 * Authors:
 *  Anthony Liguori   <aliguori@us.ibm.com>
 *  Glauber Costa     <gcosta@redhat.com>
 *
 * This work is licensed under the terms of the GNU GPL, version 2 or later.
 * See the COPYING file in the top-level directory.
 *
 */

#include "qemu/osdep.h"
#include "qemu/error-report.h"
#include "qemu/main-loop.h"
#include "qemu/thread.h"
#include "sysemu/kvm_int.h"
#include "sysemu/runstate.h"
#include "sysemu/cpus.h"
#include "qemu/guest-random.h"
#include "qapi/error.h"
#include "util/forkall-coop.h"

#include "kvm-cpus.h"

static int post_fork_setup(struct cpu_prefork_state *prefork_state){
    #ifdef DBG
    printf("[debug] tsc_khz : %p\n", -1);
    #endif
    // printf("[debug] post fork setup! %d\n", prefork_state->);
    return 0; 
}

static void *kvm_vcpu_thread_fn(void *arg)
{
    CPUState *cpu = arg;
    int r;
    #ifdef DBG
    printf("[debug] entered vcpu thread function! \n");
    #endif

    qemu_mutex_init(&cpu->vcpu_recreated_mutex);
    qemu_cond_init(&cpu->vcpu_recreated_cond);
    cpu->vcpu_recreated = false;
    cpu->should_wait = false;
    cpu->vm_forked = false;
    cpu->is_child = false; 
    cpu->forked = false;
    cpu->system_reset = false;
    
    rcu_register_thread();
    if(USE_HYPERODF){
        ski_forkall_thread_add_self_tid();
    }


    qemu_mutex_lock_iothread();
    qemu_thread_get_self(cpu->thread);
    cpu->thread_id = qemu_get_thread_id();
    cpu->can_do_io = 1;
    current_cpu = cpu;



    
    // will have to set up the copying mechanism over here.
    if(cpu->child_cpu){
        #ifdef DBG 
        printf("[debug] starting postfork!\n");
        #endif
        post_fork_setup(cpu->prefork_state);
        cpu->stopped = 0;
    }
    
    r = kvm_init_vcpu(cpu, &error_fatal);
    kvm_init_cpu_signals(cpu);
    


    /* signal CPU creation */
    cpu_thread_signal_created(cpu);
    qemu_guest_random_seed_thread_part2(cpu->random_seed);

    if(cpu->child_cpu){
        // dirty flag causes the register values to be overwritten
        cpu->vcpu_dirty = false;
    }


    do {
        if (cpu_can_run(cpu)) {
            r = kvm_cpu_exec(cpu);
            printf("[%s:%d] ended kvm_cpu_exec with r = %d\n", __func__, __LINE__, r);

            if (r == EXCP_DEBUG) {
                cpu_handle_guest_debug(cpu);
            }
        }
        
        // if(cpu->should_wait){
        //     qemu_mutex_unlock_iothread();
        //     // [TODO] [Bilal] run the routine for saving the pre fork CPU state  
        //     while(1) {
        //         int did_fork;
        //         int is_child; 

        //         ski_forkall_slave(&did_fork, &is_child);
        //         if(did_fork || is_child){
                    qemu_thread_get_self(cpu->thread);
                    cpu->thread_id = qemu_get_thread_id();
                    current_cpu = cpu;
                    cpu->vm_forked = true; 
        //             if (is_child){
        //                 cpu->is_child = true;
        //                 // [TODO] [Bilal] run the pre fork CPU routine

        //             }
                    
        //             break;
        //         }
        //         sleep(0);

        //     }
        //     qemu_mutex_lock_iothread();
        // }

        printf("[%s:%d] starting to wait for io event\n", __func__, __LINE__);

        qemu_wait_io_event(cpu);
        printf("[%d|%s:%d] ended wait for io event\n", getpid(), __func__, __LINE__);
    } while (!cpu->unplug || cpu_can_run(cpu));


    printf("[%s:%d] cpu is unplugged or cpu cannot run\n", __func__, __LINE__);

    kvm_destroy_vcpu(cpu);
    cpu_thread_signal_destroyed(cpu);
    qemu_mutex_unlock_iothread();
    rcu_unregister_thread();

    printf("[%s:%d] done with kvm_vcpu_thread_fn and we have ended it\n", __func__, __LINE__);

    return NULL;
}

void kvm_start_vcpu_thread(CPUState *cpu)
{
    char thread_name[VCPU_THREAD_NAME_SIZE];

    #ifdef DBG
    printf("[debug] VCPU Thread create called! \n");
    #endif
    printf("[%s:%d] creating the vcpu thread\n", __func__, __LINE__);

    cpu->thread = g_malloc0(sizeof(QemuThread));
    cpu->halt_cond = g_malloc0(sizeof(QemuCond));
    qemu_cond_init(cpu->halt_cond);
    snprintf(thread_name, VCPU_THREAD_NAME_SIZE, "CPU %d/KVM",
             cpu->cpu_index);
    qemu_thread_create(cpu->thread, thread_name, kvm_vcpu_thread_fn,
                       cpu, QEMU_THREAD_JOINABLE);
    printf("[%s:%d] ending the vcpu thread\n", __func__, __LINE__);
}

const CpusAccel kvm_cpus = {
    .create_vcpu_thread = kvm_start_vcpu_thread,

    .synchronize_post_reset = kvm_cpu_synchronize_post_reset,
    .synchronize_post_init = kvm_cpu_synchronize_post_init,
    .synchronize_state = kvm_cpu_synchronize_state,
    .synchronize_pre_loadvm = kvm_cpu_synchronize_pre_loadvm,
};
