/**
 * @author Kevin Alavik (https://github.com/KevinAlavik)
 * PROJECT: Pelly
 * LICENSE: MIT
 * DESCRIPTION: Main header file for cursor
*/

#pragma once

#include "../stdint.h"
#include "../stdio.h"

#include "e820.h"

#define PAGE_SIZE 4096

uint64_t div_round_up(uint64_t x, uint64_t y);
uint64_t align_up(uint64_t x, uint64_t y);
uint64_t align_down(uint64_t x, uint64_t y);

static uint64_t free_memory;
static uint8_t *bitmap;
static uint64_t bitmap_pages;
static uint64_t bitmap_size;
static uint64_t free_memory;

void bitmap_set(uint8_t* bitmap, uint64_t bit);
void bitmap_clear(uint8_t* bitmap, uint64_t bit);
uint8_t bitmap_get(uint8_t* bitmap, uint64_t bit);

void init_pmm();
void update_memory();
void* pmm_request_page();
void* pmm_request_pages(size_t numPages);
void pmm_free(void *ptr);
void pmm_free_pages(void *ptr, size_t numPages);
