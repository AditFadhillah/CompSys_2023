#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/time.h>
#include <stdint.h>
#include <errno.h>
#include <assert.h>
#include "record.h"
#include "id_query.h"


struct index_record {
    int64_t osm_id;
    const struct record *record;
};

struct indexed_data {
    struct index_record *irs;
    int n;
};

struct index_record *build_index (struct record *rs, int n) {
    // Creates an index_record struct(irs)
    struct index_record *irs = calloc(n ,sizeof(struct indexed_data));
    for (int i = 0; i < (n); i++) { // Each element in array corresponds to records and its oms_id
        irs[i].record = &rs[i];     // Saves pointer for different records
        irs[i].osm_id = rs[i].osm_id ;   // Saves oms_id for different records
    }
    return irs; // This struct contains pointer and osm_id as a pair for all records
}

struct indexed_data* mk_indexed(struct record *rs, int n) {  
    struct indexed_data *data = malloc(sizeof(struct indexed_data));
    data -> irs = build_index(rs, n);  // data stores the struct from build_index
    data -> n = n;                     // n is the number of records
    return data;
}

void free_indexed(struct indexed_data* data, struct indexed_data* irs) {
    free(data); // Free the allocated memory, to prevent memory leak
    free(irs);
}

const struct record* lookup_indexed(struct indexed_data *data, int64_t needle) {
    for (int i = 0; i < (data -> n); i++) {  // For loop to go through all the element in the struct
        if (data -> irs[i].osm_id == needle) {  // Compares the current osm_id with needle osm_id
            return data -> irs[i].record;  // Returns the record where current osm_id == needle
        };
    }
    return NULL;
}

int main(int argc, char** argv) {
    return id_query_loop(argc, argv,
                    (mk_index_fn)mk_indexed,
                    (free_index_fn)(free_indexed),
                    (lookup_fn)lookup_indexed);
}