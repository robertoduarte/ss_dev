/********************************/
/*	#include		*/
/********************************/

#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <string.h>
#include "sega_xpt.h"

/********************************/
/*	#define			*/
/********************************/

#define	TAB		0x09
#define	CR_D		0x0d
#define	CR_A		0x0a
#define	TEX_MAX		100		/* テクスチャ数の上限 */
#define	MBU_MAX		100		/* マテリアルバッファ数 */
#define	DAT_MAX		50000		/* ピクセル数の上限 */
#define	PLT_MAX		2050		/* カラーテーブル数の上限 */
#define	DRAW_DEF_T " MATE_TEXTURE | ECD_DISABLE"
#define	DRAW_DEF_S " MATE_TEXTURE | ECDSPD_DISABLE"

/********************************/
/*	構造体			*/
/********************************/

/*  テクスチャ管理用構造体  */
typedef struct Tex{
	Sint16		char_no;	/* キャラクタ番号 */
	Sint8		chr_nam[16];	/* キャラクタ名 */
	Sint16		width;		/* キャラクタ幅 */
	Sint16		height;		/* キャラクタ高 */
}  Texture;

/*  マテリアル情報格納バッファ用構造体  */
typedef struct Mbuf{
	Sint8		mat_nam[16];	/* マテリアル名 */
	Sint8		tex_nam[16];	/* テクスチャファイル名 */
	Sint16		type;		/* タイプ */
	Sint16		mode;		/* 描画モード */
				/* （ 0:color, 1:auto, 2:index, 3:texture ） */
}  Matbuf;

/********************************/
/*	関数プロトタイプ	*/
/********************************/

#ifdef __BORLANDC__
void    printUsage(void);
void	ctl_read(void);
void	mat_read(void);
void	tex_read(Sint16 no);
void	tex_red2(Sint16 no);
void	tex_red3(Sint16 no);
void	pix_wrt(Sint16 no);
void	plt_wrt(Sint16 no);
void	plt_wrt2(void);
void	drw_wrt(void);
void	ex_keyword(Sint16 j, Sint8 *buf, Sint8 *keyword);
void	ex_keyword2(Sint16 *k0, Sint8 type[8][6], Sint16 j, Sint8 *buf, Sint8 *keyword);
void	rgb_set(Uint16 huge *rgb);
#else
void    printUsage();
void	ctl_read();
void	mat_read();
void	tex_read();
void	tex_red2();
void	tex_red3();
void	pix_wrt();
void	plt_wrt();
void	plt_wrt2();
void	drw_wrt();
void	ex_keyword();
void	ex_keyword2();
void	rgb_set();
#endif

/********************************/
/*	外部変数		*/
/********************************/

#ifdef	MAIN
#ifdef __BORLANDC__
	Texture	huge txt[TEX_MAX];
	Matbuf	huge mbuf[MBU_MAX];
	Uint16	huge g_texdata[DAT_MAX];
	Uint16	huge g_pltdata[PLT_MAX];
#else
	Texture	txt[TEX_MAX];
	Matbuf	mbuf[MBU_MAX];
	Uint16	g_texdata[DAT_MAX];
	Uint16	g_pltdata[PLT_MAX];
#endif
	Sint16	g_tex_max;
	Sint16	g_tex_cnt;
	Sint16	g_t_max;
	Sint16	g_plt_max = 0;
	Uint16	g_pix_max;
	Uint16	g_plt_mod;
	Uint16	g_col_mod;
	Uint16	g_col_lim;
	Uint16	g_spd_mod;	/* 追加:透明コードを無効にする/しない */
	Uint8	g_col_tbl[16];
	Uint8	g_txc_nam[24];
	Sint8	*g_dirname;
	Sint8	*g_outname;
	Sint8	g_s3dname[150];
	Sint8	g_c_outname[150];
	Sint8	g_in_ext[8];
	Sint8	g_out_ext[8];
#else
#ifdef __BORLANDC__
	extern	Texture	huge txt[];
	extern	Matbuf	huge mbuf[];
	extern	Uint16	huge g_texdata[];
	extern	Uint16	huge g_pltdata[];
#else
	extern	Texture	txt[];
	extern	Matbuf	mbuf[];
	extern	Uint16	g_texdata[];
	extern	Uint16	g_pltdata[];
#endif
	extern	Sint16	g_tex_max;
	extern	Sint16	g_tex_cnt;
	extern	Sint16	g_t_max;
	extern	Sint16	g_plt_max;
	extern	Uint16	g_pix_max;
	extern	Uint16	g_plt_mod;
	extern	Uint16	g_col_mod;
	extern	Uint16	g_col_lim;
	extern	Uint16	g_spd_mod;	/* 追加:透明コードを有効にする/しない */
	extern	Uint8	g_col_tbl[];
	extern	Uint8	g_txc_nam[];
	extern	Sint8	*g_dirname;
	extern	Sint8	*g_outname;
	extern	Sint8	g_s3dname[];
	extern	Sint8	g_c_outname[];
	extern	Sint8	g_in_ext[];
	extern	Sint8	g_out_ext[];
#endif
