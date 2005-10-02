#include "font.h"

unsigned int PROGMEM fontIndex_org[] = {                                    0, /*   */
                                    1, /* ! */
                                    2, /* " */
                                    5, /* # */
                                    12, /* $ */
                                    16, /* % */
                                    22, /* & */
                                    27, /* ' */
                                    28, /* ( */
                                    31, /* ) */
                                    34, /* * */
                                    39, /* + */
                                    44, /* , */
                                    46, /* - */
                                    51, /* . */
                                    52, /* / */
                                    55, /* 0 */
                                    59, /* 1 */
                                    61, /* 2 */
                                    65, /* 3 */
                                    69, /* 4 */
                                    73, /* 5 */
                                    77, /* 6 */
                                    81, /* 7 */
                                    85, /* 8 */
                                    89, /* 9 */
                                    93, /* : */
                                    94, /* ; */
                                    96, /* < */
                                    99, /* = */
                                    103, /* > */
                                    106, /* ? */
                                    109, /* @ */
                                    116, /* A */
                                    121, /* B */
                                    125, /* C */
                                    129, /* D */
                                    133, /* E */
                                    136, /* F */
                                    139, /* G */
                                    143, /* H */
                                    147, /* I */
                                    148, /* J */
                                    152, /* K */
                                    156, /* L */
                                    159, /* M */
                                    164, /* N */
                                    168, /* O */
                                    172, /* P */
                                    176, /* Q */
                                    180, /* R */
                                    184, /* S */
                                    188, /* T */
                                    193, /* U */
                                    197, /* V */
                                    202, /* W */
                                    211, /* X */
                                    216, /* Y */
                                    221, /* Z */
                                    225, /* [ */
                                    227, /* \ */
                                    231, /* ] */
                                    233, /* ^ */
                                    236, /* _ */
                                    242, /* ` */
                                    244, /* a */
                                    248, /* b */
                                    252, /* c */
                                    255, /* d */
                                    259, /* e */
                                    263, /* f */
                                    266, /* g */
                                    270, /* h */
                                    274, /* i */
                                    275, /* j */
                                    277, /* k */
                                    281, /* l */
                                    282, /* m */
                                    287, /* n */
                                    291, /* o */
                                    295, /* p */
                                    299, /* q */
                                    303, /* r */
                                    306, /* s */
                                    309, /* t */
                                    311, /* u */
                                    315, /* v */
                                    318, /* w */
                                    323, /* x */
                                    326, /* y */
                                    329, /* z */
                                    333, /* { */
                                    336, /* | */
                                    337, /* } */
                                    340, /* ~ */
	346
};

