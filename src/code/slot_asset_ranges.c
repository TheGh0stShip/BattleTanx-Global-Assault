/* RODATA_VRAM 0x80075F60 */
typedef int s32;
typedef unsigned int u32;
typedef unsigned char u8;
extern s32 D_80117EB4;
extern u8 D_B03F9B60[];
extern u8 D_B03FCCB0[];
extern u8 D_B03FD8A7[];
extern u8 D_B03FD8A8[];
extern u8 D_B03FDA6A[];
extern u8 D_B03FDA70[];
extern u8 D_B03FEA2A[];
extern u8 D_B03FEA30[];
extern u8 D_B040259E[];
extern u8 D_B04025A0[];
extern u8 D_B0402B16[];
extern u8 D_B0402B18[];
extern u8 D_B0403929[];
extern u8 D_B0403930[];
extern u8 D_B0405C4C[];
extern u8 D_B0405C50[];
extern u8 D_B04077E2[];
extern u8 D_B04077E8[];
extern u8 D_B0409019[];
extern u8 D_B0409020[];
extern u8 D_B040A178[];
extern u8 D_B040E945[];
extern u8 D_B040E948[];
extern u8 D_B040FAF8[];
extern u8 D_B04107F4[];
extern u8 D_B04107F8[];
extern u8 D_B0415096[];
extern u8 D_B0415098[];
extern u8 D_B04162A0[];
extern u8 D_B0417227[];
extern u8 D_B0417228[];
extern u8 D_B0417670[];
extern u8 D_B0419A6B[];
extern u8 D_B0419A70[];
extern u8 D_B041AE90[];
extern u8 D_B041B95B[];
extern u8 D_B041B960[];
extern u8 D_B041BA29[];
extern u8 D_B041BA30[];
extern u8 D_B0420708[];
extern u8 D_B04223AA[];
extern u8 D_B04223B0[];
extern u8 D_B04232B4[];
extern u8 D_B04232B8[];
extern u8 D_B0424A72[];
extern u8 D_B0424A78[];
extern u8 D_B04266F4[];
extern u8 D_B04266F8[];
extern u8 D_B0426D32[];
extern u8 D_B0426D38[];
extern u8 D_B042AD5E[];
extern u8 D_B042AD60[];
extern u8 D_B042B853[];
extern u8 D_B042B858[];
extern u8 D_B042C4FD[];
extern u8 D_B042C500[];
extern u8 D_B043096D[];
extern u8 D_B0430970[];
extern u8 D_B0431B56[];
extern u8 D_B0431B58[];
extern u8 D_B0432758[];
extern u8 D_B04362D8[];
extern u8 D_B04380D7[];
extern u8 D_B04380D8[];
extern u8 D_B04394F5[];
extern u8 D_B04394F8[];
extern u8 D_B043DE28[];
extern u8 D_B043F98A[];
extern u8 D_B043F990[];
extern u8 D_B0440BAB[];
extern u8 D_B0440BB0[];
extern u8 D_B0443F61[];
extern u8 D_B0443F68[];
extern u8 D_B04458F9[];
extern u8 D_B0445900[];
extern u8 D_B044652D[];
extern u8 D_B0446530[];
extern u8 D_B0448DAA[];
extern u8 D_B0448DB0[];
extern u8 D_B0449C3A[];
extern u8 D_B0449C40[];
extern u8 D_B044ABAF[];
extern u8 D_B044ABB0[];
extern u8 D_B044D267[];
extern u8 D_B044D268[];
extern u8 D_B044EDCE[];
extern u8 D_B044EDD0[];
extern u8 D_B044FF18[];
extern u8 D_B045031D[];
extern u8 D_B0450320[];
extern u8 D_B0454AA7[];
extern u8 D_B0454AA8[];
extern u8 D_B04556C0[];
extern u8 D_B0456696[];
extern u8 D_B0456698[];
extern u8 D_B0459984[];
extern u8 D_B0459988[];
extern u8 D_B045A821[];
extern u8 D_B045A828[];
extern u8 D_B045B5E1[];
extern u8 D_B045B5E8[];
extern u8 D_B045EB00[];
extern u8 D_B045F54D[];
extern u8 D_B045F550[];
extern u8 D_B0462571[];
extern u8 D_B0462578[];
extern u8 D_B0463012[];
extern u8 D_B0463018[];
extern u8 D_B0465733[];
extern u8 D_B0465738[];
extern u8 D_B046662B[];
extern u8 D_B0466630[];
extern u8 D_B0468950[];
extern u8 D_B0469345[];
extern u8 D_B0469348[];
extern u8 D_B046A854[];
extern u8 D_B046A858[];
extern u8 D_B046B170[];
extern u8 D_B046B4EA[];
extern u8 D_B046B4F0[];
extern u8 D_B046B4FE[];
extern u8 D_B046B500[];
extern u8 D_B046B54A[];
extern u8 D_B046B550[];
extern u8 D_B046B6A4[];
extern u8 D_B046B6A8[];
extern u8 D_B046CE5C[];
extern u8 D_B046CE60[];
extern u8 D_B046E821[];
extern u8 D_B046E828[];
extern u8 D_B046ED1A[];
extern u8 D_B046ED20[];
extern u8 D_B046F652[];

