/****************************************************************
KPIT Cummins Infosystems Ltd, Pune, India. - 01-Sept-2003.

This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.

*****************************************************************/

#ifdef __cplusplus
extern "C" {
#endif

extern void start (void);     /* Startup code (in start.s)  */
extern void stack (void);  /* stack from linker script   */

/**----------------------------------
**  Typedef for the function pointer
**-----------------------------------*/

typedef void (*fp) (void);
#define VECT_SECT          __attribute__ ((section (".vects")))

fp HardwareVectors[] VECT_SECT = {
  start,                    /* 0  Power-on reset, Program counter (PC) */
  (fp)stack,                /* 1  Power-on reset, Stack pointer (SP)   */
  (fp)0L,                   /* 2  Reserved */
  (fp)0L,                   /* 3  Reserved */
  (fp)0L,                   /* 4  general_error, Illegal instruction exception */
  (fp)0L,                   /* 5  Reserved */
  (fp)0L,                   /* 6  general_error, Illegal slot exception */
  (fp)0L,                   /* 7  Reserved */
  (fp)0L,                   /* 8  Reserved */
  (fp)0L,                   /* 9  general_error, CPU Address error  */
  (fp)0L,                   /* 10 general_error, DMAC Address error */
  (fp)0L,                   /* 11 NMI_INT,            */
  (fp)0L,                   /* 12 User Break */
  /*************** Reserved *****************/
  (fp)0L, (fp)0L, (fp)0L, (fp)0L, (fp)0L, (fp)0L, (fp)0L, (fp)0L, /* 13-20 */
  (fp)0L, (fp)0L, (fp)0L, (fp)0L, (fp)0L, (fp)0L, (fp)0L, (fp)0L, /* 21-28 */
  (fp)0L, (fp)0L, (fp)0L, (fp)0L, (fp)0L, (fp)0L, (fp)0L, (fp)0L, /* 29-36 */
  (fp)0L, (fp)0L, (fp)0L, (fp)0L, (fp)0L, (fp)0L, (fp)0L, (fp)0L, /* 37-44 */
  (fp)0L, (fp)0L, (fp)0L, (fp)0L, (fp)0L, (fp)0L, (fp)0L, (fp)0L, /* 45-52 */
  (fp)0L, (fp)0L, (fp)0L, (fp)0L, (fp)0L, (fp)0L, (fp)0L, (fp)0L, /* 53-60 */
  (fp)0L, (fp)0L, (fp)0L,                                         /* 61-63 */
  /*************** IRQs 0-7 *****************/
  (fp)0L,                   /* 64 MA_IntHandler_IRQ0_INT,  */
  (fp)0L,                   /* 65 MA_IntHandler_IRQ1_INT,  */
  (fp)0L,                   /* 66 MA_IntHandler_IRQ2_INT,  */
  (fp)0L,                   /* 67 MA_IntHandler_IRQ3_INT,  */
  (fp)0L,                   /* 68 MA_IntHandler_IRQ4_INT,  */
  (fp)0L,                   /* 69 MA_IntHandler_IRQ5_INT,  */
  (fp)0L,                   /* 70 MA_IntHandler_IRQ6_INT,  */
  (fp)0L,                   /* 71 MA_IntHandler_IRQ7_INT,  */
  /*************** DMAC 0-3 *****************/
  (fp)0L,		   /* 72 DEI0_DMA */
  (fp)0L,                  /* 73 Reserved */
  (fp)0L, 		   /* 74 Reserved */
  (fp)0L,                  /* 75 Reserved */
  (fp)0L,		   /* 76 DEI1_DMA */
  (fp)0L,                  /* 77 Reserved */
  (fp)0L,		   /* 78 Reserved */
  (fp)0L,                  /* 79 Reserved */
  (fp)0L,                   /* 80 DEI2_DMA */
  (fp)0L,                   /* 81 Reserved */
  (fp)0L,                   /* 82 Reserved */
  (fp)0L,                   /* 83 Reserved */
  (fp)0L,                   /* 84 DEI3_DMA */
  (fp)0L,                   /* 85 Reserved */
  (fp)0L,                   /* 86 Reserved */
  (fp)0L,                   /* 87 Reserved */
  /*************** MTU 0 ********************/
  (fp)0L,                   /* 88 MTU0_TGI0a */
  (fp)0L,                   /* 89 MTU0_TGI0b */
  (fp)0L,                   /* 90 MTU0_TGI0c */
  (fp)0L,                   /* 91 MTU0_TGI0d */
  (fp)0L,                   /* 92 MTU0_TcI0v */
  (fp)0L,                   /* 93 Reserved */
  (fp)0L,                   /* 94 Reserved */
  (fp)0L,                   /* 95 Reserved */
  /*************** MTU 1 ********************/
  (fp)0L,                   /* 96  MTU1_TGI1a */
  (fp)0L,                   /* 97  MTU1_TGI1b */
  (fp)0L,                   /* 98  Reserved */
  (fp)0L,                   /* 99  Reserved */
  (fp)0L,                   /* 100 MTU1_TcI1v */
  (fp)0L,                   /* 101 MTU1_TcI1u */
  (fp)0L,                   /* 102 Reserved */
  (fp)0L,                   /* 103 Reserved */
  /*************** MTU 2 ********************/
  (fp)0L,                   /* 104 MTU2_TGI2a */
  (fp)0L,                   /* 105 MTU2_TGI2b */
  (fp)0L,                   /* 106 Reserved */
  (fp)0L,                   /* 107 Reserved */
  (fp)0L,                   /* 108 MTU2_TCI2V */
  (fp)0L,                   /* 109 MTU2_TCI2U */
  (fp)0L,                   /* 110 Reserved */
  (fp)0L,                   /* 111 Reserved */
  /*************** MTU 3 ********************/
  (fp)0L,                   /* 112 MTU3_TGI3a */
  (fp)0L,                   /* 113 MTU3_TGI3b */
  (fp)0L,                   /* 114 MTU3_TGI3c */
  (fp)0L,                   /* 115 MTU3_TGI3d */
  (fp)0L,                   /* 116 MTU3_TcI3v */
  (fp)0L,                   /* 117 Reserved */
  (fp)0L,                   /* 118 Reserved */
  (fp)0L,                   /* 119 Reserved */
  /*************** MTU 4 ********************/
  (fp)0L,                   /* 120 MTU4_TGI4a */
  (fp)0L,                   /* 121 MTU4_TGI4b */
  (fp)0L,                   /* 122 MTU4_TGI4c */
  (fp)0L,                   /* 123 MTU4_TGI4d */
  (fp)0L,                   /* 124 MTU4_TcI4v */
  (fp)0L,                   /* 125 Reserved */
  (fp)0L,                   /* 126 Reserved */
  (fp)0L,                   /* 127 Reserved */
  /*************** SCI 0 ********************/
  (fp)0L,                   /* 128 SCI0_ERI0 */
  (fp)0L,                   /* 129 SCI0_RXI0 */
  (fp)0L,                   /* 130 SCI0_TXI0 */
  (fp)0L,                   /* 131 SCI0_TEI0 */
  /*************** SCI 1 ********************/
  (fp)0L,                   /* 132 SCI1_ERI1 */
  (fp)0L,                   /* 133 SCI1_RXI1 */
  (fp)0L,                   /* 134 SCI1_TXI1 */
  (fp)0L,                   /* 135 SCI1_TEI1 */
  /*************** A/D  ********************/
  (fp)0L,                   /* 136 A/D_ADI0 */
  (fp)0L,                   /* 137 A/D_ADI1 */
  (fp)0L,                   /* 138 Reserved */
  (fp)0L,                   /* 139 Reserved */
  /*************** DTC  ********************/
  (fp)0L,                   /* 140 DTC_SWI */
  (fp)0L,                   /* 141 Reserved */
  (fp)0L,                   /* 142 Reserved */
  (fp)0L,                   /* 143 Reserved */
  /*************** CMT 0 & 1 ********************/
  (fp)0L,		    /* 144 CMT0_CMI0 */
  (fp)0L,                   /* 145 Reserved */
  (fp)0L,                   /* 146 Reserved */
  (fp)0L,                   /* 147 Reserved */
  (fp)0L,                   /* 148 CMT1_CMI1 */
  (fp)0L,                   /* 149 Reserved */
  (fp)0L,                   /* 150 Reserved */
  (fp)0L,                   /* 151 Reserved */
  /*************** WDT ********************/
  (fp)0L,		    /* 152 WDT_ITI */
  /*************** BSC ********************/
  (fp)0L,		    /* 153 BSC_CMI */
  (fp)0L,		    /* 154 Reserved */
  (fp)0L,                   /* 155 Reserved  */
  /*************** I/O ********************/
  (fp)0L,                    /* 156 I/O_OEI  */
};

#ifdef __cplusplus
} // extern "C" {
#endif
