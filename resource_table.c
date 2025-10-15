#include <stdint.h>
#include <stddef.h>
/* refer https://www.kernel.org/doc/Documentation/remoteproc.txt for resource table*/



/* check this usage 
struct fw_rsc_carveout {
 u32 type;//from struct fw_rsc_hdr
 u32 da;
 u32 pa;
 u32 len;
 u32 flags;
 u32 reserved;
 u8 name[32];
};
*/


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
  } __packed;

/**
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
  } __packed;
  /**
   * enum fw_resource_type - types of resource entries
   *
   * @RSC_CARVEOUT:   request for allocation of a physically contiguous
   *		    memory region.
   * @RSC_DEVMEM:     request to iommu_map a memory-based peripheral.
   * @RSC_TRACE:	    announces the availability of a trace buffer into which
   *		    the remote processor will be writing logs.
   * @RSC_VDEV:       declare support for a virtio device, and serve as its
   *		    virtio header.
   * @RSC_LAST:       just keep this one at the end
   * @RSC_VENDOR_START:	start of the vendor specific resource types range
   * @RSC_VENDOR_END:	end of the vendor specific resource types range
   *
   * Please note that these values are used as indices to the rproc_handle_rsc
   * lookup table, so please keep them sane. Moreover, @RSC_LAST is used to
   * check the validity of an index before the lookup table is accessed, so
   * please update it as needed.
   */
  enum fw_resource_type {
	RSC_CARVEOUT		= 0,
	RSC_DEVMEM		= 1,
	RSC_TRACE		= 2,
	RSC_VDEV		= 3,
	RSC_LAST		= 4,
	RSC_VENDOR_START	= 128,
	RSC_VENDOR_END		= 512,
  };

// --- Define the Resource Table ---
struct resource_table __attribute__((section(".resource_table"), used)) resource_table = {
    .ver = 1,            // must be 1 for the remoteproc       
    .num = 1,               
    .reserved = {0, 0},  
};





/* chat gpt generated code */


//#include <stdint.h>
//#include <stddef.h>
//
//// --- Resource Table Definitions (Standard VIRTIO types) ---
//
//#define VIRTIO_ID_CONSOLE 3
//#define VIRTIO_ID_RPMSG   7
//
//// Resource types used by remoteproc
//#define RSC_CARVEOUT      0
//#define RSC_DEVMEM        1
//#define RSC_TRACE         2
//#define RSC_VDEV          3
//
//// --- Structure Definitions ---
//
///**
// * @brief Header for the entire resource table structure.
// */
//struct resource_table {
//    uint32_t ver;           // Version number (must be 1)
//    uint32_t num_rsc;       // Number of resource entries that follow
//    uint32_t reserved[2];
//    uint32_t offset[0];     // Array of resource offsets (placed immediately after header)
//} __attribute__((packed));
//
///**
// * @brief Structure for a CARVEOUT resource.
// * Used to request a block of physical memory to be reserved and shared.
// */
//struct fw_rsc_carveout {
//    uint32_t type;          // RSC_CARVEOUT (0)
//    uint32_t da;            // Device address (Local R5F address)
//    uint32_t pa;            // Physical address (Global System address - for remoteproc)
//    uint32_t len;           // Length of the memory region
//    uint32_t flags;         // Resource flags (e.g., used to signal memory is shared)
//    uint32_t reserved[2];
//    char name[32];          // Name of the resource
//} __attribute__((packed));
//
//// --- Resource Definitions ---
//
//// Define a CARVEOUT resource for the shared DRAM region.
//// This is critical because your linker script defined the DRAM section.
//
//// NOTE: The addresses used here MUST correspond to the GLOBAL addresses
//// used in your linker_script.ld (0xA0001000) for the PA (Physical Address).
//// The DA (Device Address) is usually 0x0 for shared DRAM or the same as PA.
//// Given the AM67A architecture, we use the GLOBAL address for both DA and PA
//// for shared DRAM, as it resides outside the R5F's local TCM.
//
//// DRAM (rwx) : ORIGIN = (0xA0001000), LENGTH = (14M)
//struct fw_rsc_carveout dram_carveout = {
//    .type = RSC_CARVEOUT,
//    .da = 0xA0001000,       // Global Device Address (used by the R5F when accessing shared memory)
//    .pa = 0xA0001000,       // Physical Address (used by remoteproc for mapping)
//    .len = 0x00E00000,      // 14M = 14 * 1024 * 1024 = 14680064 = 0xE00000 bytes
//    .flags = 0,             // No special flags
//    .reserved = {0, 0},
//    .name = "Shared DRAM",
//};
//
//
//// --- The Final Resource Table ---
//
//// Array of pointers to the defined resources
//const void * const resources[] = {
//    &dram_carveout,
//};
//
//// Calculate the number of resources in the array
//#define NUM_RESOURCES (sizeof(resources) / sizeof(resources[0]))
//
//
//// --- Resource Table Definition ---
//
//// This structure MUST be placed in the .resource_table section, 
//// which your linker script aligns to 4KB and places in the RSC_TABLE memory.
//struct resource_table __attribute__((section(".resource_table"), used)) resource_table = {
//    .ver = 1,
//    .num_rsc = NUM_RESOURCES, // Now 1 resource
//    .reserved = {0, 0},
//    
//    // The offset array immediately follows the reserved fields.
//    // The linker places the resources array contents here automatically.
//    // We use C's flexible array member combined with the compiler's help.
//    .offset = {
//        // Offset of the DRAM CARVEOUT entry from the start of resource_table
//        offsetof(struct resource_table, offset[0]) + (NUM_RESOURCES * sizeof(uint32_t)),
//    },
//};