s32 func_800E8380(u32 arg0, s32 a1, u32 *a2, u32 *a3) {
    switch (arg0) {
    case 0:
        a2[0] = (u32)D_B03F9B60;
        a3[0] = (u32)D_B03FCCB0;
        if (a1) {
            a2[1] = (u32)(D_B046F652 - 0x729A2);
            a3[1] = (u32)D_B03FD8A7;
            goto ret2;
        }
        if (D_80117EB4 == 10) {
            a2[1] = (u32)D_B03FDA70;
            a3[1] = (u32)D_B03FEA2A;
            goto ret2;
        }
        a2[1] = (u32)D_B03FD8A8;
        a3[1] = (u32)D_B03FDA6A;
        goto ret2;
    case 1:
        a2[0] = (u32)D_B03FEA30;
        a3[0] = (u32)D_B040259E;
        if (a1) {
            a2[1] = (u32)D_B04025A0;
            a3[1] = (u32)D_B0402B16;
            goto ret2;
        }
        a2[1] = (u32)D_B0402B18;
        a3[1] = (u32)D_B0403929;
        goto ret2;
    case 2:
        a2[0] = (u32)D_B0403930;
        a3[0] = (u32)D_B0405C4C;
        if (a1) {
            a2[1] = (u32)D_B0405C50;
            a3[1] = (u32)D_B04077E2;
            goto ret2;
        }
        if (D_80117EB4 == 10) {
            a2[1] = (u32)D_B0409020;
            a3[1] = (u32)D_B040A178;
            goto ret2;
        }
        a2[1] = (u32)D_B04077E8;
        a3[1] = (u32)D_B0409019;
        goto ret2;
    case 3:
        a2[0] = (u32)(D_B046ED20 - 0x64BA8);
        a3[0] = (u32)D_B040E945;
        if (a1) {
            a2[1] = (u32)D_B040E948;
            a3[1] = (u32)D_B040FAF8;
            goto ret2;
        }
        a2[1] = (u32)(D_B046ED1A - 0x5F222);
        a3[1] = (u32)D_B04107F4;
        goto ret2;
    case 4:
        a2[0] = (u32)D_B04107F8;
        a3[0] = (u32)D_B0415096;
        if (a1) {
            a2[1] = (u32)D_B0415098;
            a3[1] = (u32)D_B04162A0;
            goto ret2;
        }
        if (D_80117EB4 == 10) {
            a2[1] = (u32)D_B0417228;
            a3[1] = (u32)D_B0417670;
            goto ret2;
        }
        a2[1] = (u32)(D_B046E828 - 0x58588);
        a3[1] = (u32)D_B0417227;
        goto ret2;
    case 5:
        a2[0] = (u32)(D_B046E821 - 0x571B1);
        a3[0] = (u32)D_B0419A6B;
        if (a1) {
            a2[1] = (u32)D_B0419A70;
            a3[1] = (u32)D_B041AE90;
            goto ret2;
        }
        if (D_80117EB4 == 10) {
            a2[1] = (u32)D_B041B960;
            a3[1] = (u32)D_B041BA29;
            goto ret2;
        }
        a2[1] = (u32)(D_B046CE60 - 0x51FD0);
        a3[1] = (u32)D_B041B95B;
        goto ret2;
    case 6:
        a2[0] = (u32)D_B041BA30;
        a3[0] = (u32)D_B0420708;
        if (a1) {
            a2[1] = (u32)(D_B046CE5C - 0x4C754);
            a3[1] = (u32)D_B04223AA;
            goto ret2;
        }
        a2[1] = (u32)D_B04223B0;
        a3[1] = (u32)D_B04232B4;
        goto ret2;
    case 7:
        a2[0] = (u32)D_B04232B8;
        a3[0] = (u32)D_B0424A72;
        if (a1) {
            a2[1] = (u32)D_B0424A78;
            a3[1] = (u32)D_B04266F4;
            goto ret2;
        }
        a2[1] = (u32)D_B04266F8;
        a3[1] = (u32)D_B0426D32;
        goto ret2;
    case 8:
        a2[0] = (u32)D_B0426D38;
        a3[0] = (u32)D_B042AD5E;
        if (a1) {
            a2[1] = (u32)D_B042AD60;
            a3[1] = (u32)D_B042B853;
            goto ret2;
        }
        a2[1] = (u32)D_B042B858;
        a3[1] = (u32)D_B042C4FD;
        goto ret2;
    case 9:
        a2[0] = (u32)D_B042C500;
        a3[0] = (u32)D_B043096D;
        if (a1) {
            a2[1] = (u32)D_B0430970;
            a3[1] = (u32)D_B0431B56;
            goto ret2;
        }
        a2[1] = (u32)D_B0431B58;
        a3[1] = (u32)D_B0432758;
        goto ret2;
    case 10:
        a2[0] = (u32)(D_B046B6A8 - 0x38F50);
        a3[0] = (u32)D_B04362D8;
        if (a1) {
            a2[1] = (u32)(D_B03F9B60 + 0x3C778);
            a3[1] = (u32)D_B04380D7;
            goto ret2;
        }
        a2[1] = (u32)D_B04380D8;
        a3[1] = (u32)D_B04394F5;
        goto ret2;
    case 11:
        a2[0] = (u32)D_B04394F8;
        a3[0] = (u32)D_B043DE28;
        if (a1) {
            a2[1] = (u32)(D_B03FCCB0 + 0x41178);
            a3[1] = (u32)D_B043F98A;
            goto ret2;
        }
        a2[1] = (u32)D_B043F990;
        a3[1] = (u32)D_B0440BAB;
        goto ret2;
    case 12:
        a2[0] = (u32)D_B0440BB0;
        a3[0] = (u32)D_B0443F61;
        if (a1) {
            a2[1] = (u32)D_B0443F68;
            a3[1] = (u32)D_B04458F9;
            goto ret2;
        }
        a2[1] = (u32)D_B0445900;
        a3[1] = (u32)D_B044652D;
        goto ret2;
    case 13:
        a2[0] = (u32)D_B0446530;
        a3[0] = (u32)D_B0448DAA;
        if (a1) {
            a2[1] = (u32)D_B0448DB0;
            a3[1] = (u32)D_B0449C3A;
            goto ret2;
        }
        a2[1] = (u32)D_B0449C40;
        a3[1] = (u32)D_B044ABAF;
        goto ret2;
    case 14:
        a2[0] = (u32)D_B044ABB0;
        a3[0] = (u32)D_B044D267;
        if (a1) {
            a2[1] = (u32)D_B044D268;
            a3[1] = (u32)D_B044EDCE;
            goto ret2;
        }
        if (D_80117EB4 == 10) {
            a2[1] = (u32)D_B044FF18;
            a3[1] = (u32)D_B045031D;
            goto ret2;
        }
        a2[1] = (u32)D_B044EDD0;
        a3[1] = (u32)(D_B03FD8A7 + 0x52671);
        goto ret2;
    case 15:
        a2[0] = (u32)D_B0450320;
        a3[0] = (u32)D_B0454AA7;
        if (a1) {
            a2[1] = (u32)D_B0454AA8;
            a3[1] = (u32)D_B04556C0;
            goto ret2;
        }
        a2[1] = (u32)(D_B03FD8A8 + 0x57E18);
        a3[1] = (u32)D_B0456696;
        goto ret2;
    case 26:
        a2[0] = (u32)D_B046CE60;
        a3[0] = (u32)D_B046E821;
        if (a1) {
            a2[1] = (u32)D_B046E828;
            a3[1] = (u32)D_B046ED1A;
            goto ret2;
        }
        a2[1] = (u32)D_B046ED20;
        a3[1] = (u32)D_B046F652;
        goto ret2;
    case 16:
        a2[0] = (u32)D_B0456698;
        a3[0] = (u32)D_B0459984;
        if (a1) {
            a2[1] = (u32)D_B0459988;
            a3[1] = (u32)D_B045A821;
            goto ret2;
        }
        a2[1] = (u32)D_B045A828;
        a3[1] = (u32)D_B045B5E1;
        ret2:
        return 2;
    case 17:
        a2[0] = (u32)D_B045B5E8;
        a3[0] = (u32)D_B045EB00;
        if (!a1) {
            a2[1] = (u32)(D_B03FDA6A + 0x61096);
            a3[1] = (u32)D_B045F54D;
            return 2;
        }
        /* fallthrough */
    case 18:
        a2[0] = (u32)D_B045F550;
        a3[0] = (u32)D_B0462571;
        if (!a1) {
            a2[1] = (u32)D_B0462578;
            a3[1] = (u32)D_B0463012;
            return 2;
        }
        /* fallthrough */
    case 19:
        a2[0] = (u32)D_B0463018;
        a3[0] = (u32)D_B0465733;
        if (!a1) {
            a2[1] = (u32)D_B0465738;
            a3[1] = (u32)D_B046662B;
            return 2;
        }
        /* fallthrough */
    case 20:
        a2[0] = (u32)D_B0466630;
        a3[0] = (u32)D_B0468950;
        if (!a1) {
            a2[1] = (u32)(D_B03FDA70 + 0x6AEE0);
            a3[1] = (u32)D_B0469345;
            return 2;
        }
        /* fallthrough */
    case 21:
        a2[0] = (u32)D_B0469348;
        a3[0] = (u32)D_B046A854;
        if (!a1) {
            a2[1] = (u32)D_B046A858;
            a3[1] = (u32)D_B046B170;
            return 2;
        }
        /* fallthrough */
    case 22:
        a2[0] = (u32)(D_B03FEA2A + 0x6C746);
        a3[0] = (u32)D_B046B4EA;
        if (!a1) {
            a2[1] = (u32)D_B046B4F0;
            a3[1] = (u32)D_B046B4FE;
            return 2;
        }
        /* fallthrough */
    case 23:
        a2[0] = (u32)D_B046B500;
        a3[0] = (u32)D_B046B54A;
        if (!a1) {
            a2[1] = (u32)D_B046B550;
            a3[1] = (u32)D_B046B6A4;
            return 2;
        }
        /* fallthrough */
    case 24:
        a2[0] = (u32)(D_B046B6A4 - 0x71B44);
        a3[0] = (u32)(D_B046B550 - 0x6E8A0);
        if (a1) {
            a2[1] = (u32)D_B046B6A8;
            a3[1] = (u32)D_B046CE5C;
            return 2;
        }
        /* fallthrough */
    case 25:
        a2[0] = (u32)(D_B03FEA30 + 0x6CB20);
        a3[0] = (u32)(D_B040259E + 0x69106);
        return 1;
    default:
        return 0;
    }

}
