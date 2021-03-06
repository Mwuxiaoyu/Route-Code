#include    <stdio.h>
#include    <sys/ioctl.h>
#include    <arpa/inet.h> /*inet_addr*/
#include    <string.h>
#include    <dirent.h>
#include    <linux/autoconf.h>
#include    "webs.h"
#include    "utils.h"
#include    "uttfunction.h"

#ifdef CONFIG_SWITCH_EXT
#include    <urcp_ipc.h>
#include    <sw_cfg.h>
#include    "webmsg.h"
#endif
#if SW_FEATURE_URCP
#define STR_MAC_SIZE   12
#define STR_IP_SIZE    15
#define MAX_ERROR_TYPE 5u

extern const char *urcpErrorMsg[];


extern urcp_status_t getVlanUrcp(webs_t wp,uchar* strMac,uchar* strPasswd,uint32* portNum,urcp_status_t pre_ret);
extern urcp_status_t getUrcpMirrorConf(webs_t wp,uchar* strMac,uchar* strPasswd,urcp_mirror_conf_t *mirror_buff,uint32 misc_type);
extern urcp_status_t getPortConfMemberUrcp(webs_t wp,uchar* strMac,uchar* strPasswd,uint32* portNum,urcp_status_t pre_ret);
extern urcp_status_t getQosRateLimitMemberUrcp(webs_t wp,uchar* strMac,uchar* strPasswd,uint32* portNum,urcp_status_t pre_ret);

static unsigned char gMac[STR_MAC_SIZE+1];/*页面保存时要操作的目的mac,供页面跳转后"get"使用*/
static unsigned char gIp[STR_IP_SIZE+1];
static unsigned char gPasswd[MAX_PASSWORD_LEN+1];
static const char* errorMsg[]={"找不到删除的组！"};
static uint32 isRefresh = 0u;

static int  getAggrGroupListUrcp(int eid, webs_t wp, int argc, char **argv);
static void formAggrGroupUrcp(webs_t wp, char_t *path, char_t *query);
static void formAggrGroupDelUrcp(webs_t wp, char_t *path, char_t *query);

/********************************************************************
 * 函数： formDefineAggrUrcp
 * 功能： 页面与交互转换函数
 * 参数： 无
 * 输出： 无
 * 返回： 无
 * 作者： bhou
 *******************************************************************/
extern void formDefineAggrUrcp(void)
{
    websAspDefine(T("aspOutPutAggrGroupListUrcp"), getAggrGroupListUrcp);   
    websFormDefine(T("formAggrGroupUrcp"), formAggrGroupUrcp);
    websFormDefine(T("formAggrGroupDelUrcp"), formAggrGroupDelUrcp);
}
/********************************************************************
 * 函数： getUrcpRstpLacpConf
 * 功能： 获取联动RSTP LACP配置数据结构
 * 参数： 无
 * 输出： 无
 * 返回： 无
 * 作者： bhou
 *******************************************************************/
