MEMORY {
  sfr              : ORIGIN = 0x0000, LENGTH = 0x0010 /* END=0x0010, size 16 */
  peripheral_8bit  : ORIGIN = 0x0010, LENGTH = 0x00f0 /* END=0x0100, size 240 */
  peripheral_16bit : ORIGIN = 0x0100, LENGTH = 0x0100 /* END=0x0200, size 256 */
  bsl              : ORIGIN = 0x1000, LENGTH = 0x0800 /* END=0x1800, size 2K as 4 512-byte segments */
  infomem          : ORIGIN = 0x1800, LENGTH = 0x0200 /* END=0x1a00, size 512 as 4 128-byte segments */
  infod            : ORIGIN = 0x1800, LENGTH = 0x0080 /* END=0x1880, size 128 */
  infoc            : ORIGIN = 0x1880, LENGTH = 0x0080 /* END=0x1900, size 128 */
  infob            : ORIGIN = 0x1900, LENGTH = 0x0080 /* END=0x1980, size 128 */
  infoa            : ORIGIN = 0x1980, LENGTH = 0x0080 /* END=0x1a00, size 128 */
  ram (wx)         : ORIGIN = 0x1c00, LENGTH = 0x4000 /* END=0x5c00, size 16K */
  rom (rx)         : ORIGIN = 0x5c00, LENGTH = 0xa380 /* END=0xff80, size 41856 */
  vectors          : ORIGIN = 0xff80, LENGTH = 0x0080 /* END=0x10000, size 128 as 64 2-byte segments */
  far_rom          : ORIGIN = 0x00010000, LENGTH = 0x00035c00 /* END=0x00045c00, size 215K */
  /* Remaining banks are absent */
  ram2 (wx)        : ORIGIN = 0x0000, LENGTH = 0x0000
  ram_mirror (wx)  : ORIGIN = 0x0000, LENGTH = 0x0000
  usbram (wx)      : ORIGIN = 0x0000, LENGTH = 0x0000
}
REGION_ALIAS("REGION_TEXT", rom);
REGION_ALIAS("REGION_DATA", ram);
REGION_ALIAS("REGION_FAR_ROM", far_rom);
PROVIDE (__info_segment_size = 0x80);
PROVIDE (__infod = 0x1800);
PROVIDE (__infoc = 0x1880);
PROVIDE (__infob = 0x1900);
PROVIDE (__infoa = 0x1980);
PROVIDE (__heap_base__ = 0x01C00);
PROVIDE (__heap_end__ = 0x05BFF);
