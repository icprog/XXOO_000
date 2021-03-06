/**
* mdbstat.c -- 终端状态数据
* 
* 作者: zhuzhiqiang
* 创建时间: 2009-5-13
* 最后修改时间: 2009-5-13
*/

//#include <stdio.h>
#include <string.h>

#include "plat_include/debug.h"
#include "plat_include/timeal.h"
#include "app_include/lib/bcd.h"
#include "app_include/param/capconf.h"
#include "app_include/param/mix.h"
#include "app_include/monitor/alarm.h"
#include "app_include/monitor/runstate.h"
#include "app_include/cenmet/mdbstic.h"
#include "app_include/param/meter.h"
#include "app_include/gprs/gprs_dev.h"
//#include "uplink/ppp/ppp.h"

/**
* @brief 读取当前时间
* @param buf 缓存区指针
* @param plen 返回的缓存区长度
* @return 成功0, 否则失败(参数和返回值以下同类函数相同)
*/
static int MdbStatTime(unsigned char *buf, int *plen)
{
	sysclock_t clk;

	*plen = 6; 

	SysClockReadCurrent(&clk);

	buf[0] = clk.second;
	buf[1] = clk.minute;
	buf[2] = clk.hour;
	buf[3] = clk.day;
	buf[4] = clk.month;
	buf[5] = clk.year;
	HexToBcd(buf, 6);

	buf[4] &= 0x1f;
	if(clk.week == 0) clk.week = 7;
	buf[4] |= (clk.week<<5)&0xe0;
	return 0;
}                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                               

static const unsigned char FlagParaValid[31] = {
	0xdd, 0x8b, 0x00, 0x03, 0x69, 0x00, 0x00, 0x04, 
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
};

/**
* @brief 读取参数状态
*/
static int MdbStatDataValid(unsigned char *buf, int *plen)
{
	*plen = 31;
	smallcpy(buf, (unsigned char *)FlagParaValid, 31);
	return 0;
}

/**
* @brief 读取上行通信状态
*/
static int MdbStatComm(unsigned char *buf, int *plen)
{
	*plen = 1;

	*buf = 0x08;

	if(ParaMix.bactsend) *buf |= 0x02;
	else *buf |= 0x01;

	return 0;
}

/**
* @brief 读取事件计数器当前值
*/
static int MdbStatAlarmEc(unsigned char *buf, int *plen)
{
	*plen = 2;
#ifdef ALARM_MODULE
	GetAlarmEc(buf);
#endif
	return 0;
}

/**
* @brief 读取事件状态
*/
static int MdbStatAlarmState(unsigned char *buf, int *plen)
{
	*plen = 8;

	smallcpy(buf, RunState.alarm.stat, 8);
	memset(RunStateModify.alarm.stat, 0, 8);
	
	return 0;
}

/**
* @brief 读取输入状态量
*/
static int MdbStatIsig(unsigned char *buf, int *plen)
{
	*plen = 2;
	buf[0] = RunState.isig_stat;
	buf[1] = RunState.isig_chg;
	RunStateModify.isig_chg = 0;
	
	return 0;
}

static int MdbGprsSigQuality(unsigned char *buf, int *plen)
{
	int sigdbm = 0;
	
	*plen = 2;
	sigdbm = GprsDevSigdBm * 10;

	IntToBcd(sigdbm, buf, 2);
	
	return 0;
}

typedef int (*mdbstat_pf)(unsigned char *buf, int *plen);

typedef struct {
	unsigned short id;
	mdbstat_pf pfunc;
} mdbfuncs_t;

static int MdbStatCommBytes(unsigned char *buf, int *plen)
{
	*plen = 8;
	smallcpy(buf, &MdbStic.term_day.comm_bytes, 4);
	smallcpy(buf+4, &MdbStic.term_mon.comm_bytes, 4);
	
	return 0;
}

static const mdbfuncs_t mdb_funcs[] = {
	{0x0002, MdbStatTime},
	{0x0004, MdbStatDataValid},
	{0x0008, MdbStatComm},

	{0x0040, MdbStatAlarmEc},
	{0x0080, MdbStatAlarmState},

	{0x0101, MdbStatIsig},
	{0x0102, MdbStatCommBytes},

	{0x0108, MdbGprsSigQuality},	
	
	{0, NULL},
};

/**
* @brief 读取当前状态数据
* @param metpid 测量点号, 1~MAX_CENMETP
* @param itemid 数据项编号
* @param buf 缓存区指针
* @param len 缓存区长度
* @return 成功返回实际读取长度, 失败返回负数, 无此数据项返回-2, 缓存区溢出返回-1
*/
int ReadMdbState(unsigned short metpid, unsigned short itemid, unsigned char *buf, int len)
{
	mdbfuncs_t *plist = (mdbfuncs_t *)mdb_funcs;
	int alen;

	if(0 != metpid) return -1;

	for(; 0 != plist->id; plist++) {
		if(itemid == plist->id) {
			if(NULL == plist->pfunc) return -2;

			alen = 0;
			if((*plist->pfunc)(buf, &alen)) return -2;

			if(alen > len) return -1;
			return alen;
		}
	}

	if(0 == plist->id) return -1;

	return -1;
}

