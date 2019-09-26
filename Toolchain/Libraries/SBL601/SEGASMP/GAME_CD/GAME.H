
/* [POLYGON.C]	*/
extern PDATA PD_CUBE1,PD_CUBE2,PD_PLANE1;
extern PDATA PD_BREAK1,PD_BREAK2,PD_BREAK3,PD_BREAK4;
extern PDATA PD_BREAK5,PD_BREAK6;
extern ATTR  attribute_BREAK[];
extern ATTR  attribute_CUBE1[];
extern ATTR  attribute_CUBE2[];

/* [V_BLANK.C]	*/
extern void  SetVblank(void);

/* [ACT.C]		*/
extern void	init_objects(void);
extern void	_obje1_act(void);
extern void	_obje2_act(void);
extern void	_shot1_1_act(void);
extern void	_shot1_2_act(void);
extern void	_shot2_1_act(void);
extern void	_shot2_2_act(void);
extern void _obje1_demo(void);
extern void _obje2_demo(void);
extern void _pause_act(void);

/* [GAME_SUB.C]	*/
extern void game_init(void);
extern int  player_rotation(void);
extern void chg_angle(void);
extern void disp_life(void);
extern void set_poly(ANGLE ang[XYZ] , FIXED pos[XYZ]);
extern void spr_move(trigger_t trigger , FIXED pos[XYZ], ANGLE ang[XYZ]);
extern int spr_col(FIXED posA[XYZ], FIXED posB[XYZ]);
extern void break_init(ANGLE ang[XYZ],FIXED pos[XYZ]);
extern void spr_break(void);

/* [GAME.C]		*/
extern ANGLE	ang1[XYZ],ang2[XYZ];
extern FIXED	pos1[XYZ],pos2[XYZ];
extern ANGLE	angS1_1[XYZ],angS1_2[XYZ],angS2_1[XYZ],angS2_2[XYZ];
extern FIXED	posS1_1[XYZ],posS1_2[XYZ],posS2_1[XYZ],posS2_2[XYZ];
extern ANGLE	angB1[XYZ],angB2[XYZ],angB3[XYZ],angB4[XYZ];
extern FIXED	posB1[XYZ],posB2[XYZ],posB3[XYZ],posB4[XYZ];
extern ANGLE	angB5[XYZ],angB6[XYZ];
extern FIXED	posB5[XYZ],posB6[XYZ];
extern trigger_t	trigger1,trigger2;
extern Uint16	shot1_1,shot1_2,shot2_1,shot2_2;
extern Uint16	wait1,wait2;
extern Uint16	colflg1,colflg2;
extern Uint16	life1,life2;
extern Uint16	submode,gameflg;
extern Uint16	endtimer,endflg;
extern Uint16	fadetimer;
extern Uint16	initflg_demo;


