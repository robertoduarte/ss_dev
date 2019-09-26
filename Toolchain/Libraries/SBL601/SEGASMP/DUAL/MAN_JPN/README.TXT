
--- �X���[�u CPU �T���v���v���O������ README.txt

�����́A�X���[�u CPU �̎g�p���@�̃T���v���v���O�����ł��B
�����̃t�@�C���́A�\�t�g�E�F�A�E���C�u����������p���܂������A
�X���[�u CPU �����s����s�񉉎Z���W���[���́A�Ӑ}�I�Ƀ}�X�^ CPU �Ƃ�
�ʂ̖��O�����Ă���܂��B�islv_mtx3.src�j
���G���g�����g�ȃ��W���[���^�֐��̏ꍇ�ɂ́A�}�X�^ CPU �ƃX���[�u CPU ��
�S�����l�Ɏ��s���邱�Ƃ��ł��܂��B

Makefile:	GNU make �p makefile
slvsamp.abs:	���s�t�@�C��
slvsamp.c:	���C���E���W���[��
slvsamp.map:	���̃T���v���v���O�����̃}�b�v�t�@�C��
slvsamp.sub:	���̃T���v���v���O�����̃����N�w��t�@�C��

slave.lib:	slaveSH.c ���琶������鏉�������C�u����
slaveSH.c:	�X���[�u CPU ���������C�u�����E���C�����W���[��
slaveSH.sub:	�X���[�u CPU ���������C�u�����E�����N�w��t�@�C��
slv_mtx3.h:	�X���[�u CPU �p�s�񉉎Z���W���[���E�w�b�_
slv_mtx3.src:	�X���[�u CPU �p�s�񉉎Z���W���[��

spr_mtx3.h:	�}�X�^ CPU �p�s�񉉎Z���W���[���E�w�b�_
spr_mtx3.src:	�}�X�^ CPU �p�s�񉉎Z���W���[��

smpstrt1.src:	SHC �̎��s�����������W���[���i�P�j
smpstrt2.c:	SHC �̎��s�����������W���[���i�Q�j
sega_spr.h:	�X�v���C�g��{���C�u�����w�b�_
sega_xpt.h:	���ʃw�b�_�t�@�C��
spr_fmat.src:	32 bit �Œ菬���_���Z���W���[��
spr_sinc.src:	SIN, COS ���Z���W���[���i�P�j
spr_xpt.c:	SIN, COS ���Z���W���[���i�Q�j

dual.bat:DOS BAT file
