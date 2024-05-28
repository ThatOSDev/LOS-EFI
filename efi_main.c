
/*
 BIG THANK YOU TO THE FOLLOWING PEOPLE WHO HELPED ME TO MAKE THIS OS POSSIBLE
    ANGEL
    QUESO
    JESTERJUNK
    STRYKER
    AND ALL OF MY YOUTUBE SUBSCRIBERS
	
*/

#include "EFI/efilib.h"
#include "EFI/gdt.h"

EFI_STATUS efi_main(EFI_HANDLE image, EFI_SYSTEM_TABLE *system)
{
	InitEFI(image, system);
	
	HitAnyKey();
	
    uint64_t ENTRY_POINT = 0;
	biStruct.LoaderBaseAddress = readFile(u"los-efi\\loader.bin", &ENTRY_POINT);
    uint8_t* OSloader = (uint8_t*)biStruct.LoaderBaseAddress;

	if(biStruct.LoaderBaseAddress == NULL)
	{
		setTextColor(EFI_RED);
		wprintf(u"ERROR : biStruct.LoaderBaseAddress is NULL !\r\n");
		setTextColor(EFI_GREEN);
	}
	
	if(ENTRY_POINT <= 0)
	{
		setTextColor(EFI_RED);
		wprintf(u"ERROR : Entry Point is ZERO !\r\n");
		setTextColor(EFI_GREEN);
	}

	// RSDP
	EFI_CONFIGURATION_TABLE* configTable = SystemTable->ConfigurationTable;
	unsigned long long* tempRSDP = NULL;
	for(uint64_t index = 0; index < SystemTable->NumberOfTableEntries; index++)
	{
		if(CompareGuid(&configTable[index].VendorGuid, &ACPI_20_TABLE_GUID))
		{
			if(strcmp((char*)"RSD PTR ", (char*)configTable->VendorTable) == 0)
			{
				tempRSDP = (void*)configTable->VendorTable;
			}
		}
		configTable++;
	}
	
    // Memory Map
    uint64_t                MemoryMapSize = 0;
    EFI_MEMORY_DESCRIPTOR  *MemoryMap = NULL;
    uint64_t                MapKey = 0;
    uint64_t                DescriptorSize = 0;
    uint32_t                DescriptorVersion = 0;

	SystemTable->BootServices->GetMemoryMap(&MemoryMapSize, MemoryMap, &MapKey, &DescriptorSize, &DescriptorVersion);
	MemoryMapSize += 2 * DescriptorSize;
	SystemTable->BootServices->AllocatePool(2, MemoryMapSize, (void **)&MemoryMap);
	SystemTable->BootServices->GetMemoryMap(&MemoryMapSize, MemoryMap, &MapKey, &DescriptorSize, &DescriptorVersion);

	biStruct.MMap               = MemoryMap;
	biStruct.MMapSize           = MemoryMapSize;
	biStruct.MMapDescriptorSize = DescriptorSize;
	biStruct.rsdp               = tempRSDP;

    void (*KernelBinFile)(BLOCKINFO*) = ((__attribute__((ms_abi)) void (*)(BLOCKINFO*)) &OSloader[ENTRY_POINT]);

    SystemTable->BootServices->ExitBootServices(ImageHandle, MapKey);
	
	gdt_init();

    KernelBinFile(&biStruct);

	while(1){__asm__ ("hlt");}
	
    return ERROR_STATUS;
}