extern urcp_status_t getUrcpRstpLacpEnableConf(webs_t wp,uchar* strMac,uchar* strPasswd,urcp_rstp_lacp_conf_t *rstp_lacp_buff)
{
    uint32 wait_time_us=0u;
    uint32 max_wait_time=MAX_URCP_WEB_TIME_OUT_SEC;
    max_wait_time = max_wait_time * 1000u *1000u ;/*转化为微妙*/
    uint32 uip;

    urcp_rstp_lacp_msg_t  urcp_rstp_lacp_msg;
    urcp_status_t ret=URCP_STATUS_FAIL;/*默认为读取失败*/

    uip=ip2int(wp->ipaddr);
    memset(&urcp_rstp_lacp_msg, 0, sizeof(struct _urcp_rstp_lacp_msg_t));
    /*转换mac地址*/
    str2mac(strMac, urcp_rstp_lacp_msg.dst_mac);
    /*要操作的mac不能为空*/
    strncpy(urcp_rstp_lacp_msg.password, strPasswd, MAX_PASSWORD_LEN);
    urcp_rstp_lacp_msg.user_ip = uip;
    /*发消息,通知交换机进程发包*/
    sendwebmsgbuffer(MSG_TYPE_URCP,GET_RSTP_LACP , (char*)(&urcp_rstp_lacp_msg), sizeof(struct _urcp_rstp_lacp_msg_t));
    /*等待1会*/
    usleep(URCP_WEB_WAIT_INTERVAL_USEC);/*等待100ms即0.1s*/
    wait_time_us += URCP_WEB_WAIT_INTERVAL_USEC;

    while(1)
    {
	/*从misc buffer中读取信息*/
	memset(rstp_lacp_buff, 0, sizeof(struct _urcp_rstp_lacp_conf_t));
	ret = urcp_read_rstp_lacp_conf(urcp_rstp_lacp_msg.dst_mac, uip , MISC_GET_GENERAL, rstp_lacp_buff);
	if(ret == URCP_STATUS_WAIT)
	{
	    usleep(URCP_WEB_WAIT_INTERVAL_USEC);/*再等待*/
	    wait_time_us += URCP_WEB_WAIT_INTERVAL_USEC;
	    if(wait_time_us > max_wait_time)
	    {
		ret = URCP_STATUS_TIME_OUT;
	    }
	}
	if(ret != URCP_STATUS_WAIT)
	{
	    break;
	}
    }
    return ret;
}
/***
 *打印联动rstp lacp信息到页面
 *并返回读取misc buffer的状态
 *portNum获取联动设备端口个数
 *pre_ret提供前一次联动操作是否成功，否则本函数只打印，不发联动包
 *bhou
 */
extern urcp_status_t getRstpLacpEnableUrcp(webs_t wp,uchar* strMac,uchar* strPasswd,uint32* portNum,urcp_status_t pre_ret)
{
    uint32 uindex=0u;
    uint32 i=0u;
    char  str_buf[128];

    urcp_rstp_lacp_conf_t rstp_lacp_buff;
    urcp_status_t ret=pre_ret;/*默认为前一次操作的状态*/

    memset(str_buf,0,sizeof(str_buf));
    memset(&rstp_lacp_buff,0,sizeof(urcp_rstp_lacp_conf_t));

    if(pre_ret == URCP_STATUS_OK)/*若之前有一次操作错误，则本次读取没必要*/
    {
	ret = getUrcpRstpLacpEnableConf(wp,strMac,strPasswd, &rstp_lacp_buff);/*获取联动rstp_lacp结构体内容*/
    }
    websWrite(wp, "var lacp = new Array("); /*start print lacp enable*/
    if(ret != URCP_STATUS_OK)/*not get right*/
    {
	memset(&rstp_lacp_buff,0,sizeof(urcp_rstp_lacp_conf_t));/*清空，但为页面显示正确还得显示*/
    }
    if(portNum!=NULL)
    {
	(*portNum) = rstp_lacp_buff.port_num;/*端口个数*/
    }
    for (uindex = 0u; uindex < rstp_lacp_buff.port_num; uindex++)
    {
	if(uindex > 0u)/*非首位 则打印分隔符*/
	{
	    websWrite(wp,",");
	    sprintf(&str_buf[i],",");
	    i = i+1u;

	}
	if( (rstp_lacp_buff.lacp_enable &(uint32)(1u<<uindex)) >0u )/*prinf lacp enable to web*/
	{
	    websWrite(wp,"1");
	}
	else
	{
	    websWrite(wp,"0");
	}
	if( (rstp_lacp_buff.rstp_enable &(uint32)(1u<<uindex)) >0u )/*get rstp enable*/
	{
	    sprintf(&str_buf[i],"1");
	    i = i+1u;
	}
	else
	{
	    sprintf(&str_buf[i],"0");
	    i = i+1u;
	}
    }/*end for*/
    str_buf[sizeof(str_buf)-1u] = '\0';/*最后一位置0，防止内存泄露*/
    websWrite(wp, ");"); 
    websWrite(wp,"var rstp = new Array(%s);",str_buf);/*print rstp enable to web*/
    return ret;
}
/********************************************************************
 * 函数： getAggrUrcpSetRet
 * 功能： 获取联动汇聚操作结果
 * 参数： 无
 * 输出： 无
 * 返回： 无
 * 作者： bhou
 *******************************************************************/
