#include <yaul.h>
#include "Trigonometry.h"

void main(void)
{
    dbgio_init();
    dbgio_dev_default_init(DBGIO_DEV_VDP2_ASYNC);
    dbgio_dev_font_load();

    dbgio_printf("0 Degrees:\nSin:%f Cos:%f\nTan:%f\n",
                 Trigonometry_Sin(DEGREES_TO_ANGLE(0)),
                 Trigonometry_Cos(DEGREES_TO_ANGLE(0)),
                 Trigonometry_Tan(DEGREES_TO_ANGLE(0)));

    dbgio_printf("45 Degrees:\nSin:%f Cos:%f\nTan:%f\n",
                 Trigonometry_Sin(DEGREES_TO_ANGLE(45)),
                 Trigonometry_Cos(DEGREES_TO_ANGLE(45)),
                 Trigonometry_Tan(DEGREES_TO_ANGLE(45)));
    
    dbgio_printf("90 Degrees:\nSin:%f Cos:%f\nTan:%f\n",
                 Trigonometry_Sin(DEGREES_TO_ANGLE(90)),
                 Trigonometry_Cos(DEGREES_TO_ANGLE(90)),
                 Trigonometry_Tan(DEGREES_TO_ANGLE(90)));

    dbgio_printf("135 Degrees:\nSin:%f Cos:%f\nTan:%f\n",
                 Trigonometry_Sin(DEGREES_TO_ANGLE(135)),
                 Trigonometry_Cos(DEGREES_TO_ANGLE(135)),
                 Trigonometry_Tan(DEGREES_TO_ANGLE(135)));
    
    dbgio_printf("180 Degrees:\nSin:%f Cos:%f\nTan:%f\n",
                 Trigonometry_Sin(DEGREES_TO_ANGLE(180)),
                 Trigonometry_Cos(DEGREES_TO_ANGLE(180)),
                 Trigonometry_Tan(DEGREES_TO_ANGLE(180)));
    
    dbgio_printf("225 Degrees: Sin:%f Cos:%f\nTan:%f\n",
                 Trigonometry_Sin(DEGREES_TO_ANGLE(225)),
                 Trigonometry_Cos(DEGREES_TO_ANGLE(225)),
                 Trigonometry_Tan(DEGREES_TO_ANGLE(225)));
    
    dbgio_printf("270 Degrees:\nSin:%f Cos:%f\nTan:%f\n",
                 Trigonometry_Sin(DEGREES_TO_ANGLE(270)),
                 Trigonometry_Cos(DEGREES_TO_ANGLE(270)),
                 Trigonometry_Tan(DEGREES_TO_ANGLE(270)));
    
    dbgio_printf("315 Degrees:\nSin:%f Cos:%f\nTan:%f\n",
                 Trigonometry_Sin(DEGREES_TO_ANGLE(315)),
                 Trigonometry_Cos(DEGREES_TO_ANGLE(315)),
                 Trigonometry_Tan(DEGREES_TO_ANGLE(315)));

    dbgio_printf("Atan x:1.0 y:1.0001 = %f\n", Trigonometry_Atan2(FIX16(1.0f), FIX16(1.0001f)));
    dbgio_printf("Atan x:-1.0 y:1.0001 = %f\n", Trigonometry_Atan2(FIX16(-1.0f), FIX16(1.001f)));
    dbgio_printf("Atan x:-1.0 y:-1.0001 = %f\n", Trigonometry_Atan2(FIX16(-1.0f), FIX16(-1.0001f)));
    dbgio_printf("Atan x:1.0 y:-1.0001 = %f\n", Trigonometry_Atan2(FIX16(1.0f), FIX16(-1.0001f)));

    dbgio_flush();
    vdp2_sync();
    vdp2_sync_wait();
}

static void
_vblank_out_handler(void *work __unused)
{
    smpc_peripheral_intback_issue();
}

void user_init(void)
{
    smpc_peripheral_init();

    vdp2_tvmd_display_res_set(VDP2_TVMD_INTERLACE_NONE, VDP2_TVMD_HORZ_NORMAL_A,
                              VDP2_TVMD_VERT_224);

    vdp2_scrn_back_color_set(VDP2_VRAM_ADDR(3, 0x01FFFE),
                             COLOR_RGB1555(1, 0, 3, 15));

    vdp_sync_vblank_out_set(_vblank_out_handler, NULL);

    vdp2_tvmd_display_set();
}