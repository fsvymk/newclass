#!/usr/bin/env python
# -*- coding: utf-8 -*-

#include <names.h>
#include “kpa_config.h”

#define HW_SEQ_MKT_2    0x0002
#define HW_SEQ_CTRL_ROOT    0x0004

HW_SEQUENCE (HW_SEQ_CTRL_ROOT) {
    KPA_2M2C_0:0,
    KPA_IP1_1:2,
    KPA_IP1_1:3,
    KPA_IP1_1:4,
    @HW_SEQ_MKT_2      ; HW_SEQUENCE(HW_SEQ_MKT_2) becomes the part of the
                                              ; current  sequence
};

HW_SEQUENCE (HW_SEQ_MKT_2) {
    KPA_MKT_0:0,
    KPA_MKT_0:1,
    KPA_MKT_0:2,
    KPA_MKT_0:3,
    KPA_MKT_0:4,
    KPA_MKT_0:5,
    KPA_MKT_1:2,
    KPA_MKT_1:3,
    KPA_MKT_1:4
};



#define IP1_UPDATE EVENT_L00

module  (  KPA_IP1  ,  SEQ_CTRL_ROOT    )       {




        procedure ( IP1_UPDATE) {
                A := Ctrl;
                Ctrl := A & 0xFFFE;
                rg[11] := Vx + dVx;
                rg[10] := Vx - dVx;
                rg[1] := 0xA710;
                Ctrl := 0x0002 | iif ( A & 0x0020, A | 0x8000, A);
        }
}

module(KPA_IP2, HW_SEQ_CTRL){VAR_EVENTS{Ctrl ::: IP1_UPDATE}}

module	(	KPA_IP3	, 		 HW_CTRL_ROOT	)	 {
        uword Vx : rg:7, dVx:rg:8;
        uword Ctrl:port:0;
        word A, B, C;
}

module (KPA_IP4, HW_SEQ_CTRL_ROOT) {
        uword Vx : rg:7, dVx:rg:8;
        uword Ctrl:port:0;
        word A, B, C;

        VAR_EVENTS {
                Ctrl ::: IP1_UPDATE
        }

        procedure ( IP1_UPDATE) {
                A := Ctrl;
                Ctrl := A & 0xFFFE;
                rg[11] := Vx + dVx;
                rg[10] := Vx - dVx;
                rg[1] := 0xA710;
                Ctrl := 0x0002 | iif ( A & 0x0020, A | 0x8000, A);
        }

        procedure Run (EVENT_GRUN) {
                Ctrl := 0;
                Vx := 0;
                dVx := 0;
                B := 0;
                while (1) {
                        rg[3+B] := 0;
                        if (B > 5)
                                break;
                        else
                                B := B + 1;
                        if (Ctrl & 0x0200) {
                                local_send_event (IP1_UPDATE);
                                end_proc ();
                        }
                }
                debug_prnt ("Module IP1 was run at %02d:%02d", m_pos, m_channel);
        }
}
