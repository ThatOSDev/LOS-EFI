# LOS-EFI
# **LCARS - Library Computer Access Retrieval System**  
  
LCARS is based on the idea from the TV show Star Trek - Next Generation. Those panels you see on the bridge of the enterprise are known as LCARS displays. Others have attempted to make this, but they made a shell over windows or Linux. Mine is *FROM SCRATCH*, thus not using a linux kernel or anything related to MS Windows.  
  
*NOTE 1*: I make NO MONEY on this project. YES, I'm talking to you CBS ! So you can't sue me or force a take down !  
  
  
# BIG THANK YOU TO THE FOLLOWING PEOPLE WHO HELPED ME TO MAKE THIS OS POSSIBLE
	- ANGEL  
	- QUESO  
	- JESTERJUNK  
	- STRYKER  
	- AND ALL OF MY YOUTUBE SUBSCRIBERS  
  
This Operating System is in development using the EFI as the boot loader.  
  
*UPDATED : Fixed GDT code to finally work with GCC 13.2. You can now use the GCC from the link below. Although I use -std=c17, you can use -std=c11 and -std=c2x. However -std=c99 still fails.*  
Using GCC 13.2 from this link.  
https://nuwen.net/mingw.html 
  
*OSFMount*  
https://www.osforensics.com/tools/mount-disk-images.html  
  
*QEMU*  
https://www.qemu.org/  
  
*NOTE 2* : All three of the above programs are in my windows path. You can do a search on how to do that.  
  
*NOTE 3* : Do to the folks who like to cause problems for other youtubers, I am showing my OFFICIAL youtube channel here. Do NOT trust the scammers who pretend to be me !  
https://www.youtube.com/@ThatOSDev/videos  
  
*NOTE 4* : Although this is an LCARS Operating System, there will be a simplistic Windows-Like GUI as well. Flat colors for the windows, nothing fancy. This can be improved over time. But this is mainly for those who do not like the LCARS interface.  
  
  
# ** IMPLEMENTED EFI **  

	* EFI Text
	* EFI Keyboard
	* EFI Graphics (GOP)
	* EFI Rectangle and Line Drawing
	* EFI Display Screen Resolutions
	* EFI Based BMP Image Loading - Displays my own Wallpaper now
	* EFI RSDP
	* EFI TPL Timing
	* EFI Delay - Microseconds
	* EFI Based PRINTF with Variadic Functions
	* EFI File Loading and executing  
  
# ** IMPLEMENTED LOADER / KERNEL **  

	* Basic Graphics  
	* A Basic home-brew FONT so I can test with. (I want to eventually load a TTF.)  
	* A Basic Print. (NOTE : I will be updating this later to printf using Variadic Functions.)
	* GDT loaded from BOOTX64.EFI. But seems to be working. Will know more once paging begins.  
	* MMap seems to be working now. We can total up conventional memory.  
	* Bitmap manipulation is working.  
  
# ** TO BE ADDED **  

	* Paging  
	* Mouse & Gesture ( Touch Screen ) support
	* FNT - Bitmap Font File
	* TTF - True Type Font
	* Reading Raw Sectors from Drive  
	* Writing Raw Sectors to Drive  
	* Format and Partition drive with the EFI Environment  
	* Operating System Installation with just the EFI Environment  
	* Text Editor  
	* Assembler  
	* Networking  
	* Blutooth  
	* Simple Web Browser  
	* Getting some sort of C compiler into the OS  

![Current Progress](progress1.png)  
 
