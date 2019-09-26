/********************************/
/*	#include		*/
/********************************/
#define __BORLANDC__  1

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
#define	ANG_SEQ		0		/* 回転順序（ＺＹＸ）*/

#if 0
#define	VERT_MAX	5000		/* 頂点数の上限 */
#define	FACE_MAX	1000		/* 面数の上限 */
#define	OBJ_MAX		100		/* オブジェクト数の上限 */
#define	CLS_MAX		100		/* クラスタ数の上限 */
#define	SLP_MAX		200		/* スリップ中の面数の上限 */
#define	TEX_MAX		100		/* テクスチャ数の上限 */
#define	MBU_MAX		100		/* マテリアルバッファ数 */
#endif
#define	VERT_MAX	2500		/* 頂点数の上限 */
#define	FACE_MAX	1000		/* 面数の上限 */
#define	OBJ_MAX		200		/* オブジェクト数の上限 */
#define	CLS_MAX		200		/* クラスタ数の上限 */
#define	SLP_MAX		800		/* スリップ中の面数の上限 */
#define	TEX_MAX		100		/* テクスチャ数の上限 */
#define	MBU_MAX		100		/* マテリアルバッファ数 */

#define	DRAW_DEF_C " MATE_COLOR | COLOR_5 | ECDSPD_DISABLE"
#define	DRAW_DEF_A " MATE_AUTO_SHADING | COLOR_5 | ECDSPD_DISABLE"
#define	DRAW_DEF_I " MATE_SHADING_TBL | COLOR_5 | ECDSPD_DISABLE"
#define	DRAW_DEF_T " MATE_TEXTURE | COLOR_5 | ECDSPD_DISABLE"

/********************************/
/*	構造体			*/
/********************************/

/*  オブジェクト管理用構造体  */
typedef struct Obj{
	Sint8		u_obj_name[16];	/* オブジェクト固有名 */
	Sint8		l_obj_name[16];	/* オブジェクトライブラリ名 */
	Float32		scale_x;	/* Ｘ方向倍率（指定値） */
	Float32		scale_y;	/* Ｙ方向倍率（指定値） */
	Float32		scale_z;	/* Ｚ方向倍率（指定値） */
	Float32		scl_x_r;	/* Ｘ方向倍率（継承計算後の値） */
	Float32		scl_y_r;	/* Ｙ方向倍率（継承計算後の値） */
	Float32		scl_z_r;	/* Ｚ方向倍率（継承計算後の値） */
	Sint16		rev_x;		/* Ｘ方向反転フラグ（ 0:off, 1:on ）*/
	Sint16		rev_y;		/* Ｙ方向反転フラグ（ 0:off, 1:on ）*/
	Sint16		rev_z;		/* Ｚ方向反転フラグ（ 0:off, 1:on ）*/
	Sint16		shade;		/* シェーディング方式 */
					/* （ 0:no, 1:flat, 2:gouraud ） */
	Sint16		mode;		/* 描画モード */
				/* （ 0:color, 1:auto, 2:index, 3:texture ） */
	Sint16		p_type;		/* 面種別タイプ */
					/* （ 0:polygon, 1:polyline ） */
	Sint16		tex_no;		/* テクスチャ番号 */
	Sint16		v_max;		/* 頂点数 */
	Sint16		f_max;		/* 面数 */
}  Object;

/*  クラスタ管理用構造体  */
typedef struct Clu{
	Sint8		u_cls_name[16];	/* クラスタ固有名 */
	Sint16		ang_seq;	/* 回転順序 */
	Float32		angle_x;	/* Ｘ方向回転 */
	Float32		angle_y;	/* Ｙ方向回転 */
	Float32		angle_z;	/* Ｚ方向回転 */
	Float32		point_x;	/* Ｘ方向座標 */
	Float32		point_y;	/* Ｙ方向座標 */
	Float32		point_z;	/* Ｚ方向座標 */
	Sint16		nest;		/* ネスティング・レベル */
	Object	  	*object; 	/* オブジェクト構造体へのポインタ */
	struct Clu 	*c_child;	/* 子クラスタ用ポインタ */
	struct Clu 	*c_next;	/* ｎｅｘｔ用ポインタ */
}  Cluster;

