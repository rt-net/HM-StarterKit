/************************************************************************
*
* Device     : RX/RX600/RX63N,RX631
*
* File Name  : vect.h
*
* Abstract   : Definition of Vector.
*
* History    : 0.10  (2011-02-21)  [Hardware Manual Revision : 0.01]
*            : 0.11  (2011-06-20)  [Hardware Manual Revision : 0.50]
*            : 1.00  (2012-06-12)  [Hardware Manual Revision : 1.00]
*            : 1.10  (2013-02-18)  [Hardware Manual Revision : 1.00]
*            : 1.10a (2013-06-18)  [Hardware Manual Revision : 1.00]
*            : 1.8   (2015-04-22)  [Hardware Manual Revision : 1.80]
*
* NOTE       : THIS IS A TYPICAL EXAMPLE.
*
* Copyright (C) 2015 (2011 - 2013) Renesas Electronics Corporation.
*
************************************************************************/

// Exception(Supervisor Instruction)
#pragma interrupt (Excep_SuperVisorInst)
void Excep_SuperVisorInst(void);

// Exception(Access Instruction)
#pragma interrupt (Excep_AccessInst)
void Excep_AccessInst(void);

// Exception(Undefined Instruction)
#pragma interrupt (Excep_UndefinedInst)
void Excep_UndefinedInst(void);

// Exception(Floating Point)
#pragma interrupt (Excep_FloatingPoint)
void Excep_FloatingPoint(void);

// NMI
#pragma interrupt (NonMaskableInterrupt)
void NonMaskableInterrupt(void);

// Dummy
#pragma interrupt (Dummy)
void Dummy(void);

// BRK
#pragma interrupt (Excep_BRK(vect=0))
void Excep_BRK(void);

// vector  1 reserved
// vector  2 reserved
// vector  3 reserved
// vector  4 reserved
// vector  5 reserved
// vector  6 reserved
// vector  7 reserved
// vector  8 reserved
// vector  9 reserved
// vector 10 reserved
// vector 11 reserved
// vector 12 reserved
// vector 13 reserved
// vector 14 reserved
// vector 15 reserved

// BSC BUSERR
#pragma interrupt (Excep_BSC_BUSERR(vect=16))
void Excep_BSC_BUSERR(void);

// FCU FIFERR
#pragma interrupt (Excep_FCU_FIFERR(vect=21))
void Excep_FCU_FIFERR(void);

// FCU FRDYI
#pragma interrupt (Excep_FCU_FRDYI(vect=23))
void Excep_FCU_FRDYI(void);

// ICU SWINT
#pragma interrupt (Excep_ICU_SWINT(vect=27))
void Excep_ICU_SWINT(void);

// CMT0 CMI0
#pragma interrupt (Excep_CMT0_CMI0(vect=28))
void Excep_CMT0_CMI0(void);

// CMT1 CMI1
#pragma interrupt (Excep_CMT1_CMI1(vect=29))
void Excep_CMT1_CMI1(void);

// CMT2 CMI2
#pragma interrupt (Excep_CMT2_CMI2(vect=30))
void Excep_CMT2_CMI2(void);

// CMT3 CMI3
#pragma interrupt (Excep_CMT3_CMI3(vect=31))
void Excep_CMT3_CMI3(void);

// ETHER EINT
#pragma interrupt (Excep_ETHER_EINT(vect=32))
void Excep_ETHER_EINT(void);

// USB0 D0FIFO0
#pragma interrupt (Excep_USB0_D0FIFO0(vect=33))
void Excep_USB0_D0FIFO0(void);

// USB0 D1FIFO0
#pragma interrupt (Excep_USB0_D1FIFO0(vect=34))
void Excep_USB0_D1FIFO0(void);

// USB0 USBI0
#pragma interrupt (Excep_USB0_USBI0(vect=35))
void Excep_USB0_USBI0(void);

// USB1 D0FIFO1
#pragma interrupt (Excep_USB1_D0FIFO1(vect=36))
void Excep_USB1_D0FIFO1(void);

// USB1 D1FIFO1
#pragma interrupt (Excep_USB1_D1FIFO1(vect=37))
void Excep_USB1_D1FIFO1(void);

