# openswan userland AES library
# Copyright (C) 2004	Michael Richardson <mcr@xelerance.com>
# 
# This program is free software; you can redistribute it and/or modify it
# under the terms of the GNU General Public License as published by the
# Free Software Foundation; either version 2 of the License, or (at your
# option) any later version.  See <http://www.fsf.org/copyleft/gpl.txt>.
# 
# This program is distributed in the hope that it will be useful, but
# WITHOUT ANY WARRANTY; without even the implied warranty of MERCHANTABILITY
# or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU General Public License
# for more details.
#
# RCSID $Id: Makefile,v 1.1 2012/09/27 21:06:12 yu.dehai Exp $


OPENSWANSRCDIR?=$(shell cd ../../..; pwd)

include ${OPENSWANSRCDIR}/Makefile.inc
include ${OPENSWANSRCDIR}/Makefile.ver

LIBRARY=crypto/libaes

AES_CORE_OBJ:=aes.o
AES_CORE_SRC:=aes.c

ASM-$(ARCH_ASM):=1
ASM_X86:=$(ASM-i586)$(ASM-i686)
ifneq ($(strip $(ASM_X86)),)
AES_CORE_OBJ:= aes-i586.o
AES_CORE_SRC:= aes-i586.S
endif

BASE_SRCS=aes_xcbc_mac.c aes_cbc.o 
SRCS=${BASE_SRCS} ${AES_CORE_SRC}

OBJS=${BASE_SRCS:.c=.o} ${AES_CORE_OBJ}

HDRS=	

SRCDIR=${OPENSWANSRCDIR}/linux/net/ipsec/aes
VPATH += ${SRCDIR}

LIB=libaes.a
LIBRARY=crypto/libaes
ONEFILE=test_main.c

include ${srcdir}../../Makefile.library

