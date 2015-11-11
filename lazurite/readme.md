#TwitterLedDisplay.cについて

##概要
Raspberry Piから送信されたLEDのビットマップデータをML620Q504内蔵データFlashメモリに保存し、LEDドットマトリクスを制御して表示を行うプログラムです。

Lazurite Sub-GHzは"TwitterBeacon"というテキストメッセージをRaspberry Piに送信して１秒間、Raspberry Piに表示用データのリクエストを送信します。１秒以内にデータを受信したらその値をFlashメモリに保存します。Raspberry Piからの送信がなければ保存されているデータを表示します。
Raspberry Pi側のプログラムは、"get_tl_display.rb"を使用してください。
##注意事項

 1. 保存できる文字列は127文字です。それ以上の動作は保証していません。気をつけて使用してください。
 2. 美咲フォントを使用しています。表示できないデータがあるのでご注意ください。

##TwitterLedDisplayの変更方法
###LedDotMatrix.init();
LedDotMatrixの初期設定をします。
端子は2-10がライブラリ内で強制的にアサインされています。
###void LedDotMatrix.setMemory(uint8_t *up, uint16_t up_size, uint8_t *lo, uint16_t lo_size)
ROMまたはRAMに格納されているデータを表示を行います。
uint8_t *up	上段に表示するデータの先頭ポインタを指定してください。NULLを指定すると何も設定を変更しません。
uint16_t up_size	上段に表示するデータのサイズを指定します。文字数*8を指定してください。
uint8_t *lo	下段に表示するデータの先頭ポインタを指定します。NULLを指定すると、何も設定を変更しません。
uint16_t lo_size	下段に表示するデータのサイズを指定します。文字数*8を指定してください。

###void LedDotMatrix.setFlash(uint8_t up_sector, uint16_t up_offset, uint16_t up_size, uint8_t lo_sector,uint16_t lo_offset, uint16_t lo_size)
Flashメモリに格納されたデータの表示を行います。
uint8_t up_sector	上段に表示するメモリのセクタを指定します。
uint16_t　up_offset	上段に表示するメモリの番地を指定します。
uint16_t up_size	上段に表示をするデータのサイズを指定します。文字列*8を指定してください。サイズを0にすると何も設定を変更しません。
uint8_t lo_sector	下段に表示するメモリのセクタを指定します。
uint16_t lo_offset	下段に表示するメモリの番地を指定します。
uint16_t lo_size	下段に表示するデータのサイズを指定します。文字数*8を指定してください。サイズを0にすると何も設定を変更しません。
###void LedDotMatrix.shift(int speed, bool up_shift, bool lo_shift)
LedDotMatrixの表示を行います。
int speed	1文字の表示速度を指定するものです。400で約1秒間の表示となります。
bool up_shift	上段の文字のスクロールの有無を指定します。falseでスクロールなし、trueでスクロールありです。
bool lo_shift	下段の文字のスクロールの有無を指定します。falseでスクロールなし、trueでスクロールありです。