// USB1 USBI1
#pragma interrupt (Excep_USB1_USBI1(vect=38))
void Excep_USB1_USBI1(void);

// RSPI0 SPRI0
#pragma interrupt (Excep_RSPI0_SPRI0(vect=39))
void Excep_RSPI0_SPRI0(void);

// RSPI0 SPTI0
#pragma interrupt (Excep_RSPI0_SPTI0(vect=40))
void Excep_RSPI0_SPTI0(void);

// RSPI0 SPII0
#pragma interrupt (Excep_RSPI0_SPII0(vect=41))
void Excep_RSPI0_SPII0(void);

// RSPI1 SPRI1
#pragma interrupt (Excep_RSPI1_SPRI1(vect=42))
void Excep_RSPI1_SPRI1(void);

// RSPI1 SPTI1
#pragma interrupt (Excep_RSPI1_SPTI1(vect=43))
void Excep_RSPI1_SPTI1(void);

// RSPI1 SPII1
#pragma interrupt (Excep_RSPI1_SPII1(vect=44))
void Excep_RSPI1_SPII1(void);

// RSPI2 SPRI2
#pragma interrupt (Excep_RSPI2_SPRI2(vect=45))
void Excep_RSPI2_SPRI2(void);

// RSPI2 SPTI2
#pragma interrupt (Excep_RSPI2_SPTI2(vect=46))
void Excep_RSPI2_SPTI2(void);

// RSPI2 SPII2
#pragma interrupt (Excep_RSPI2_SPII2(vect=47))
void Excep_RSPI2_SPII2(void);

// CAN0 RXF0
#pragma interrupt (Excep_CAN0_RXF0(vect=48))
void Excep_CAN0_RXF0(void);

// CAN0 TXF0
#pragma interrupt (Excep_CAN0_TXF0(vect=49))
void Excep_CAN0_TXF0(void);

// CAN0 RXM0
#pragma interrupt (Excep_CAN0_RXM0(vect=50))
void Excep_CAN0_RXM0(void);

// CAN0 TXM0
#pragma interrupt (Excep_CAN0_TXM0(vect=51))
void Excep_CAN0_TXM0(void);

// CAN1 RXF1
#pragma interrupt (Excep_CAN1_RXF1(vect=52))
void Excep_CAN1_RXF1(void);

// CAN1 TXF1
#pragma interrupt (Excep_CAN1_TXF1(vect=53))
void Excep_CAN1_TXF1(void);

// CAN1 RXM1
#pragma interrupt (Excep_CAN1_RXM1(vect=54))
void Excep_CAN1_RXM1(void);

// CAN1 TXM1
#pragma interrupt (Excep_CAN1_TXM1(vect=55))
void Excep_CAN1_TXM1(void);

// CAN2 RXF2
#pragma interrupt (Excep_CAN2_RXF2(vect=56))
void Excep_CAN2_RXF2(void);

// CAN2 TXF2
#pragma interrupt (Excep_CAN2_TXF2(vect=57))
void Excep_CAN2_TXF2(void);

// CAN2 RXM2
#pragma interrupt (Excep_CAN2_RXM2(vect=58))
void Excep_CAN2_RXM2(void);

// CAN2 TXM2
#pragma interrupt (Excep_CAN2_TXM2(vect=59))
void Excep_CAN2_TXM2(void);

// RTC CUP
#pragma interrupt (Excep_RTC_CUP(vect=62))
void Excep_RTC_CUP(void);

// ICU IRQ0
#pragma interrupt (Excep_ICU_IRQ0(vect=64))
void Excep_ICU_IRQ0(void);

// ICU IRQ1
#pragma interrupt (Excep_ICU_IRQ1(vect=65))
void Excep_ICU_IRQ1(void);

// ICU IRQ2
#pragma interrupt (Excep_ICU_IRQ2(vect=66))
void Excep_ICU_IRQ2(void);

// ICU IRQ3
#pragma interrupt (Excep_ICU_IRQ3(vect=67))
void Excep_ICU_IRQ3(void);

// ICU IRQ4
#pragma interrupt (Excep_ICU_IRQ4(vect=68))
void Excep_ICU_IRQ4(void);

