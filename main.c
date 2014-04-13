/*
 *  Simple test program --  Test sets and prints descriptors in Global
 *                          Descriptor Table (GDT).
 *                          This is i386 specific test.
 */

#include <bsp.h>
#include <stdlib.h>
#include <stdio.h>
#include <stdint.h>
#include <string.h>
#include <libcpu/cpu.h>

#define TRUE   1
#define FALSE  0

void printDesc0(segment_descriptors* desc){
    printf("\nLimit:\t%u\n"   ,(desc->limit_19_16<<16) + desc->limit_15_0);
    printf("Base:\t%u\n"      ,(desc->base_address_31_24<<24) + (desc->base_address_23_16<<16) + desc->base_address_15_0);
    printf("Type:\t%u\n"      ,desc->type);
    printf("No/sys:\t%u\n"    ,desc->descriptor_type);
    printf("Priv:\t%u\n"      ,desc->privilege);
    printf("Presnt:\t%u\n"    ,desc->present);
    printf("Avail:\t%u\n"     ,desc->available);
    printf("64bit:\t%u\n"     ,desc->fixed_value_bits);
    printf("16/32:\t%u\n"     ,desc->operation_size);
    printf("Granul:\t%u\n"    ,desc->granularity);
}

int pow2(uint8_t pow){
    int res = 1;
    while(pow--)res *= 2;
    return res;
}

void printBits(unsigned int bits, uint8_t length){
    uint8_t i;
    for(i = 0;i<length;i++){
        if(i == 8)printf(" ");
        bits = bits & (pow2(length-i)-1);
        printf("%u", bits&pow2(length-i-1)?1:0);
    }
}

void printDesc(segment_descriptors* desc){
    unsigned int base, limit;
    uint8_t* ptr = (uint8_t*)desc;
    base = desc->base_address_15_0 + (desc->base_address_23_16<<16) +
    (desc->base_address_31_24<<24);
    limit = desc->limit_15_0 + (desc->limit_19_16<<16);
    printf("__________________________________________\n");
    printf("|  Base  |G|D|L|A|Limit|P|Pr|S|Type|  Base  |\n");
    printf("| 31-24  |r|B| |v|19-16|r|iv|N|    | 23-16  |\n");
    printf("|");printBits(desc->base_address_31_24, 8);
    printf("|%u|%u|%u|%u|", desc->granularity,
    desc->operation_size, desc->fixed_value_bits, desc->available);
    printBits(desc->limit_19_16,4);
    printf(" |%u|", desc->present);
    printBits(desc->privilege,2);
    printf("|%u|", desc->descriptor_type);
    printBits(desc->type,4);
    printf("|");
    printBits(desc->base_address_23_16,8);
    printf("| %x %x %x %x\n", *(ptr+7), *(ptr+6), *(ptr+5), *(ptr+4));
    printf("_____________________________________________\n");
    printf("|      Base 15-0      |     Limit 15-0      |\n");
    printf("|  ");
    printBits(desc->base_address_15_0,16);
    printf("  |  ");
    printBits(desc->limit_15_0,16);
    printf("  | %x %x %x %x\n", *(ptr+3), *(ptr+2), *(ptr+1), *(ptr+0));
    printf("---------------------------------------------\n");
    printf("Base :\t0x%x\t\t(%u)\n", base, base);
    printf("Limit:\t0x%x\t\t(%u)\t", limit, limit);
    printf("%s\n", desc->granularity?"4kB pages":"bytes");
    if(desc->descriptor_type){
        printf("Type :\t%s\t", desc->type&8?"Code":"Data");
        if(desc->type&8){
            printf("%s\t%s\t", desc->type&4?"Conform   ":"nonConform",
            desc->type&2?"Readable   ":"nonReada   ");
        }else{
            printf("%s\t%s\t", desc->type&4?"ExpandDown":"ExpandUp  ",
            desc->type&2?"Writable   ":"nonWritable");
        }
        printf("%s\n", desc->type&1?"Accessed":"nonAccessed");
    }
}

rtems_task Init(rtems_task_argument ignored)
{
    unsigned                    gdt_limit;
    segment_descriptors*        gdt_entry_tbl;
    uint16_t                    segment_selector;

    printf("%d", GDT_SIZE);
    i386_get_info_from_GDTR (&gdt_entry_tbl, &gdt_limit);
    printf("GDT position: %p\tGDT limit: %d\n", gdt_entry_tbl, gdt_limit);
    for(segment_selector=0;segment_selector<(gdt_limit+1)/8;segment_selector++){
        printf("(%d)", segment_selector);
        printDesc(&gdt_entry_tbl[segment_selector]);
        _IBMPC_inch();
    }
    
//    i386_put_gdt_entry (unsigned short segment_selector, unsigned int base, 
//                        unsigned int limit, segment_descriptors* sd_flags);
    exit(0);
}

/* configuration information */

/* NOTICE: the clock driver is explicitly disabled */
#define CONFIGURE_APPLICATION_DOES_NOT_NEED_CLOCK_DRIVER
#define CONFIGURE_APPLICATION_NEEDS_CONSOLE_DRIVER

#define CONFIGURE_RTEMS_INIT_TASKS_TABLE
#define CONFIGURE_MAXIMUM_TASKS 1

#define CONFIGURE_INIT
#include <rtems/confdefs.h>
/* end of file */
