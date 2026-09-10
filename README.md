# Linked List

A lightweight, zero-allocation intrusive circular doubly linked list in strict C99, specifically engineered for real-time operating system (RTOS) kernels and hard real-time embedded applications.

---

## Key Features

* **Zero Dynamic Memory Allocation**: Nodes are embedded directly within application or kernel structures (e.g., Task Control Blocks, timers, or wait queues). Completely complies with MISRA C:2012 Rule 21.3 (no `<stdlib.h>` dynamic allocation).
* **Deterministic $O(1)$ Execution**: All primary operations—insertion at head/tail, insertion before/after an existing node, and removal—execute in constant, bounded time with zero loop iterations.
* **Inlined Fast Accessors**: Core queue operations (`pop_head`, `pop_tail`, `peek_head`, `peek_tail`, `is_empty`, `count`) are implemented as `static inline` functions in [`linked_list.h`](linked_list.h) for zero function-call overhead.
* **Direct Kernel Primitives**: High-throughput unchecked primitives (`linked_list_insert_head_direct`, `linked_list_insert_tail_direct`, `linked_list_remove_direct`) compile to 4 store instructions on ARM Cortex-M for latency-critical RTOS dispatchers where arguments are pre-verified.
* **Circular Sentinel Architecture**: Employs an internal sentinel root node that eliminates edge cases, null pointer dereferences at list boundaries, and branching overhead.
* **Safe Container Retrieval**: Provides the type-safe `LINKED_LIST_CONTAINER_OF` macro to resolve enclosing parent structures from embedded list nodes without dynamic casting.
* **Defensive Parameter Validation**: Strictly validates pointers against `NULL` and prevents duplicate node insertions or corruption of active list chains in public API variants.
* **MISRA C:2012 Compliant**: Written to satisfy MISRA C:2012 guidelines (C-style block comments exclusively, explicit typing, explicit conversion discipline).

---

## Data Structures

```c
/**
 * @brief Intrusive doubly linked list node embedded within target structures.
 */
typedef struct LinkedListNode {
    struct LinkedListNode* next;
    struct LinkedListNode* prev;
} LinkedListNode;

/**
 * @brief Circular doubly linked list container with an embedded sentinel root.
 */
typedef struct LinkedList {
    LinkedListNode root;
    size_t count;
} LinkedList;
```

---

## API Reference

Declared in [`linked_list.h`](linked_list.h):

```c
/* Initialization & Inspection */
bool linked_list_init(LinkedList* list);
bool linked_list_node_init(LinkedListNode* node);
bool linked_list_node_is_linked(const LinkedListNode* node);
static inline bool linked_list_is_empty(const LinkedList* list);
static inline size_t linked_list_count(const LinkedList* list);

/* Deterministic O(1) Inlined Queue Operations */
static inline LinkedListNode* linked_list_pop_head(LinkedList* list);
static inline LinkedListNode* linked_list_pop_tail(LinkedList* list);
static inline LinkedListNode* linked_list_peek_head(const LinkedList* list);
static inline LinkedListNode* linked_list_peek_tail(const LinkedList* list);

/* Direct Kernel Primitives (Unchecked, 4 Store Instructions) */
static inline void linked_list_insert_head_direct(LinkedList* list, LinkedListNode* node);
static inline void linked_list_insert_tail_direct(LinkedList* list, LinkedListNode* node);
static inline void linked_list_remove_direct(LinkedList* list, LinkedListNode* node);

/* Defensive O(1) Insertions */
bool linked_list_insert_head(LinkedList* list, LinkedListNode* node);
bool linked_list_insert_tail(LinkedList* list, LinkedListNode* node);
bool linked_list_insert_before(LinkedList* list, LinkedListNode* target, LinkedListNode* node);
bool linked_list_insert_after(LinkedList* list, LinkedListNode* target, LinkedListNode* node);

/* Defensive O(1) Removal & Traversal */
bool linked_list_remove(LinkedList* list, LinkedListNode* node);
LinkedListNode* linked_list_next(const LinkedList* list, const LinkedListNode* current);
LinkedListNode* linked_list_prev(const LinkedList* list, const LinkedListNode* current);

/* Cleanup */
void linked_list_clear(LinkedList* list);
```

---

## Usage Example

### Embedding in a Task Control Block (RTOS Pattern)

```c
#include "linked_list.h"
#include <stdint.h>
#include <stdio.h>

typedef struct TaskControlBlock {
    uint32_t task_id;
    uint8_t priority;
    LinkedListNode state_node; /* Intrusive node for ready/blocked lists */
} TaskControlBlock;

void example(void)
{
    LinkedList ready_list;
    linked_list_init(&ready_list);

    TaskControlBlock task1 = { .task_id = 1U, .priority = 5U };
    TaskControlBlock task2 = { .task_id = 2U, .priority = 10U };

    linked_list_node_init(&task1.state_node);
    linked_list_node_init(&task2.state_node);

    /* Enqueue tasks into ready list */
    linked_list_insert_tail(&ready_list, &task1.state_node);
    linked_list_insert_tail(&ready_list, &task2.state_node);

    /* Dequeue highest-priority task from head */
    LinkedListNode* popped_node = linked_list_pop_head(&ready_list);
    if (popped_node != NULL) {
        TaskControlBlock* current_task = LINKED_LIST_CONTAINER_OF(
            popped_node, TaskControlBlock, state_node
        );
        /* Execute task ... */
    }
}
```

---

## Standards Compliance

* **Language Standard**: ISO/IEC 9899:1999 (C99).
* **MISRA C:2012**: Adheres strictly to all Mandatory and Required rules.
* **Workspace Guidelines**: Fully aligned with workspace [`GEMINI.md`](../GEMINI.md) standards.

---

## License

This project is licensed under the MIT License. See [LICENSE](LICENSE) for details.