// ICU IRQ5
#pragma interrupt (Excep_ICU_IRQ5(vect=69))
void Excep_ICU_IRQ5(void);

// ICU IRQ6
#pragma interrupt (Excep_ICU_IRQ6(vect=70))
void Excep_ICU_IRQ6(void);

// ICU IRQ7
#pragma interrupt (Excep_ICU_IRQ7(vect=71))
void Excep_ICU_IRQ7(void);

// ICU IRQ8
#pragma interrupt (Excep_ICU_IRQ8(vect=72))
void Excep_ICU_IRQ8(void);

// ICU IRQ9
#pragma interrupt (Excep_ICU_IRQ9(vect=73))
void Excep_ICU_IRQ9(void);

// ICU IRQ10
#pragma interrupt (Excep_ICU_IRQ10(vect=74))
void Excep_ICU_IRQ10(void);

// ICU IRQ11
#pragma interrupt (Excep_ICU_IRQ11(vect=75))
void Excep_ICU_IRQ11(void);

// ICU IRQ12
#pragma interrupt (Excep_ICU_IRQ12(vect=76))
void Excep_ICU_IRQ12(void);

// ICU IRQ13
#pragma interrupt (Excep_ICU_IRQ13(vect=77))
void Excep_ICU_IRQ13(void);

// ICU IRQ14
#pragma interrupt (Excep_ICU_IRQ14(vect=78))
void Excep_ICU_IRQ14(void);

// ICU IRQ15
#pragma interrupt (Excep_ICU_IRQ15(vect=79))
void Excep_ICU_IRQ15(void);

// USB USBR0
#pragma interrupt (Excep_USB_USBR0(vect=90))
void Excep_USB_USBR0(void);

// USB USBR1
#pragma interrupt (Excep_USB_USBR1(vect=91))
void Excep_USB_USBR1(void);

// RTC ALM
#pragma interrupt (Excep_RTC_ALM(vect=92))
void Excep_RTC_ALM(void);

// RTC PRD
#pragma interrupt (Excep_RTC_PRD(vect=93))
void Excep_RTC_PRD(void);

// AD ADI0
#pragma interrupt (Excep_AD_ADI0(vect=98))
void Excep_AD_ADI0(void);

// S12AD S12ADI0
#pragma interrupt (Excep_S12AD_S12ADI0(vect=102))
void Excep_S12AD_S12ADI0(void);

// ICU GROUP0
#pragma interrupt (Excep_ICU_GROUP0(vect=106))
void Excep_ICU_GROUP0(void);

// ICU GROUP1
#pragma interrupt (Excep_ICU_GROUP1(vect=107))
void Excep_ICU_GROUP1(void);

// ICU GROUP2
#pragma interrupt (Excep_ICU_GROUP2(vect=108))
void Excep_ICU_GROUP2(void);

// ICU GROUP3
#pragma interrupt (Excep_ICU_GROUP3(vect=109))
void Excep_ICU_GROUP3(void);

// ICU GROUP4
#pragma interrupt (Excep_ICU_GROUP4(vect=110))
void Excep_ICU_GROUP4(void);

// ICU GROUP5
#pragma interrupt (Excep_ICU_GROUP5(vect=111))
void Excep_ICU_GROUP5(void);

// ICU GROUP6
#pragma interrupt (Excep_ICU_GROUP6(vect=112))
void Excep_ICU_GROUP6(void);

// ICU GROUP12
#pragma interrupt (Excep_ICU_GROUP12(vect=114))
void Excep_ICU_GROUP12(void);

// SCI12 SCIX0
#pragma interrupt (Excep_SCI12_SCIX0(vect=122))
void Excep_SCI12_SCIX0(void);

// SCI12 SCIX1
#pragma interrupt (Excep_SCI12_SCIX1(vect=123))
void Excep_SCI12_SCIX1(void);

// SCI12 SCIX2
#pragma interrupt (Excep_SCI12_SCIX2(vect=124))
void Excep_SCI12_SCIX2(void);

// SCI12 SCIX3
#pragma interrupt (Excep_SCI12_SCIX3(vect=125))
void Excep_SCI12_SCIX3(void);

