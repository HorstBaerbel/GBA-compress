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
