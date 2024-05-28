
#ifndef EFI_LIB_H
#define EFI_LIB_H

#include "efi.h"
#include "clib.h"

#define MAX_LENGTH 22

             // ARGB             
#define ORANGE 0xffffa500
#define CYAN   0xff00ffff
#define RED    0xffff0000
#define GREEN  0xff00ff00
#define BLUE   0xff0000ff
#define PURPLE 0xff800080
#define GRAY   0xff888888
#define WHITE  0xffffffff
#define BLACK  0xff000000

EFI_HANDLE                       ImageHandle;
EFI_SYSTEM_TABLE                *SystemTable;
EFI_STATUS                       ERROR_STATUS;
EFI_GRAPHICS_OUTPUT_PROTOCOL    *gop;
EFI_SIMPLE_FILE_SYSTEM_PROTOCOL *Volume;
EFI_FILE_PROTOCOL               *RootFS;

uint32_t DisplayWidth  = 0;
uint32_t DisplayHeight = 0;

typedef struct BLOCKINFO
{
    void*                  BaseAddress;
    uint64_t               BufferSize;
    uint32_t               ScreenWidth;
    uint32_t               ScreenHeight;
    uint32_t               PixelsPerScanLine;
	void*                  LoaderBaseAddress;
	uint64_t*              LoaderSize;
	EFI_MEMORY_DESCRIPTOR* MMap;
	uint64_t               MMapSize;
	uint64_t               MMapDescriptorSize;
	uint64_t*              rsdp;
} __attribute__((__packed__)) BLOCKINFO;

BLOCKINFO biStruct;

typedef struct {
	uint32_t cols;  // Left / Right
	uint32_t rows;  // Up / Down
} Timer_Context;

EFI_EVENT timer_event;

// From the GNU-EFI
int64_t RtCompareGuid(EFI_GUID* Guid1, EFI_GUID* Guid2)
{
    int32_t *g1, *g2, r;
    g1 = (int32_t *) Guid1;
    g2 = (int32_t *) Guid2;
    r  = g1[0] - g2[0];
    r |= g1[1] - g2[1];
    r |= g1[2] - g2[2];
    r |= g1[3] - g2[3];
    return r;
}

int64_t CompareGuid(EFI_GUID* Guid1, EFI_GUID* Guid2)
{
    return RtCompareGuid (Guid1, Guid2);
}

void Delay(uint64_t d)
{
    // The Stall function is set as microseconds.
    SystemTable->BootServices->Stall(d);
}

void clearScreen()
{
	SystemTable->ConOut->ClearScreen(SystemTable->ConOut);
}

void setTextColor(uint64_t color)
{
	SystemTable->ConOut->SetAttribute(SystemTable->ConOut, color);
}

void SetTextPosition(uint32_t Col, uint32_t Row)
{
    SystemTable->ConOut->SetCursorPosition(SystemTable->ConOut, Col, Row);
}

void printUInt64Digits(uint64_t num, uint64_t base)
{
    char16_t uint64Str[MAX_LENGTH] = {'\0'};
	
	const char16_t* digits = u"0123456789ABCDEF";
	uint64_t i = 0;

	do
	{
		uint64Str[i++] = digits[num % base];
        num /= base;
	} while(num > 0);
	
    uint64Str[i--] = '\0';
	
	for(uint64_t j = 0; j < i; j++, i--)
	{
		char16_t temp = uint64Str[i];
		uint64Str[i]  = uint64Str[j];
		uint64Str[j]  = temp;
	}
	
	SystemTable->ConOut->OutputString(SystemTable->ConOut, uint64Str);
}

void printIntDigits(int32_t num)
{
    char16_t int32Str[12] = {'\0'};
	
	const char16_t* digits = u"0123456789";
	uint64_t i = 0;
	int negative = (num < 0);
	if(negative) {num = -num;}
	
	do
	{
		int32Str[i++] = digits[num % 10];
		num /= 10;
	} while(num > 0);
	
	if(negative) {int32Str[i++] = '-';}
	
	int32Str[i--] = '\0';
	
	for(uint64_t j = 0; j < i; j++, i--)
	{
		char16_t temp = int32Str[i];
		int32Str[i] = int32Str[j];
		int32Str[j] = temp;
	}
	
	SystemTable->ConOut->OutputString(SystemTable->ConOut, int32Str);
}

