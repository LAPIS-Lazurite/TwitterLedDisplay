#TwitterLedDisplay.c�ɂ���

##�T�v
Raspberry Pi���瑗�M���ꂽLED�̃r�b�g�}�b�v�f�[�^��ML620Q504�����f�[�^Flash�������ɕۑ����ALED�h�b�g�}�g���N�X�𐧌䂵�ĕ\�����s���v���O�����ł��B

Lazurite Sub-GHz��"TwitterBeacon"�Ƃ����e�L�X�g���b�Z�[�W��Raspberry Pi�ɑ��M���ĂP�b�ԁARaspberry Pi�ɕ\���p�f�[�^�̃��N�G�X�g�𑗐M���܂��B�P�b�ȓ��Ƀf�[�^����M�����炻�̒l��Flash�������ɕۑ����܂��BRaspberry Pi����̑��M���Ȃ���Εۑ�����Ă���f�[�^��\�����܂��B
Raspberry Pi���̃v���O�����́A"get_tl_display.rb"���g�p���Ă��������B
##���ӎ���

 1. �ۑ��ł��镶�����127�����ł��B����ȏ�̓���͕ۏ؂��Ă��܂���B�C�����Ďg�p���Ă��������B
 2. ����t�H���g���g�p���Ă��܂��B�\���ł��Ȃ��f�[�^������̂ł����ӂ��������B

##TwitterLedDisplay�̕ύX���@
###LedDotMatrix.init();
LedDotMatrix�̏����ݒ�����܂��B
�[�q��2-10�����C�u�������ŋ����I�ɃA�T�C������Ă��܂��B
###void LedDotMatrix.setMemory(uint8_t *up, uint16_t up_size, uint8_t *lo, uint16_t lo_size)
ROM�܂���RAM�Ɋi�[����Ă���f�[�^��\�����s���܂��B
uint8_t *up	��i�ɕ\������f�[�^�̐擪�|�C���^���w�肵�Ă��������BNULL���w�肷��Ɖ����ݒ��ύX���܂���B
uint16_t up_size	��i�ɕ\������f�[�^�̃T�C�Y���w�肵�܂��B������*8���w�肵�Ă��������B
uint8_t *lo	���i�ɕ\������f�[�^�̐擪�|�C���^���w�肵�܂��BNULL���w�肷��ƁA�����ݒ��ύX���܂���B
uint16_t lo_size	���i�ɕ\������f�[�^�̃T�C�Y���w�肵�܂��B������*8���w�肵�Ă��������B

###void LedDotMatrix.setFlash(uint8_t up_sector, uint16_t up_offset, uint16_t up_size, uint8_t lo_sector,uint16_t lo_offset, uint16_t lo_size)
Flash�������Ɋi�[���ꂽ�f�[�^�̕\�����s���܂��B
uint8_t up_sector	��i�ɕ\�����郁�����̃Z�N�^���w�肵�܂��B
uint16_t�@up_offset	��i�ɕ\�����郁�����̔Ԓn���w�肵�܂��B
uint16_t up_size	��i�ɕ\��������f�[�^�̃T�C�Y���w�肵�܂��B������*8���w�肵�Ă��������B�T�C�Y��0�ɂ���Ɖ����ݒ��ύX���܂���B
uint8_t lo_sector	���i�ɕ\�����郁�����̃Z�N�^���w�肵�܂��B
uint16_t lo_offset	���i�ɕ\�����郁�����̔Ԓn���w�肵�܂��B
uint16_t lo_size	���i�ɕ\������f�[�^�̃T�C�Y���w�肵�܂��B������*8���w�肵�Ă��������B�T�C�Y��0�ɂ���Ɖ����ݒ��ύX���܂���B
###void LedDotMatrix.shift(int speed, bool up_shift, bool lo_shift)
LedDotMatrix�̕\�����s���܂��B
int speed	1�����̕\�����x���w�肷����̂ł��B400�Ŗ�1�b�Ԃ̕\���ƂȂ�܂��B
bool up_shift	��i�̕����̃X�N���[���̗L�����w�肵�܂��Bfalse�ŃX�N���[���Ȃ��Atrue�ŃX�N���[������ł��B
bool lo_shift	���i�̕����̃X�N���[���̗L�����w�肵�܂��Bfalse�ŃX�N���[���Ȃ��Atrue�ŃX�N���[������ł��B
