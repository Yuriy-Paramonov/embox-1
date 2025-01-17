/**
 * @file
 * @brief
 *
 * @author Aleksey Zhmulin
 * @date 06.11.22
 */

#include <inttypes.h>

#include <kernel/printk.h>
#include <debug/gdbstub.h>
#include <hal/test/traps_core.h>

#include "exceptions.h"

extern uint32_t _get_mmu_instruction_fault_status(void);

fault_handler_t __ifault_table[0x10];

void arm_pabt_handler(excpt_context_t *ctx) {
	uint32_t fault_status;

	fault_status = _get_mmu_instruction_fault_status() & 0xf;
	if (__ifault_table[fault_status]) {
		__ifault_table[fault_status](fault_status, &ctx->ptregs);
	}
#if GDBSTUB_IMPL
	else if (fault_status == FAULT_DEBUG) {
		gdb_serial_stub((void *)&ctx->ptregs);
		__asm__ __volatile__("mcr p15, 0, %0, c7, c5, 0" : : "r"(0));
	}
#endif
	else {
		printk("\nUnresolvable prefetch abort exception!\n");
		printk("Fault status = %" PRIu32 "\n", fault_status);
		PRINT_PTREGS(&ctx->ptregs);
		while (1)
			;
	}
}
