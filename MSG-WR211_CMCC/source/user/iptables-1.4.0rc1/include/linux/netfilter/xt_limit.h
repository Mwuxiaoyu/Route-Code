#ifndef _XT_RATE_H
#define _XT_RATE_H

/* timings are in milliseconds. */
//#define XT_LIMIT_SCALE 250000
#define XT_LIMIT_SCALE 10000

/* 1/10,000 sec period => max of 10,000/sec.  Min rate is then 429490
   seconds, or one every 59 hours. */
struct xt_rateinfo {
	u_int32_t avg;    /* Average secs between packets * scale */
	u_int32_t burst;  /* Period multiplier for upper limit. */

	/* Used internally by the kernel */
	unsigned long prev;
	/* Ugly, ugly fucker. */
	struct xt_rateinfo *master;

	u_int32_t credit;
	u_int32_t credit_cap, cost;
};
#endif /*_XT_RATE_H*/
