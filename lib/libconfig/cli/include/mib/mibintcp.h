    /*
     * Notice: file generated by MIBMAKE from mibintcp.mdf
     * DO NOT EDIT THIS FILE!!!  Edit mibintcp.mdf
     */
#line 3 "mibintcp.mdf"
/*
 * $Id: mibintcp.h,v 1.1.1.1 2011/08/17 11:11:56 brwang Exp $
 *
 *      Copyright (c) 1996 UTT Technologies, Inc.
 *      All rights reserved.
 *      Use of copyright notice does not imply publication.
 *
 *
 *                      CONFIDENTIAL INFORMATION
 *                      ------------------------
 *      This Document contains Confidential Information or Trade Secrets,
 *      or both, which are the property of UTT Technologies, Inc.
 *      This document may not be copied, reproduced, reduced to any
 *      electronic medium or machine readable form or otherwise duplicated
 *      and the information herein may not be used, disseminated or
 *      otherwise disclosed, except with the prior written consent of
 *      UTT Technologies, Inc.
 */

#line 32 "mibintcp.mdf"

#if (( ! defined( MIBINTCP_H ) ) && (( MDF_TELNET == FYES ) || ( MDF_TCP_CLEAR == FYES )))
#define MIBINTCP_H

#include "mibgen.h"
#include "mibglob.h"

#define TCP_CLEAR_MAX_PATTERN_LENGTH            64
#define TCP_LOGIN_HOST_MAX			4

    /*
     * How to build the factory instance of this profile.
     */
extern ConstPtr
mibFactoryTcpOptions( MibProfileType	mibType,
		     Uint32		info );

    /*
     * Our parameter table is made external as the profile is used
     * within other profiles.
     */
extern const MibFieldComplexParams	tcpOptionsParams;


    /*
     * TcpOptions:
     *
     * Raw TCP configuration options.
     *
     *  recvPassword The password received from the far end. For future use. We
     *                                  currently use the password in the PPP
     *                                  sub-profile.
     *
     *  fill1
     *
     *  tcpRawEnabled Hidden, for answer profile.
     *
     *  loginHost0 The first TCP CLEAR login host.
     *
     *  loginPort0 The TCP port of the first login-host.
     *
     *  loginHost1 The second TCP CLEAR login host.
     *
     *  loginPort1 The TCP port of the second login-host.
     *
     *  loginHost2 The third TCP CLEAR login host.
     *
     *  loginPort2 The TCP port of the third login-host.
     *
     *  loginHost3 The fourth TCP CLEAR login host.
     *
     *  loginPort3 The TCP port of the fourth login-host.
     *
     *  detectEndOfPacket Set to yes to attempt packet detection
     *
     *  endOfPacketPattern End of packet pattern to match
     *
     *  packetFlushLength Set the maximum packet length before flush
     *
     *  packetFlushTime Set the maximum packet hold time before flush
     */
typedef struct st_tcpOptions {
#line 74 "mibintcp.mdf"
#if ( PLATFORM & TNT )
    char                                recvPassword[ MAX_PASSWORD_LENGTH + 1 ];
#endif /* ( PLATFORM & TNT ) */
#line 82 "mibintcp.mdf"
#if ( ! ( PLATFORM & TNT ))
    char                                fill1;
#endif /* ( ! ( PLATFORM & TNT )) */
#line 88 "mibintcp.mdf"
#if ( MDF_TCP_CLEAR == FYES ) || ( MDF_TELNET == FYES )
    Boolean                             tcpRawEnabled;
#endif /* ( MDF_TCP_CLEAR == FYES ) || ( MDF_TELNET == FYES ) */
#line 95 "mibintcp.mdf"
    char                                loginHost0[ MAX_HOST_NAME_LENGTH + 1 ];
#line 100 "mibintcp.mdf"
    Uint16                              loginPort0;
#line 106 "mibintcp.mdf"
    char                                loginHost1[ MAX_HOST_NAME_LENGTH + 1 ];
#line 111 "mibintcp.mdf"
    Uint16                              loginPort1;
#line 117 "mibintcp.mdf"
    char                                loginHost2[ MAX_HOST_NAME_LENGTH + 1 ];
#line 122 "mibintcp.mdf"
    Uint16                              loginPort2;
#line 128 "mibintcp.mdf"
    char                                loginHost3[ MAX_HOST_NAME_LENGTH + 1 ];
#line 133 "mibintcp.mdf"
    Uint16                              loginPort3;
#line 140 "mibintcp.mdf"
#if ( MDF_TCP_CLEAR == FYES )
    Bool                                detectEndOfPacket;
#endif /* ( MDF_TCP_CLEAR == FYES ) */
#line 146 "mibintcp.mdf"
#if ( MDF_TCP_CLEAR == FYES )
    char                                endOfPacketPattern[ TCP_CLEAR_MAX_PATTERN_LENGTH + 1 ];
#endif /* ( MDF_TCP_CLEAR == FYES ) */
#line 152 "mibintcp.mdf"
#if ( MDF_TCP_CLEAR == FYES )
    Uint16                              packetFlushLength;
#endif /* ( MDF_TCP_CLEAR == FYES ) */
#line 158 "mibintcp.mdf"
#if ( MDF_TCP_CLEAR == FYES )
    Uint16                              packetFlushTime;
#endif /* ( MDF_TCP_CLEAR == FYES ) */
}
#if ( USERIF & CLI )
__PACK__
#endif
TcpOptions;
#line 230 "mibintcp.mdf"

#endif /* (( ! defined( MIBINTCP_H ) && (( MDF_TELNET == FYES )) || (MDF_TCP_CLEAR == FYES) ) */