static urcp_status_t getAggrUrcpSetRet(webs_t wp,uchar* strMac)
{
    uint32 wait_time_us=0u;
    uint32 max_wait_time=MAX_URCP_WEB_TIME_OUT_SEC;
    max_wait_time = max_wait_time * 1000u *1000u ;/*转化为微妙*/
    uint32 uip;
    unsigned char std_mac[MAC_LEN];

    memset(std_mac,0,sizeof(std_mac));

    urcp_status_t ret=URCP_STATUS_FAIL;/*默认为读取失败*/

    uip=ip2int(wp->ipaddr);
    /*转换mac地址*/
    str2mac(strMac, std_mac);
    while(1)
    {
	usleep(URCP_WEB_WAIT_INTERVAL_USEC);/*再等待*/
	/*从misc buffer中读取信息*/
	ret = urcp_read_aggr_conf(std_mac, uip , MISC_SET_GENERAL, (urcp_aggr_conf_t*)NULL);
	if((ret == URCP_STATUS_WAIT) || (ret==URCP_STATUS_FOUND_NONE))
	{
	    /*URCP_STATUS_FOUND_NONE 表示可能urcp程序还没来得及创建节点信息与发包，系统就又调度执行goahead程序了*/
	    wait_time_us += URCP_WEB_WAIT_INTERVAL_USEC;
	    if(wait_time_us > max_wait_time)
	    {
		ret = URCP_STATUS_TIME_OUT;
	    }
	}
	if((ret!=URCP_STATUS_WAIT) && (ret!=URCP_STATUS_FOUND_NONE))
	{
	    break;
	}
    }
    return ret;
}

/********************************************************************
 * 函数： getUrcpAggrConf
 * 功能： 获取联动汇聚配置数据结构
 * 参数： 无
 * 输出： 无
 * 返回： 无
 * 作者： bhou
 *******************************************************************/
extern urcp_status_t getUrcpAggrConf(webs_t wp,uchar* strMac,uchar* strPasswd,urcp_aggr_conf_t *aggr_buff)
{
    uint32 wait_time_us=0u;
    uint32 max_wait_time=MAX_URCP_WEB_TIME_OUT_SEC;
    max_wait_time = max_wait_time * 1000u *1000u ;/*转化为微妙*/
    uint32 uip;

    urcp_aggr_msg_t  urcp_aggr_msg;
    urcp_status_t ret=URCP_STATUS_FAIL;/*默认为读取失败*/

    uip=ip2int(wp->ipaddr);
    memset(&urcp_aggr_msg, 0, sizeof(struct _urcp_aggr_msg_t));
    /*转换mac地址*/
    str2mac(strMac, urcp_aggr_msg.dst_mac);
    /*要操作的mac不能为空*/
    strncpy(urcp_aggr_msg.password, strPasswd, MAX_PASSWORD_LEN);
    urcp_aggr_msg.user_ip = uip;
    urcp_aggr_msg.opt_type = MISC_GET_GENERAL;/*读取操作*/
    /*发消息,通知交换机进程发包*/
    sendwebmsgbuffer(MSG_TYPE_URCP, AGGR_CONFIG, (char*)(&urcp_aggr_msg), sizeof(struct _urcp_aggr_msg_t));
    /*等待1会*/
    usleep(URCP_WEB_WAIT_INTERVAL_USEC);/*等待100ms即0.1s*/
    wait_time_us += URCP_WEB_WAIT_INTERVAL_USEC;
    while(1)
    {
	/*从misc buffer中读取信息*/
	memset(aggr_buff, 0, sizeof(struct _urcp_aggr_conf_t));
	ret = urcp_read_aggr_conf(urcp_aggr_msg.dst_mac, uip , MISC_GET_GENERAL, aggr_buff);
	if(ret == URCP_STATUS_WAIT)
	{
	    usleep(URCP_WEB_WAIT_INTERVAL_USEC);/*再等待*/
	    wait_time_us += URCP_WEB_WAIT_INTERVAL_USEC;
	    if(wait_time_us > max_wait_time)
	    {
		ret = URCP_STATUS_TIME_OUT;
	    }
	}
	if(ret != URCP_STATUS_WAIT)
	{
	    break;
	}
    }
    return ret;
}