void wprintf(char16_t* txt, ...)
{
	char16_t charStr[2]   = {'\0'};

	va_list args;
	va_start(args, txt);
	
	for(uint64_t i = 0; txt[i] != '\0'; i++)
	{
		if(txt[i] == '%')
		{
			i++;
			switch(txt[i])
			{
                case 'c': {
                    charStr[0] = (char16_t)va_arg(args, int);
                    SystemTable->ConOut->OutputString(SystemTable->ConOut, charStr);
					break;
                }
				case 's':
				{
					uint16_t* varString = va_arg(args, uint16_t*);
					SystemTable->ConOut->OutputString(SystemTable->ConOut, varString);
					break;
				}
				case 'd':
				{
					int32_t number = va_arg(args, int32_t);
					printIntDigits(number);
					break;
				}
                case 'b':
				{
					uint64_t bin = va_arg(args, uint64_t);
					printUInt64Digits(bin, 2);
					break;
                }
				case 'x':
				{
					uint64_t hex = va_arg(args, uint64_t);
					printUInt64Digits(hex, 16);
					break;
				}
				case 'l':
				{
					if(txt[i+1] == 'l' && txt[i+2] == 'u')
					{
						i+=2;
                        uint64_t num = va_arg(args, uint64_t);
						printUInt64Digits(num, 10);
					}
					break;
				}

				default:
				{
					SystemTable->ConOut->SetAttribute(SystemTable->ConOut, EFI_RED);
				    SystemTable->ConOut->OutputString(SystemTable->ConOut, u"\r\nERROR : Invalid format: %");
					SystemTable->ConOut->OutputString(SystemTable->ConOut, &txt[i]);
					SystemTable->ConOut->OutputString(SystemTable->ConOut, u"\r\n");
					return;
					break;
				}
			}
		} else {
			charStr[0] = txt[i];
			charStr[1] = '\0';
			SystemTable->ConOut->OutputString(SystemTable->ConOut, charStr);
		}
	}

	va_end(args);
}

void HitAnyKey()
{
    SystemTable->ConIn->Reset(SystemTable->ConIn, 1);
    EFI_INPUT_KEY Key;

    do
	{
		Delay(1);
	} while((SystemTable->ConIn->ReadKeyStroke(SystemTable->ConIn, &Key)) == EFI_NOT_READY);
}

void ResetKeyboard()
{
    SystemTable->ConIn->Reset(SystemTable->ConIn, 1);
}

EFI_INPUT_KEY CheckKeystroke;
int32_t GetKey(char16_t key)
{
    if(CheckKeystroke.UnicodeChar == key)
    {
        return 1;
    } else {
        return 0;
    }
}

EFI_STATUS CheckKey()
{
    return SystemTable->ConIn->ReadKeyStroke(SystemTable->ConIn, &CheckKeystroke);
}

EFI_GRAPHICS_OUTPUT_BLT_PIXEL* SetGraphicsColor(uint32_t color)
{
    EFI_GRAPHICS_OUTPUT_BLT_PIXEL* GColor = {0};
    GColor->Reserved = color >> 24;
    GColor->Red      = color >> 16;
    GColor->Green    = color >> 8;
    GColor->Blue     = color;
    return GColor;
}

void MakeRectangle(uint32_t xPos, uint32_t yPos, uint32_t w, uint32_t h, uint32_t c)
{
    uint32_t x;
    uint32_t y      = yPos;
    uint32_t width  = xPos + w;
    uint32_t height = yPos + h;

    for(y = yPos; y <= height; y++)
    {
        for(x = xPos; x <= width; x++)
        {
            *(x + (y * biStruct.PixelsPerScanLine) + (uint32_t*)(biStruct.BaseAddress)) = c;
        }
    }
}

void InitializeGOP()
{
    SystemTable->BootServices->LocateProtocol(&EFI_GRAPHICS_OUTPUT_PROTOCOL_GUID, 0, (void**)&gop);
	
    biStruct.BaseAddress        = (void*)gop->Mode->FrameBufferBase;
    biStruct.BufferSize         = gop->Mode->FrameBufferSize;
    biStruct.ScreenWidth        = gop->Mode->Info->HorizontalResolution;
    biStruct.ScreenHeight       = gop->Mode->Info->VerticalResolution;
    biStruct.PixelsPerScanLine  = gop->Mode->Info->PixelsPerScanLine;
}

// FILE
void InitializeFILESYSTEM()
{
    EFI_LOADED_IMAGE_PROTOCOL *LoadedImage;
	EFI_DEVICE_PATH_PROTOCOL *DevicePath;
	
    if((SystemTable->BootServices->HandleProtocol(ImageHandle, &EFI_LOADED_IMAGE_PROTOCOL_GUID, (void**)&LoadedImage)) == EFI_SUCCESS)
	{
		if((SystemTable->BootServices->HandleProtocol(LoadedImage->DeviceHandle, &EFI_DEVICE_PATH_PROTOCOL_GUID, (void**)&DevicePath)) == EFI_SUCCESS)
		{
			if((SystemTable->BootServices->HandleProtocol(LoadedImage->DeviceHandle, &EFI_SIMPLE_FILE_SYSTEM_PROTOCOL_GUID, (void**)&Volume)) == EFI_SUCCESS)
			{
				if((Volume->OpenVolume(Volume, &RootFS)) != EFI_SUCCESS)
				{
					wprintf(u"Loading Root File System FAILED!\r\n");
				}
			} else {
		        wprintf(u"Volume Handle FAILED!\r\n");
	        }
		} else {
		    wprintf(u"DevicePath FAILED!\r\n");
	    }
	} else {
		wprintf(u"LoadedImage FAILED!\r\n");
	}
}

