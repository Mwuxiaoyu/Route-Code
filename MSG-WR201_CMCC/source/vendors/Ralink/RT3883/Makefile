KERNELZ = $(IMAGEDIR)/zImage
SWID =  $(shell date +%y%m%d)
RAMDISK  = $(IMAGEDIR)/ramdisk
IMAGE = $(IMAGEDIR)/$(USER)-$(SWID)_uImage.bin
ROMFS_DIR = $(ROOTDIR)/romfs

#ifeq ($(CONFIG_IP1725), y)
#UTTVERSION = 1.0.0
#UTTARCH = SR
#UTTMODEL = 3050E
#else
#UTTVERSION = 1.0.0
#UTTARCH = nv
#UTTMODEL = 3520G
#endif

RAMDISK_SIZE  := $(shell echo $(CONFIG_BLK_DEV_RAM_SIZE))
MKIMAGE_DIR = $(ROOTDIR)/user/mkimage
CUR_DIR = $(shell pwd)
COMP = lzma

ROMFS_DIRS = \
	tmp \
	bin \
	dev dev/pts \
	home \
	etc \
	etc_ro etc_ro/web etc_ro/ppp etc_ro/ppp/peers etc_ro/ppp/plugins etc_ro/ppp/3g \
	etc_ro/wlan etc_ro/Wireless etc_ro/xml etc_ro/usb \
	etc_ro/linuxigd etc_ro/Wireless/RT2860AP etc_ro/Wireless/RT61AP etc_ro/Wireless/iNIC \
	lib lib/modules lib/ipsec \
	mnt \
	proc \
	sbin \
	sys \
	media \
	usr usr/bin usr/sbin usr/codepages \
	var 

clean:
	rm -f mkimage

romfs: 
	[ -d $(ROMFSDIR)/$$i ] || mkdir -p $(ROMFSDIR)
	for i in $(ROMFS_DIRS); do \
		[ -d $(ROMFSDIR)/$$i ] || mkdir -p $(ROMFSDIR)/$$i; \
	done
	#generate device node
ifneq	($(LINUXDIR),linux-2.4.x)
	@if [ \! -d $(ROMFSDIR)/dev ];then mkdir -p $(ROMFSDIR)/dev;fi;
#./makedevlinks $(ROMFSDIR)
endif
	$(ROMFSINST) /etc_ro/rcS
	$(ROMFSINST) /etc_ro/inittab
	$(ROMFSINST) /etc_ro/motd
	$(ROMFSINST) /etc/fstab

#	$(ROMFSINST) -e CONFIG_RALINK_RT3052_2T2R /etc_ro/wlan/RT3052_AP_2T2R_V1_1.bin
#	$(ROMFSINST) -e CONFIG_RALINK_RT3051_1T2R /etc_ro/wlan/RT3051_AP_1T2R_V1_0.bin
#	$(ROMFSINST) -e CONFIG_RALINK_RT3050_1T1R /etc_ro/wlan/RT3050_AP_1T1R_V1_0.bin
#
#	$(ROMFSINST) /etc_ro/Wireless/RT2860AP/RT2860_default_novlan
#	$(ROMFSINST) /etc_ro/Wireless/RT2860AP/RT2860_default_vlan
#	$(ROMFSINST) /etc_ro/Wireless/RT2860AP/default_conf

romfs.post:: 
	[ -d $(IMAGEDIR) ] || mkdir -p $(IMAGEDIR)
#	cp -f $(ROOTDIR)/libresolv.so.0 $(ROOTDIR)/romfs/lib/
	
	#Squash File System
	 rm -f $(RAMDISK)  
ifeq	($(LINUXDIR),linux-2.4.x)
	$(CONFIG_CROSS_COMPILER_PATH)/mksquashfs_lzma-3.0 $(ROMFSDIR) $(RAMDISK)
else
	#linux2.6.x support squashfs with lzma
ifeq	($(CONFIG_BLK_DEV_INITRD),)
	$(CONFIG_CROSS_COMPILER_PATH)/mksquashfs_lzma-3.2 $(ROMFSDIR) $(RAMDISK)
endif
endif

ifeq	($(CONFIG_BLK_DEV_INITRD),y)
ifeq	($(LINUXDIR),linux-2.4.x)
	gzip -9 -f $(RAMDISK)
	cp $(RAMDISK).gz $(ROOTDIR)/$(LINUXDIR)/arch/mips/ramdisk/ramdisk.gz 2>/dev/null
endif
endif


mkimage: $(MKIMAGE_DIR)/crc32.c $(MKIMAGE_DIR)/mkimage.c 
	$(HOSTCC) $(HOST_CFLAGS) -DUSE_HOSTCC -o $@ -I$(MKIMAGE_DIR)/include $^ 

image: mkimage
	$(CROSS_COMPILE)objcopy -O binary -R .note -R .comment -S $(ROOTDIR)/$(LINUXDIR)/vmlinux  $(KERNELZ)
	cd $(IMAGEDIR) ; rm -f $(KERNELZ).*; $(COMP) -9 -f -S .$(COMP) $(KERNELZ)

