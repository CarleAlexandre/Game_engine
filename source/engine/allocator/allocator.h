#ifndef HAVEN_ALLOC_H
# define HAVEN_ALLOC_H

#pragma once
#include <stddef.h>
#include <stdint.h>
#include <stdbool.h>
#include <stdlib.h>
#include <assert.h>

// ================================================================
// Memory System Core
// ================================================================

/**
 * @brief 
 * 
 */
void	memory_system_init();

/**
 * @brief 
 * 
 */
void	memory_system_shutdown();

/**
 * @brief 
 * 
 */
void	memory_system_print();

// ================================================================
// Stack Allocator (Temporary/Fast)
// ================================================================

/**
 * @brief return a pointer to position x in stack buffer
 * 
 * @param size 
 * @param alignment 
 * @return void* 
 */
void	*allocator_stack_alloc(size_t size, size_t alignment);

/**
 * @brief call this function to reset memory offset
 * and  swap stack memory buffer (in this order)
 * 
 */
void	allocator_stack_swap(void);

/**
 * @brief 
 * 
 */
void	allocator_stack_init(void);

/**
 * @brief 
 * 
 */
void	allocator_stack_delete(void);

// ================================================================
// Pool Allocator (Fixed-Size Objects) WIP
// ================================================================


#endif