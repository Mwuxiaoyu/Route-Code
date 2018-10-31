    /*
     * Notice: file generated by MIBMAKE from mibinip.mdf
     * DO NOT EDIT THIS FILE!!!  Edit mibinip.mdf
     */
#line 3 "mibinip.mdf"
/*
 * $Id: mibinip.h,v 1.1.1.1 2011/08/17 11:11:56 brwang Exp $
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

#line 32 "mibinip.mdf"

#if (( ! defined( MIBINIP_H ) ) && ( MDF_IP_ROUTE == FYES ))
# define MIBINIP_H

# include "mibgen.h"
# include "mibglob.h"
# include "mibrip.h"
# include "mibospf.h"
# include "mibtos.h"
# include "mibfiltr.h"
# include "mibinses.h"

    /*
     * How to build the factory instance of this profile.
     */
extern ConstPtr
mibFactoryIpOptions( MibProfileType	mibType,
		     Uint32		info );

    /*
     * Our parameter table is made external as the profile is used
     * within other profiles.
     */
extern const MibFieldComplexParams	ipOptionsParams;


    /*
     * IpOptions:
     *
     * IP protocol configuration options.
     *
     *  pppIpNcp Enable/disable IP routing for this connection.
     *
     *  vjCompEnable Enable/disable Van Jacabson TCP Header prediction
     *                                  processing on this connection if
     *                                  supported by the selected encapsulation
     *                                  protocol.
     *
     *  assignAddress Hidden, for answer profile.
     *
     *  fill1 filler, 32-bit boundary
     *
     *  hostAddress The remote address with netmask of the named host.
     *
     *  netmask The netmask of the remote address.
     *
     *  wanAlias Field is deprecated; use the 'remote-address' field to specify
     *                                  the address of the remote host.
     *
     *  ifAddress The interface address with netmask, if required, of the local
     *                                  end. If left zero the interface is
     *                                  assumed to be an unnumbered interface.
     *
     *  ifNetmask The netmask of the local interface address.
     *
     *  metric The routing metric, typically measured in number of hops, to be
     *                                  used in the routing table entry created
     *                                  for this connection when the connection
     *                                  is active.
     *
     *  downMetric The routing metric, typically measured in number of hops, to
     *                                  be used in the routing table entry
     *                                  created for this connection when the
     *                                  connection is inactive.
     *
     *  preference The preference used for comparing this route to other
     *                                  routes. Preference has a higher
     *                                  priority than metric. As with the
     *                                  metric field, the lower the number, the
     *                                  higher the preference.
     *
     *  downPreference The preference used for comparing the down-metric route
     *                                  to other routes. Preference has a
     *                                  higher priority than metric. As with
     *                                  the metric field, the lower the number,
     *                                  the higher the preference.
     *
     *  privateRoute Enable/disable distribution of the route created for this
     *                                  connection via routing protocols.
     *                                  Private routes are used internally, but
     *                                  never advertized to the outside world.
     *
     *  multicastAllowed Enable this connection as a multicast traffic client.
     *
     *  fill2 filler when multicastAllowed not used.
     *
     *  assignPoolIndex The number of the address pool from which an address
     *                                  for this host should be obtained when
     *                                  using assigned addresses.
     *
     *  authPoolAdrs Internal field, not settable from user interface.
     *
     *  ipDirect The address of the next-hop when IP direct routing is used.
     *
     *  routingMode Specify if RIP or RIP-2 should be used over this connection
     *                                  and if used in both directions or not.
     *
     *  routeFilter The name of this filter. All filters are referenced by name
     *                                  so a name must be assigned to active
     *                                  filters.
     *
     *  checkIpSrc Enable/disable source ip checking. Packets with source ip
     *                                  address which doesn't match with
     *                                  negotiated remote ip address will be
     *                                  dropped.
     *
     *  checkIpDst Enable/disable dst ip checking. Packets with dst ip
     *                                  address which doesn't match with
     *                                  negotiated remote ip address will be
     *                                  dropped.
     *
     *  ospf OSPF routing protocol options for this connection.
     *
     *  multicastRateLimit The multicast rate limit in seconds.
     *
     *  multicastGroupLeaveDelay The multicast group leave in seconds. This is
     *                                  only for IGMP Version 2
     *
     *  clientDnsPrimaryAddress User specific DNS Primary Address.
     *
     *  clientDnsSecondaryAddress User specific DNS Secondary Address.
     *
     *  clientDnsAddrAssign A flag to control assigning user specific DNS
     *                                  Addresses.
     *
     *  clientDefNextHop The default gateway to be used for traffic from this
     *                                  connection, if no specific route is
     *                                  found in the routing table. If left
     *                                  zero, the system's default route will
     *                                  be used.
     *
     *  tosOpt Define type of service for this connection.
     *
     *  tosFilter The name of type-of-service filter. All filters are
     *                                  referenced by name so a name must be
     *                                  assigned to active filters.
     *
     *  itosFilter The number of type-of-service filter.
     */
