.section .rodata
    .global IMAGE_LZ4_0_DATA
    .type IMAGE_LZ4_0_DATA, %object
    .align 2
IMAGE_LZ4_0_DATA:
    .incbin "image_lz4_0"
.section .rodata
    .global IMAGE_LZ4_1_DATA
    .type IMAGE_LZ4_1_DATA, %object
    .align 2
IMAGE_LZ4_1_DATA:
    .incbin "image_lz4_1"
.section .rodata
    .global IMAGE_LZ4_2_DATA
    .type IMAGE_LZ4_2_DATA, %object
    .align 2
IMAGE_LZ4_2_DATA:
    .incbin "image_lz4_2"
.section .rodata
    .global IMAGE_LZ4_3_DATA
    .type IMAGE_LZ4_3_DATA, %object
    .align 2
IMAGE_LZ4_3_DATA:
    .incbin "image_lz4_3"
.section .rodata
    .global IMAGE_LZ4_4_DATA
    .type IMAGE_LZ4_4_DATA, %object
    .align 2
IMAGE_LZ4_4_DATA:
    .incbin "image_lz4_4"
.section .rodata
    .global IMAGE_LZ4_5_DATA
    .type IMAGE_LZ4_5_DATA, %object
    .align 2
IMAGE_LZ4_5_DATA:
    .incbin "image_lz4_5"
.section .rodata
    .global IMAGE_LZ4_6_DATA
    .type IMAGE_LZ4_6_DATA, %object
    .align 2
IMAGE_LZ4_6_DATA:
    .incbin "image_lz4_6"
.section .rodata
    .global IMAGE_LZ4_7_DATA
    .type IMAGE_LZ4_7_DATA, %object
    .align 2
IMAGE_LZ4_7_DATA:
    .incbin "image_lz4_7"
IMAGE_LZ4_DATA_END:
    .global IMAGE_LZ4_DATA_SIZE
    .type IMAGE_LZ4_DATA_SIZE, %object
    .align 2
IMAGE_LZ4_DATA_SIZE:
    .int IMAGE_LZ4_DATA_END - IMAGE_LZ4_0_DATA

.section .rodata
    .global IMAGE_LZ77_0_DATA
    .type IMAGE_LZ77_0_DATA, %object
    .align 2
IMAGE_LZ77_0_DATA:
    .incbin "image_lz77_0"
.section .rodata
    .global IMAGE_LZ77_1_DATA
    .type IMAGE_LZ77_1_DATA, %object
    .align 2
IMAGE_LZ77_1_DATA:
    .incbin "image_lz77_1"
.section .rodata
    .global IMAGE_LZ77_2_DATA
    .type IMAGE_LZ77_2_DATA, %object
    .align 2
IMAGE_LZ77_2_DATA:
    .incbin "image_lz77_2"
.section .rodata
    .global IMAGE_LZ77_3_DATA
    .type IMAGE_LZ77_3_DATA, %object
    .align 2
IMAGE_LZ77_3_DATA:
    .incbin "image_lz77_3"
.section .rodata
    .global IMAGE_LZ77_4_DATA
    .type IMAGE_LZ77_4_DATA, %object
    .align 2
IMAGE_LZ77_4_DATA:
    .incbin "image_lz77_4"
.section .rodata
    .global IMAGE_LZ77_5_DATA
    .type IMAGE_LZ77_5_DATA, %object
    .align 2
IMAGE_LZ77_5_DATA:
    .incbin "image_lz77_5"
.section .rodata
    .global IMAGE_LZ77_6_DATA
    .type IMAGE_LZ77_6_DATA, %object
    .align 2
IMAGE_LZ77_6_DATA:
    .incbin "image_lz77_6"
.section .rodata
    .global IMAGE_LZ77_7_DATA
    .type IMAGE_LZ77_7_DATA, %object
    .align 2
IMAGE_LZ77_7_DATA:
    .incbin "image_lz77_7"
IMAGE_LZ77_DATA_END:
    .global IMAGE_LZ77_DATA_SIZE
    .type IMAGE_LZ77_DATA_SIZE, %object
    .align 2
IMAGE_LZ77_DATA_SIZE:
    .int IMAGE_LZ77_DATA_END - IMAGE_LZ77_0_DATA