// TPU0 TGI0A
#pragma interrupt (Excep_TPU0_TGI0A(vect=126))
void Excep_TPU0_TGI0A(void);

// TPU0 TGI0B
#pragma interrupt (Excep_TPU0_TGI0B(vect=127))
void Excep_TPU0_TGI0B(void);

// TPU0 TGI0C
#pragma interrupt (Excep_TPU0_TGI0C(vect=128))
void Excep_TPU0_TGI0C(void);

// TPU0 TGI0D
#pragma interrupt (Excep_TPU0_TGI0D(vect=129))
void Excep_TPU0_TGI0D(void);

// TPU1 TGI1A
#pragma interrupt (Excep_TPU1_TGI1A(vect=130))
void Excep_TPU1_TGI1A(void);

// TPU1 TGI1B
#pragma interrupt (Excep_TPU1_TGI1B(vect=131))
void Excep_TPU1_TGI1B(void);

// TPU2 TGI2A
#pragma interrupt (Excep_TPU2_TGI2A(vect=132))
void Excep_TPU2_TGI2A(void);

// TPU2 TGI2B
#pragma interrupt (Excep_TPU2_TGI2B(vect=133))
void Excep_TPU2_TGI2B(void);

// TPU3 TGI3A
#pragma interrupt (Excep_TPU3_TGI3A(vect=134))
void Excep_TPU3_TGI3A(void);

// TPU3 TGI3B
#pragma interrupt (Excep_TPU3_TGI3B(vect=135))
void Excep_TPU3_TGI3B(void);

// TPU3 TGI3C
#pragma interrupt (Excep_TPU3_TGI3C(vect=136))
void Excep_TPU3_TGI3C(void);

// TPU3 TGI3D
#pragma interrupt (Excep_TPU3_TGI3D(vect=137))
void Excep_TPU3_TGI3D(void);

// TPU4 TGI4A
#pragma interrupt (Excep_TPU4_TGI4A(vect=138))
void Excep_TPU4_TGI4A(void);

// TPU4 TGI4B
#pragma interrupt (Excep_TPU4_TGI4B(vect=139))
void Excep_TPU4_TGI4B(void);

// TPU5 TGI5A
#pragma interrupt (Excep_TPU5_TGI5A(vect=140))
void Excep_TPU5_TGI5A(void);

// TPU5 TGI5B
#pragma interrupt (Excep_TPU5_TGI5B(vect=141))
void Excep_TPU5_TGI5B(void);

// TPU6 TGI6A
#pragma interrupt (Excep_TPU6_TGI6A(vect=142))
void Excep_TPU6_TGI6A(void);

// MTU0 TGIA0
//#pragma interrupt (Excep_MTU0_TGIA0(vect=142))
//void Excep_MTU0_TGIA0(void);

// TPU6 TGI6B
#pragma interrupt (Excep_TPU6_TGI6B(vect=143))
void Excep_TPU6_TGI6B(void);

// MTU0 TGIB0
//#pragma interrupt (Excep_MTU0_TGIB0(vect=143))
//void Excep_MTU0_TGIB0(void);

// TPU6 TGI6C
#pragma interrupt (Excep_TPU6_TGI6C(vect=144))
void Excep_TPU6_TGI6C(void);

// MTU0 TGIC0
//#pragma interrupt (Excep_MTU0_TGIC0(vect=144))
//void Excep_MTU0_TGIC0(void);

// TPU6 TGI6D
#pragma interrupt (Excep_TPU6_TGI6D(vect=145))
void Excep_TPU6_TGI6D(void);

// MTU0 TGID0
//#pragma interrupt (Excep_MTU0_TGID0(vect=145))
//void Excep_MTU0_TGID0(void);

// MTU0 TGIE0
#pragma interrupt (Excep_MTU0_TGIE0(vect=146))
void Excep_MTU0_TGIE0(void);

// MTU0 TGIF0
#pragma interrupt (Excep_MTU0_TGIF0(vect=147))
void Excep_MTU0_TGIF0(void);

