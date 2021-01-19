/*
     Application initialization program sample
                               V1.10  '94.11.11  S.U and T.S
     About operation timing and function:
        Various devices that are undefined while the license screen is displayed
        Clears the status and its memory.
       The process takes about 0.1 seconds.
        After finishing, move on to the execution of the pre-read program.
        Define its execution start address in APP_ENTRY.
     About linkage:
       Please link immediately after sys_init.obj of IP.
       If you use the program as it is, it will take about 360H bytes and the IP size will exceed 1000H.
     Notes on changing the program:
        Do not add functions containing executable statements before main ().*/

#include "sega_sys.h"

// Execution start address of the pre-read program (jump after completion of main processing)）
#define APP_ENTRY (0x6004000)

// Routine to initialize the device
static void vd1Comfil(void);    // VDP1 Clipping initialization
static void vd2Ramfil(void);    // VDP2 VRAM clear
static void colRamfil(void);    // Color RAM clear
static void sndRamfil(Sint32);  // Sound RAM clear
static void scuDspInit(void);   // Initialization of CPU DSP
static void msh2PeriInit(void); // Master SH peripheral module initialization
static void sndDspInit(void);   // Sound DSP clear

// Other subroutines
static void vbIrtn(void);  // VB-In interrupt processing
static void vbOrtn(void);  // VB-Out interrupt processing
static void syncVbI(void); // VB-In For synchronous processing
static void memset_w(volatile Sint16 *, Sint16, Sint32);
static void memcpy_w(volatile Sint16 *, Sint16 *, Sint32);
static void blkmfil_w(volatile Sint16 *, Sint16, Sint32);
// Word block fill
static void blkmfil_l(volatile Sint32 *, Sint32, Sint32);
// Longword block fill

// Information about the current screen size (while the license screen is displayed)
#define XRES (320)           // Horizontal size of license screen
#define SCLIP_UX (XRES - 1)  //             〃
#define SCLIP_UY_N (224 - 1) //   〃  (For NTSC)
#define SCLIP_UY_P (256 - 1) //   〃   (For PAL)

// Base address of device to be processed
#define SND_RAM ((volatile Sint32 *)0x25a00000)
#define VD1_VRAM ((volatile Sint16 *)0x25c00000)
#define VD1_REG ((volatile Sint16 *)0x25d00000)
// VD2_VRAM excludes VRAM area used in license display
#define VD2_VRAM ((volatile Sint32 *)0x25e08004)
// COL_RAM excludes color RAM area used in license display
#define COL_RAM ((volatile Sint16 *)0x25f00020)
#define VD2_REG ((volatile Sint16 *)0x25f80000)
#define SCSP_DSP_RAM ((volatile Sint16 *)0x25b00800)

// SMPC register
#define SMPC_REG(ofs) (*(volatile Uint8 *)(0x20100000 + ofs))

// SCU register
#define DSP_PGM_CTRL_PORT (*(volatile Sint32 *)0x25fe0080)
#define DSP_PGM_RAM_PORT (*(volatile Sint32 *)0x25fe0084)
#define DSP_DATA_RAM_ADRS_PORT (*(volatile Sint32 *)0x25fe0088)
#define DSP_DATA_RAM_DATA_PORT (*(volatile Sint32 *)0x25fe008c)

// SCSP sound RAM size register
#define SCSP_SNDRAMSZ (*(volatile Sint8 *)0x25b00400)

// SH2 peripheral module register
#define MSH2_DMAC_SAR(ofs) (*(volatile Sint32 *)(0xffffff80 + ofs))
#define MSH2_DMAC_DAR(ofs) (*(volatile Sint32 *)(0xffffff84 + ofs))
#define MSH2_DMAC_TCR(ofs) (*(volatile Sint32 *)(0xffffff88 + ofs))
#define MSH2_DMAC_CHCR(ofs) (*(volatile Sint32 *)(0xffffff8c + ofs))
#define MSH2_DMAC_DRCR(sel) (*(volatile Sint8 *)(0xfffffe71 + sel))
#define MSH2_DMAC_DMAOR (*(volatile Sint32 *)(0xffffffb0))
#define MSH2_DIVU_CONT (*(volatile Sint32 *)(0xffffffb8))

