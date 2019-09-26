//------------------------------------------------------------------------
//
//	SATURN.H
//	Saturn-specific hardware header
//
//	CONFIDENTIAL
//	Copyright (c) 1995-1996, Sega Technical Institute
//
//	AUTHOR
//  Russell Bornsch++, 4/96
//
//	TARGET
//	GCC for SH2
//
//	REVISION
//	  8/7/96 - RAB - Cleanup for demo release
//
//------------------------------------------------------------------------

#define SH2_TIER  ((volatile unsigned char *)0xFFFFFE10)
#define SH2_TCSR  ((volatile unsigned char *)0xFFFFFE11)
#define SH2_FRC   ((volatile unsigned short*)0xFFFFFE12)
#define SH2_FRCH  ((volatile unsigned char *)0xFFFFFE12)
#define SH2_FRCL  ((volatile unsigned char *)0xFFFFFE13)
#define SH2_OCRAB  ((volatile unsigned short*)0xFFFFFE14)
#define SH2_OCRABH  ((volatile unsigned char *)0xFFFFFE14)
#define SH2_OCRABL  ((volatile unsigned char *)0xFFFFFE15)
#define SH2_TCR   ((volatile unsigned char *)0xFFFFFE16)
#define SH2_TOCR  ((volatile unsigned char *)0xFFFFFE17)
#define SH2_ICRA  ((volatile unsigned short*)0xFFFFFE18)
#define SH2_ICRAH  ((volatile unsigned char *)0xFFFFFE18)
#define SH2_ICRAL  ((volatile unsigned char *)0xFFFFFE19)

#define SH2_TCR_CKS0 0x01
#define SH2_TCR_CKS1 0x02
#define SH2_TCR_BUFEB 0x04
#define SH2_TCR_BUFEA 0x08
#define SH2_TCR_IEDGA 0x80

#define SH2_TCSR_CCLRA 0x01
#define SH2_TCSR_OVF 0x02
#define SH2_TCSR_OCFB 0x04
#define SH2_TCSR_OCFA 0x08
#define SH2_TCSR_ICFA 0x80

extern void SatInitTime(void);
extern long SatGetTime(void);


