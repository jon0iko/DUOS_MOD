/*
 * Copyright (c) 2022 
 * Computer Science and Engineering, University of Dhaka
 * Credit: CSE Batch 25 (starter) and Prof. Mosaddek Tushar
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 * 1. Redistributions of source code must retain the above copyright
 *    notice, this list of conditions and the following disclaimer.
 * 2. Redistributions in binary form must reproduce the above copyright
 *    notice, this list of conditions and the following disclaimer in the
 *    documentation and/or other materials provided with the distribution.
 * 3. Neither the name of the University nor the names of its contributors
 *    may be used to endorse or promote products derived from this software
 *    without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE UNIVERSITY AND CONTRIBUTORS ``AS IS'' AND
 * ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
 * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
 * ARE DISCLAIMED.  IN NO EVENT SHALL THE UNIVERSITY OR CONTRIBUTORS BE LIABLE
 * FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
 * DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS
 * OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION)
 * HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT
 * LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY
 * OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF
 * SUCH DAMAGE.
*/

#include <syscall.h>
#include <syscall_def.h>
#include <errno.h>
#include <errmsg.h>
#include <types.h>
#include <kmain.h>

static TCB_TypeDef task_list[MAX_TASKS];
static uint16_t current_task_id = 1111;
static uint16_t current_task_i = 0;



uint32_t syscall_execution(uint32_t syscall_num, uint32_t arg1, uint32_t arg2, uint32_t arg3)
{
    switch (syscall_num) {
        case SYS_getpid:
            return (uint32_t)sys_getpid();
            
        case SYS___time:
            return (uint32_t)sys_time();
            
        case SYS_write:
            return (uint32_t)sys_write((int)arg1, (const char*)arg2, (uint32_t)arg3);
            
        case SYS_read:
            return (uint32_t)sys_read((int)arg1, (char*)arg2, (uint32_t)arg3);
            
        case SYS_yield:
            sys_yield();
            return 0;
            
        case SYS_reboot:
            sys_reboot();
            return 0; // Should not reach here
            
        case SYS__exit:
            sys_exit();
            return 0; // Should not reach here
            
        default:
            return 0xFFFFFFFF; // Invalid syscall
    }
}

void syscall_init(void)
{
	for (uint16_t i = 0; i < MAX_TASKS; i++)
	{
		task_list[i].magic_number = 0xFECABAA0;
		task_list[i].task_id = current_task_id + i;
		task_list[i].psp = NULL;
		task_list[i].status = TASK_BLOCKED_STATE;
		task_list[i].execution_time = 0;
		task_list[i].waiting_time = 0;
		task_list[i].digital_sinature = 0x00000001;
	}


	task_list[0].status = TASK_READY_STATE;
	task_list[0].task_id = current_task_id;
	current_task_i = 0;
}



uint32_t sys_getpid(void)
{
    if (current_task_i < MAX_TASKS) {
        return (uint32_t)task_list[current_task_i].task_id;
    }
    return current_task_id; // Return current task ID if no valid task
}