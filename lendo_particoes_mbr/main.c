#include <stdio.h>
#include <stdlib.h>

#pragma pack(push,1)
typedef struct {
    unsigned char boot;
    unsigned char chs_first[3]; 
    unsigned char type;
    unsigned char chs_last[3];
    unsigned int lba_first;
    unsigned int sectors;
} Partition;

struct MBR {
    unsigned char code[440];
    unsigned int disk_signature;
    unsigned short unused;
    Partition partitions[4];
    unsigned short signature;
};
#pragma pack(pop)

int main() {
    FILE *fp;
    struct MBR mbr;

    fp = fopen("mbr_copy.bin", "rb");
    if (fp == NULL) {
        printf("Error opening file!\n");
        return 1;
    }
    printf("Open file success!\n");

    int size_read = fread(&mbr, sizeof(struct MBR), 1, fp);
    if (size_read <= 0) {
        printf("Error when reading file\n");
        return 1;
    }

    for (int i = 0; i < 4; i++) {
        Partition p = mbr.partitions[i];
        printf("\nPartition %d:\n", i + 1);
        printf("\tBoot: 0x%02x\n", p.boot);
        printf("\tStart: %u\n", p.lba_first);
	printf("\tEnd: %u\n", (p.lba_first + p.sectors));
        printf("\tSectors: %u\n", p.sectors);
	printf("\tSize: %.1fGB\n", (double)((p.sectors * 512.0)/1073741824.0L));
	printf("\tType: 0x%02x\n", p.type);
    }

    fclose(fp);
    return 0;
}