unsigned char PROGMEM fontData_org[] = {
                                0x00, /*          */
                                0x2f, /*   # #### */
                                0x03, /*       ## */
                                0x00, /*          */
                                0x03, /*       ## */
                                0x10, /*    #     */
                                0x34, /*   ## #   */
                                0x1c, /*    ###   */
                                0x36, /*   ## ##  */
                                0x1c, /*    ###   */
                                0x16, /*    # ##  */
                                0x04, /*      #   */
                                0x24, /*   #  #   */
                                0x2a, /*   # # #  */
                                0x6b, /*  ## # ## */
                                0x12, /*    #  #  */
                                0x26, /*   #  ##  */
                                0x15, /*    # # # */
                                0x0b, /*     # ## */
                                0x34, /*   ## #   */
                                0x2a, /*   # # #  */
                                0x19, /*    ##  # */
                                0x1a, /*    ## #  */
                                0x25, /*   #  # # */
                                0x21, /*   #    # */
                                0x1a, /*    ## #  */
                                0x08, /*     #    */
                                0x03, /*       ## */
                                0x3c, /*   ####   */
                                0x42, /*  #    #  */
                                0x81, /* #      # */
                                0x81, /* #      # */
                                0x42, /*  #    #  */
                                0x3c, /*   ####   */
                                0x04, /*      #   */
                                0x15, /*    # # # */
                                0x0e, /*     ###  */
                                0x15, /*    # # # */
                                0x04, /*      #   */
                                0x08, /*     #    */
                                0x08, /*     #    */
                                0x3e, /*   #####  */
                                0x08, /*     #    */
                                0x08, /*     #    */
                                0x80, /* #        */
                                0x60, /*  ##      */
                                0x08, /*     #    */
                                0x08, /*     #    */
                                0x08, /*     #    */
                                0x08, /*     #    */
                                0x08, /*     #    */
                                0x20, /*   #      */
                                0x30, /*   ##     */
                                0x0c, /*     ##   */
                                0x03, /*       ## */
                                0x1e, /*    ####  */
                                0x21, /*   #    # */
                                0x21, /*   #    # */
                                0x1e, /*    ####  */
                                0x02, /*       #  */
                                0x3f, /*   ###### */
                                0x22, /*   #   #  */
                                0x31, /*   ##   # */
                                0x29, /*   # #  # */
                                0x26, /*   #  ##  */
                                0x12, /*    #  #  */
                                0x21, /*   #    # */
                                0x25, /*   #  # # */
                                0x1a, /*    ## #  */
                                0x0c, /*     ##   */
                                0x0a, /*     # #  */
                                0x3f, /*   ###### */
                                0x08, /*     #    */
                                0x17, /*    # ### */
                                0x25, /*   #  # # */
                                0x25, /*   #  # # */
                                0x19, /*    ##  # */
                                0x1c, /*    ###   */
                                0x26, /*   #  ##  */
                                0x25, /*   #  # # */
                                0x18, /*    ##    */
                                0x01, /*        # */
                                0x39, /*   ###  # */
                                0x05, /*      # # */
                                0x03, /*       ## */
                                0x1a, /*    ## #  */
                                0x25, /*   #  # # */
                                0x25, /*   #  # # */
                                0x1a, /*    ## #  */
                                0x06, /*      ##  */
                                0x29, /*   # #  # */
                                0x19, /*    ##  # */
                                0x0e, /*     ###  */
                                0x24, /*   #  #   */
                                0x80, /* #        */
                                0x64, /*  ##  #   */
                                0x08, /*     #    */
                                0x14, /*    # #   */
                                0x22, /*   #   #  */
                                0x24, /*   #  #   */
                                0x24, /*   #  #   */
                                0x24, /*   #  #   */
                                0x24, /*   #  #   */
                                0x22, /*   #   #  */
                                0x14, /*    # #   */
                                0x08, /*     #    */
                                0x01, /*        # */
                                0x2d, /*   # ## # */
                                0x02, /*       #  */
                                0x1c, /*    ###   */
                                0x22, /*   #   #  */
                                0x59, /*  # ##  # */
                                0x55, /*  # # # # */
                                0x4d, /*  #  ## # */
                                0x11, /*    #   # */
                                0x0e, /*     ###  */
                                0x30, /*   ##     */
                                0x0c, /*     ##   */
                                0x0b, /*     # ## */
                                0x0c, /*     ##   */
                                0x30, /*   ##     */
                                0x3f, /*   ###### */
                                0x25, /*   #  # # */
                                0x25, /*   #  # # */
                                0x1a, /*    ## #  */
                                0x1e, /*    ####  */
                                0x21, /*   #    # */
                                0x21, /*   #    # */
                                0x12, /*    #  #  */
                                0x3f, /*   ###### */
                                0x21, /*   #    # */
                                0x21, /*   #    # */
                                0x1e, /*    ####  */
                                0x3f, /*   ###### */
                                0x25, /*   #  # # */
                                0x25, /*   #  # # */
                                0x3f, /*   ###### */
                                0x05, /*      # # */
                                0x05, /*      # # */
                                0x1e, /*    ####  */
                                0x21, /*   #    # */
                                0x29, /*   # #  # */
                                0x1a, /*    ## #  */
                                0x3f, /*   ###### */
                                0x04, /*      #   */
                                0x04, /*      #   */
                                0x3f, /*   ###### */
                                0x3f, /*   ###### */
                                0x10, /*    #     */
                                0x20, /*   #      */
                                0x20, /*   #      */
                                0x1f, /*    ##### */
                                0x3f, /*   ###### */
                                0x0c, /*     ##   */
                                0x12, /*    #  #  */
                                0x21, /*   #    # */
                                0x3f, /*   ###### */
                                0x20, /*   #      */
                                0x20, /*   #      */
                                0x3f, /*   ###### */
                                0x02, /*       #  */
                                0x04, /*      #   */
                                0x02, /*       #  */
                                0x3f, /*   ###### */
                                0x3f, /*   ###### */
                                0x06, /*      ##  */
                                0x18, /*    ##    */
                                0x3f, /*   ###### */
                                0x1e, /*    ####  */
                                0x21, /*   #    # */
                                0x21, /*   #    # */
                                0x1e, /*    ####  */
                                0x3f, /*   ###### */
                                0x09, /*     #  # */
                                0x09, /*     #  # */
                                0x06, /*      ##  */
                                0x1e, /*    ####  */
                                0x21, /*   #    # */
                                0x21, /*   #    # */
                                0x5e, /*  # ####  */
                                0x3f, /*   ###### */
                                0x09, /*     #  # */
                                0x19, /*    ##  # */
                                0x26, /*   #  ##  */
                                0x12, /*    #  #  */
                                0x25, /*   #  # # */
                                0x29, /*   # #  # */
                                0x12, /*    #  #  */
                                0x01, /*        # */
                                0x01, /*        # */
                                0x3f, /*   ###### */
                                0x01, /*        # */
                                0x01, /*        # */
                                0x1f, /*    ##### */
                                0x20, /*   #      */
                                0x20, /*   #      */
                                0x1f, /*    ##### */
                                0x03, /*       ## */
                                0x0c, /*     ##   */
                                0x30, /*   ##     */
                                0x0c, /*     ##   */
                                0x03, /*       ## */
                                0x03, /*       ## */
                                0x0c, /*     ##   */
                                0x30, /*   ##     */
                                0x0c, /*     ##   */
                                0x03, /*       ## */
                                0x0c, /*     ##   */
                                0x30, /*   ##     */
                                0x0c, /*     ##   */
                                0x03, /*       ## */
                                0x21, /*   #    # */
                                0x12, /*    #  #  */
                                0x0c, /*     ##   */
                                0x12, /*    #  #  */
                                0x21, /*   #    # */
                                0x01, /*        # */
                                0x02, /*       #  */
                                0x3c, /*   ####   */
                                0x02, /*       #  */
                                0x01, /*        # */
                                0x31, /*   ##   # */
                                0x29, /*   # #  # */
                                0x25, /*   #  # # */
                                0x23, /*   #   ## */
                                0x7f, /*  ####### */
                                0x41, /*  #     # */
                                0x03, /*       ## */
                                0x0c, /*     ##   */
                                0x30, /*   ##     */
                                0xc0, /* ##       */
                                0x41, /*  #     # */
                                0x7f, /*  ####### */
                                0x02, /*       #  */
                                0x01, /*        # */
                                0x02, /*       #  */
                                0x40, /*  #       */
                                0x40, /*  #       */
                                0x40, /*  #       */
                                0x40, /*  #       */
                                0x40, /*  #       */
                                0x40, /*  #       */
                                0x01, /*        # */
                                0x02, /*       #  */
                                0x18, /*    ##    */
                                0x24, /*   #  #   */
                                0x24, /*   #  #   */
                                0x3c, /*   ####   */
                                0x3f, /*   ###### */
                                0x24, /*   #  #   */
                                0x24, /*   #  #   */
                                0x18, /*    ##    */
                                0x18, /*    ##    */
                                0x24, /*   #  #   */
                                0x24, /*   #  #   */
                                0x18, /*    ##    */
                                0x24, /*   #  #   */
                                0x24, /*   #  #   */
                                0x3f, /*   ###### */
                                0x18, /*    ##    */
                                0x24, /*   #  #   */
                                0x2c, /*   # ##   */
                                0x08, /*     #    */
                                0x3e, /*   #####  */
                                0x05, /*      # # */
                                0x01, /*        # */
                                0x18, /*    ##    */
                                0xa4, /* # #  #   */
                                0xa4, /* # #  #   */
                                0x7c, /*  #####   */
                                0x3f, /*   ###### */
                                0x04, /*      #   */
                                0x04, /*      #   */
                                0x38, /*   ###    */
                                0x3d, /*   #### # */
                                0x80, /* #        */
                                0x7d, /*  ##### # */
                                0x3f, /*   ###### */
                                0x08, /*     #    */
                                0x18, /*    ##    */
                                0x24, /*   #  #   */
                                0x3f, /*   ###### */
                                0x3c, /*   ####   */
                                0x04, /*      #   */
                                0x3c, /*   ####   */
                                0x04, /*      #   */
                                0x38, /*   ###    */
                                0x3c, /*   ####   */
                                0x04, /*      #   */
                                0x04, /*      #   */
                                0x38, /*   ###    */
                                0x18, /*    ##    */
                                0x24, /*   #  #   */
                                0x24, /*   #  #   */
                                0x18, /*    ##    */
                                0xfc, /* ######   */
                                0x24, /*   #  #   */
                                0x24, /*   #  #   */
                                0x18, /*    ##    */
                                0x18, /*    ##    */
                                0x24, /*   #  #   */
                                0x24, /*   #  #   */
                                0xfc, /* ######   */
                                0x3c, /*   ####   */
                                0x08, /*     #    */
                                0x04, /*      #   */
                                0x28, /*   # #    */
                                0x2c, /*   # ##   */
                                0x14, /*    # #   */
                                0x1e, /*    ####  */
                                0x24, /*   #  #   */
                                0x1c, /*    ###   */
                                0x20, /*   #      */
                                0x20, /*   #      */
                                0x3c, /*   ####   */
                                0x0c, /*     ##   */
                                0x30, /*   ##     */
                                0x0c, /*     ##   */
                                0x0c, /*     ##   */
                                0x30, /*   ##     */
                                0x0c, /*     ##   */
                                0x30, /*   ##     */
                                0x0c, /*     ##   */
                                0x24, /*   #  #   */
                                0x18, /*    ##    */
                                0x24, /*   #  #   */
                                0xcc, /* ##  ##   */
                                0x30, /*   ##     */
                                0x0c, /*     ##   */
                                0x24, /*   #  #   */
                                0x34, /*   ## #   */
                                0x2c, /*   # ##   */
                                0x24, /*   #  #   */
                                0x18, /*    ##    */
                                0x66, /*  ##  ##  */
                                0x81, /* #      # */
                                0xff, /* ######## */
                                0x81, /* #      # */
                                0x66, /*  ##  ##  */
                                0x18, /*    ##    */
                                0x02, /*       #  */
                                0x01, /*        # */
                                0x01, /*        # */
                                0x02, /*       #  */
                                0x02, /*       #  */
                                0x01, /*        # */
};
font font_ = {8, fontIndex_org, fontData_org, ' ', '~', '.', 1};
