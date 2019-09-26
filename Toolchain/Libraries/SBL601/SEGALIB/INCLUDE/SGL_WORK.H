#if		!defined( __SGL_WORK_H )
	#define	__SGL_WORK_H
	
	#include	"sl_def.h"
	
	typedef unsigned long	ulong;
	typedef unsigned short	ushort;
	typedef unsigned char	uchar;
	
	#define	BYTE_SIZE	sizeof( uchar )
	#define	WORD_SIZE	sizeof( ushort )
	#define	LONG_SIZE	sizeof( ulong )
	
	#define	MAX_TRANSFER	20
	#define	Zmaxcnt			128		/*	Do not change	*/
	#define	SPRITE_SIZE		0x20
	#define	SRT_SIZE		(SPRITE_SIZE+LONG_SIZE)	/*	Parameter size	*/
	
	struct	SGL_WORK	{
		struct	SGL_WORK_EVENT	{
			ulong	EventTop;		/* はじめに実行されるイベント	*/
			ulong	EventLast;		/* 最後に実行されるイベント	*/
			ulong	EventNow;		/* 実行中のイベント	*/
			ushort	EventCount;		/* 残りイベント数	*/
			ushort	WorkCount;		/* 残りワーク数	*/
			uchar	MainMode;
			uchar	SubMode;
			uchar	SynchConst;
			uchar	SynchCount;
			ulong	UserFunction;
			ushort	TransCount;
			uchar	TransRequest;
			uchar	PauseFlag;
		} EVENT;
		
		struct	SGL_WORK_MATRIX	{
			ulong	mtptr;				/* Matrix pointer	*/
			ushort	MatrixCount;		/* Matrix nest count	*/
			ushort	IntCount;			/* Interrupt counter	*/
		} MATRIX;
		
		struct	SGL_WORK_POLYGON	{
			ulong	MsPbufPtr;			/* 次に定義するポイントのバッファポインタ	*/
			ulong	SlPbufPtr;			/* 次に定義するポイントのバッファポインタ	*/
			ulong	SpritePtr;			/* スプライトのパラメータポインタ	*/
			ulong	MsSdataPtr;
			ulong	SlSdataPtr;
			ulong	ZbufPtr;
			ulong	FormTbl;			/* Texture form table	*/
			ulong	SprbufBias;
			ulong	ComRdPtr;
			ulong	ComWrPtr;
			ulong	MsLightVector[XYZ];
			ulong	SlLightVector[XYZ];
			ulong	ColorOffset;		/* Color offset data table	*/
			ulong	MsScreenDist;
			ulong	SlScreenDist;
			ushort	MsZlimit;
			uchar	MsWindowNumber;
			uchar	WinUseFlag;
			ushort	TotalPolygon;		/* バッファに登録されているポリゴン数	*/
			ushort	TotalVertices;
			ushort	MsScreenLeft;
			ushort	MsScreenTop;
			ushort	MsScreenRight;
			ushort	MsScreenBottom;
			ushort	MsScreenSizeX;		/* スクリーンサイズ(Ｘ)	*/
			ushort	MsScreenSizeY;		/* スクリーンサイズ(Ｙ)	*/
			ushort	MsScreenHalfX;
			ushort	MsScreenHalfY;
			ushort	SlScreenLeft;
			ushort	SlScreenTop;
			ushort	SlScreenRight;
			ushort	SlScreenBottom;
			ushort	SlScreenSizeX;		/* スクリーンサイズ(Ｘ)	*/
			ushort	SlScreenSizeY;		/* スクリーンサイズ(Ｙ)	*/
			ushort	SlScreenHalfX;
			ushort	SlScreenHalfY;
			ushort	MsClipXAdder;
			ushort	MsClipYAdder;
			ushort	SlClipXAdder;
			ushort	SlClipYAdder;
			ushort	SlZlimit;
			ushort	WinPtr;
			ushort	DispPolygons;
			uchar	DMAEndFlag;
			uchar	DMASetFlag;
			uchar	SlWindowNumber;
			ushort	PutCount;
			uchar	MsZdpsfcnt;
			uchar	SlZdpsfcnt;
		} POLYGON;
		
		struct	SGL_WORK_OPTION	{
			uchar	Resolution;		/* Resolution mode	*/
			uchar	NbPCMBf;		/* Number of PCM buffer	*/
			ushort	PCMBufFlag;
			uchar	TrayCheckFlag;	/* CD Tray check flag	*/
			uchar	SoundCount;
			ushort	FRT_Count;		/* Free running timer count	*/
			uchar	SCUMC_ID;		/* Scu Memory copy ID number	*/
			uchar	DMASt_CPU0;		/* DMA Status CPU Channel 0	*/
			uchar	DMASt_CPU1;		/* DMA Status CPU Channel 1	*/
			uchar	DMASt_SCU0;		/* DMA Status SCU Level 0	*/
			uchar	DMASt_SCU1;		/* DMA Status SCU Level 1	*/
			uchar	DMASt_SCU2;		/* DMA Status SCU Level 2	*/
		} OPTION;
		
		struct	SGL_WORK_SCROLL	{
			ushort	VDP2_TVMD;		/* ＴＶ画面モード	*/
			ushort	VDP2_EXTEN;		/* 外部信号イネーブル	*/
			ushort	VDP2_TVSTAT;	/* 画面ステータス	*/
			ushort	VDP2_VRSIZE;	/* ＶＲＡＭサイズ	*/
			ushort	VDP2_HCNT;		/* Ｈカウンター	*/
			ushort	VDP2_VCNT;		/* Ｖカウンター	*/
			uchar	RotTransFlag;	/* Rotate scroll transfer flag	*/
			ushort	VDP2_RAMCTL;	/* ＲＡＭコントロール	*/
			ushort	VDP2_CYCA0L;	/* ＶＲＡＭサイクルパターン（バンクＡ０、Ｔ０ー３）	*/
			ushort	VDP2_CYCA0U;	/* ＶＲＡＭサイクルパターン（バンクＡ０、Ｔ４ー７）	*/
			ushort	VDP2_CYCA1L;	/* ＶＲＡＭサイクルパターン（バンクＡ１、Ｔ０ー３）	*/
			ushort	VDP2_CYCA1U;	/* ＶＲＡＭサイクルパターン（バンクＡ１、Ｔ４ー７）	*/
			ushort	VDP2_CYCB0L;	/* ＶＲＡＭサイクルパターン（バンクＢ０、Ｔ０ー３）	*/
			ushort	VDP2_CYCB0U;	/* ＶＲＡＭサイクルパターン（バンクＢ０、Ｔ４ー７）	*/
			ushort	VDP2_CYCB1L;	/* ＶＲＡＭサイクルパターン（バンクＢ１、Ｔ０ー３）	*/
			ushort	VDP2_CYCB1U;	/* ＶＲＡＭサイクルパターン（バンクＢ１、Ｔ４ー７）	*/
			ushort	VDP2_BGON;		/* 画面表示イネーブル	*/
			ushort	VDP2_MZCTL;		/* モザイクコントロール	*/
			ushort	VDP2_SFSEL;		/* 特殊機能コードセレクト	*/
			ushort	VDP2_SFCODE;	/* 特殊機能コード	*/
			ushort	VDP2_CHCTLA;	/* キャラクタコントロール（ＮＢＧ０、ＮＢＧ１）	*/
			ushort	VDP2_CHCTLB;	/* キャラクタコントロール（ＮＢＧ２、ＮＢＧ３、ＲＢＧ０）	*/
			ushort	VDP2_BMPNA;		/* ビットマップパレットナンバー(ＮＢＧ０、１)	*/
			ushort	VDP2_BMPNB;		/* ビットマップパレットナンバー(ＲＢＧ０)	*/
			ushort	VDP2_PNCN0;		/* パターンネームコントロール（ＮＢＧ０）	*/
			ushort	VDP2_PNCN1;		/* パターンネームコントロール（ＮＢＧ１）	*/
			ushort	VDP2_PNCN2;		/* パターンネームコントロール（ＮＢＧ２）	*/
			ushort	VDP2_PNCN3;		/* パターンネームコントロール（ＮＢＧ３）	*/
			ushort	VDP2_PNCR;		/* パターンネームコントロール（ＲＢＧ０）	*/
			ushort	VDP2_PLSZ;		/* プレーンサイズ	*/
			ushort	VDP2_MPOFN;		/* マップオフセット（ＮＢＧ０ー３）	*/
			ushort	VDP2_MPOFR;		/* マップオフセット（回転パラメータＡ、Ｂ）	*/
			ushort	VDP2_MPABN0;	/* マップ（ＮＢＧ０プレーンＡ、Ｂ）	*/
			ushort	VDP2_MPCDN0;	/* マップ（ＮＢＧ０プレーンＣ、Ｄ）	*/
			ushort	VDP2_MPABN1;	/* マップ（ＮＢＧ１プレーンＡ、Ｂ）	*/
			ushort	VDP2_MPCDN1;	/* マップ（ＮＢＧ１プレーンＣ、Ｄ）	*/
			ushort	VDP2_MPABN2;	/* マップ（ＮＢＧ２プレーンＡ、Ｂ）	*/
			ushort	VDP2_MPCDN2;	/* マップ（ＮＢＧ２プレーンＣ、Ｄ）	*/
			ushort	VDP2_MPABN3;	/* マップ（ＮＢＧ３プレーンＡ、Ｂ）	*/
			ushort	VDP2_MPCDN3;	/* マップ（ＮＢＧ３プレーンＣ、Ｄ）	*/
			ushort	VDP2_MPABRA;	/* マップ（回転パラメータＡプレーンＡ、Ｂ）	*/
			ushort	VDP2_MPCDRA;	/* マップ（回転パラメータＡプレーンＣ、Ｄ）	*/
			ushort	VDP2_MPEFRA;	/* マップ（回転パラメータＡプレーンＥ、Ｆ）	*/
			ushort	VDP2_MPGHRA;	/* マップ（回転パラメータＡプレーンＧ、Ｈ）	*/
			ushort	VDP2_MPIJRA;	/* マップ（回転パラメータＡプレーンＩ、Ｊ）	*/
			ushort	VDP2_MPKLRA;	/* マップ（回転パラメータＡプレーンＫ、Ｌ）	*/
			ushort	VDP2_MPMNRA;	/* マップ（回転パラメータＡプレーンＭ、Ｎ）	*/
			ushort	VDP2_MPOPRA;	/* マップ（回転パラメータＡプレーンＯ、Ｐ）	*/
			ushort	VDP2_MPABRB;	/* マップ（回転パラメータＢプレーンＡ、Ｂ）	*/
			ushort	VDP2_MPCDRB;	/* マップ（回転パラメータＢプレーンＣ、Ｄ）	*/
			ushort	VDP2_MPEFRB;	/* マップ（回転パラメータＢプレーンＥ、Ｆ）	*/
			ushort	VDP2_MPGHRB;	/* マップ（回転パラメータＢプレーンＧ、Ｈ）	*/
			ushort	VDP2_MPIJRB;	/* マップ（回転パラメータＢプレーンＩ、Ｊ）	*/
			ushort	VDP2_MPKLRB;	/* マップ（回転パラメータＢプレーンＫ、Ｌ）	*/
			ushort	VDP2_MPMNRB;	/* マップ（回転パラメータＢプレーンＭ、Ｎ）	*/
			ushort	VDP2_MPOPRB;	/* マップ（回転パラメータＢプレーンＯ、Ｐ）	*/
			union	{
				ulong	Nbg0_PosX;	/* 画面スクロール値（ＮＢＧ０、水平方向固定小数）	*/
				ulong	VDP2_SCXN0;	/* 画面スクロール値（ＮＢＧ０、水平方向固定小数）	*/
				struct	{
					ushort	VDP2_SCXIN0;	/* 画面スクロール値（ＮＢＧ０、水平方向整数部）	*/
					ushort	VDP2_SCXDN0;	/* 画面スクロール値（ＮＢＧ０、水平方向小数部）	*/
				}x;
			}x1;
			union	{
				ulong	Nbg0_PosY;	/* 画面スクロール値（ＮＢＧ０、水平方向固定小数）	*/
				ulong	VDP2_SCYN0;	/* 画面スクロール値（ＮＢＧ０、垂直方向固定小数）	*/
				struct	{
					ushort	VDP2_SCYIN0;	/* 画面スクロール値（ＮＢＧ０、垂直方向整数部）	*/
					ushort	VDP2_SCYDN0;	/* 画面スクロール値（ＮＢＧ０、垂直方向小数部）	*/
				}x;
			}x2;
			union	{
				ulong	VDP2_ZMXN0;	/* 座標増分（ＮＢＧ０、水平方向固定小数）	*/
				struct	{
					ushort	VDP2_ZMXIN0;	/* 座標増分（ＮＢＧ０、水平方向整数部）	*/
					ushort	VDP2_ZMXDN0;	/* 座標増分（ＮＢＧ０、水平方向小数部）	*/
				}x;
			}x3;
			union	{
				ulong	VDP2_ZMYN0;	/* 座標増分（ＮＢＧ０、垂直方向固定小数）	*/
				struct	{
					ushort	VDP2_ZMYIN0;	/* 座標増分（ＮＢＧ０、垂直方向整数部）	*/
					ushort	VDP2_ZMYDN0;	/* 座標増分（ＮＢＧ０、垂直方向小数部）	*/
				}x;
			}x4;
			union	{
				ulong	Nbg1_PosX;	/* 画面スクロール値（ＮＢＧ１、水平方向固定小数）	*/
				ulong	VDP2_SCXN1;	/* 画面スクロール値（ＮＢＧ１、水平方向固定小数）	*/
				struct	{
					ushort	VDP2_SCXIN1;	/* 画面スクロール値（ＮＢＧ１、水平方向整数部）	*/
					ushort	VDP2_SCXDN1;	/* 画面スクロール値（ＮＢＧ１、水平方向小数部）	*/
				}x;
			}x5;
			union	{
				ulong	Nbg1_PosY;	/* 画面スクロール値（ＮＢＧ１、水平方向固定小数）	*/
				ulong	VDP2_SCYN1;	/* 画面スクロール値（ＮＢＧ１、垂直方向固定小数）	*/
				struct	{
					ushort	VDP2_SCYIN1;	/* 画面スクロール値（ＮＢＧ１、垂直方向整数部）	*/
					ushort	VDP2_SCYDN1;	/* 画面スクロール値（ＮＢＧ１、垂直方向小数部）	*/
				}x;
			}x6;
			union	{
				ulong	VDP2_ZMXN1;	/* 座標増分（ＮＢＧ１、水平方向固定小数）	*/
				struct	{
					ushort	VDP2_ZMXIN1;	/* 座標増分（ＮＢＧ１、水平方向整数部）	*/
					ushort	VDP2_ZMXDN1;	/* 座標増分（ＮＢＧ１、水平方向小数部）	*/
				}x;
			}x7;
			union	{
				ulong	VDP2_ZMYN1;	/* 座標増分（ＮＢＧ１、垂直方向固定小数）	*/
				struct	{
					ushort	VDP2_ZMYIN1;	/* 座標増分（ＮＢＧ１、垂直方向整数部）	*/
					ushort	VDP2_ZMYDN1;	/* 座標増分（ＮＢＧ１、垂直方向小数部）	*/
				}x;
			}x8;
			ushort	VDP2_SCXN2;		/* 画面スクロール値（ＮＢＧ２、水平方向）	*/
			ushort	VDP2_SCYN2;		/* 画面スクロール値（ＮＢＧ２、垂直方向）	*/
			ushort	VDP2_SCXN3;		/* 画面スクロール値（ＮＢＧ３、水平方向）	*/
			ushort	VDP2_SCYN3;		/* 画面スクロール値（ＮＢＧ３、垂直方向）	*/
			ushort	VDP2_ZMCTL;		/* 縮小イネーブル	*/
			ushort	VDP2_SCRCTL;	/* ライン＆縦セルスクロールコントロール	*/
			ulong	VDP2_VCSTA;		/* 縦セルスクロールテーブルアドレス	*/
			ulong	VDP2_LSTA0;		/* ＮＢＧ０用ラインスクロールテーブルアドレス	*/
			ulong	VDP2_LSTA1;		/* ＮＢＧ１用ラインスクロールテーブルアドレス	*/
			ulong	VDP2_LCTA;		/* ラインカラー画面テーブルアドレス	*/
			ulong	VDP2_BKTA;		/* バック画面テーブルアドレス	*/
			ushort	VDP2_RPMD;		/* 回転パラメータモード	*/
			ushort	VDP2_RPRCTL;	/* 回転パラメータリードコントロール	*/
			ushort	VDP2_KTCTL;		/* 係数テーブルコントロール	*/
			ushort	VDP2_KTAOF;		/* 係数テーブルアドレスオフセット	*/
			ushort	VDP2_OVPNRA;	/* 画面オーバーパターンネーム	*/
			ushort	VDP2_OVPNRB;	/* 画面オーバーパターンネーム	*/
			ulong	VDP2_RPTA;		/* 回転パラメータテーブルアドレス	*/
			ushort	VDP2_WPSX0;		/* ウィンドウポジション(H start)	*/
			ushort	VDP2_WPSY0;		/* ウィンドウポジション(V start)	*/
			ushort	VDP2_WPEX0;		/* ウィンドウポジション(H stop)	*/
			ushort	VDP2_WPEY0;		/* ウィンドウポジション(V stop)	*/
			ushort	VDP2_WPSX1;		/* ウィンドウポジション(H start)	*/
			ushort	VDP2_WPSY1;		/* ウィンドウポジション(V start)	*/
			ushort	VDP2_WPEX1;		/* ウィンドウポジション(H stop)	*/
			ushort	VDP2_WPEY1;		/* ウィンドウポジション(V stop)	*/
			ushort	VDP2_WCTLA;		/* ウィンドウコントロール	*/
			ushort	VDP2_WCTLB;		/* ウィンドウコントロール	*/
			ushort	VDP2_WCTLC;		/* ウィンドウコントロール	*/
			ushort	VDP2_WCTLD;		/* ウィンドウコントロール	*/
			ulong	VDP2_LWTA0;		/* ラインウィンドウテーブルアドレス	*/
			ulong	VDP2_LWTA1;		/* ラインウィンドウテーブルアドレス	*/
			ushort	VDP2_SPCTL;		/* スプライトコントロール	*/
			ushort	VDP2_SDCTL;		/* シャドウコントロール	*/
			ushort	VDP2_CRAOFA;	/* カラーＲＡＭアドレスオフセット（ＮＢＧ０ー３）	*/
			ushort	VDP2_CRAOFB;	/* カラーＲＡＭアドレスオフセット（ＲＢＧ０、スプライト）	*/
			ushort	VDP2_LNCLEN;	/* ラインカラー画面イネーブル	*/
			ushort	VDP2_SFPRMD;	/* 特殊プライオリティモード	*/
			ushort	VDP2_CCCTL;		/* カラー演算コントロール	*/
			ushort	VDP2_SFCCMD;	/* 特殊カラー演算モード	*/
			ushort	VDP2_PRISA;		/* プライオリティナンバー	*/
			ushort	VDP2_PRISB;		/* プライオリティナンバー	*/
			ushort	VDP2_PRISC;		/* プライオリティナンバー	*/
			ushort	VDP2_PRISD;		/* プライオリティナンバー	*/
			ushort	VDP2_PRINA;		/* プライオリティナンバー	*/
			ushort	VDP2_PRINB;		/* プライオリティナンバー	*/
			ushort	VDP2_PRIR;		/* プライオリティナンバー	*/
			ushort	VDP2_CCRSA;		/* カラー演算割合(スプライト０、１)	*/
			ushort	VDP2_CCRSB;		/* カラー演算割合(スプライト２、３)	*/
			ushort	VDP2_CCRSC;		/* カラー演算割合(スプライト４、５)	*/
			ushort	VDP2_CCRSD;		/* カラー演算割合(スプライト６、７)	*/
			ushort	VDP2_CCRNA;		/* カラー演算割合(ＮＢＧ０、１)	*/
			ushort	VDP2_CCRNB;		/* カラー演算割合(ＮＢＧ２、３)	*/
			ushort	VDP2_CCRR;		/* カラー演算割合(ＲＧＢ０)	*/
			ushort	VDP2_CCRLB;		/* カラー演算割合(ラインカラー画面、バック画面)	*/
			ushort	VDP2_CLOFEN;	/* カラーオフセットイネーブル	*/
			ushort	VDP2_CLOFSL;	/* カラーオフセットセレクト	*/
			ushort	VDP2_COAR;		/* カラーオフセットＡ(Ｒｅｄ)	*/
			ushort	VDP2_COAG;		/* カラーオフセットＡ(Ｇｒｅｅｎ)	*/
			ushort	VDP2_COAB;		/* カラーオフセットＡ(Ｂｌｕｅ)	*/
			ushort	VDP2_COBR;		/* カラーオフセットＢ(Ｒｅｄ)	*/
			ushort	VDP2_COBG;		/* カラーオフセットＢ(Ｇｒｅｅｎ)	*/
			ushort	VDP2_COBB;		/* カラーオフセットＢ(Ｂｌｕｅ)	*/
		} SCROLL;
		
		struct	SGL_WORK_SCROLL_PTR	{
			ulong	ScrRotPtr;		/* Current rotation buffer pointer	*/
			ulong	nbg0_char_adr;
			ulong	nbg1_char_adr;
			ulong	nbg2_char_adr;
			ulong	nbg3_char_adr;
			ulong	ra_char_adr;
			ulong	rb_char_adr;
			ulong	nbg0_page_adr;
			ulong	nbg1_page_adr;
			ulong	nbg2_page_adr;
			ulong	nbg3_page_adr;
			ulong	ra_page_adr;
			ulong	rb_page_adr;
			ulong	rpara_vram_adr;
			ulong	k_table_adr;
			ulong	RotScrParA[26];
			ulong	RotScrParB[26];
			ulong	Nbg2_PosX;
			ulong	Nbg2_PosY;
			ulong	Nbg3_PosX;
			ulong	Nbg3_PosY;
		} SCROLL_PTR;
		
		struct	SGL_WORK_POLYGON_WINDOW	{
			ushort	Window1_data[2];
			ushort	Window2_data[2];
			ushort	End_Sprite[2];
			ushort	Window1_Left;
			ushort	Window1_Top;
			ushort	Window2_Left;
			ushort	Window2_Top;
			ushort	Window1_Right;
			ushort	Window1_Bottom;
			ushort	Window2_Right;
			ushort	Window2_Bottom;
			ushort	Win1Zlimit[2];
			ushort	Win2Zlimit;
		} POLYGON_WINDOW;
		
		struct	SGL_WORK_POLYGON_BASE	{
			ushort	Center1_data[2];
			ushort	Center2_data[4];
			ushort	Center1_PosX;
			ushort	Center1_PosY;
			ushort	Center2_PosX;
			ushort	Center2_PosY;
		} POLYGON_BASE;
		
		struct	SGL_WORK_OPTION2	{
			ulong	RandWork;		/* Random number generation	*/
			ulong	CDReadExec;
		} OPTION2;
		
		struct	SGL_WORK_DMA_TRANS	{
			uchar	VRN0;		/* Vector Number	*/
			uchar	VRE0;		/* Reserve	*/
			uchar	DRCR0;		/* D.M.A. Request / Answer select	*/
			uchar	DMAOR;		/* D.M.A. Operation	*/
			ulong	SAR0;		/* Source address	*/
			ulong	DAR0;		/* Destination address	*/
			ulong	TCR0;		/* Transfer count	*/
			ulong	CHCR0;		/* Channel control	*/
			uchar	VRN1;		/* Vector Number	*/
			uchar	VRE1;		/* Reserve	*/
			uchar	DRCR1;		/* D.M.A. Request / Answer select	*/
			ulong	SAR1;		/* Source address	*/
			ulong	DAR1;		/* Destination address	*/
			ulong	TCR1;		/* Transfer count	*/
			ulong	CHCR1;		/* Channel control	*/
		} DMA_TRANS;
		
		struct	SGL_WORK_SOUND	{
			ulong	PCMPtr;			/* PCM Work buffer	*/
			ulong	SoundPtr;		/* Sound command buffer pointer	*/
			ulong	SndTrnsFunc;	/* Sound command transfer function	*/
		} SOUND;
		
		struct	SGL_WORK_PERIPHERAL_SYSTEM	{
			ulong	SmpcComWtPtr;		/* コマンドキャッシュ書き込み位置	*/
			ulong	SmpcComRdPtr;		/* コマンドキャッシュ読み出し位置	*/
			ulong	SmpcResOffset;		/* リザルトデータバッファオフセット	*/
			ulong	SmpcResPointer;		/* リザルトデータバッファポインター	*/
			ulong	SmpcPerPointer;		/* ペリフェラルバッファポインター	*/
			ulong	SmpcIntBackData;	/* インタラプトバックパラメータ	*/
			ulong	SmpcMemSetData;		/* ＳＭＰＣメモリ設定パラメータ	*/
			ulong	SmpcTimeSetData[2];	/* 時刻設定パラメータ	*/
		} PERIPHERAL_SYSTEM;
		
		struct	SGL_WORK_PERIPHERAL_BUFFER	{
			uchar	SmpcSemaphore;		/* ＳＭＰＣセマフォ	*/
			uchar	SmpcChangeFlag;		/* バッファ更新リクエストフラグ	*/
			uchar	SmpcControlFlag;	/* 制御フラグ	*/
			uchar	SmpcResetCount;		/* リセットボタンカウンター	*/
			uchar	SmpcComNumber;		/* ＳＭＰＣコマンド番号	*/
			uchar	SmpcIntBackCom;		/* インタラプトバックコマンド番号	*/
			uchar	SmpcPerControl;		/* ペリフェラル取得制御パラメータ	*/
			uchar	SmpcPerCommand;		/* ペリフェラル取得コマンドパラメータ	*/
			uchar	SmpcPortDir1;		/* ポート１入出力方向	*/
			uchar	SmpcPortDir2;		/* ポート２入出力方向	*/
			uchar	SmpcPortSelect;		/* 入出力セレクトモード	*/
			uchar	SmpcPortExt;		/* 外部ラッチ入力モード	*/
			uchar	SmpcPerMode;		/* ペリフェラルポートモード	*/
		} PERIPHERAL_BUFFER;
	};
	
	#define	AD_SGL_WORK		0x060ffc00
	#define	__SGL_WORK		(( struct SGL_WORK * )AD_SGL_WORK )
	
	#define	EventTop		__SGL_WORK->EVENT.EventTop
	#define	EventLast		__SGL_WORK->EVENT.EventLast
	#define	EventNow		__SGL_WORK->EVENT.EventNow
	#define	EventCount		__SGL_WORK->EVENT.EventCount
	#define	WorkCount		__SGL_WORK->EVENT.WorkCount
	#define	MainMode		__SGL_WORK->EVENT.MainMode
	#define	SubMode			__SGL_WORK->EVENT.SubMode
	#define	SynchConst		__SGL_WORK->EVENT.SynchConst
	#define	SynchCount		__SGL_WORK->EVENT.SynchCount
	#define	UserFunction	__SGL_WORK->EVENT.UserFunction
	#define	TransCount		__SGL_WORK->EVENT.TransCount
	#define	TransRequest	__SGL_WORK->EVENT.TransRequest
	#define	PauseFlag		__SGL_WORK->EVENT.PauseFlag
	
	#define	mtptr			__SGL_WORK->MATRIX.mtptr
	#define	MatrixCount		__SGL_WORK->MATRIX.MatrixCount
	#define	IntCount		__SGL_WORK->MATRIX.IntCount
	
	#define	MsPbufPtr		__SGL_WORK->POLYGON.MsPbufPtr
	#define	SlPbufPtr		__SGL_WORK->POLYGON.SlPbufPtr
	#define	SpritePtr		__SGL_WORK->POLYGON.SpritePtr
	#define	MsSdataPtr		__SGL_WORK->POLYGON.MsSdataPtr
	#define	SlSdataPtr		__SGL_WORK->POLYGON.SlSdataPtr
	#define	ZbufPtr			__SGL_WORK->POLYGON.ZbufPtr
	#define	FormTbl			__SGL_WORK->POLYGON.FormTbl
	#define	SprbufBias		__SGL_WORK->POLYGON.SprbufBias
	#define	ComRdPtr		__SGL_WORK->POLYGON.ComRdPtr
	#define	ComWrPtr		__SGL_WORK->POLYGON.ComWrPtr
	#define	MsLightVector	__SGL_WORK->POLYGON.MsLightVector
	#define	SlLightVector	__SGL_WORK->POLYGON.SlLightVector
	#define	ColorOffset		__SGL_WORK->POLYGON.ColorOffset
	#define	MsScreenDist	__SGL_WORK->POLYGON.MsScreenDist
	#define	SlScreenDist	__SGL_WORK->POLYGON.SlScreenDist
	#define	MsZlimit		__SGL_WORK->POLYGON.MsZlimit
	#define	MsWindowNumber	__SGL_WORK->POLYGON.MsWindowNumber
	#define	WinUseFlag		__SGL_WORK->POLYGON.WinUseFlag
	#define	TotalPolygon	__SGL_WORK->POLYGON.TotalPolygon
	#define	TotalVertices	__SGL_WORK->POLYGON.TotalVertices
	#define	MsScreenLeft	__SGL_WORK->POLYGON.MsScreenLeft
	#define	MsScreenTop		__SGL_WORK->POLYGON.MsScreenTop
	#define	MsScreenRight	__SGL_WORK->POLYGON.MsScreenRight
	#define	MsScreenBottom	__SGL_WORK->POLYGON.MsScreenBottom
	#define	MsScreenSizeX	__SGL_WORK->POLYGON.MsScreenSizeX
	#define	MsScreenSizeY	__SGL_WORK->POLYGON.MsScreenSizeY
	#define	MsScreenHalfX	__SGL_WORK->POLYGON.MsScreenHalfX
	#define	MsScreenHalfY	__SGL_WORK->POLYGON.MsScreenHalfY
	#define	SlScreenLeft	__SGL_WORK->POLYGON.SlScreenLeft
	#define	SlScreenTop		__SGL_WORK->POLYGON.SlScreenTop
	#define	SlScreenRight	__SGL_WORK->POLYGON.SlScreenRight
	#define	SlScreenBottom	__SGL_WORK->POLYGON.SlScreenBottom
	#define	SlScreenSizeX	__SGL_WORK->POLYGON.SlScreenSizeX
	#define	SlScreenSizeY	__SGL_WORK->POLYGON.SlScreenSizeY
	#define	SlScreenHalfX	__SGL_WORK->POLYGON.SlScreenHalfX
	#define	SlScreenHalfY	__SGL_WORK->POLYGON.SlScreenHalfY
	#define	MsClipXAdder	__SGL_WORK->POLYGON.MsClipXAdder
	#define	MsClipYAdder	__SGL_WORK->POLYGON.MsClipYAdder
	#define	SlClipXAdder	__SGL_WORK->POLYGON.SlClipXAdder
	#define	SlClipYAdder	__SGL_WORK->POLYGON.SlClipYAdder
	#define	SlZlimit		__SGL_WORK->POLYGON.SlZlimit
	#define	WinPtr			__SGL_WORK->POLYGON.WinPtr
	#define	DispPolygons	__SGL_WORK->POLYGON.DispPolygons
	#define	DMAEndFlag		__SGL_WORK->POLYGON.DMAEndFlag
	#define	DMASetFlag		__SGL_WORK->POLYGON.DMASetFlag
	#define	SlWindowNumber	__SGL_WORK->POLYGON.SlWindowNumber
	#define	PutCount		__SGL_WORK->POLYGON.PutCount
	#define	MsZdpsfcnt		__SGL_WORK->POLYGON.MsZdpsfcnt
	#define	SlZdpsfcnt		__SGL_WORK->POLYGON.SlZdpsfcnt
	
	#define	Resolution		__SGL_WORK->OPTION.Resolution
	#define	NbPCMBf			__SGL_WORK->OPTION.NbPCMBf
	#define	PCMBufFlag		__SGL_WORK->OPTION.PCMBufFlag
	#define	TrayCheckFlag	__SGL_WORK->OPTION.TrayCheckFlag
	#define	SoundCount		__SGL_WORK->OPTION.SoundCount
	#define	FRT_Count		__SGL_WORK->OPTION.FRT_Count
	#define	SCUMC_ID		__SGL_WORK->OPTION.SCUMC_ID
	#define	DMASt_CPU0		__SGL_WORK->OPTION.DMASt_CPU0
	#define	DMASt_CPU1		__SGL_WORK->OPTION.DMASt_CPU1
	#define	DMASt_SCU0		__SGL_WORK->OPTION.DMASt_SCU0
	#define	DMASt_SCU1		__SGL_WORK->OPTION.DMASt_SCU1
	#define	DMASt_SCU2		__SGL_WORK->OPTION.DMASt_SCU2
	
	#define	VDP2_TVMD		__SGL_WORK->SCROLL.VDP2_TVMD
	#define	VDP2_EXTEN		__SGL_WORK->SCROLL.VDP2_EXTEN
	#define	VDP2_TVSTAT		__SGL_WORK->SCROLL.VDP2_TVSTAT
	#define	VDP2_VRSIZE		__SGL_WORK->SCROLL.VDP2_VRSIZE
	#define	VDP2_HCNT		__SGL_WORK->SCROLL.VDP2_HCNT
	#define	VDP2_VCNT		__SGL_WORK->SCROLL.VDP2_VCNT
	#define	RotTransFlag	__SGL_WORK->SCROLL.RotTransFlag
	#define	VDP2_RAMCTL		__SGL_WORK->SCROLL.VDP2_RAMCTL
	#define	VDP2_CYCA0L		__SGL_WORK->SCROLL.VDP2_CYCA0L
	#define	VDP2_CYCA0U		__SGL_WORK->SCROLL.VDP2_CYCA0U
	#define	VDP2_CYCA1L		__SGL_WORK->SCROLL.VDP2_CYCA1L
	#define	VDP2_CYCA1U		__SGL_WORK->SCROLL.VDP2_CYCA1U
	#define	VDP2_CYCB0L		__SGL_WORK->SCROLL.VDP2_CYCB0L
	#define	VDP2_CYCB0U		__SGL_WORK->SCROLL.VDP2_CYCB0U
	#define	VDP2_CYCB1L		__SGL_WORK->SCROLL.VDP2_CYCB1L
	#define	VDP2_CYCB1U		__SGL_WORK->SCROLL.VDP2_CYCB1U
	#define	VDP2_BGON		__SGL_WORK->SCROLL.VDP2_BGON
	#define	VDP2_MZCTL		__SGL_WORK->SCROLL.VDP2_MZCTL
	#define	VDP2_SFSEL		__SGL_WORK->SCROLL.VDP2_SFSEL
	#define	VDP2_SFCODE		__SGL_WORK->SCROLL.VDP2_SFCODE
	#define	VDP2_CHCTLA		__SGL_WORK->SCROLL.VDP2_CHCTLA
	#define	VDP2_CHCTLB		__SGL_WORK->SCROLL.VDP2_CHCTLB
	#define	VDP2_BMPNA		__SGL_WORK->SCROLL.VDP2_BMPNA
	#define	VDP2_BMPNB		__SGL_WORK->SCROLL.VDP2_BMPNB
	#define	VDP2_PNCN0		__SGL_WORK->SCROLL.VDP2_PNCN0
	#define	VDP2_PNCN1		__SGL_WORK->SCROLL.VDP2_PNCN1
	#define	VDP2_PNCN2		__SGL_WORK->SCROLL.VDP2_PNCN2
	#define	VDP2_PNCN3		__SGL_WORK->SCROLL.VDP2_PNCN3
	#define	VDP2_PNCR		__SGL_WORK->SCROLL.VDP2_PNCR
	#define	VDP2_PLSZ		__SGL_WORK->SCROLL.VDP2_PLSZ
	#define	VDP2_MPOFN		__SGL_WORK->SCROLL.VDP2_MPOFN
	#define	VDP2_MPOFR		__SGL_WORK->SCROLL.VDP2_MPOFR
	#define	VDP2_MPABN0		__SGL_WORK->SCROLL.VDP2_MPABN0
	#define	VDP2_MPCDN0		__SGL_WORK->SCROLL.VDP2_MPCDN0
	#define	VDP2_MPABN1		__SGL_WORK->SCROLL.VDP2_MPABN1
	#define	VDP2_MPCDN1		__SGL_WORK->SCROLL.VDP2_MPCDN1
	#define	VDP2_MPABN2		__SGL_WORK->SCROLL.VDP2_MPABN2
	#define	VDP2_MPCDN2		__SGL_WORK->SCROLL.VDP2_MPCDN2
	#define	VDP2_MPABN3		__SGL_WORK->SCROLL.VDP2_MPABN3
	#define	VDP2_MPCDN3		__SGL_WORK->SCROLL.VDP2_MPCDN3
	#define	VDP2_MPABRA		__SGL_WORK->SCROLL.VDP2_MPABRA
	#define	VDP2_MPCDRA		__SGL_WORK->SCROLL.VDP2_MPCDRA
	#define	VDP2_MPEFRA		__SGL_WORK->SCROLL.VDP2_MPEFRA
	#define	VDP2_MPGHRA		__SGL_WORK->SCROLL.VDP2_MPGHRA
	#define	VDP2_MPIJRA		__SGL_WORK->SCROLL.VDP2_MPIJRA
	#define	VDP2_MPKLRA		__SGL_WORK->SCROLL.VDP2_MPKLRA
	#define	VDP2_MPMNRA		__SGL_WORK->SCROLL.VDP2_MPMNRA
	#define	VDP2_MPOPRA		__SGL_WORK->SCROLL.VDP2_MPOPRA
	#define	VDP2_MPABRB		__SGL_WORK->SCROLL.VDP2_MPABRB
	#define	VDP2_MPCDRB		__SGL_WORK->SCROLL.VDP2_MPCDRB
	#define	VDP2_MPEFRB		__SGL_WORK->SCROLL.VDP2_MPEFRB
	#define	VDP2_MPGHRB		__SGL_WORK->SCROLL.VDP2_MPGHRB
	#define	VDP2_MPIJRB		__SGL_WORK->SCROLL.VDP2_MPIJRB
	#define	VDP2_MPKLRB		__SGL_WORK->SCROLL.VDP2_MPKLRB
	#define	VDP2_MPMNRB		__SGL_WORK->SCROLL.VDP2_MPMNRB
	#define	VDP2_MPOPRB		__SGL_WORK->SCROLL.VDP2_MPOPRB
	#define	Nbg0_PosX		__SGL_WORK->SCROLL.x1.Nbg0_PosX
	#define	VDP2_SCXN0		__SGL_WORK->SCROLL.x1.VDP2_SCXN0
	#define	VDP2_SCXIN0		__SGL_WORK->SCROLL.x1.x.VDP2_SCXIN0
	#define	VDP2_SCXDN0		__SGL_WORK->SCROLL.x1.x.VDP2_SCXDN0
	#define	Nbg0_PosY		__SGL_WORK->SCROLL.x2.Nbg0_PosY
	#define	VDP2_SCYN0		__SGL_WORK->SCROLL.x2.VDP2_SCYN0
	#define	VDP2_SCYIN0		__SGL_WORK->SCROLL.x2.x.VDP2_SCYIN0
	#define	VDP2_SCYDN0		__SGL_WORK->SCROLL.x2.x.VDP2_SCYDN0
	#define	VDP2_ZMXN0		__SGL_WORK->SCROLL.x3.VDP2_ZMXN0
	#define	VDP2_ZMXIN0		__SGL_WORK->SCROLL.x3.x.VDP2_ZMXIN0
	#define	VDP2_ZMXDN0		__SGL_WORK->SCROLL.x3.x.VDP2_ZMXDN0
	#define	VDP2_ZMYN0		__SGL_WORK->SCROLL.x4.VDP2_ZMYN0
	#define	VDP2_ZMYIN0		__SGL_WORK->SCROLL.x4.x.VDP2_ZMYIN0
	#define	VDP2_ZMYDN0		__SGL_WORK->SCROLL.x4.x.VDP2_ZMYDN0
	#define	Nbg1_PosX		__SGL_WORK->SCROLL.x5.Nbg1_PosX
	#define	VDP2_SCXN1		__SGL_WORK->SCROLL.x5.VDP2_SCXN1
	#define	VDP2_SCXIN1		__SGL_WORK->SCROLL.x5.x.VDP2_SCXIN1
	#define	VDP2_SCXDN1		__SGL_WORK->SCROLL.x5.x.VDP2_SCXDN1
	#define	Nbg1_PosY		__SGL_WORK->SCROLL.x6.Nbg1_PosY
	#define	VDP2_SCYN1		__SGL_WORK->SCROLL.x6.VDP2_SCYN1
	#define	VDP2_SCYIN1		__SGL_WORK->SCROLL.x6.x.VDP2_SCYIN1
	#define	VDP2_SCYDN1		__SGL_WORK->SCROLL.x6.x.VDP2_SCYDN1
	#define	VDP2_ZMXN1		__SGL_WORK->SCROLL.x7.VDP2_ZMXN1
	#define	VDP2_ZMXIN1		__SGL_WORK->SCROLL.x7.x.VDP2_ZMXIN1
	#define	VDP2_ZMXDN1		__SGL_WORK->SCROLL.x7.x.VDP2_ZMXDN1
	#define	VDP2_ZMYN1		__SGL_WORK->SCROLL.x8.VDP2_ZMYN1
	#define	VDP2_ZMYIN1		__SGL_WORK->SCROLL.x8.x.VDP2_ZMYIN1
	#define	VDP2_ZMYDN1		__SGL_WORK->SCROLL.x8.x.VDP2_ZMYDN1
	#define	VDP2_SCXN2		__SGL_WORK->SCROLL.VDP2_SCXN2
	#define	VDP2_SCYN2		__SGL_WORK->SCROLL.VDP2_SCYN2
	#define	VDP2_SCXN3		__SGL_WORK->SCROLL.VDP2_SCXN3
	#define	VDP2_SCYN3		__SGL_WORK->SCROLL.VDP2_SCYN3
	#define	VDP2_ZMCTL		__SGL_WORK->SCROLL.VDP2_ZMCTL
	#define	VDP2_SCRCTL		__SGL_WORK->SCROLL.VDP2_SCRCTL
	#define	VDP2_VCSTA		__SGL_WORK->SCROLL.VDP2_VCSTA
	#define	VDP2_LSTA0		__SGL_WORK->SCROLL.VDP2_LSTA0
	#define	VDP2_LSTA1		__SGL_WORK->SCROLL.VDP2_LSTA1
	#define	VDP2_LCTA		__SGL_WORK->SCROLL.VDP2_LCTA
	#define	VDP2_BKTA		__SGL_WORK->SCROLL.VDP2_BKTA
	#define	VDP2_RPMD		__SGL_WORK->SCROLL.VDP2_RPMD
	#define	VDP2_RPRCTL		__SGL_WORK->SCROLL.VDP2_RPRCTL
	#define	VDP2_KTCTL		__SGL_WORK->SCROLL.VDP2_KTCTL
	#define	VDP2_KTAOF		__SGL_WORK->SCROLL.VDP2_KTAOF
	#define	VDP2_OVPNRA		__SGL_WORK->SCROLL.VDP2_OVPNRA
	#define	VDP2_OVPNRB		__SGL_WORK->SCROLL.VDP2_OVPNRB
	#define	VDP2_RPTA		__SGL_WORK->SCROLL.VDP2_RPTA
	#define	VDP2_WPSX0		__SGL_WORK->SCROLL.VDP2_WPSX0
	#define	VDP2_WPSY0		__SGL_WORK->SCROLL.VDP2_WPSY0
	#define	VDP2_WPEX0		__SGL_WORK->SCROLL.VDP2_WPEX0
	#define	VDP2_WPEY0		__SGL_WORK->SCROLL.VDP2_WPEY0
	#define	VDP2_WPSX1		__SGL_WORK->SCROLL.VDP2_WPSX1
	#define	VDP2_WPSY1		__SGL_WORK->SCROLL.VDP2_WPSY1
	#define	VDP2_WPEX1		__SGL_WORK->SCROLL.VDP2_WPEX1
	#define	VDP2_WPEY1		__SGL_WORK->SCROLL.VDP2_WPEY1
	#define	VDP2_WCTLA		__SGL_WORK->SCROLL.VDP2_WCTLA
	#define	VDP2_WCTLB		__SGL_WORK->SCROLL.VDP2_WCTLB
	#define	VDP2_WCTLC		__SGL_WORK->SCROLL.VDP2_WCTLC
	#define	VDP2_WCTLD		__SGL_WORK->SCROLL.VDP2_WCTLD
	#define	VDP2_LWTA0		__SGL_WORK->SCROLL.VDP2_LWTA0
	#define	VDP2_LWTA1		__SGL_WORK->SCROLL.VDP2_LWTA1
	#define	VDP2_SPCTL		__SGL_WORK->SCROLL.VDP2_SPCTL
	#define	VDP2_SDCTL		__SGL_WORK->SCROLL.VDP2_SDCTL
	#define	VDP2_CRAOFA		__SGL_WORK->SCROLL.VDP2_CRAOFA
	#define	VDP2_CRAOFB		__SGL_WORK->SCROLL.VDP2_CRAOFB
	#define	VDP2_LNCLEN		__SGL_WORK->SCROLL.VDP2_LNCLEN
	#define	VDP2_SFPRMD		__SGL_WORK->SCROLL.VDP2_SFPRMD
	#define	VDP2_CCCTL		__SGL_WORK->SCROLL.VDP2_CCCTL
	#define	VDP2_SFCCMD		__SGL_WORK->SCROLL.VDP2_SFCCMD
	#define	VDP2_PRISA		__SGL_WORK->SCROLL.VDP2_PRISA
	#define	VDP2_PRISB		__SGL_WORK->SCROLL.VDP2_PRISB
	#define	VDP2_PRISC		__SGL_WORK->SCROLL.VDP2_PRISC
	#define	VDP2_PRISD		__SGL_WORK->SCROLL.VDP2_PRISD
	#define	VDP2_PRINA		__SGL_WORK->SCROLL.VDP2_PRINA
	#define	VDP2_PRINB		__SGL_WORK->SCROLL.VDP2_PRINB
	#define	VDP2_PRIR		__SGL_WORK->SCROLL.VDP2_PRIR
	#define	VDP2_CCRSA		__SGL_WORK->SCROLL.VDP2_CCRSA
	#define	VDP2_CCRSB		__SGL_WORK->SCROLL.VDP2_CCRSB
	#define	VDP2_CCRSC		__SGL_WORK->SCROLL.VDP2_CCRSC
	#define	VDP2_CCRSD		__SGL_WORK->SCROLL.VDP2_CCRSD
	#define	VDP2_CCRNA		__SGL_WORK->SCROLL.VDP2_CCRNA
	#define	VDP2_CCRNB		__SGL_WORK->SCROLL.VDP2_CCRNB
	#define	VDP2_CCRR		__SGL_WORK->SCROLL.VDP2_CCRR
	#define	VDP2_CCRLB		__SGL_WORK->SCROLL.VDP2_CCRLB
	#define	VDP2_CLOFEN		__SGL_WORK->SCROLL.VDP2_CLOFEN
	#define	VDP2_CLOFSL		__SGL_WORK->SCROLL.VDP2_CLOFSL
	#define	VDP2_COAR		__SGL_WORK->SCROLL.VDP2_COAR
	#define	VDP2_COAG		__SGL_WORK->SCROLL.VDP2_COAG
	#define	VDP2_COAB		__SGL_WORK->SCROLL.VDP2_COAB
	#define	VDP2_COBR		__SGL_WORK->SCROLL.VDP2_COBR
	#define	VDP2_COBG		__SGL_WORK->SCROLL.VDP2_COBG
	#define	VDP2_COBB		__SGL_WORK->SCROLL.VDP2_COBB
	#define	VDP2_PRMSIZE	sizeof( struct SGL_WORK_SCROLL )
	
	#define	ScrRotPtr		__SGL_WORK->SCROLL_PTR.ScrRotPtr
	#define	nbg0_char_adr	__SGL_WORK->SCROLL_PTR.nbg0_char_adr
	#define	nbg1_char_adr	__SGL_WORK->SCROLL_PTR.nbg1_char_adr
	#define	nbg2_char_adr	__SGL_WORK->SCROLL_PTR.nbg2_char_adr
	#define	nbg3_char_adr	__SGL_WORK->SCROLL_PTR.nbg3_char_adr
	#define	ra_char_adr		__SGL_WORK->SCROLL_PTR.ra_char_adr
	#define	rb_char_adr		__SGL_WORK->SCROLL_PTR.rb_char_adr
	#define	nbg0_page_adr	__SGL_WORK->SCROLL_PTR.nbg0_page_adr
	#define	nbg1_page_adr	__SGL_WORK->SCROLL_PTR.nbg1_page_adr
	#define	nbg2_page_adr	__SGL_WORK->SCROLL_PTR.nbg2_page_adr
	#define	nbg3_page_adr	__SGL_WORK->SCROLL_PTR.nbg3_page_adr
	#define	ra_page_adr		__SGL_WORK->SCROLL_PTR.ra_page_adr
	#define	rb_page_adr		__SGL_WORK->SCROLL_PTR.rb_page_adr
	#define	rpara_vram_adr	__SGL_WORK->SCROLL_PTR.rpara_vram_adr
	#define	k_table_adr		__SGL_WORK->SCROLL_PTR.k_table_adr
	#define	RotScrParA		__SGL_WORK->SCROLL_PTR.RotScrParA
	#define	RotScrParB		__SGL_WORK->SCROLL_PTR.RotScrParB
	#define	Nbg2_PosX		__SGL_WORK->SCROLL_PTR.Nbg2_PosX
	#define	Nbg2_PosY		__SGL_WORK->SCROLL_PTR.Nbg2_PosY
	#define	Nbg3_PosX		__SGL_WORK->SCROLL_PTR.Nbg3_PosX
	#define	Nbg3_PosY		__SGL_WORK->SCROLL_PTR.Nbg3_PosY
	
	#define	Window1_data	__SGL_WORK->POLYGON_WINDOW.Window1_data
	#define	Window2_data	__SGL_WORK->POLYGON_WINDOW.Window2_data
	#define	End_Sprite		__SGL_WORK->POLYGON_WINDOW.End_Sprite
	#define	Window1_Left	__SGL_WORK->POLYGON_WINDOW.Window1_Left
	#define	Window1_Top		__SGL_WORK->POLYGON_WINDOW.Window1_Top
	#define	Window2_Left	__SGL_WORK->POLYGON_WINDOW.Window2_Left
	#define	Window2_Top		__SGL_WORK->POLYGON_WINDOW.Window2_Top
	#define	Window1_Right	__SGL_WORK->POLYGON_WINDOW.Window1_Right
	#define	Window1_Bottom	__SGL_WORK->POLYGON_WINDOW.Window1_Bottom
	#define	Window2_Right	__SGL_WORK->POLYGON_WINDOW.Window2_Right
	#define	Window2_Bottom	__SGL_WORK->POLYGON_WINDOW.Window2_Bottom
	#define	Win1Zlimit		__SGL_WORK->POLYGON_WINDOW.Win1Zlimit
	#define	Win2Zlimit		__SGL_WORK->POLYGON_WINDOW.Win2Zlimit
	
	#define	Center1_data	__SGL_WORK->POLYGON_BASE.Center1_data
	#define	Center2_data	__SGL_WORK->POLYGON_BASE.Center2_data
	#define	Center1_PosX	__SGL_WORK->POLYGON_BASE.Center1_PosX
	#define	Center1_PosY	__SGL_WORK->POLYGON_BASE.Center1_PosY
	#define	Center2_PosX	__SGL_WORK->POLYGON_BASE.Center2_PosX
	#define	Center2_PosY	__SGL_WORK->POLYGON_BASE.Center2_PosY
	
	#define	RandWork		__SGL_WORK->OPTION2.RandWork
	#define	CDReadExec		__SGL_WORK->OPTION2.CDReadExec
	
	#define	VRN0			__SGL_WORK->DMA_TRANS.VRN0
	#define	VRE0			__SGL_WORK->DMA_TRANS.VRE0
	#define	DRCR0			__SGL_WORK->DMA_TRANS.DRCR0
	#define	DMAOR			__SGL_WORK->DMA_TRANS.DMAOR
	#define	SAR0			__SGL_WORK->DMA_TRANS.SAR0
	#define	DAR0			__SGL_WORK->DMA_TRANS.DAR0
	#define	TCR0			__SGL_WORK->DMA_TRANS.TCR0
	#define	CHCR0			__SGL_WORK->DMA_TRANS.CHCR0
	#define	VRN1			__SGL_WORK->DMA_TRANS.VRN1
	#define	VRE1			__SGL_WORK->DMA_TRANS.VRE1
	#define	DRCR1			__SGL_WORK->DMA_TRANS.DRCR1
	#define	SAR1			__SGL_WORK->DMA_TRANS.SAR1
	#define	DAR1			__SGL_WORK->DMA_TRANS.DAR1
	#define	TCR1			__SGL_WORK->DMA_TRANS.TCR1
	#define	CHCR1			__SGL_WORK->DMA_TRANS.CHCR1
	
	#define	PCMPtr			__SGL_WORK->SOUND.PCMPtr
	#define	SoundPtr		__SGL_WORK->SOUND.SoundPtr
	#define	SndTrnsFunc		__SGL_WORK->SOUND.SndTrnsFunc
	
	#define	SmpcComWtPtr	__SGL_WORK->PERIPHERAL_SYSTEM.SmpcComWtPtr
	#define	SmpcComRdPtr	__SGL_WORK->PERIPHERAL_SYSTEM.SmpcComRdPtr
	#define	SmpcResOffset	__SGL_WORK->PERIPHERAL_SYSTEM.SmpcResOffset
	#define	SmpcResPointer	__SGL_WORK->PERIPHERAL_SYSTEM.SmpcResPointer
	#define	SmpcPerPointer	__SGL_WORK->PERIPHERAL_SYSTEM.SmpcPerPointer
	#define	SmpcIntBackData	__SGL_WORK->PERIPHERAL_SYSTEM.SmpcIntBackData
	#define	SmpcMemSetData	__SGL_WORK->PERIPHERAL_SYSTEM.SmpcMemSetData
	#define	SmpcTimeSetData	__SGL_WORK->PERIPHERAL_SYSTEM.SmpcTimeSetData
	
	#define	SmpcSemaphore	__SGL_WORK->PERIPHERAL_BUFFER.SmpcSemaphore
	#define	SmpcChangeFlag	__SGL_WORK->PERIPHERAL_BUFFER.SmpcChangeFlag
	#define	SmpcControlFlag	__SGL_WORK->PERIPHERAL_BUFFER.SmpcControlFlag
	#define	SmpcResetCount	__SGL_WORK->PERIPHERAL_BUFFER.SmpcResetCount
	#define	SmpcComNumber	__SGL_WORK->PERIPHERAL_BUFFER.SmpcComNumber
	#define	SmpcIntBackCom	__SGL_WORK->PERIPHERAL_BUFFER.SmpcIntBackCom
	#define	SmpcPerControl	__SGL_WORK->PERIPHERAL_BUFFER.SmpcPerControl
	#define	SmpcPerCommand	__SGL_WORK->PERIPHERAL_BUFFER.SmpcPerCommand
	#define	SmpcPortDir1	__SGL_WORK->PERIPHERAL_BUFFER.SmpcPortDir1
	#define	SmpcPortDir2	__SGL_WORK->PERIPHERAL_BUFFER.SmpcPortDir2
	#define	SmpcPortSelect	__SGL_WORK->PERIPHERAL_BUFFER.SmpcPortSelect
	#define	SmpcPortExt		__SGL_WORK->PERIPHERAL_BUFFER.SmpcPortExt
	#define	SmpcPerMode		__SGL_WORK->PERIPHERAL_BUFFER.SmpcPerMode
	
	#define	MAX_VERTICES	2440			/* 同一フレームで使用できる頂点の数	*/
	#define	MAX_POLYGON		1800			/* 同一フレームで使用できるポリゴンの数	*/
	#define	AD_WORK_AREA	0x060C0000		/* ワークエリア先頭アドレス	*/
	#define	__SGL_WORK2		(( struct SGL_WORK2 * )AD_WORK_AREA )
	#define	SMPC_CASH_SIZE	0x20			/* コマンドキャッシュサイズ	*/
	#define	PER_DATA_MAX	(1 + 1 + 15)	/* ペリフェラル最大データ数	*/
	#define	PER_RESULT_MAX	30				/* 最大ペリフェラル数	*/
	#define	SMPC_RESULT_SIZE	((PER_DATA_MAX * PER_RESULT_MAX + 2 + 0x1f) & 0x1fe0)
	
	struct	smpcDt	{				/* ＲＴＣ時刻	*/
		ushort	smpcYear;			/* 西暦年号	*/
		uchar	smpcMonth;			/* 曜日・月	*/
		uchar	smpcDate;			/* 日	*/
		uchar	smpcHour;			/* 時	*/
		uchar	smpcMinute;			/* 分	*/
		uchar	smpcSecond;			/* 秒	*/
		uchar	smpcDummyRTC;		/* ダミー	*/
	};
	#define	smpcDtSize	sizeof( struct smpcDt )
	
	struct smpcSt	{				/* ＳＭＰＣステータス	*/
		uchar		smpcCond;		/* 状態ステータス	*/
		uchar		smpcDummy1S;	/* ダミー１	*/
		ushort		smpcDummy2S;	/* ダミー２	*/
		struct smpcDt	smpcRtc;		/* ＲＴＣ時刻	*/
		uchar		smpcCtg;		/* カートリッジコード	*/
		uchar		smpcArea;		/* エリアコード	*/
		ushort		smpcSystem;		/* システムステータス	*/
		ulong		smpcSmem;		/* ＳＭＰＣメモリ保持データ	*/
	};
	#define	smpcStSize	sizeof( struct smpcSt )
	
	struct	smpc0	{				/* デジタルデバイス	*/
		uchar		smpc0Id;		/* ペリフェラルＩＤ	*/
		uchar		smpc0Ext;		/* 拡張データサイズ	*/
		ushort		smpc0Data;		/* ボタン現在データ	*/
		ushort		smpc0Push;		/* ボタン押下データ	*/
		ushort		smpc0Pull;		/* ボタン引上データ	*/
		ulong		smpc0Dummy2[4];	/* ダミー２	*/
	};
	#define	smpc0Size	sizeof( struct smpc0 )
	
	struct	SGL_WORK2	{
		struct	SGL_WORK2_POLYGON_BUF	{
			ulong	SortList[XYZ][MAX_POLYGON+5];
			ulong	TransList[XYZ][MAX_TRANSFER];
			ulong	Zbuffer[Zmaxcnt];			/*	128	*/
			ulong	Zbuffer2[Zmaxcnt];			/*	128	*/
			ulong	Zbuf_nest[Zmaxcnt*2];		/*	256 x 1	*/
			uchar	SlSpritebuf[(MAX_POLYGON+5)*SRT_SIZE];
			union	{
				uchar	MsSpritebuf[1];			/*	Use from Upper address	*/
				uchar	Spritebuf2[(MAX_POLYGON+5)*SRT_SIZE];
			}x;
			uchar	Pbuffer[MAX_VERTICES*(XYZ+WORD_SIZE*2)];
			uchar	CLOfstBuf[33*32];
		} POLYGON_BUF;
		
		struct	SGL_WORK2_PERIPHERAL_WORKAREA	{
			ulong	Smpc_Status;
			ulong	Smpc_Result;
			ulong	Smpc_Peripheral;
			ushort	Smpc_ResultSize;
			uchar	Per_Connect1;
			uchar	Per_Connect2;
			uchar	SmpcCommandCash[SMPC_CASH_SIZE];
			uchar	SmpcStatus[smpcStSize];
			uchar	SmpcResult[SMPC_RESULT_SIZE];
			uchar	SmpcPeripheral[smpc0Size*PER_RESULT_MAX];
		} PERIPHERAL_WORKAREA;
		
		struct	SGL_WORK2_COMMAND	{
			uchar	CommandBuf[MAX_POLYGON*32];
		} COMMAND;
	};
	
	#define	SortList	__SGL_WORK2->POLYGON_BUF.SortList
	#define	TransList	__SGL_WORK2->POLYGON_BUF.TransList
	#define	Zbuffer		__SGL_WORK2->POLYGON_BUF.Zbuffer
	#define	Zbuffer2	__SGL_WORK2->POLYGON_BUF.Zbuffer2
	#define	Zbuf_nest	__SGL_WORK2->POLYGON_BUF.Zbuf_nest
	#define	SlSpritebuf	__SGL_WORK2->POLYGON_BUF.SlSpritebuf
	#define	MsSpritebuf	__SGL_WORK2->POLYGON_BUF.x.MsSpritebuf
	#define	Spritebuf2	__SGL_WORK2->POLYGON_BUF.x.Spritebuf2
	#define	Pbuffer		__SGL_WORK2->POLYGON_BUF.Pbuffer
	#define	CLOfstBuf	__SGL_WORK2->POLYGON_BUF.CLOfstBuf
	
	#define	Smpc_Status		__SGL_WORK2->PERIPHERAL_WORKAREA.Smpc_Status
	#define	Smpc_Result		__SGL_WORK2->PERIPHERAL_WORKAREA.Smpc_Result
	#define	Smpc_Peripheral	__SGL_WORK2->PERIPHERAL_WORKAREA.Smpc_Peripheral
	#define	Smpc_ResultSize	__SGL_WORK2->PERIPHERAL_WORKAREA.Smpc_ResultSize
	#define	Per_Connect1	__SGL_WORK2->PERIPHERAL_WORKAREA.Per_Connect1
	#define	Per_Connect2	__SGL_WORK2->PERIPHERAL_WORKAREA.Per_Connect2
	#define	SmpcCommandCash	__SGL_WORK2->PERIPHERAL_WORKAREA.SmpcCommandCash
	#define	SmpcStatus		__SGL_WORK2->PERIPHERAL_WORKAREA.SmpcStatus
	#define	SmpcResult		__SGL_WORK2->PERIPHERAL_WORKAREA.SmpcResult
	#define	SmpcPeripheral	__SGL_WORK2->PERIPHERAL_WORKAREA.SmpcPeripheral
	
	#define	CommandBuf		__SGL_WORK2->COMMAND.CommandBuf
	
	struct	SGL_SCL_WORK	{
		ulong		RP_XST;		/*	スクリーン画面スタート座標 Ｘst	*/
		ulong		RP_YST;		/*	スクリーン画面スタート座標 Ｙst	*/
		ulong		RP_ZST;		/*	スクリーン画面スタート座標 Ｚst	*/
		ulong		RP_DXST;	/*	スクリーン画面垂直方向座標増分 ｄＸst	*/
		ulong		RP_DYST;	/*	スクリーン画面垂直方向座標増分 ｄＹst	*/
		ulong		RP_DX;		/*	スクリーン画面水平方向座標増分 ｄＸ	*/
		ulong		RP_DY;		/*	スクリーン画面水平方向座標増分 ｄＹ	*/
		ulong		RP_MATA;	/*	回転マトリクスパラメータＡ	*/
		ulong		RP_MATB;	/*	回転マトリクスパラメータＢ	*/
		ulong		RP_MATC;	/*	回転マトリクスパラメータＣ	*/
		ulong		RP_MATD;	/*	回転マトリクスパラメータＤ	*/
		ulong		RP_MATE;	/*	回転マトリクスパラメータＥ	*/
		ulong		RP_MATF;	/*	回転マトリクスパラメータＦ	*/
		struct	{
			ushort		RP_PX;		/*	視点座標 Ｐx	*/
			ushort		RP_PY;		/*	視点座標 Ｐy	*/
			ushort		RP_PZ;		/*	視点座標 Ｐz	*/
			ushort		filler;
		} x1;
		struct	{
			ushort		RP_CX;		/*	中心座標 Ｃx	*/
			ushort		RP_CY;		/*	中心座標 Ｃy	*/
			ushort		RP_CZ;		/*	中心座標 Ｃz	*/
			ushort		filler;
		} x2;
		ulong		RP_MX;		/*	平行移動量 Ｍx	*/
		ulong		RP_MY;		/*	平行移動量 Ｍy	*/
		ulong		RP_KX;		/*	拡大縮小係数 kx	*/
		ulong		RP_KY;		/*	拡大縮小係数 ky	*/
		ulong		RP_KAST;	/*	係数テーブルスタートアドレス ＫＡst	*/
		ulong		RP_DKAST;	/*	係数テーブル垂直方向アドレス増分 ＫＡst	*/
		ulong		RP_DKA; 	/*	係数テーブル水平方向アドレス増分 ＫＡ	*/
		union	{
			ulong	RP_SIZE;
			ulong	RP_KTABLE;	/*	係数テーブルスタートアドレス	*/
		} x3;
		ushort		RP_KMODE;	/*	係数テーブルモード	*/
	};
	
	#define	__SGL_SCL_WORK	((struct SGL_SCL_WORK * )ScrRotPtr)
	
	#define	RP_XST		__SGL_SCL_WORK->RP_XST
	#define	RP_YST		__SGL_SCL_WORK->RP_YST
	#define	RP_ZST		__SGL_SCL_WORK->RP_ZST
	#define	RP_DXST		__SGL_SCL_WORK->RP_DXST
	#define	RP_DYST		__SGL_SCL_WORK->RP_DYST
	#define	RP_DX		__SGL_SCL_WORK->RP_DX
	#define	RP_DY		__SGL_SCL_WORK->RP_DY
	#define	RP_MATA		__SGL_SCL_WORK->RP_MATA
	#define	RP_MATB		__SGL_SCL_WORK->RP_MATB
	#define	RP_MATC		__SGL_SCL_WORK->RP_MATC
	#define	RP_MATD		__SGL_SCL_WORK->RP_MATD
	#define	RP_MATE		__SGL_SCL_WORK->RP_MATE
	#define	RP_MATF		__SGL_SCL_WORK->RP_MATF
	#define	RP_PX		__SGL_SCL_WORK->x1.RP_PX
	#define	RP_PY		__SGL_SCL_WORK->x1.RP_PY
	#define	RP_PZ		__SGL_SCL_WORK->x1.RP_PZ
	#define	RP_CX		__SGL_SCL_WORK->x2.RP_CX
	#define	RP_CY		__SGL_SCL_WORK->x2.RP_CY
	#define	RP_CZ		__SGL_SCL_WORK->x2.RP_CZ
	#define	RP_MX		__SGL_SCL_WORK->RP_MX
	#define	RP_MY		__SGL_SCL_WORK->RP_MY
	#define	RP_KX		__SGL_SCL_WORK->RP_KX
	#define	RP_KY		__SGL_SCL_WORK->RP_KY
	#define	RP_KAST		__SGL_SCL_WORK->RP_KAST
	#define	RP_DKAST	__SGL_SCL_WORK->RP_DKAST
	#define	RP_DKA		__SGL_SCL_WORK->RP_DKA
	#define	RP_SIZE		__SGL_SCL_WORK->x3.RP_SIZE
	#define	RP_KTABLE	__SGL_SCL_WORK->x3.RP_KTABLE
	#define	RP_KMODE	__SGL_SCL_WORK->RP_KMODE
	
#endif