/***
 *打印联动汇聚信息到页面
 *并返回读取misc buffer的状态
 *portNum获取联动设备端口个数
 *pre_ret提供前一次联动操作是否成功，否则本函数只打印，不发联动包
 *bhou
 */
extern urcp_status_t getAggrMemberUrcp(webs_t wp,uchar* strMac,uchar* strPasswd,uint32* portNum,urcp_status_t pre_ret)
{
    uint32 uindex=0u;
    uint32  umember,i;
    int icount = 0;
    int pcount = 0;
    uint16 ugroup = 0u;

    urcp_aggr_conf_t aggr_buff;
    urcp_status_t ret=pre_ret;/*默认为前一次操作的状态*/

    memset(&aggr_buff,0,sizeof(urcp_aggr_conf_t));

    if(pre_ret == URCP_STATUS_OK)/*若之前有一次操作错误，则本次读取没必要*/
    {
	ret = getUrcpAggrConf(wp,strMac,strPasswd, &aggr_buff);
    }
    websWrite(wp, "var aggGrpNos = new Array();\n");/*组号*/
    websWrite(wp, "var aggLists = new Array();\n");/*成员列表*/
    websWrite(wp, "var aggGrpNames = new Array();\n");/*组名*/
    if( ret == URCP_STATUS_OK)/*读取正确*/
    {
	if(portNum!=NULL)
	{
	    (*portNum) = aggr_buff.port_num;/*端口个数*/
	}
	for (uindex = 0u; uindex < aggr_buff.port_num; uindex++)
	{
	    /*组号*/
	    ugroup = aggr_buff.aggr_group[uindex].index;
	    /*成员*/
	    umember = aggr_buff.aggr_group[uindex].member;
	    if ((ugroup>0u) && (umember>0u))/*含有汇聚组成员*/
	    {
		websWrite(wp, "aggGrpNos[%d] = \"%hu\";", icount, ugroup);	 
		websWrite(wp, "aggLists[%d] = \"", icount);
		for (i=0u; i < aggr_buff.port_num; i++)
		{
		    if ( (umember & (((uint32)1u<<i))) > 0u)/*含有该成员*/
		    {
			if (pcount == 0)
			{
			    websWrite(wp, "%d", (i+1u));
			    pcount++;
			}
			else
			{
			    websWrite(wp, " %d", (i+1u));
			    pcount++;
			}
		    }
		}
		websWrite(wp, "\";");
		/*remark*/
		websWrite(wp, "aggGrpNames[%d] = \"%s\";", icount, aggr_buff.aggr_group[uindex].remark);
		websWrite(wp, "\n");
		icount++;
	    }
	}/*end for*/
    }/*end if ret == URCP_STATUS_OK*/
    return ret;
}

/****
 *打印远程镜像端口到web页面
 *若远程镜像源端口不存在，则打印结果为0
 *bhou
 */
