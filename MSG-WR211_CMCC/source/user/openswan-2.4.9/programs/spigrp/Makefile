# Makefile for miscelaneous programs
# Copyright (C) 2002  Michael Richardson	<mcr@freeswan.org>
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
# RCSID $Id: Makefile,v 1.1.1.1 2011/08/17 11:10:43 brwang Exp $

OPENSWANSRCDIR?=$(shell cd ../..; pwd)
include ${OPENSWANSRCDIR}/Makefile.inc

PROGRAM=spigrp
EXTRA5PROC=${PROGRAM}.5

LIBS=${OPENSWANLIB}

include ${srcdir}../Makefile.program

#
# $Log: Makefile,v $
# Revision 1.1.1.1  2011/08/17 11:10:43  brwang
# 3520G start
#
# Revision 1.1.1.1  2011/03/15 05:24:28  lei.minghuan
# import ralink
#
# Revision 1.1.1.1  2009/12/14 12:44:47  brwang
# ralink wlan
#
# Revision 1.1.1.1  2007-10-08 08:01:06  steven
#
#
# Revision 1.7  2004/12/18 18:13:44  mcr
# 	adjusted Makefiles so that ${OBJDIR} builds work.
#
# Revision 1.6  2004/04/12 12:05:53  ken
# Use OPENSWANLIBs
#
# Revision 1.5  2004/04/03 19:44:49  ken
# FREESWANSRCDIR -> OPENSWANSRCDIR (patch by folken)
#
# Revision 1.4  2002/06/03 20:25:31  mcr
# 	man page for files actually existant in /proc/net changed back to
# 	ipsec_foo via new EXTRA5PROC process.
#
# Revision 1.3  2002/06/02 21:51:41  mcr
# 	changed TOPDIR->FREESWANSRCDIR in all Makefiles.
# 	(note that linux/net/ipsec/Makefile uses TOPDIR because this is the
# 	kernel sense.)
#
# Revision 1.2  2002/04/26 01:21:26  mcr
# 	while tracking down a missing (not installed) /etc/ipsec.conf,
# 	MCR has decided that it is not okay for each program subdir to have
# 	some subset (determined with -f) of possible files.
# 	Each subdir that defines $PROGRAM, MUST have a PROGRAM.8 file as well as a PROGRAM file.
# 	Optional PROGRAM.5 files have been added to the makefiles.
#
# Revision 1.1  2002/04/24 07:55:32  mcr
# 	#include ${srcdir}..tches and Makefiles for post-reorg compilation.
#
#
#

