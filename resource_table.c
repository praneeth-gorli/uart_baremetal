#include <stdint.h>
#include <stddef.h>

// --- Standard Resource Table Definitions ---
#define RP_RSC_TYPE_NULL        0
#define RP_RSC_TYPE_CARVEOUT    1               // Memory Carveout Resource



// Resource Table structure definition (must match kernel definitions)
struct resource_table {
    uint32_t ver;
    uint32_t num_rsc;
    uint32_t reserved[2];
    uint32_t offset[0]; // array of resource offsets
};


// --- Define the Resource Table ---
// The offsets array must point to the start of each resource structure.
struct resource_table __attribute__((section(".resource_table"), used)) resource_table = {
    .ver = 1,                   
    .num_rsc = 0,               
    .reserved = {0, 0},  
};