static urcp_status_t getMirrorPortUrcp(webs_t wp,uchar* strMac,uchar* strPasswd,urcp_status_t pre_ret)
{
    urcp_status_t ret = pre_ret;
    urcp_mirror_conf_t mirror_buff;
    uint32 u_mport = 0u;
    memset(&mirror_buff,0,sizeof(urcp_mirror_conf_t));
    if(pre_ret == URCP_STATUS_OK)/*若之前的联动信息没有获取成功，则本次获取没有必要*/
    {
	ret = getUrcpMirrorConf(wp,strMac,strPasswd,&mirror_buff,MISC_GET_GENERAL);/*get mirror conf*/
    }
    if(ret == URCP_STATUS_OK)/*本次获取成功*/
    {
	if(mirror_buff.mirror_source > 0u)
	{
	    u_mport = mirror_buff.mirror_port;
	}
    }
    websWrite(wp, "var mirrorPort = %d;\n", u_mport);
    return ret;

}
/********************************************************************
 * 函数： getAggrGroupListUrcp 
 * 功能： 页面显示函数
 * 参数： 略
 * 返回： 略
 * 输出： 略
 * 作者： bhou
 * 日期� 201-04-09
 ********************************************************************/ 
static int getAggrGroupListUrcp(int eid, webs_t wp, int argc, char **argv)
{
    uint32 portNum=PORT_COUNT;
    uint32 webPortNum = 0u;/*页面打印的端口个数*/
    urcp_status_t ret = URCP_STATUS_OK;
    unsigned char *strMac, *strIp, *strPasswd, *urcpSet;
    uint32 is_set=0u;

    /*判断是否由formAggrUrcp()跳转过来的*/
    urcpSet = websGetVar(wp, "urcpSet", T(""));
    if(strcmp(urcpSet, T(""))==0)
    {
	/*第一次打开*/
	strMac = websGetVar(wp, "macStr", T(""));
	strPasswd = websGetVar(wp, "passStr", T(""));
	strIp = websGetVar(wp, "ipStr", T(""));
	isRefresh = 0u;/*刷新标志位复位*/
	is_set = 0u;
    }
    else
    {
	/*页面保存后的跳转*/
	strMac = gMac;
	strPasswd = gPasswd;
	strIp = gIp;
	is_set =  1u;
    }
    websWrite(wp, "var macStr = \"%s\";\n", strMac);
    websWrite(wp, "var ipStr = \"%s\";\n", strIp);
    websWrite(wp, "var passStr = \"%s\";\n", strPasswd);
    if(strcmp(strMac, T(""))==0)
    {
	is_set = 0u;/*此时不用再读取是否设置成功*/
	ret=URCP_STATUS_FAIL;/*按照本次操作为非法处理，则后面只打印，不发联动包*/
	/*后面打印的变量已经没有意义，为了页面的显示不出现错误还得往下走*/
    }

    if(is_set == 1u)
    {
	if(isRefresh == 0u)
	{/*设置后的首次跳转*/
	    ret = getAggrUrcpSetRet(wp,strMac);/*读取设置结果*/
	    isRefresh = 1u;/*保存后。再刷新则不读取设置结果。因设置结果缓存可能被清空*/
	}
    }

    ret = getAggrMemberUrcp(wp,strMac,strPasswd,&portNum,ret);/*打印汇聚信息*/
    ret = getRstpLacpEnableUrcp(wp,strMac,strPasswd,&portNum,ret);/*打印rstp lacp enable信息*/
    ret = getMirrorPortUrcp(wp,strMac, strPasswd,ret); /*print mirror  port to web page*/
    ret = getPortConfMemberUrcp(wp,strMac,strPasswd,&portNum,ret);/*打印端口设置本页面成员*/
    ret = getQosRateLimitMemberUrcp(wp,strMac, strPasswd,&portNum,ret); /*打印速率限制*/
    ret = getVlanUrcp(wp,strMac, strPasswd,&portNum,ret); /*print vlan to web page**/
    if(ret==URCP_STATUS_OK)
    {
	/*成功*/
	websWrite(wp, "var urcpError = \"\";\n");
	webPortNum=portNum;/*不正确的情况下，页面打印端口个数为0（否则则可能显示错误）*/
    }
    else if(ret<=MAX_ERROR_TYPE)
    {
	/*失败*/
        websWrite(wp, "var urcpError = \"%s\";\n", urcpErrorMsg[ret]);
    }
    else
    {
	/*失败*/
	websWrite(wp, "var urcpError = \"%s\";\n", urcpErrorMsg[0]);
    }
    websWrite(wp, "var numPorts = %d;\n", webPortNum);
    getTheErrorMsg(eid, wp, argc, argv);
    return 0;

}