typedef struct st_ipOptions {
#line 112 "mibinip.mdf"
    Boolean                             pppIpNcp;
#line 117 "mibinip.mdf"
    Boolean                             vjCompEnable;
#line 124 "mibinip.mdf"
    Boolean                             assignAddress;
#line 130 "mibinip.mdf"
    char                                fill1;
#line 136 "mibinip.mdf"
    IpAddress                           hostAddress;
    Uint32                          	Interval;
    Uint32                          	Retry;
    Uint32                          	weight1;
    Uint32                          	weight2;
    IpAddress                           keepaliveAddress;
#line 141 "mibinip.mdf"
    IpAddress                           netmask;
#line 147 "mibinip.mdf"
    IpAddress                           wanAlias;
#line 154 "mibinip.mdf"
    IpAddress                           ifAddress;
#line 161 "mibinip.mdf"
    IpAddress                           ifNetmask;
#line 167 "mibinip.mdf"
    Uint16                              metric;
#line 174 "mibinip.mdf"
    Uint16                              downMetric;
#line 182 "mibinip.mdf"
    Uint32                              preference;
#line 189 "mibinip.mdf"
    Uint32                              downPreference;
#line 197 "mibinip.mdf"
    Boolean                             privateRoute;
#line 204 "mibinip.mdf"
#if ( ( MDF_IGMP == FYES ) && ( MDF_MROUTING == FYES ) )
    Boolean                             multicastAllowed;
#endif /* ( ( MDF_IGMP == FYES ) && ( MDF_MROUTING == FYES ) ) */
#line 210 "mibinip.mdf"
#if (! ( ( MDF_IGMP == FYES ) && ( MDF_MROUTING == FYES ) ))
    Uint8                               fill2;
#endif /* (! ( ( MDF_IGMP == FYES ) && ( MDF_MROUTING == FYES ) )) */
#line 217 "mibinip.mdf"
    Uint16                              assignPoolIndex;
#line 223 "mibinip.mdf"
    IpAddress                           authPoolAdrs;
#line 229 "mibinip.mdf"
    IpAddress                           ipDirect;
#line 234 "mibinip.mdf"
    RoutingMode                         routingMode;
#line 240 "mibinip.mdf"
    char                                routeFilter[ MAX_PROFILE_NAME_LENGTH + 1 ];
#line 245 "mibinip.mdf"
    Boolean                             checkIpSrc;
    Boolean                             checkIpDst;
#line 251 "mibinip.mdf"
#if ( MDF_OSPF == FYES )
    MibProfOspf                         ospf;
#endif /* ( MDF_OSPF == FYES ) */
#line 257 "mibinip.mdf"
#if ( ( MDF_IGMP == FYES ) && ( MDF_MROUTING == FYES ) )
    Uint16                              multicastRateLimit;
#endif /* ( ( MDF_IGMP == FYES ) && ( MDF_MROUTING == FYES ) ) */
#line 263 "mibinip.mdf"
#if ( ( MDF_IGMP == FYES ) && ( MDF_MROUTING == FYES ) )
    Uint16                              multicastGroupLeaveDelay;
#endif /* ( ( MDF_IGMP == FYES ) && ( MDF_MROUTING == FYES ) ) */
#line 269 "mibinip.mdf"
#if ( ( MDF_DNS == FYES ) && ( MDF_IP == FYES ) )
    IpAddress                           clientDnsPrimaryAddress;
#endif /* ( ( MDF_DNS == FYES ) && ( MDF_IP == FYES ) ) */
#line 275 "mibinip.mdf"
#if ( ( MDF_DNS == FYES ) && ( MDF_IP == FYES ) )
    IpAddress                           clientDnsSecondaryAddress;
#endif /* ( ( MDF_DNS == FYES ) && ( MDF_IP == FYES ) ) */
#line 281 "mibinip.mdf"
#if ( ( MDF_DNS == FYES ) && ( MDF_IP == FYES ) )
    Boolean                             clientDnsAddrAssign;
#endif /* ( ( MDF_DNS == FYES ) && ( MDF_IP == FYES ) ) */
#line 287 "mibinip.mdf"
    IpAddress                           clientDefNextHop;
#line 295 "mibinip.mdf"
#if ( IPTOS == FYES )
    MibProfIpTos                        tosOpt;
#endif /* ( IPTOS == FYES ) */
#line 301 "mibinip.mdf"
#if ( MDF_IPTOS_CLI == FYES )
    char                                tosFilter[ MAX_FILTER_NAME_LENGTH + 1 ];
#endif /* ( MDF_IPTOS_CLI == FYES ) */
#line 307 "mibinip.mdf"
#if ( MDF_IPTOS_LCD == FYES )
    Uint16                              itosFilter;
#endif /* ( MDF_IPTOS_LCD == FYES ) */
}
#if ( USERIF & CLI )
__PACK__
#endif
IpOptions;
#line 426 "mibinip.mdf"

# endif /* (( ! defined( MIBINIP_H ) ) && ( MDF_IP_ROUTE == FYES )) */
