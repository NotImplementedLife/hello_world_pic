#pragma once

#define MOV_R7_R7 ".BYTE 0x3f, 0x46"
#define MOV_R6_R6 ".BYTE 0x36, 0x46"
#define MOV_R5_R5 ".BYTE 0x2D, 0x46"

// works with this: https://github.com/NotImplementedLife/desmume-wasm/releases/tag/win
#define DESMUME_LOG_CODE(MOV_X_X, message) asm volatile ( \
	"push {r6} \r\n\t" "mov r6, %0 \r\n\t" \
	MOV_X_X "\r\n\t" "pop {r6}\r\n\t" \
	:: "r"(message));

#define DEBUG

#ifdef DEBUG
	#define dbg_set_step(step, msg) step = (msg);
	#define dbg_log_step(step) { DESMUME_LOG_CODE(MOV_R7_R7, "An error occured"); DESMUME_LOG_CODE(MOV_R7_R7, step); }
#else
	#define dbg_set_step(step, msg) ;
	#define dbg_log_step(step) ;
#endif