/*  ポリゴンの頂点の管理用構造体  */
typedef struct {
	Float32		co_x;		/* Ｘ方向座標 */
	Float32		co_y;		/* Ｙ方向座標 */
	Float32		co_z;		/* Ｚ方向座標 */
	Float32		co_x_s;		/* Ｘ方向座標（倍率計算結果格納用） */
	Float32		co_y_s;		/* Ｙ方向座標（倍率計算結果格納用） */
	Float32		co_z_s;		/* Ｚ方向座標（倍率計算結果格納用） */
}  Vtex;

/*  ポリゴンの面の管理用構造体  */
typedef struct {
	Sint16		v_no_0;		/* 頂点番号　（１点目） */
	Sint16		v_no_1;		/* 頂点番号　（２点目） */
	Sint16		v_no_2;		/* 頂点番号　（３点目） */
	Sint16		v_no_3;		/* 頂点番号　（４点目） */
	Sint8		mat_nam[16];	/* マテリアル名 */
	Sint16		z_sort;		/* ＺＳＯＲＴ基準 */
					/* （ 0:AVE, 1:MIN, 2:MAX ） */
	Sint16		direction;	/* テクスチャの向き */
}  Face;

/*  テクスチャ管理用構造体  */
typedef struct Tex{
	Sint16		char_no;	/* キャラクタ番号 */
	Sint8		chr_nam[16];	/* キャラクタ名 */
	Sint8		clr_mod[16];	/* カラーモード */
	Sint16		color;		/* カラー       */
	Sint16		width;		/* キャラクタ幅 */
	Sint16		height;		/* キャラクタ高 */
	Sint8		lup_ptr[16];	/* ルックアップテーブルポインタ */
}  Texture;

/*  マテリアル情報格納バッファ用構造体  */
typedef struct Mbuf{
	Sint8		mat_nam[16];	/* マテリアル名 */
	Sint8		tex_nam[16];	/* テクスチャファイル名 */
	Sint16		type;		/* タイプ */
	Sint16		color;		/* カラー */
	Sint16		direction;	/* 頂点の並び */
	Sint16		mode;		/* 描画モード */
				/* （ 0:color, 1:auto, 2:index, 3:texture ） */
}  Matbuf;

/********************************/
/*	関数プロトタイプ	*/
/********************************/

#ifdef __BORLANDC__
void    printUsage(void);
void	val_init(void);
void	ctl_read(void);
void	ct_read2(void);
void	obj_read(void);
void	hi_anlze(void);
void	mat_read(void);
void	drw_wrt(void);
void	tex_wrt(void);
void	mdl_read(Sint16	no);
void	mdl_proc(Sint16	no);
void	slp_read(Sint16	no);
void	mt_read2(Sint8 *mat_name, Sint16 *m_bufno);
void	t3d_wrt(Sint16	no);
void	cls_wrt(void);
void	ex_keyword(Sint16 j, Sint8 *buf,Sint8 *keyword);
void	ex_keyword2(Sint16 *k0, Sint8 type[8][6], Sint16 j, Sint8 *buf, Sint8 *keyword);
void	ex_keyword3(Sint16 *direct, Sint16 j, Sint8 *buf, Sint8 *keyword);
void	ex_keyword4(Sint16 j, Sint8 *buf, Sint8 *keyword);
void	obj_copy(Sint16	*j);
#else
void    printUsage();
void	val_init();
void	ctl_read();
void	ct_read2();
void	obj_read();
void	hi_anlze();
void	mat_read();
void	drw_wrt();
void	tex_wrt();
void	mdl_read();
void	mdl_proc();
void	slp_read();
void	mt_read2();
void	t3d_wrt();
void	cls_wrt();
void	ex_keyword();
void	ex_keyword2();
void	ex_keyword3();
void	ex_keyword4();
void	obj_copy();
#endif

/********************************/
/*	外部変数		*/
/********************************/