uint64_t* GetFileSize (EFI_FILE_PROTOCOL* FileNameHandle)
{
    uint64_t* Size = 0;
    FileNameHandle->SetPosition(FileNameHandle, 0xFFFFFFFFFFFFFFFFULL);
    FileNameHandle->GetPosition(FileNameHandle, Size);
	FileNameHandle->SetPosition(FileNameHandle, 0);
    return Size;
}

void closeFile(EFI_FILE_PROTOCOL* FileHandle)
{
    FileHandle->Close(FileHandle);
}

EFI_FILE_PROTOCOL* openFile(uint16_t* FileName)
{
    Volume->OpenVolume(Volume, &RootFS);
    EFI_FILE_PROTOCOL* FileHandle = NULL;
    RootFS->Open(RootFS, &FileHandle, FileName, 0x0000000000000001, 0);
    
    return FileHandle;
}

void* readFile(uint16_t* FileName, uint64_t* entry)
{
	void* OS_Buffer = NULL;
    EFI_FILE_PROTOCOL* m_FileHandle = openFile(FileName);

	biStruct.LoaderSize = 0;
	m_FileHandle->SetPosition(m_FileHandle, 0xFFFFFFFFFFFFFFFFULL);
	m_FileHandle->GetPosition(m_FileHandle, biStruct.LoaderSize);
	m_FileHandle->SetPosition(m_FileHandle, 0);
	SystemTable->BootServices->AllocatePool(EfiLoaderData, *biStruct.LoaderSize, (void**)&OS_Buffer);
	m_FileHandle->Read(m_FileHandle, biStruct.LoaderSize, OS_Buffer);
	
	if(entry != NULL)
	{
		uint8_t* OSloader = (uint8_t*)OS_Buffer;
	
		uint8_t p1,p2,p3,p4;
		p1 = *OSloader;
		OSloader+=1;
		p2 = *OSloader;
		OSloader+=1;
		p3 = *OSloader;
		OSloader+=1;
		p4 = *OSloader;

		if(p1 == 100 && p2 == 134)
		{
			OSloader+=37;
			p1 = *OSloader;
			OSloader+=1;
			p2 = *OSloader;
			OSloader+=1;
			p3 = *OSloader;
			OSloader+=1;
			p4 = *OSloader;

			// 86 64    <---- BIN
			// 45 4C 46 <---- ELF
			*entry = (p4 << 24) | (p3 << 16) | (p2 << 8) | p1;
		} else {
			setTextColor(EFI_RED);
			wprintf(u"ERROR : Unable to find ENTRY POINT !\r\n");
			setTextColor(EFI_GREEN);
		}
		m_FileHandle->SetPosition(m_FileHandle, *entry);
		closeFile(m_FileHandle);
	}

	return OS_Buffer;
}

void freeFileMemory(void* fileHandle)
{
	SystemTable->BootServices->FreePool(fileHandle);
}

void DrawLCARSLogoBMPImage()
{
	void* imageFile = NULL;
    DisplayWidth  = 512;
    DisplayHeight = 512;
    imageFile = readFile(u"EFI\\Boot\\loslogo.bmp", NULL);

	EFI_GRAPHICS_OUTPUT_BLT_PIXEL GColor;

    GColor.Reserved = 0xFF;
	uint8_t* tempColor = (uint8_t*)imageFile;
  
    for(int i = 0; i < 54; i++)
	{
		tempColor++;
	}
	
    for(uint32_t y = DisplayHeight; y > 0; y--)
  	{
  		for(uint32_t x = 0; x < DisplayWidth; x++)
  		{
  			GColor.Blue    = *tempColor;
  			tempColor++;
            GColor.Green   = *tempColor;
  			tempColor++;
            GColor.Red     = *tempColor;
  			tempColor++;
  			gop->Blt(gop, &GColor, EfiBltVideoFill, 0, 0, x, y, 1, 1, 0);
  		}
  	}

	freeFileMemory(imageFile);
}

void CPU_HALT()
{
	while(1)
	{
		Delay(1);
	};
}

void InitEFI(EFI_HANDLE handle, EFI_SYSTEM_TABLE  *table)
{
	ImageHandle   = handle;
	SystemTable   = table;
	ERROR_STATUS  = 0;
	SystemTable->ConOut->Reset(SystemTable->ConOut, 1);
	
	setTextColor(EFI_GREEN);

	InitializeGOP();

	InitializeFILESYSTEM();

	DrawLCARSLogoBMPImage();
}

#endif  // EFI_LIB_H

