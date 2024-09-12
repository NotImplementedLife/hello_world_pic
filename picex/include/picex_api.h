#pragma once

#ifdef __cplusplus
extern "C" {
#endif

typedef struct {
	void* (*malloc)(unsigned int);
	void (*free)(void*);
} picex_main_api_t;

extern picex_main_api_t picex_main_api;

#ifdef __cplusplus
}
#endif