/**
 *汇聚联动操作，form公用函数
 *保存页面ip，mac，password的值。并初始化msg结构体
 *返回 ：1u 成功 ,0u失败
 *bhou
 */
static uint32 formAggrProcessForUrcp(webs_t wp,urcp_aggr_msg_t *urcp_aggr_msg)
{

    uint32 ret = 1u;
    uint32 portCt;
    uchar *str, *strMac, *strIp, *strPasswd;

    memset(gMac, 0, sizeof(gMac));
    memset(gIp, 0, sizeof(gIp));
    memset(gPasswd, 0, sizeof(gPasswd));

    strMac = websGetVar(wp, "macStr", T(""));/*远程设备mac地址*/
    if(strcmp(strMac, T(""))!=0)
    {
	str = websGetVar(wp, "numPorts", T(""));/*远程设备端口个数*/
	portCt = (uint32)strtoul(str, NULL, 10);/*转化为数字*/

	if((portCt>0u)&&(portCt<=MAX_URCP_PORT_NUM))/*不在此范围以下操作可能内存溢出*/
	{

	    /*读取页面变量*/
	    strIp = websGetVar(wp, "ipStr", T(""));/*远程设备ip*/
	    strPasswd = websGetVar(wp, "passStr", T(""));/*远程设备密码*/

	    /*保存本次操作地址*/
	    memcpy(gMac, strMac, sizeof(gMac)-1u);
	    memcpy(gIp, strIp, sizeof(gIp)-1u);
	    strncpy(gPasswd, strPasswd, sizeof(gPasswd)-1u);

	    memset(urcp_aggr_msg, 0, sizeof(struct _urcp_aggr_msg_t));/*初始化消息*/
	    /*页面读的mac地址格式转换*/
	    str2mac(strMac, urcp_aggr_msg->dst_mac);
	    strncpy(urcp_aggr_msg->password, strPasswd, MAX_PASSWORD_LEN);
	    urcp_aggr_msg->user_ip = ip2int(wp->ipaddr);
	    urcp_aggr_msg->port_ct = portCt;
	    urcp_aggr_msg->opt_type = MISC_SET_GENERAL;/*设置*/
	}
	else
	{
	    ret = 0u;
	    /*端口数不对*/
	    setTheErrorMsg("get the port_num error!");
	}
    }
    else
    {
	ret = 0u;
	/*没有目的交换机mac，出错*/
	setTheErrorMsg("get the destination host MAC error!");
    }
    return ret;
}

/*******************************************************************
 *函数： formAggrGroupUrcp
 *功能： 页面提交函数（编辑汇聚组，添加或修改）
 *参数： 
 *返回： 无
 *作者： bhou
 *创建： 2011-04-12
 ******************************************************************/
