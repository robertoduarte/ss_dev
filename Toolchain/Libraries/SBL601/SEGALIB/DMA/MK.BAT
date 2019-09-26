@echo off
IF EXIST sega_dma.lib del sega_dma.lib
..\..\sh\bin\shc -sub=dma.shc dma_cpu0.c
..\..\sh\bin\shc -sub=dma.shc dma_cpu1.c
..\..\sh\bin\shc -sub=dma.shc dma_cpu2.c
..\..\sh\bin\shc -sub=dma.shc dma_cpu3.c
..\..\sh\bin\shc -sub=dma.shc dma_cpu4.c
..\..\sh\bin\shc -sub=dma.shc dma_cpu5.c
..\..\sh\bin\shc -sub=dma.shc dma_scu0.c
..\..\sh\bin\shc -sub=dma.shc dma_scu1.c
..\..\sh\bin\lbr /sub=dma.lbr