ifeq	($(CONFIG_BLK_DEV_INITRD),)
ifeq	($(CONFIG_ROOTFS_IN_FLASH_NO_PADDING),)
	@#============================================================
	@# uboot hdr(64B) + mtdblock0(192KB) + mtdblock1(64KB) + 
	@# mtdblock2(64KB) + mtdblock3 + mtdblock4 + mtdblock5
	@# 
	@# Padding size of mtdblock3 is equal to "MTD_KRN_PART_SIZE 
	@# - 64B - the size of compressed kernel image"
	@#
	@# In Other Words:
	@# MTD Kernel Partition Size is MUST bigger than 
	@# "Original Kernel Image Size + 64B"
	@#============================================================
	#========<< This is README not Warning Message>>==============
	# Please make sure that original kernel image size can't 
	# bigger or equal to Kernel Partition Size. 
	#
	# For Example:
	# 
	# Original kernel image size is 563421 Bytes(0x898DD Bytes).
	# Flash sector size is 0x10000, so MTD kernel 
	# partition size MUST bigger than 0x90000.
	#
	# #make menuconfig
	#
	# Kernel/Library/Defaults Selection  --->
	#   [*] Customize Kernel Settings (NEW)
	#      Machine selection  --->
	#        (90000) MTD Kernel Partition Size (Unit:Bytes)
	#=============================================================
	# Original Kernel Image Size
	  @wc -c $(KERNELZ).$(COMP)
	# Padded Kernel Image Size 
ifeq    ($(findstring 0x, $(CONFIG_MTD_KERNEL_PART_SIZ)),0x)
	  @SIZE=`wc -c $(KERNELZ).$(COMP) | awk '{ print $$1 }'` ; \
	   MTD_KRN_PART_SIZE=`printf "%d" $(CONFIG_MTD_KERNEL_PART_SIZ)` ; \
	   PAD=`expr $$MTD_KRN_PART_SIZE - 64 - $$SIZE` ; \
	   dd if=/dev/zero count=1 bs=$$PAD 2> /dev/null | tr \\000 \\377 >> $(KERNELZ).$(COMP)
else
	  @SIZE=`wc -c $(KERNELZ).$(COMP) | awk '{ print $$1 }'` ; \
	   MTD_KRN_PART_SIZE=`printf "%d" 0x$(CONFIG_MTD_KERNEL_PART_SIZ)` ; \
	   PAD=`expr $$MTD_KRN_PART_SIZE - 64 - $$SIZE` ; \
	   dd if=/dev/zero count=1 bs=$$PAD 2> /dev/null | tr \\000 \\377 >> $(KERNELZ).$(COMP)
endif
endif
	@wc -c $(KERNELZ).$(COMP)
	# Original RootFs Size 
	  @du -sb $(ROMFSDIR)
	# Compressed RootFs Size 
	  @wc -c $(RAMDISK)
	# Padded Kernel Image + Compressed Rootfs Size
	  @cat $(RAMDISK) >> $(KERNELZ).$(COMP) 
	  @wc -c $(KERNELZ).$(COMP)
	#===========================================
endif

ifeq	($(CONFIG_ROOTFS_IN_FLASH_NO_PADDING),y)
	@cd $(IMAGEDIR) ; \
	ISIZE=`wc -c $(KERNELZ).$(COMP) | awk '{print $$1}'` ; \
	RSIZE=`wc -c $(RAMDISK) | awk '{print $$1}'` ; \
	KRN_SIZE=`expr $$ISIZE - $$RSIZE + 64` ; \
	ENTRY=`readelf -h $(ROOTDIR)/$(LINUXDIR)/vmlinux | grep "Entry" | awk '{print $$4}'` ; \
	$(CUR_DIR)/mkimage -A mips -O linux -T kernel -C $(COMP) -a 80000000 -e $$ENTRY -k $$KRN_SIZE -n "Linux Kernel Image" -d $(KERNELZ).$(COMP) $(IMAGE)
else
	$(CUR_DIR)/mkimage -A mips -O linux -T kernel -C $(COMP) -a 80060000 -e $(shell readelf -h $(ROOTDIR)/$(LINUXDIR)/vmlinux | grep "Entry" | awk '{print $$4}') -n "Linux Kernel Image"  -d $(KERNELZ).$(COMP) $(IMAGE)
	@cd $(IMAGEDIR) ; $(CUR_DIR)/makesoft $(UTTARCH) $(IMAGEDIR)/$(UTTARCH)$(UTTVERSION)-$(SWID).bin $(shell readelf -h $(ROOTDIR)/$(LINUXDIR)/vmlinux | grep "Entry" | awk '{print $$4}') 0x80060000 linux $(KERNELZ).$(COMP) $(UTTVERSION) $(UTTHWMODEL)
#	@cd $(IMAGEDIR) ; $(CUR_DIR)/mkimage -A mips -O linux -T kernel -C $(COMP) -a 80000000 -e $(shell readelf -h $(ROOTDIR)/$(LINUXDIR)/vmlinux | grep "Entry" | awk '{print $$4}') -n "Linux Kernel Image"  -d $(KERNELZ).$(COMP) $(UTTARCH)$(UTTVERSION)-$(SWID).bin
	@cp $(IMAGEDIR)/$(UTTARCH)$(UTTVERSION)-$(SWID).bin $(UTT_MACHINE)/bin
	@rm $(IMAGEDIR)/$(UTTARCH)$(UTTVERSION)-$(SWID).bin -f
endif

