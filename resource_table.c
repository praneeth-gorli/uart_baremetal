#include <stdint.h>
#include <stddef.h>

// --- Standard Resource Table Definitions ---
#define RP_RSC_TABLE_MAGIC      0x4F4C4152      // "RALA" (backwards)
#define RP_RSC_TYPE_NULL        0
#define RP_RSC_TYPE_CARVEOUT    1               // Memory Carveout Resource
#define RP_RSC_TYPE_TRACE       2

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

// Carveout resource definition
struct fw_rsc_carveout {
    uint32_t type;
    uint32_t da;
    uint32_t pa;
    uint32_t len;
    uint32_t flags;
    uint32_t reserved;
    uint8_t name[32];
};


// --- Define Resources ---
// 1. CARVEOUT for BTCM (0x41010000, 32KB)
// DA (Device Address): The local address on the R5F (0x41010000)
// PA (Physical Address): The physical address for the host/DDR (usually the same in this context)
struct fw_rsc_carveout rsc_btcm __attribute__((aligned(4))) = {
    .type   = RP_RSC_TYPE_CARVEOUT,
    .da     = 0x41010000,       // BTCM start address
    .pa     = 0x41010000,       // Physical address (same as DA for TCM)
    .len    = 0x8000,           // 32 KB
    .flags  = 0,
    .reserved = 0,
    .name   = "r5f-btcm",
};

// --- Define the Resource Table ---
// The offsets array must point to the start of each resource structure.
// NOTE: __attribute__((section(".resource_table"), used)) is essential
struct resource_table __attribute__((section(".resource_table"), used)) resource_table = {
    .ver = 1,                   // <-- CRITICAL FIX: Change to Version 1
    .num_rsc = 0,               // One resource entry (the BTCM carveout)
    .reserved = {0, 0},
    .offset = {
        (uint32_t)offsetof(struct resource_table, rsc_btcm),
    },
    // The rsc_btcm structure will follow immediately after the offset array in memory
};

