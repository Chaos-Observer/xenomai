/*
 * Copyright (C) 2001-2007 Philippe Gerum <rpm@xenomai.org>.
 *
 * Xenomai is free software; you can redistribute it and/or modify it
 * under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 * Xenomai is distributed in the hope that it will be useful, but
 * WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with Xenomai; if not, write to the Free Software
 * Foundation, Inc., 59 Temple Place - Suite 330, Boston, MA
 * 02111-1307, USA.
 */

#ifndef _XENO_ASM_X86_64_BITS_SHADOW_H
#define _XENO_ASM_X86_64_BITS_SHADOW_H

#ifndef __KERNEL__
#error "Pure kernel header included from user-space!"
#endif

static inline void xnarch_init_shadow_tcb(xnarchtcb_t * tcb,
					  struct xnthread *thread,
					  const char *name)
{
	struct task_struct *task = current;

	tcb->user_task = task;
	tcb->active_task = NULL;
	tcb->rsp = 0;
	tcb->rspp = &task->thread.rsp;
	tcb->ripp = &task->thread.rip;
	tcb->fpup = &task->thread.i387;
}

static inline void xnarch_grab_xirqs(rthal_irq_handler_t handler)
{
	unsigned irq;

	for (irq = 0; irq < IPIPE_NR_XIRQS; irq++)
		rthal_virtualize_irq(rthal_current_domain,
				     irq,
				     handler, NULL, NULL, IPIPE_HANDLE_MASK);
}

static inline void xnarch_lock_xirqs(rthal_pipeline_stage_t * ipd, int cpuid)
{
	unsigned irq;

	for (irq = 0; irq < IPIPE_NR_XIRQS; irq++) {
		switch (irq) {
#ifdef CONFIG_SMP
		case RTHAL_CRITICAL_IPI:
		case IPIPE_FIRST_APIC_IRQ + CALL_FUNCTION_VECTOR - FIRST_SYSTEM_VECTOR:
		case IPIPE_FIRST_APIC_IRQ + RESCHEDULE_VECTOR - FIRST_SYSTEM_VECTOR:

			/* Never lock out these ones. */
			continue;
#endif /* CONFIG_SMP */

		default:

			if (irq >= INVALIDATE_TLB_VECTOR_START &&
			    irq <= INVALIDATE_TLB_VECTOR_END)
				/* Don't lock TLB invalidate vectors either */
				continue;

			rthal_lock_irq(ipd, cpuid, irq);
		}
	}
}

static inline void xnarch_unlock_xirqs(rthal_pipeline_stage_t * ipd, int cpuid)
{
	unsigned irq;

	for (irq = 0; irq < IPIPE_NR_XIRQS; irq++) {
		switch (irq) {
#ifdef CONFIG_SMP
		case RTHAL_CRITICAL_IPI:
		case IPIPE_FIRST_APIC_IRQ + CALL_FUNCTION_VECTOR - FIRST_SYSTEM_VECTOR:
		case IPIPE_FIRST_APIC_IRQ + RESCHEDULE_VECTOR - FIRST_SYSTEM_VECTOR:

			continue;
#endif /* CONFIG_SMP */

		default:

			if (irq >= INVALIDATE_TLB_VECTOR_START &&
			    irq <= INVALIDATE_TLB_VECTOR_END)
				continue;

			rthal_unlock_irq(ipd, irq);
		}
	}
}

static inline int xnarch_local_syscall(struct pt_regs *regs)
{
	return -ENOSYS;
}

static void xnarch_schedule_tail(struct task_struct *prev)
{
}

#endif /* !_XENO_ASM_X86_64_BITS_SHADOW_H */
