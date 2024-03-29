
#include "ECS\Entity.hpp"

struct Position
{
    int x, y;
};

struct Velocity
{
    int x, y;
};

void main()
{
    using EntityST = Entity<Position, Velocity>;

    EntityST::Create(Position(10, 20),
                     Velocity(1, 2));

    while (1)
    {
        EntityST::ForEach(
            [](Position &p, Velocity &v)
            {
                p.x += v.x;
                p.y += v.y;

                dbgio_printf("Position x:%d y:%d\n", p.x, p.y);
            });

        dbgio_flush();
        vdp2_sync();
        vdp2_sync_wait();
    }
}

void user_init()
{
    vdp2_tvmd_display_res_set(VDP2_TVMD_INTERLACE_NONE, VDP2_TVMD_HORZ_NORMAL_A, VDP2_TVMD_VERT_256);
    vdp2_scrn_back_screen_color_set(VDP2_VRAM_ADDR(3, 0x01FFFE), COLOR_RGB1555(1, 0, 3, 15));
    vdp2_tvmd_display_set();
    dbgio_dev_default_init(DBGIO_DEV_VDP2_ASYNC);
    dbgio_dev_font_load();
}