// TPU7 TGI7A
#pragma interrupt (Excep_TPU7_TGI7A(vect=148))
void Excep_TPU7_TGI7A(void);

// MTU1 TGIA1
//#pragma interrupt (Excep_MTU1_TGIA1(vect=148))
//void Excep_MTU1_TGIA1(void);

// TPU7 TGI7B
#pragma interrupt (Excep_TPU7_TGI7B(vect=149))
void Excep_TPU7_TGI7B(void);

// MTU1 TGIB1
//#pragma interrupt (Excep_MTU1_TGIB1(vect=149))
//void Excep_MTU1_TGIB1(void);

// TPU8 TGI8A
#pragma interrupt (Excep_TPU8_TGI8A(vect=150))
void Excep_TPU8_TGI8A(void);

// MTU2 TGIA2
//#pragma interrupt (Excep_MTU2_TGIA2(vect=150))
//void Excep_MTU2_TGIA2(void);

// TPU8 TGI8B
#pragma interrupt (Excep_TPU8_TGI8B(vect=151))
void Excep_TPU8_TGI8B(void);

// MTU2 TGIB2
//#pragma interrupt (Excep_MTU2_TGIB2(vect=151))
//void Excep_MTU2_TGIB2(void);

// TPU9 TGI9A
#pragma interrupt (Excep_TPU9_TGI9A(vect=152))
void Excep_TPU9_TGI9A(void);

// MTU3 TGIA3
//#pragma interrupt (Excep_MTU3_TGIA3(vect=152))
//void Excep_MTU3_TGIA3(void);

// TPU9 TGI9B
#pragma interrupt (Excep_TPU9_TGI9B(vect=153))
void Excep_TPU9_TGI9B(void);

// MTU3 TGIB3
//#pragma interrupt (Excep_MTU3_TGIB3(vect=153))
//void Excep_MTU3_TGIB3(void);

// TPU9 TGI9C
#pragma interrupt (Excep_TPU9_TGI9C(vect=154))
void Excep_TPU9_TGI9C(void);

// MTU3 TGIC3
//#pragma interrupt (Excep_MTU3_TGIC3(vect=154))
//void Excep_MTU3_TGIC3(void);

// TPU9 TGI9D
#pragma interrupt (Excep_TPU9_TGI9D(vect=155))
void Excep_TPU9_TGI9D(void);

// MTU3 TGID3
//#pragma interrupt (Excep_MTU3_TGID3(vect=155))
//void Excep_MTU3_TGID3(void);

// TPU10 TGI10A
#pragma interrupt (Excep_TPU10_TGI10A(vect=156))
void Excep_TPU10_TGI10A(void);

// MTU4 TGIA4
//#pragma interrupt (Excep_MTU4_TGIA4(vect=156))
//void Excep_MTU4_TGIA4(void);

// TPU10 TGI10B
#pragma interrupt (Excep_TPU10_TGI10B(vect=157))
void Excep_TPU10_TGI10B(void);

// MTU4 TGIB4
//#pragma interrupt (Excep_MTU4_TGIB4(vect=157))
//void Excep_MTU4_TGIB4(void);

// MTU4 TGIC4
#pragma interrupt (Excep_MTU4_TGIC4(vect=158))
void Excep_MTU4_TGIC4(void);

// MTU4 TGID4
#pragma interrupt (Excep_MTU4_TGID4(vect=159))
void Excep_MTU4_TGID4(void);

// MTU4 TCIV4
#pragma interrupt (Excep_MTU4_TCIV4(vect=160))
void Excep_MTU4_TCIV4(void);

// MTU5 TGIU5
#pragma interrupt (Excep_MTU5_TGIU5(vect=161))
void Excep_MTU5_TGIU5(void);

// MTU5 TGIV5
#pragma interrupt (Excep_MTU5_TGIV5(vect=162))
void Excep_MTU5_TGIV5(void);

// MTU5 TGIW5
#pragma interrupt (Excep_MTU5_TGIW5(vect=163))
void Excep_MTU5_TGIW5(void);

// TPU11 TGI11A
#pragma interrupt (Excep_TPU11_TGI11A(vect=164))
void Excep_TPU11_TGI11A(void);

