#include <linux/module.h>
#include <linux/init.h>
#include <linux/mmzone.h>   // for pglist_data, NODE_DATA
#include <linux/mm.h>
#include <linux/topology.h> // for nr_node_ids, node_online

MODULE_LICENSE("GPL");
MODULE_AUTHOR("Mohan Kumar");
MODULE_DESCRIPTION("NUMA Node Info Kernel Module");
MODULE_VERSION("1.0");

static int __init numa_info_init(void)
{
    int nid;

    pr_info("NUMA Info Module Loaded\n");

    for_each_online_node(nid) {
        struct pglist_data *pgdat = NODE_DATA(nid);

        pr_info("Node %d is online\n", nid);
        pr_info("  Total pages: %lu\n", pgdat->node_present_pages);
        pr_info("  Managed pages: %lu\n", pgdat->node_present_pages);
        pr_info("  Zone DMA: free pages = %lu\n",
            zone_page_state(&pgdat->node_zones[ZONE_DMA], NR_FREE_PAGES));
        pr_info("  Zone Normal: free pages = %lu\n",
            zone_page_state(&pgdat->node_zones[ZONE_NORMAL], NR_FREE_PAGES));
#ifdef CONFIG_HIGHMEM
        pr_info("  Zone HighMem: free pages = %lu\n",
            zone_page_state(&pgdat->node_zones[ZONE_HIGHMEM], NR_FREE_PAGES));
#endif
    }

    return 0;
}

static void __exit numa_info_exit(void)
{
    pr_info("NUMA Info Module Unloaded\n");
}

module_init(numa_info_init);
module_exit(numa_info_exit);

