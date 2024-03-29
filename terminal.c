/*
	Copyright 2012-2014 Benjamin Vedder	benjamin@vedder.se

	This program is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with this program.  If not, see <http://www.gnu.org/licenses/>.
    */

/*
 * terminal.c
 *
 *  Created on: 26 dec 2013
 *      Author: benjamin
 */

#include "ch.h"
#include "hal.h"
#include "terminal.h"
#include "comm.h"
#include "mcpwm.h"
#include "main.h"

#include <string.h>
#include <stdio.h>

void terminal_process_string(char *str) {
	enum { kMaxArgs = 64 };
	int argc = 0;
	char *argv[kMaxArgs];
	static char buffer[256];

	char *p2 = strtok(str, " ");
	while (p2 && argc < kMaxArgs) {
		argv[argc++] = p2;
		p2 = strtok(0, " ");
	}

	if (argc == 0) {
		comm_print("No command received\n");
		return;
	}

	if (strcmp(argv[0], "ping") == 0) {
		comm_print("pong\n");
	} else if (strcmp(argv[0], "stop") == 0) {
		mcpwm_set_duty(0);
		comm_print("Motor stopped\n");
	} else if (strcmp(argv[0], "last_adc_duration") == 0) {
		sprintf(buffer, "Latest ADC duration: %.4f ms", (double)(mcpwm_get_last_adc_isr_duration() * 1000.0));
		comm_print(buffer);
		sprintf(buffer, "Latest injected ADC duration: %.4f ms", (double)(mcpwm_get_last_inj_adc_isr_duration() * 1000.0));
		comm_print(buffer);
		sprintf(buffer, "Latest main ADC duration: %.4f ms\n", (double)(main_get_last_adc_isr_duration() * 1000.0));
				comm_print(buffer);
	} else if (strcmp(argv[0], "kv") == 0) {
		sprintf(buffer, "Calculated KV: %.2f rpm/volt\n", (double)mcpwm_get_kv_filtered());
		comm_print(buffer);
	} else if (strcmp(argv[0], "mem") == 0) {
		size_t n, size;
		n = chHeapStatus(NULL, &size);
		sprintf(buffer, "core free memory : %u bytes", chCoreStatus());
		comm_print(buffer);
		sprintf(buffer, "heap fragments   : %u", n);
		comm_print(buffer);
		sprintf(buffer, "heap free total  : %u bytes\n", size);
		comm_print(buffer);
	} else if (strcmp(argv[0], "threads") == 0) {
		Thread *tp;
		static const char *states[] = {THD_STATE_NAMES};
		comm_print("    addr    stack prio refs     state           name time    ");
		comm_print("-------------------------------------------------------------");
		tp = chRegFirstThread();
		do {
			sprintf(buffer, "%.8lx %.8lx %4lu %4lu %9s %14s %lu",
					(uint32_t)tp, (uint32_t)tp->p_ctx.r13,
					(uint32_t)tp->p_prio, (uint32_t)(tp->p_refs - 1),
					states[tp->p_state], tp->p_name, (uint32_t)tp->p_time);
			comm_print(buffer);
			tp = chRegNextThread(tp);
		} while (tp != NULL);
		comm_print("");
	} else if (strcmp(argv[0], "fault") == 0) {
		comm_print_fault_code(mcpwm_get_fault());
	} else if (strcmp(argv[0], "rpm") == 0) {
		sprintf(buffer, "Electrical RPM: %.2f rpm\n", (double)mcpwm_get_rpm());
		comm_print(buffer);
	} else if (strcmp(argv[0], "tacho") == 0) {
		sprintf(buffer, "Tachometer counts: %i\n", mcpwm_get_tachometer_value(0));
		comm_print(buffer);
	} else if (strcmp(argv[0], "tim") == 0) {
		TIM_Cmd(TIM1, DISABLE);
		int t1_cnt = TIM1->CNT;
		int t8_cnt = TIM8->CNT;
		int duty = TIM1->CCR1;
		int top = TIM1->ARR;
		int voltage_samp = TIM8->CCR1;
		int current1_samp = TIM1->CCR4;
		int current2_samp = TIM8->CCR2;
		TIM_Cmd(TIM1, ENABLE);
		sprintf(buffer, "Tim1 CNT: %i", t1_cnt);
		comm_print(buffer);
		sprintf(buffer, "Tim8 CNT: %u", t8_cnt);
		comm_print(buffer);
		sprintf(buffer, "Duty cycle: %u", duty);
		comm_print(buffer);
		sprintf(buffer, "Top: %u", top);
		comm_print(buffer);
		sprintf(buffer, "Voltage sample: %u", voltage_samp);
		comm_print(buffer);
		sprintf(buffer, "Current 1 sample: %u", current1_samp);
		comm_print(buffer);
		sprintf(buffer, "Current 2 sample: %u\n", current2_samp);
		comm_print(buffer);
	}

	// Setters
	else if (strcmp(argv[0], "set_hall_table") == 0) {
		if (argc == 4) {
			int dir = -1;
			int fwd_add = -1;
			int rev_add = -1;
			sscanf(argv[1], "%i", &dir);
			sscanf(argv[2], "%i", &fwd_add);
			sscanf(argv[3], "%i", &rev_add);

			if (dir >= 0 && fwd_add >= 0 && rev_add >= 0) {
				mcpwm_init_hall_table(dir, fwd_add, rev_add);
				sprintf(buffer, "New hall sensor dir: %i fwd_add %i rev_add %i\n",
						dir, fwd_add, rev_add);
				comm_print(buffer);
			} else {
				comm_print("Invalid argument(s).\n");
			}
		} else {
			comm_print("This command requires three arguments.\n");
		}
	}

	// The help command
	else if (strcmp(argv[0], "help") == 0) {
		comm_print("Valid commands are:");
		comm_print("help");
		comm_print("  Show this help");

		comm_print("ping");
		comm_print("  Print pong here to see if the reply works");

		comm_print("stop");
		comm_print("  Stop the motor");

		comm_print("last_adc_duration");
		comm_print("  The time the latest ADC interrupt consumed");

		comm_print("kv");
		comm_print("  The calculated kv of the motor");

		comm_print("mem");
		comm_print("  Show memory usage");

		comm_print("threads");
		comm_print("  List all threads");

		comm_print("fault");
		comm_print("  Prints the current fault code");

		comm_print("rpm");
		comm_print("  Prints the current electrical RPM");

		comm_print("tacho");
		comm_print("  Prints tachometer value");

		comm_print("tim");
		comm_print("  Prints tim1 and tim8 settings");

		comm_print("set_hall_table [dir] [fwd_add] [rev_add]");
		comm_print("  Update the hall sensor lookup table\n");
	} else {
		sprintf(buffer, "Invalid command: %s\n"
				"type help to list all available commands\n", argv[0]);
		comm_print(buffer);
	}
}