// TPU11 TGI11B
#pragma interrupt (Excep_TPU11_TGI11B(vect=165))
void Excep_TPU11_TGI11B(void);

// POE OEI1
#pragma interrupt (Excep_POE_OEI1(vect=166))
void Excep_POE_OEI1(void);

// POE OEI2
#pragma interrupt (Excep_POE_OEI2(vect=167))
void Excep_POE_OEI2(void);

// TMR0 CMIA0
#pragma interrupt (Excep_TMR0_CMIA0(vect=170))
void Excep_TMR0_CMIA0(void);

// TMR0 CMIB0
#pragma interrupt (Excep_TMR0_CMIB0(vect=171))
void Excep_TMR0_CMIB0(void);

// TMR0 OVI0
#pragma interrupt (Excep_TMR0_OVI0(vect=172))
void Excep_TMR0_OVI0(void);

// TMR1 CMIA1
#pragma interrupt (Excep_TMR1_CMIA1(vect=173))
void Excep_TMR1_CMIA1(void);

// TMR1 CMIB1
#pragma interrupt (Excep_TMR1_CMIB1(vect=174))
void Excep_TMR1_CMIB1(void);

// TMR1 OVI1
#pragma interrupt (Excep_TMR1_OVI1(vect=175))
void Excep_TMR1_OVI1(void);

// TMR2 CMIA2
#pragma interrupt (Excep_TMR2_CMIA2(vect=176))
void Excep_TMR2_CMIA2(void);

// TMR2 CMIB2
#pragma interrupt (Excep_TMR2_CMIB2(vect=177))
void Excep_TMR2_CMIB2(void);

// TMR2 OVI2
#pragma interrupt (Excep_TMR2_OVI2(vect=178))
void Excep_TMR2_OVI2(void);

// TMR3 CMIA3
#pragma interrupt (Excep_TMR3_CMIA3(vect=179))
void Excep_TMR3_CMIA3(void);

// TMR3 CMIB3
#pragma interrupt (Excep_TMR3_CMIB3(vect=180))
void Excep_TMR3_CMIB3(void);

// TMR3 OVI3
#pragma interrupt (Excep_TMR3_OVI3(vect=181))
void Excep_TMR3_OVI3(void);

// RIIC0 EEI0
#pragma interrupt (Excep_RIIC0_EEI0(vect=182))
void Excep_RIIC0_EEI0(void);

// RIIC0 RXI0
#pragma interrupt (Excep_RIIC0_RXI0(vect=183))
void Excep_RIIC0_RXI0(void);

// RIIC0 TXI0
#pragma interrupt (Excep_RIIC0_TXI0(vect=184))
void Excep_RIIC0_TXI0(void);

// RIIC0 TEI0
#pragma interrupt (Excep_RIIC0_TEI0(vect=185))
void Excep_RIIC0_TEI0(void);

// RIIC1 EEI1
#pragma interrupt (Excep_RIIC1_EEI1(vect=186))
void Excep_RIIC1_EEI1(void);

// RIIC1 RXI1
#pragma interrupt (Excep_RIIC1_RXI1(vect=187))
void Excep_RIIC1_RXI1(void);

// RIIC1 TXI1
#pragma interrupt (Excep_RIIC1_TXI1(vect=188))
void Excep_RIIC1_TXI1(void);

// RIIC1 TEI1
#pragma interrupt (Excep_RIIC1_TEI1(vect=189))
void Excep_RIIC1_TEI1(void);

// RIIC2 EEI2
#pragma interrupt (Excep_RIIC2_EEI2(vect=190))
void Excep_RIIC2_EEI2(void);

// RIIC2 RXI2
#pragma interrupt (Excep_RIIC2_RXI2(vect=191))
void Excep_RIIC2_RXI2(void);

// RIIC2 TXI2
#pragma interrupt (Excep_RIIC2_TXI2(vect=192))
void Excep_RIIC2_TXI2(void);

// RIIC2 TEI2
#pragma interrupt (Excep_RIIC2_TEI2(vect=193))
void Excep_RIIC2_TEI2(void);

// RIIC3 EEI3
#pragma interrupt (Excep_RIIC3_EEI3(vect=194))
void Excep_RIIC3_EEI3(void);