#ifdef	MAIN
#ifdef __BORLANDC__
	Object	huge obj[OBJ_MAX];
	Cluster	huge cls[CLS_MAX];
	Vtex	huge vert[VERT_MAX];
	Vtex	huge *vn[VERT_MAX];
	Face	huge face[FACE_MAX];
	Texture	huge txt[TEX_MAX];
	Matbuf	huge mbuf[MBU_MAX];
	Float64	huge hx_s[FACE_MAX], huge hy_s[FACE_MAX], huge hz_s[FACE_MAX];
	Float64	huge hx_g[VERT_MAX], huge hy_g[VERT_MAX], huge hz_g[VERT_MAX];
	Float64	huge cx_s[FACE_MAX], huge cy_s[FACE_MAX], huge cz_s[FACE_MAX];
#else
	Object	obj[OBJ_MAX];
	Cluster	cls[CLS_MAX];
	Vtex	vert[VERT_MAX];
	Vtex	*vn[VERT_MAX];
	Face	face[FACE_MAX];
	Texture	txt[TEX_MAX];
	Matbuf	mbuf[MBU_MAX];
	Float64	hx_s[FACE_MAX], hy_s[FACE_MAX], hz_s[FACE_MAX];
	Float64	hx_g[VERT_MAX], hy_g[VERT_MAX], hz_g[VERT_MAX];
	Float64	cx_s[FACE_MAX], cy_s[FACE_MAX], cz_s[FACE_MAX];
#endif
	Sint16	g_face_max;
	Sint16	g_vert_max;
	Sint16	g_cls_max;
	Sint16	g_tex_max;
	Sint16	g_t_max;
	Sint16	g_nest_max;
	Sint16	g_u_obj_max;
	Sint16	g_obj_cnt;
	Sint16	g_s_drt;
	Sint16	g_ctr_c;
	Sint16	g_mb_no = -1;
	Sint16	g_n_calc;
	Sint16	g_face_rev;
	Sint16	g_clstr;
	Sint16	g_dgt;
	Sint8	*g_dirname;
	Sint8	*g_outname;
	Sint8	g_s3dname[150];
	Sint8	g_c_outname[150];
	Sint8	g_version[6];
	Sint8	g_scale[8];
	Sint8	g_top_cls[16];
	Sint8	g_dgt_nam[32];
	Sint8	g_in_ext[8];
	Sint8	g_out_ext[8];
#else
#ifdef __BORLANDC__
	extern	Object	huge obj[];
	extern	Cluster	huge cls[];
	extern	Vtex	huge vert[];
	extern	Vtex	huge *vn[];
	extern	Face	huge face[];
	extern	Texture	huge txt[];
	extern	Matbuf	huge mbuf[];
	extern	Float64	huge hx_s[], huge hy_s[], huge hz_s[];
	extern	Float64	huge hx_g[], huge hy_g[], huge hz_g[];
	extern	Float64	huge cx_s[], huge cy_s[], huge cz_s[];
#else
	extern	Object	obj[];
	extern	Cluster	cls[];
	extern	Vtex	vert[];
	extern	Vtex	*vn[];
	extern	Face	face[];
	extern	Texture	txt[];
	extern	Matbuf	mbuf[];
	extern	Float64	hx_s[], hy_s[], hz_s[];
	extern	Float64	hx_g[], hy_g[], hz_g[];
	extern	Float64	cx_s[], cy_s[], cz_s[];
#endif
	extern	Sint16	g_face_max;
	extern	Sint16	g_vert_max;
	extern	Sint16	g_cls_max;
	extern	Sint16	g_tex_max;
	extern	Sint16	g_t_max;
	extern	Sint16	g_nest_max;
	extern	Sint16	g_u_obj_max;
	extern	Sint16	g_obj_cnt;
	extern	Sint16	g_s_drt;
	extern	Sint16	g_ctr_c;
	extern	Sint16	g_mb_no;
	extern	Sint16	g_n_calc;
	extern	Sint16	g_face_rev;
	extern	Sint16	g_clstr;
	extern	Sint16	g_dgt;
	extern	Sint8	*g_dirname;
	extern	Sint8	*g_outname;
	extern	Sint8	g_s3dname[];
	extern	Sint8	g_c_outname[];
	extern	Sint8	g_version[];
	extern	Sint8	g_scale[];
	extern	Sint8	g_top_cls[];
	extern	Sint8	g_dgt_nam[];
	extern	Sint8	g_in_ext[];
	extern	Sint8	g_out_ext[];
#endif
