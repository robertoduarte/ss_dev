#include <yaul.h>

void main(void)
{
    dbgio_init();
    dbgio_dev_default_init(DBGIO_DEV_VDP2_ASYNC);
    dbgio_dev_font_load();

    dbgio_printf("Hello World from Template Project!\n");

    dbgio_flush();
    vdp2_sync();
    vdp2_sync_wait();
}

void user_init(void)
{
    smpc_peripheral_init();

    vdp2_tvmd_display_res_set(VDP2_TVMD_INTERLACE_NONE, VDP2_TVMD_HORZ_NORMAL_A,
                              VDP2_TVMD_VERT_224);

    vdp2_scrn_back_color_set(VDP2_VRAM_ADDR(3, 0x01FFFE),
                             RGB1555(1, 0, 3, 15));

    vdp2_tvmd_display_set();
}
