#!/bin/sh

if [ $# != 1 ];then
    echo "Usage: l2tp-down.sh <channel name>"
    exit 0;
fi
chnl_name=$1
pppname=`cat /var/run/${chnl_name}.ppp |sed "s/ .*$//g"`
process=`cat /var/run/${pppname}.pid`
serverHost=`uttcli getbynm pptpClient $chnl_name serverHost`
kill -9 $process
process=`cat /var/run/${pppname}.pid`
kill -9 $process
#otherpid=`ps |grep "pppd file /etc/options.${chnl_name} $"|sed "s/^ *//g"|sed "s/ .*$//g"`
#for i in $otherpid
#do
#    kill -9 $i >/dev/null 2>&1
#done
route del $serverHost

rm /var/run/${chnl_name}.ppp
rm /var/run/${pppname}.pid
echo 0 >/var/run/${chnl_name}link.status
# vpn free session plus
if [ $process ]
then
vpnHandle plus 
fi

#SOHO开启VPN，断开时删除DNS和路由
if [ $VPN_PROXY == $FYES ]; then
    vpndns1=$(sed -n '1p' /var/run/vpn.dns)
    vpndns2=$(sed -n '2p' /var/run/vpn.dns)
    setname=sys_vpn
    otherpid=`ps |grep "pppd"|grep -v "pppoe"|sed "s/^ *//g"|sed "s/ .*$//g"`
    kill -9 $otherpid >/dev/null 2>&1
    iptables -t mangle -D PREROUTING -m conntrack --ctstate NEW -m set --set $setname dst -j MARK --set-mark 0x10/0xfe
    iptables -t mangle -D PREROUTING -m conntrack --ctstate NEW -m set --set $setname dst -j CONNMARK --set-mark 0x10/0xfe
    dnsmasqConf.sh del $vpndns1 $vpndns2 $pppname $setname
    sleep 1
fi

exit;