static void formAggrGroupUrcp(webs_t wp, char_t *path, char_t *query)
{
    char *action = NULL;
    char *cmember = NULL;
    char *cremark = NULL;
    char *group = NULL;

    uint32 i;
    uint32 umember=0u;
    char chk_name[32];
    urcp_aggr_msg_t  urcp_aggr_msg;

    memset(&urcp_aggr_msg,0,sizeof(urcp_aggr_msg_t));
    memset(chk_name,0,sizeof(chk_name));
    isRefresh = 0u;/*刷新标志位复位*/

    if(formAggrProcessForUrcp(wp,&urcp_aggr_msg) > 0u)
    {

	/*get value from web*/
	action = websGetVar(wp, "actions", T(""));/*操作类型 添加或修改 ，联动操作目前不需要*/
	cremark = websGetVar(wp, "aggName", T(""));/*组名称*/
	group = websGetVar(wp, "aggNo", T(""));/*组号*/


	/*set member to index 0*/
	for (i = 0u; i < urcp_aggr_msg.port_ct; i++)
	{
	    sprintf(chk_name,"chkPort%u",i);/*html控件名*/
	    cmember = websGetVar(wp,chk_name, T(""));
	    if (strcmp(cmember, "on") == 0)
	    {
		umember  |= (uint32)(1u << i);
	    }
	}

	/*初始化aggr_group结构*/
	urcp_aggr_msg.aggr_group.index = (uint16)strtoul(group, NULL, 10);
	urcp_aggr_msg.aggr_group.member = umember;
	memcpy(urcp_aggr_msg.aggr_group.remark,cremark,MAX_REMARK_LEN);


	/*发消息,通知交换机进程发包*/
	sendwebmsgbuffer(MSG_TYPE_URCP, AGGR_CONFIG, (char*)(&urcp_aggr_msg), sizeof(struct _urcp_aggr_msg_t));
    }

    /*urcpSet表示加载的页面是设置后跳转到的，故需读取是否设置成功的标志*/
    websRedirect(wp, "switch/aggregation.asp?urcpSet=1");
    return;
}

/*******************************************************************
 *函数： formAggrGroupDelUrcp
 *功能： 删除汇聚组
 *参数： 
 *返回： 无
 *作者： bhou
 *创建： 2011-04-12
 ******************************************************************/
static void formAggrGroupDelUrcp(webs_t wp, char_t *path, char_t *query)
{

    char *del_str = NULL;
    unsigned short uindex;
    char *cid;
    char *idnext;
    uint32 umember=0u;
    urcp_aggr_msg_t  urcp_aggr_msg;

    isRefresh = 0u;/*刷新标志位复位*/
    memset(&urcp_aggr_msg,0,sizeof(urcp_aggr_msg_t));
    if(formAggrProcessForUrcp(wp,&urcp_aggr_msg) > 0u)
    {
	/*get value from web*/
	del_str = websGetVar(wp, T("delstr"), T(""));

	/*get delstr from web*/
	if ((strcmp(del_str, "") == 0) || (del_str == NULL))
	{
	    setTheErrorMsg(T(errorMsg[0]));
	}
	else
	{
	    /*delstr is "group 1, group 2, ....."*/
	    idnext = strtok(del_str, DELSEPARATE);
	    /*分割*/
	    while(idnext != NULL)
	    {
		cid = idnext;
		idnext = strtok(NULL, DELSEPARATE);
		uindex = (unsigned short)strtoul(cid, NULL, 10);
		if( (uindex > 0u)&&(uindex<=urcp_aggr_msg.port_ct) )/*合法汇聚组号*/
		{
		    umember |= ((uint32)1u<<(uindex-1u));/*置1表示删除汇聚index*/
		}
	    }

	    /*用正常情况下用不到的汇聚组号0表示删除操作，且其成员表示要删除的汇聚组掩码*/
	    urcp_aggr_msg.aggr_group.index =  0u;/*组号（为0表示删除）*/
	    urcp_aggr_msg.aggr_group.member = umember;/*成员（欲删除的组号掩码）*/

	    /*发消息,通知交换机进程发包*/
	    sendwebmsgbuffer(MSG_TYPE_URCP, AGGR_CONFIG, (char*)(&urcp_aggr_msg), sizeof(struct _urcp_aggr_msg_t));

	}
    }
    /*urcpSet表示加载的页面是设置后跳转到的，故需读取是否设置成功的标志*/
    websRedirect(wp, "switch/aggregation.asp?urcpSet=1");
    return;
}
#endif
