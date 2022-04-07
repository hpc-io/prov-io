#ifndef _PROVIO_INCLUDE_STAT_H_
#define _PROVIO_INCLUDE_STAT_H_

#include <stddef.h>
#include <stdbool.h>
#include <stdio.h>


typedef struct Stat {
    unsigned long TOTAL_PROV_OVERHEAD;
    unsigned long TOTAL_NATIVE_H5_TIME;
    unsigned long PROV_WRITE_TOTAL_TIME;
    unsigned long FILE_LL_TOTAL_TIME;       //record file linked list overhead
    unsigned long DS_LL_TOTAL_TIME;         //dataset
    unsigned long GRP_LL_TOTAL_TIME;        //group
    unsigned long DT_LL_TOTAL_TIME;         //datatype
    unsigned long ATTR_LL_TOTAL_TIME;       //attribute
    // Redland: PROV graph serialization time.
    unsigned long PROV_SERIALIZE_TIME;      //
} Stat;

typedef struct {
    const char* key;  // key is NULL if this slot is empty
    void* value;
} duration_entry;

typedef struct {
    duration_entry* entries;  // hash slots
    size_t capacity;    // size of _entries array
    size_t length;      // number of items in hash table
} duration_ht;

typedef struct {
    const char* key;  // current key
    void* value;      // current value

    // Don't use these fields directly.
    duration_ht* _table;       // reference to hash table being iterated
    size_t _index;    // current index into ht._entries
} hti;

/* Helper methods */
unsigned long get_time_usec(void);
void _dic_init_int(void);


/* Stat hash table user methods */
duration_ht* stat_create(int capacity);
void stat_destroy(duration_ht* table);
void accumulate_duration(duration_ht* counts, const char* func_name,
                            unsigned long elapsed);
// Dump to file, print if leave as NULL
void stat_print(int MPI_RANK, Stat* prov_stat, 
        duration_ht* counts, const char* path);
void stat_print_(int MPI_RANK, Stat* prov_stat, 
        duration_ht* counts, FILE* stat_file_handle);

#endif