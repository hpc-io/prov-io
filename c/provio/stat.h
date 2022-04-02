#ifndef _PROVIO_INCLUDE_STAT_H_
#define _PROVIO_INCLUDE_STAT_H_

#include <stddef.h>
#include <stdbool.h>
#include <stdio.h>



typedef struct {
    const char* key;  // key is NULL if this slot is empty
    void* value;
} duration_entry;

typedef struct {
    duration_entry* entries;  // hash slots
    size_t capacity;    // size of _entries array
    size_t length;      // number of items in hash table
} duration_ht;

duration_ht* counts;

typedef struct {
    const char* key;  // current key
    void* value;      // current value

    // Don't use these fields directly.
    duration_ht* _table;       // reference to hash table being iterated
    size_t _index;    // current index into ht._entries
} hti;

/* Helper methods */
unsigned long get_time_usec(void);


/* Stat hash table user methods */
duration_ht* stat_create(void);
void stat_destroy(duration_ht* table);
void accumulate_duration(duration_ht* counts, const char* func_name,
                            unsigned long elapsed);
// Dump to file, print if leave as NULL
void stat_print(duration_ht* counts, const char* path);

#endif

