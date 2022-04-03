#include <stdlib.h>
#include <stdint.h>
#include <string.h>
#include <sys/time.h>

// #include "provio.h"
#include "stat.h"


// To be modified later
#define INITIAL_CAPACITY 62     // 62 H5VL_provenance methods in total
#define STAT_FUNC_MOD 733       //a reasonably big size to avoid expensive collision handling, make sure it works with 62 function names.


//shorten function id: use hash value
static char* FUNC_DIC[STAT_FUNC_MOD];

unsigned long get_time_usec(void) {
    struct timeval tp;
    gettimeofday(&tp, NULL);
    return (unsigned long)((1000000 * tp.tv_sec) + tp.tv_usec);
}

void _dic_init_int(void){
    for(int i = 0; i < STAT_FUNC_MOD; i++){
        FUNC_DIC[i] = 0;
    }
}

void _dic_init(void){
    for(int i = 0; i < STAT_FUNC_MOD; i++){
        FUNC_DIC[i] = NULL;
    }
}


unsigned int genHash(const char *msg) {
    unsigned long hash = 0;
    unsigned long c;
    unsigned int func_index;
    const char* tmp = msg;

    while (0 != (c = (unsigned long)(*msg++))) {//SDBM hash
        hash = c + (hash << 6) + (hash << 16) - hash;
    }

    msg = tmp;//restore string head address
    func_index = (unsigned int)(hash % STAT_FUNC_MOD);
    if(!FUNC_DIC[func_index]) {
        FUNC_DIC[func_index] = strdup(msg);
        //printf("received msg = %s, hash index = %d, result msg = %s\n", msg, func_index, FUNC_DIC[func_index]);
    }

    return func_index;
}

duration_ht* stat_create(void) {
    // Allocate space for hash table struct.
    duration_ht* table = malloc(sizeof(duration_ht));
    if (table == NULL) {
        return NULL;
    }
    table->length = 0;
    table->capacity = INITIAL_CAPACITY;

    // Allocate (zero'd) space for entry buckets.
    table->entries = calloc(table->capacity, sizeof(duration_entry));
    if (table->entries == NULL) {
        free(table); // error, free table before we return!
        return NULL;
    }
    return table;
}

void stat_destroy(duration_ht* table) {
    // First free allocated keys.
    for (size_t i = 0; i < table->capacity; i++) {
        if (table->entries[i].key != NULL) {
            free((void*)table->entries[i].key);
        }
    }

    // Then free entries array and table itself.
    free(table->entries);
    free(table);
}

void* stat_get(duration_ht* table, const char* key) {
    // AND hash with capacity-1 to ensure it's within entries array.
    uint64_t hash = genHash(key);
    size_t index = (size_t)(hash & (uint64_t)(table->capacity - 1));

    // Loop till we find an empty entry.
    while (table->entries[index].key != NULL) {
        if (strcmp(key, table->entries[index].key) == 0) {
            // Found key, return value.
            return table->entries[index].value;
        }
        // Key wasn't in this slot, move to next (linear probing).
        index++;
        if (index >= table->capacity) {
            // At end of entries array, wrap around.
            index = 0;
        }
    }
    return NULL;
}

const char* stat_set_entry(duration_entry* entries, size_t capacity,
        const char* key, void* value, size_t* plength) {
    // AND hash with capacity-1 to ensure it's within entries array.
    uint64_t hash = genHash(key);
    size_t index = (size_t)(hash & (uint64_t)(capacity - 1));

    // Loop till we find an empty entry.
    while (entries[index].key != NULL) {
        if (strcmp(key, entries[index].key) == 0) {
            // Found key (it already exists), update value.
            entries[index].value = value;
            return entries[index].key;
        }
        // Key wasn't in this slot, move to next (linear probing).
        index++;
        if (index >= capacity) {
            // At end of entries array, wrap around.
            index = 0;
        }
    }

    // Didn't find key, allocate+copy if needed, then insert it.
    if (plength != NULL) {
        key = strdup(key);
        if (key == NULL) {
            return NULL;
        }
        (*plength)++;
    }
    entries[index].key = (char*)key;
    entries[index].value = value;
    return key;
}


size_t stat_length(duration_ht* table) {
    return table->length;
}


hti stat_iterator(duration_ht* table) {
    hti it;
    it._table = table;
    it._index = 0;
    return it;
}

bool stat_next(hti* it) {
    // Loop till we've hit end of entries array.
    duration_ht* table = it->_table;
    while (it->_index < table->capacity) {
        size_t i = it->_index;
        it->_index++;
        if (table->entries[i].key != NULL) {
            // Found next non-empty item, update iterator key and value.
            duration_entry entry = table->entries[i];
            it->key = entry.key;
            it->value = entry.value;
            return true;
        }
    }
    return false;
}


void accumulate_duration(duration_ht* counts, const char* func_name,
                            unsigned long elapsed) {

    void* accumulated_duration = stat_get(counts, func_name);
    if (accumulated_duration != NULL) {
            // Already exists, increment int that value points to.
            unsigned long* accumulated_duration_new = (unsigned long*)accumulated_duration;
            (*accumulated_duration_new) += elapsed;
        }
    else {
         // Word not found, allocate space for new int and set to 1.
        int* accumulated_duration = malloc(sizeof(unsigned long));
        if (accumulated_duration == NULL) {
        //     exit_nomem();
            exit(1);
        }
        *accumulated_duration = elapsed;
        if (stat_set_entry(counts->entries, counts->capacity, func_name, accumulated_duration, &counts->length) == NULL) {
        //     exit_nomem();
            exit(1);
        }
    }
}

/* Initialize file handle within this function with given path */
void stat_print(duration_ht* counts, const char* path) {
    FILE* stat_file_handle;
    if (path) {
        stat_file_handle = fopen(path,"w");
    }
    char pline[2048];
    hti it = stat_iterator(counts);
    // Iteratively print out accumulated duration hash table, freeing values as we go.
    while (stat_next(&it)) {
        sprintf(pline,
            "%s %d us\n", it.key, *(int*)it.value);
        if (stat_file_handle != NULL) {
            fputs(pline, stat_file_handle);
        }
        else {
            printf("%s", pline);
        }        
        free(it.value);
    }
    fclose(stat_file_handle);
}

/* Write to a given handle */
void stat_print_(duration_ht* counts, FILE* stat_file_handle) {
    char pline[2048];
    hti it = stat_iterator(counts);
    // Iteratively print out accumulated duration hash table, freeing values as we go.
    while (stat_next(&it)) {
        sprintf(pline,
            "%s %d us\n", it.key, *(int*)it.value);
        if (stat_file_handle != NULL) {
            fputs(pline, stat_file_handle);
        }
        else {
            printf("%s", pline);
        }        
        free(it.value);
    }
}