// RIIC3 RXI3
#pragma interrupt (Excep_RIIC3_RXI3(vect=195))
void Excep_RIIC3_RXI3(void);

// RIIC3 TXI3
#pragma interrupt (Excep_RIIC3_TXI3(vect=196))
void Excep_RIIC3_TXI3(void);

// RIIC3 TEI3
#pragma interrupt (Excep_RIIC3_TEI3(vect=197))
void Excep_RIIC3_TEI3(void);

// DMAC DMAC0I
#pragma interrupt (Excep_DMAC_DMAC0I(vect=198))
void Excep_DMAC_DMAC0I(void);

// DMAC DMAC1I
#pragma interrupt (Excep_DMAC_DMAC1I(vect=199))
void Excep_DMAC_DMAC1I(void);

// DMAC DMAC2I
#pragma interrupt (Excep_DMAC_DMAC2I(vect=200))
void Excep_DMAC_DMAC2I(void);

// DMAC DMAC3I
#pragma interrupt (Excep_DMAC_DMAC3I(vect=201))
void Excep_DMAC_DMAC3I(void);

// EXDMAC EXDMAC0I
#pragma interrupt (Excep_EXDMAC_EXDMAC0I(vect=202))
void Excep_EXDMAC_EXDMAC0I(void);

// EXDMAC EXDMAC1I
#pragma interrupt (Excep_EXDMAC_EXDMAC1I(vect=203))
void Excep_EXDMAC_EXDMAC1I(void);

// DEU DEU0
#pragma interrupt (Excep_DEU_DEU0(vect=206))
void Excep_DEU_DEU0(void);

// DEU DEU1
#pragma interrupt (Excep_DEU_DEU1(vect=207))
void Excep_DEU_DEU1(void);

// PDC PCDFI
#pragma interrupt (Excep_PDC_PCDFI(vect=208))
void Excep_PDC_PCDFI(void);

// PDC PCFEI
#pragma interrupt (Excep_PDC_PCFEI(vect=209))
void Excep_PDC_PCFEI(void);

// PDC PCERI
#pragma interrupt (Excep_PDC_PCERI(vect=210))
void Excep_PDC_PCERI(void);

// SCI0 RXI0
#pragma interrupt (Excep_SCI0_RXI0(vect=214))
void Excep_SCI0_RXI0(void);

// SCI0 TXI0
#pragma interrupt (Excep_SCI0_TXI0(vect=215))
void Excep_SCI0_TXI0(void);

// SCI0 TEI0
#pragma interrupt (Excep_SCI0_TEI0(vect=216))
void Excep_SCI0_TEI0(void);

// SCI1 RXI1
#pragma interrupt (Excep_SCI1_RXI1(vect=217))
void Excep_SCI1_RXI1(void);

// SCI1 TXI1
#pragma interrupt (Excep_SCI1_TXI1(vect=218))
void Excep_SCI1_TXI1(void);

// SCI1 TEI1
#pragma interrupt (Excep_SCI1_TEI1(vect=219))
void Excep_SCI1_TEI1(void);

// SCI2 RXI2
#pragma interrupt (Excep_SCI2_RXI2(vect=220))
void Excep_SCI2_RXI2(void);

// SCI2 TXI2
#pragma interrupt (Excep_SCI2_TXI2(vect=221))
void Excep_SCI2_TXI2(void);

// SCI2 TEI2
#pragma interrupt (Excep_SCI2_TEI2(vect=222))
void Excep_SCI2_TEI2(void);

// SCI3 RXI3
#pragma interrupt (Excep_SCI3_RXI3(vect=223))
void Excep_SCI3_RXI3(void);

// SCI3 TXI3
#pragma interrupt (Excep_SCI3_TXI3(vect=224))
void Excep_SCI3_TXI3(void);

// SCI3 TEI3
#pragma interrupt (Excep_SCI3_TEI3(vect=225))
void Excep_SCI3_TEI3(void);

// SCI4 RXI4
#pragma interrupt (Excep_SCI4_RXI4(vect=226))
void Excep_SCI4_RXI4(void);