// Information about main processing
// Currently displaying license, use 16-bit 1024 color mode
// Clear VDP2 VRAM and color RAM in four separate steps
// VRAM is 20,000H bytes at one time (excluding the part being displayed)
// The color RAM is 200H bytes at a time (excluding the part being displayed)
#define MSETDIV (4)
#define BLKMSK_VD2_VRAM (0x1fffc)
#define BLKMSK_COL_RAM (0x001fe)
// Clear sound RAM in 3 steps
#define M68000_VECTBLSZ (0x00400 / sizeof(Sint32))
#define BLKMSK_SND_RAM (0x003fc)
// Sound DSP RAM size
#define SCSP_DSP_RAMSZ (0x00400)

// Information about interrupt processing
#define VBI_NUM (0x40)   // VB-in interrupt number
#define VBO_NUM (0x41)   // VB out interrupt number
#define VB_MASK (0x0003) // Two SCU interrupt masks

// Static variables
static Sint16 yBottom, ewBotRight;
static Sint16 vdp1cmds[48];
static volatile Sint16 vbIcnt = 0, sequence = 0;
static volatile Sint32 *vramptr = VD2_VRAM;
static volatile Sint16 *cramptr = COL_RAM;

// Main processing
int __main(void)
{
        // Note: If you take the AUTO variable, the APP_ENTRY program
        // Stack is slightly wasted when control transfers

        yBottom = (VD2_REG[2] & 1) ? SCLIP_UY_P : SCLIP_UY_N;
        ewBotRight = ((XRES / 8) << 9) + (yBottom);
        // Screen Vertical upper limit = 223 (NTSC) or 255 (pal)

        SYS_SETUINT(VBI_NUM, vbIrtn); // VB-in process registration
        SYS_SETUINT(VBO_NUM, vbOrtn); // VB Out 〃
        SYS_CHGSCUIM(~VB_MASK, 0);    // Enable two VB interrupts

        vd1Comfil(); // Initialization of VDP1
        for (sequence = 0; sequence < MSETDIV; sequence++)
        {
                syncVbI();           // Synchronized to clear color RAM
                colRamfil();         // Color RAM clear
                vd2Ramfil();         // VDP2RAM clear
                sndRamfil(sequence); // Sound RAM clear
        }

        scuDspInit();   // SCU DSP initialization
        msh2PeriInit(); // SH peripheral module
        sndDspInit();   // Sound DSP 〃

        SYS_CHGSCUIM(-1, VB_MASK);           // Disable two VB interrupts
        SYS_SETUINT(VBI_NUM, (void (*)())0); // hook
        SYS_SETUINT(VBO_NUM, (void (*)())0); // Re-initialization

        ((void (*)())APP_ENTRY)(); //Next execution start address
}

static void memset_w(volatile Sint16 *buf, Sint16 pattern, Sint32 size)
{
        register Sint32 i;

        for (i = 0; i < size; i += sizeof(Sint16))
        {
                *buf++ = pattern;
        }
}

static void memcpy_w(volatile Sint16 *dst, Sint16 *src, Sint32 size)
{
        register Sint32 i;

        for (i = 0; i < size; i += sizeof(Sint16))
        {
                *dst++ = *src++;
        }
}

static void blkmfil_w(volatile Sint16 *buf, Sint16 pattern, Sint32 brkmsk)
{
        register Sint32 i;
        i = (volatile Sint32)buf & brkmsk;
        for (; i <= brkmsk; i += sizeof(Sint16))
        {
                *buf++ = pattern;
        }
}

static void blkmfil_l(volatile Sint32 *buf, Sint32 pattern, Sint32 brkmsk)
{
        register Sint32 i;
        i = (volatile Sint32)buf & brkmsk;
        for (; i <= brkmsk; i += sizeof(Sint32))
        {
                *buf++ = pattern;
        }
}

// VB-in only increments static variables by interrupt
static void vbIrtn(void)
{
        vbIcnt++;
}

