.section .rodata
    .global IMAGE_LZ10_DATA
    .type IMAGE_LZ10_DATA, %object
    .align 2
IMAGE_LZ10_DATA:
    .incbin "image_lz10"
IMAGE_LZ10_END:
    .global IMAGE_LZ10_SIZE
    .type IMAGE_LZ10_SIZE, %object
    .align 2
IMAGE_LZ10_SIZE:
    .int IMAGE_LZ10_END - IMAGE_LZ10_DATA

.section .rodata
    .global IMAGE_LZ4_DATA
    .type IMAGE_LZ4_DATA, %object
    .align 2
IMAGE_LZ4_DATA:
    .incbin "image_lz4"
IMAGE_LZ4_END:
    .global IMAGE_LZ4_SIZE
    .type IMAGE_LZ4_SIZE, %object
    .align 2
IMAGE_LZ4_SIZE:
    .int IMAGE_LZ4_END - IMAGE_LZ4_DATA