// SCI4 TXI4
#pragma interrupt (Excep_SCI4_TXI4(vect=227))
void Excep_SCI4_TXI4(void);

// SCI4 TEI4
#pragma interrupt (Excep_SCI4_TEI4(vect=228))
void Excep_SCI4_TEI4(void);

// SCI5 RXI5
#pragma interrupt (Excep_SCI5_RXI5(vect=229))
void Excep_SCI5_RXI5(void);

// SCI5 TXI5
#pragma interrupt (Excep_SCI5_TXI5(vect=230))
void Excep_SCI5_TXI5(void);

// SCI5 TEI5
#pragma interrupt (Excep_SCI5_TEI5(vect=231))
void Excep_SCI5_TEI5(void);

// SCI6 RXI6
#pragma interrupt (Excep_SCI6_RXI6(vect=232))
void Excep_SCI6_RXI6(void);

// SCI6 TXI6
#pragma interrupt (Excep_SCI6_TXI6(vect=233))
void Excep_SCI6_TXI6(void);

// SCI6 TEI6
#pragma interrupt (Excep_SCI6_TEI6(vect=234))
void Excep_SCI6_TEI6(void);

// SCI7 RXI7
#pragma interrupt (Excep_SCI7_RXI7(vect=235))
void Excep_SCI7_RXI7(void);

// SCI7 TXI7
#pragma interrupt (Excep_SCI7_TXI7(vect=236))
void Excep_SCI7_TXI7(void);

// SCI7 TEI7
#pragma interrupt (Excep_SCI7_TEI7(vect=237))
void Excep_SCI7_TEI7(void);

// SCI8 RXI8
#pragma interrupt (Excep_SCI8_RXI8(vect=238))
void Excep_SCI8_RXI8(void);

// SCI8 TXI8
#pragma interrupt (Excep_SCI8_TXI8(vect=239))
void Excep_SCI8_TXI8(void);

// SCI8 TEI8
#pragma interrupt (Excep_SCI8_TEI8(vect=240))
void Excep_SCI8_TEI8(void);

// SCI9 RXI9
#pragma interrupt (Excep_SCI9_RXI9(vect=241))
void Excep_SCI9_RXI9(void);

// SCI9 TXI9
#pragma interrupt (Excep_SCI9_TXI9(vect=242))
void Excep_SCI9_TXI9(void);

// SCI9 TEI9
#pragma interrupt (Excep_SCI9_TEI9(vect=243))
void Excep_SCI9_TEI9(void);

// SCI10 RXI10
#pragma interrupt (Excep_SCI10_RXI10(vect=244))
void Excep_SCI10_RXI10(void);

// SCI10 TXI10
#pragma interrupt (Excep_SCI10_TXI10(vect=245))
void Excep_SCI10_TXI10(void);

// SCI10 TEI10
#pragma interrupt (Excep_SCI10_TEI10(vect=246))
void Excep_SCI10_TEI10(void);

// SCI11 RXI11
#pragma interrupt (Excep_SCI11_RXI11(vect=247))
void Excep_SCI11_RXI11(void);

// SCI11 TXI11
#pragma interrupt (Excep_SCI11_TXI11(vect=248))
void Excep_SCI11_TXI11(void);

// SCI11 TEI11
#pragma interrupt (Excep_SCI11_TEI11(vect=249))
void Excep_SCI11_TEI11(void);

// SCI12 RXI12
#pragma interrupt (Excep_SCI12_RXI12(vect=250))
void Excep_SCI12_RXI12(void);

// SCI12 TXI12
#pragma interrupt (Excep_SCI12_TXI12(vect=251))
void Excep_SCI12_TXI12(void);

// SCI12 TEI12
#pragma interrupt (Excep_SCI12_TEI12(vect=252))
void Excep_SCI12_TEI12(void);

// IEB IEBINT
#pragma interrupt (Excep_IEB_IEBINT(vect=253))
void Excep_IEB_IEBINT(void);

//;<<VECTOR DATA START (POWER ON RESET)>>
//;Power On Reset PC
extern void PowerON_Reset_PC(void);                                                                                                                
//;<<VECTOR DATA END (POWER ON RESET)>>