// VB out only controls VDP1 register by interrupt
static void vbOrtn(void)
{
        register volatile Sint16 *vdp1r;
        // Clear frame buffer with erase light
        vdp1r = VD1_REG;
        *vdp1r++ = 0x0; // 1/60 second automatic drawing mode
        *vdp1r++ = 0x0;
        *vdp1r++ = 0x2;
        *vdp1r++ = 0x0;      // Erase light is transparent
        *vdp1r++ = 0x0;      //  on Upper left coordinate
        *vdp1r = ewBotRight; //  under Lower right coordinate
}

// VBI synchronization waits when called, exits and returns immediately after VBI
static void syncVbI(void)
{
        register Sint32 cur_cnt_value;
        // Waiting to clear the color RAM
        cur_cnt_value = vbIcnt;
        while (cur_cnt_value == vbIcnt)
                ;
}

// Make VDP1 read system clipping and local coordinates
static void vd1Comfil(void)
{
        register Sint16 *cmdbuf;

        memset_w((cmdbuf = vdp1cmds), 0, sizeof(vdp1cmds));
        cmdbuf[0] = 0x0009;
        cmdbuf[10] = SCLIP_UX;
        cmdbuf[11] = yBottom;
        cmdbuf[16] = 0x000a;
        cmdbuf[32] = 0x8000;
        memcpy_w(VD1_VRAM, vdp1cmds, sizeof(vdp1cmds));
}

// Clear VDP2 RAM by 1/4
static void vd2Ramfil(void)
{
        blkmfil_l(vramptr, 0, BLKMSK_VD2_VRAM);
}

// Clear color RAM by 1/4
static void colRamfil(void)
{
        blkmfil_w(cramptr, 0, BLKMSK_COL_RAM);
}

// Clear sound RAM in 3 steps
static void sndRamfil(Sint32 initstep)
{
        register volatile Sint32 *memptr;

        switch (initstep)
        {
        case 0:
                SMPC_REG(31) = 7; // Stop M68000
                break;
        case 1:
                SCSP_SNDRAMSZ = 2; // Sound RAM size setting
                // Sound RAM top 400H
                memptr = SND_RAM; // Fill (Vector) with 400H
                blkmfil_l(memptr, 0x400, BLKMSK_SND_RAM);
                *memptr = 0x0007fffc; // Set SP initial value
                memptr += M68000_VECTBLSZ;
                *memptr = 0x4e7160fc; // NOP at address 400H
                // BRA @ -2 Write instruction
                break;
        case 2:
                SMPC_REG(31) = 6; // M68000 activation (infinite waiting)
                break;
                // Remarks: There is one int
        } // Because of the SMPC status
} // Omit set / check

static void msh2PeriInit(void)
{
        register Sint32 i, ofs, dummy;

        ofs = 0;
        for (i = 0; i < 2; i++)
        { // Initialize DMAC registers
                MSH2_DMAC_SAR(ofs) = 0x00000000;
                MSH2_DMAC_DAR(ofs) = 0x00000000;
                MSH2_DMAC_TCR(ofs) = 0x00000001;
                dummy = MSH2_DMAC_CHCR(ofs);
                MSH2_DMAC_CHCR(ofs) = 0x00000000;
                MSH2_DMAC_DRCR(i) = 0x00;
                ofs = 0x10;
        }
        dummy = MSH2_DMAC_DMAOR;
        MSH2_DMAC_DMAOR = 0x00000000;
        // Disable DIVU interrupt
        MSH2_DIVU_CONT = 0x00000000;
}

static void scuDspInit(void)
{
        register Sint32 i;

        DSP_PGM_CTRL_PORT = 0x0; // DSP stopped

        for (i = 0; i < 256; i++)
                DSP_PGM_RAM_PORT = 0xf0000000; // END instruction fill

        for (i = 0; i < 256; i++)
        { // DSP RAM clear
                DSP_DATA_RAM_ADRS_PORT = i;
                DSP_DATA_RAM_DATA_PORT = 0x0;
        }
}

static void sndDspInit(void)
{
        memset_w(SCSP_DSP_RAM, 0, SCSP_DSP_RAMSZ);
        // Sound DSP
} // Program area clear
