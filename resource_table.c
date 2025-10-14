#include <stdint.h>
#include <stddef.h>

// --- Standard Resource Table Definitions ---
#define RP_RSC_TYPE_NULL        0
#define RP_RSC_TYPE_CARVEOUT    1               // Memory Carveout Resource


struct rsc_btcm_carveout {
    uint32_t type;
    uint32_t da;
    uint32_t pa;
    uint32_t len;
    uint32_t flags;
    uint8_t name[32];
};

// Resource Table structure definition (must match kernel definitions)
struct resource_table {
    uint32_t ver;
    uint32_t num_rsc;
    uint32_t reserved[2];
    uint32_t offset[0]; // array of resource offsets
    struct rsc_btcm_carveout rsc_btcm;
};


// --- Define the Resource Table ---
// The offsets array must point to the start of each resource structure.
struct resource_table __attribute__((section(".resource_table"), used)) resource_table = {
    .ver = 1,                   
    .num_rsc = 1,               
    .reserved = {0, 0},
    .offset = {
        (uint32_t)offsetof(struct resource_table, rsc_btcm),
    },
    // The rsc_btcm structure will follow immediately after the offset array in memory
  .rsc_btcm = {              
        .type  = RP_RSC_TYPE_CARVEOUT,
        .da    = 0x41010000,    // Device address (BTCM VMA)
        .pa    = 0x41010000,    // Physical address (typically same as DA for TCM)
        .len   = 0x8000,        // 32 KB (0x8000 bytes)
        .flags = 0,
        .name  = "BTCM"         // Name is optional but helpful
    },  
};

