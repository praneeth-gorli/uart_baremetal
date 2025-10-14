#include <stdint.h>
#include <stddef.h>


// Resource Table structure definition 
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

