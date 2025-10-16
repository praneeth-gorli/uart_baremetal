/* refer https://www.kernel.org/doc/Documentation/remoteproc.txt for resource table*/

#include <stdint.h>
#include <stddef.h>

#define NUM_RESOURCES     (sizeof(resources) / sizeof(resources[0]))
#define RSC_CARVEOUT      0

/**
  * @brief Structure for a CARVEOUT resource.
  * Used to request a block of physical memory to be reserved and shared.
  */
struct fw_rsc_carveout {
    uint32_t type;          // //from struct fw_rsc_hdr like RSC_CARVEOUT (0)
    uint32_t da;            // Device address (Local R5F address)
    uint32_t pa;            // Physical address (Global System address - for remoteproc)
    uint32_t len;           // Length of the memory region
    uint32_t flags;         // Resource flags (e.g., used to signal memory is shared)
    uint32_t reserved[2];
    char name[32];          // Name of the resource
} __attribute__((packed));


/**
 * struct resource_table - firmware resource table header
 * @ver: version number 
 * @num: number of resource entries
 * @reserved: reserved (must be zero)
 * @offset: array of offsets pointing at the various resource entries
 *
 * The header of the resource table, as expressed by this structure,
 * contains a version number (should we need to change this format in the
 * future), the number of available resource entries, and their offsets
 * in the table.
 */
struct resource_table {
    uint32_t ver;
    uint32_t num;
    uint32_t reserved[2];
    uint32_t offset[0];
} __attribute__((packed));

/*
 * struct fw_rsc_hdr - firmware resource entry header
 * @type: resource type
 * @data: resource data
 *
 * Every resource entry begins with a 'struct fw_rsc_hdr' header providing
 * its @type. The content of the entry itself will immediately follow
 * this header, and it should be parsed according to the resource type.
 */
struct fw_rsc_hdr {
    uint32_t type;
    uint8_t data[0];
} __attribute__((packed));


// DRAM ( rwx ) : ORIGIN = (0x00A2000000), LENGTH = (14M)
struct fw_rsc_carveout dram_carveout = {
    .type = RSC_CARVEOUT,
    .da = 0xA2000000,       // Device Address (used by the R5F when accessing shared memory)
    .pa = 0xA2000000,       // Physical Address (used by remoteproc for mapping)
    .len = 0x00E00000,      // 14M = 14 * 1024 * 1024 = 14680064 = 0xE00000 bytes
    .flags = 0,             // No special flags
    .reserved = {0, 0},
    .name = "DRAM",
};


//  ATCM ( rwx ) : ORIGIN = (0x0), LENGTH = (32K)
struct fw_rsc_carveout atcm_carveout = {
    .type = RSC_CARVEOUT,
    .da = 0x00000000,       // Device Address (used by the R5F when accessing memory)
    .pa = 0x78400000,       // Physical Address (used by remoteproc for mapping)
    .len = 32768,           // 32K = 32*1024 = 32768 bytes
    .flags = 0,             // No special flags
    .reserved = {0, 0},
    .name = "ATCM",
};

// BTCM ( rwx ) : ORIGIN = (0x41010000), LENGTH = (32K)
struct fw_rsc_carveout btcm_carveout = {
    .type = RSC_CARVEOUT,
    .da = 0x78500000,       // Device Address (used by the R5F when accessing shared memory)
    .pa = 0x41010000,       // Physical Address (used by remoteproc for mapping)
    .len = 32768,           // 32K = 32*1024 = 32768 bytes
    .flags = 0,             // No special flags
    .reserved = {0, 0},
    .name = "BTCM",
};

// Array of pointers to the defined resources
const void * const resources[] = {
    &dram_carveout,&atcm_carveout,&btcm_carveout
};


// --- Define the Resource Table ---
struct resource_table __attribute__((section(".resource_table"), used)) resource_table = {
    .ver = 1,            // must be 1 for the remoteproc       
    .num = 1,            // count of number of resources   
    .reserved = {0, 0},  // reserved must be 0
    .offset =  offsetof(struct resource_table, offset[0]) + (NUM_RESOURCES * sizeof(uint32_t)), 
};





