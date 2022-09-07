#include <yaul.h>
#include <assert.h>
#include <stdlib.h>
#include "ECS/ECS.h"

void _vblank_out_handler(void *work __unused)
{
    smpc_peripheral_intback_issue();
}

void user_init(void)
{
    vdp2_tvmd_display_res_set(VDP2_TVMD_INTERLACE_NONE, VDP2_TVMD_HORZ_NORMAL_A,
                              VDP2_TVMD_VERT_224);

    vdp2_scrn_back_screen_color_set(VDP2_VRAM_ADDR(3, 0x01FFFE),
                                    COLOR_RGB1555(1, 0, 3, 15));

    vdp_sync_vblank_out_set(_vblank_out_handler, NULL);

    cpu_intc_mask_set(0);

    vdp2_tvmd_display_set();
}

int main(void)
{
    dbgio_dev_default_init(DBGIO_DEV_VDP2_ASYNC);
    dbgio_dev_font_load();

    ECS_Init();
    ECS_Get_Motion(ECS_CreateEntity(CId_Motion))->acceleration.x = 10;
    ECS_Get_Health(ECS_CreateEntity(CId_Health))->current = 200;

    EntityId id = ECS_CreateEntity(CId_Health);
    ECS_Get_Health(id)->current = 100;
    ECS_AddComponents(id, CId_Motion);
    ECS_Get_Motion(id)->acceleration.x = 20;

    ECS_ForEach(CId_Motion,LAMBDA(
        {
            dbgio_printf("Hello from id: %d!\n", id);
            dbgio_printf("X Acceleration: %d\n", ECS_Access_Motion(accessor)->acceleration.x);
        }));

    dbgio_printf("\n");

    ECS_ForEach(CId_Health, LAMBDA(
        {
            dbgio_printf("Hello from id: %d!\n", id);
            dbgio_printf("Current Health: %d\n", ECS_Access_Health(accessor)->current);
        }));

    dbgio_printf("\n");

    ECS_ForEach(CId_Motion | CId_Health,LAMBDA(
        {
            dbgio_printf("Hello from id: %d!\n", id);
            dbgio_printf("X Acceleration: %d\n", ECS_Access_Motion(accessor)->acceleration.x);
            dbgio_printf("Current Health: %d\n", ECS_Access_Health(accessor)->current);
        }));
        
    dbgio_flush();
    vdp2_sync();
    vdp2_sync_wait();

    return 1;
}
