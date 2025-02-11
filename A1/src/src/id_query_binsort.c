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

struct binsort_data {
    struct index_record *irs;
    int n;
};


int sort (const void *osm_id_a, const void *osm_id_b) {
    return *(int*)osm_id_a - *(int*)osm_id_b; // Returns the differences between two osm_id
}

struct index_record *build_index(struct record *rs, int n) {
    // Creates an index_record array(data)
    // Each element in struct corresponds to records and its oms_id
    struct index_record *data = calloc(n ,sizeof(struct binsort_data));     
    for (int i = 0; i < (n); i++) { 
        data[i].record = &rs[i];           // Stores the address of records
        data[i].osm_id = rs[i].osm_id ;    // Stores oms_ids
    }
    // Run quicksort with value from (sort) as comparison
    qsort(data, n, sizeof(struct index_record), sort);

    return data;
}

struct binsort_data* mk_binsort(struct record *rs, int n) {  
    struct binsort_data *data = malloc(sizeof(struct binsort_data));
    data -> irs = build_index(rs, n);
    data -> n = n;
    return data;
}

void free_binsort(struct binsort_data* data) {
    free(data); // Free the allocated memory, to prevent memory leak
}

const struct record* lookup_binsort(struct binsort_data *data, int64_t needle) {
    int low = 0;
    int high = data -> n - 1;
    // Repeat until the pointers low and high meet each other (Binary sort)
    while (low <= high) {
        int mid = low + (high - low) / 2;

        if (data -> irs[mid].osm_id == needle)
        return data -> irs[mid].record;

        if (data -> irs[mid].osm_id < needle)
        low = mid + 1;

        else
            high = mid - 1;
    }
      return NULL;
}


int main(int argc, char** argv) {
    return id_query_loop(argc, argv,
                    (mk_index_fn)mk_binsort,
                    (free_index_fn)free_binsort,
                    (lookup_fn)lookup_binsort);
}