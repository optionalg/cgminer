/*
 * Copyright 2013 Con Kolivas <kernel@kolivas.org>
 *
 * This program is free software; you can redistribute it and/or modify it
 * under the terms of the GNU General Public License as published by the Free
 * Software Foundation; either version 3 of the License, or (at your option)
 * any later version.  See COPYING for more details.
 */

#ifndef COINTERRA_H
#define COINTERRA_H

#define CTA_READBUF_SIZE 8192
#define CTA_MSG_SIZE 64
#define CTA_READ_TIMEOUT 1
#define CTA_READ_INTERVAL 100
#define CTA_SCAN_INTERVAL 500
#define CTA_RESET_TIMEOUT 200

#define CTA_INIT_DIFF		32*0.9999847412109375

#if 0
/* FIXME: how big should this be? */
#define CTA_MAX_QUEUE 2300
#else
#define CTA_MAX_QUEUE (32 / CTA_NROLL_TIME)
#endif

#define CTA_NROLL_TIME 2

/* Offsets into buffer */
#define CTA_MSG_TYPE		2
#define CTA_RESET_TYPE		3
#define CTA_RESET_DIFF		4
#define CTA_RESET_LOAD		5
#define CTA_DRIVER_TAG		3
#define CTA_MCU_TAG		5
#define CTA_MCU_CORE		5
#define CTA_MCU_ASIC		6
#define CTA_MCU_PIPE		8
#define CTA_MATCH_NOFFSET	45
#define CTA_MATCH_NONCE		60
#define CTA_WDONE_NONCES	11
#define CTA_MSG_RECVD		3
#define CTA_WORK_MIDSTATE	9
#define CTA_WORK_DATA		41
#define CTA_WORK_NROLL		53
#define CTA_WORK_DIFFBITS	55
#define CTA_REQWORK_REQUESTS	3

/* Received message types */
#define CTA_RECV_UNUSED		0
#define CTA_RECV_REQWORK	1
#define CTA_RECV_MATCH		2
#define CTA_RECV_WDONE		3
#define CTA_RECV_STATREAD	4
#define CTA_RECV_STATSET	5
#define CTA_RECV_INFO		6
#define CTA_RECV_MSG            7
#define CTA_RECV_RDONE		8
#define CTA_RECV_STATDEBUG	10

/* Sent message types */
#define CTA_SEND_UNUSED		0
#define CTA_SEND_RESET		1
#define CTA_SEND_WORK		2
#define CTA_SEND_SETPERF	3
#define CTA_SEND_REQUEST	4
#define CTA_SEND_FMATCH		5
#define CTA_SEND_IDENTIFY	6

/* Types of reset in CTA_RESET_TYPE */
#define CTA_RESET_NONE		0
#define CTA_RESET_UPDATE	1
#define CTA_RESET_NEW		2
#define CTA_RESET_INIT		3

#define CTA_INFO_HWREV		3
#define CTA_INFO_SERNO		5
#define CTA_INFO_NUMASICS	9
#define CTA_INFO_NUMDIES	10
#define CTA_INFO_NUMCORES	11
#define CTA_INFO_BOARDNUMBER	13
#define CTA_INFO_FWREV_MAJ	19
#define CTA_INFO_FWREV_MIN	20
#define CTA_INFO_FWREV_MIC	21
#define CTA_INFO_FWDATE_YEAR	23
#define CTA_INFO_FWDATE_MONTH	25
#define CTA_INFO_FWDATE_DAY	26
#define CTA_INFO_INITDIFFBITS	27
#define CTA_INFO_MINDIFFBITS	28
#define CTA_INFO_MAXDIFFBITS	29

#define CTA_STAT_CORETEMPS	3
#define CTA_STAT_AMBTEMP_LOW	19
#define CTA_STAT_AMBTEMP_AVG	21
#define CTA_STAT_AMBTEMP_HIGH	23
#define CTA_STAT_PUMP_TACHS	25
#define CTA_STAT_FAN_TACHS	29
#define CTA_STAT_CORE_VOLTS	37
#define CTA_STAT_VOLTS33	53
#define CTA_STAT_VOLTS12	55
#define CTA_STAT_INACTIVE	57
#define CTA_STAT_ACTIVE		59

#define CTA_STAT_PERFMODE	3
#define CTA_STAT_FANSPEEDS	11
#define CTA_STAT_DIES_ACTIVE	15
#define CTA_STAT_PIPES_ENABLED	16
#define CTA_STAT_MIN_FAN_SPEED 	24
#define CTA_STAT_UPTIME		25
#define CTA_STAT_HEARTBEATS	29
#define CTA_STAT_CORE_FREQS	45

#define CTA_STAT_UNDERRUNS	3
#define CTA_STAT_HW_ERRORS	5
#define CTA_STAT_UPTIME_MS	21
#define CTA_STAT_HASHES		25
#define CTA_STAT_FLUSHED_HASHES	33

#define CTA_CORES		8
#define CTA_PUMPS		2
#define CTA_FANS		4

#define CTA_REQ_MSGTYPE		3
#define CTA_REQ_INTERVAL	5


int opt_cta_load;

struct cointerra_info {
	/* Info data */
	uint16_t hwrev;
	uint32_t serial;
	uint8_t asics;
	uint8_t dies;
	uint16_t cores;
	uint8_t board_number;
	uint8_t fwrev[3];
	uint16_t fw_year;
	uint8_t fw_month;
	uint8_t fw_day;
	uint8_t init_diffbits;
	uint8_t min_diffbits;
	uint8_t max_diffbits;

	/* Status readings data */
	uint16_t coretemp[CTA_CORES];
	uint16_t ambtemp_low;
	uint16_t ambtemp_avg;
	uint16_t ambtemp_high;
	uint16_t pump_tachs[CTA_PUMPS];
	uint16_t fan_tachs[CTA_FANS];
	uint16_t corevolts[CTA_CORES];
	uint16_t volts33;
	uint16_t volts12;
	uint16_t inactive;
	uint16_t active;
	uint16_t corefreqs[CTA_CORES];
	uint32_t uptime;

	/* Status settings data */
	uint8_t coreperf[CTA_CORES];
	uint8_t fanspeed[CTA_FANS];
	uint8_t dies_active;
	uint8_t pipes_enabled[CTA_CORES];

	/* Status debug data */
	uint16_t underruns;
	uint16_t hw_errors[CTA_CORES];

	/* Running total from debug messages */
	int tot_underruns;
	uint16_t tot_hw_errors[CTA_CORES];
	uint64_t tot_hashes;
	uint64_t tot_reset_hashes;
	uint64_t tot_flushed_hashes;

	/* Calculated totals based on work done and nonces found */
	uint64_t hashes;
	uint64_t tot_calc_hashes;

	/* Calculated totals based on shares returned */
	uint64_t share_hashes;
	int requested;
	uint16_t work_id;
	int no_matching_work;
	time_t last_pipe_nonce[1024];
	unsigned char pipe_bitmap[128];

	struct thr_info *thr;
	pthread_mutex_t lock;
	pthread_mutex_t sendlock;
	pthread_cond_t wake_cond;
	pthread_t read_thr;
	cgsem_t reset_sem;
};

#endif /* COINTERRA_H */
