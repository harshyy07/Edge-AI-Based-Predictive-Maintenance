// Auto-generated TinyML Random Forest from MaFaulDA dataset
#define RF_N_TREES 10
#define RF_N_CLASSES 5
#define RF_N_FEATURES 12

static void rf_tree_0(const float *x, float *proba) {
  node_0: if (x[8] <= 63844.738281f) goto node_1; else goto node_46;
  node_1: if (x[9] <= 2707.222290f) goto node_2; else goto node_31;
  node_2: if (x[8] <= 9819.092285f) goto node_3; else goto node_18;
  node_3: if (x[6] <= 22094.726562f) goto node_4; else goto node_11;
  node_4: if (x[9] <= 1077.622925f) goto node_5; else goto node_8;
  node_5: if (x[6] <= 22045.898438f) goto node_6; else goto node_7;
  node_6: { proba[0] += 1.0000f; proba[1] += 0.0000f; proba[2] += 0.0000f; proba[3] += 0.0000f; proba[4] += 0.0000f; return; }
  node_7: { proba[0] += 0.0000f; proba[1] += 1.0000f; proba[2] += 0.0000f; proba[3] += 0.0000f; proba[4] += 0.0000f; return; }
  node_8: if (x[6] <= 22045.898438f) goto node_9; else goto node_10;
  node_9: { proba[0] += 1.0000f; proba[1] += 0.0000f; proba[2] += 0.0000f; proba[3] += 0.0000f; proba[4] += 0.0000f; return; }
  node_10: { proba[0] += 0.0000f; proba[1] += 0.9863f; proba[2] += 0.0000f; proba[3] += 0.0137f; proba[4] += 0.0000f; return; }
  node_11: if (x[2] <= 2.474633f) goto node_12; else goto node_15;
  node_12: if (x[1] <= 2.746250f) goto node_13; else goto node_14;
  node_13: { proba[0] += 0.0000f; proba[1] += 0.0000f; proba[2] += 0.9211f; proba[3] += 0.0789f; proba[4] += 0.0000f; return; }
  node_14: { proba[0] += 0.0000f; proba[1] += 0.0000f; proba[2] += 0.5172f; proba[3] += 0.4828f; proba[4] += 0.0000f; return; }
  node_15: if (x[3] <= 0.062168f) goto node_16; else goto node_17;
  node_16: { proba[0] += 0.0000f; proba[1] += 0.0000f; proba[2] += 0.9839f; proba[3] += 0.0161f; proba[4] += 0.0000f; return; }
  node_17: { proba[0] += 0.0000f; proba[1] += 0.0000f; proba[2] += 0.0000f; proba[3] += 1.0000f; proba[4] += 0.0000f; return; }
  node_18: if (x[9] <= 1590.675415f) goto node_19; else goto node_24;
  node_19: if (x[2] <= 2.486593f) goto node_20; else goto node_21;
  node_20: { proba[0] += 1.0000f; proba[1] += 0.0000f; proba[2] += 0.0000f; proba[3] += 0.0000f; proba[4] += 0.0000f; return; }
  node_21: if (x[11] <= 2850.098633f) goto node_22; else goto node_23;
  node_22: { proba[0] += 0.0000f; proba[1] += 0.0000f; proba[2] += 1.0000f; proba[3] += 0.0000f; proba[4] += 0.0000f; return; }
  node_23: { proba[0] += 0.1250f; proba[1] += 0.6250f; proba[2] += 0.2500f; proba[3] += 0.0000f; proba[4] += 0.0000f; return; }
  node_24: if (x[0] <= 1.200043f) goto node_25; else goto node_28;
  node_25: if (x[9] <= 1820.510437f) goto node_26; else goto node_27;
  node_26: { proba[0] += 0.5000f; proba[1] += 0.0000f; proba[2] += 0.5000f; proba[3] += 0.0000f; proba[4] += 0.0000f; return; }
  node_27: { proba[0] += 0.0741f; proba[1] += 0.6667f; proba[2] += 0.1481f; proba[3] += 0.1111f; proba[4] += 0.0000f; return; }
  node_28: if (x[7] <= 90527.351562f) goto node_29; else goto node_30;
  node_29: { proba[0] += 0.6143f; proba[1] += 0.3143f; proba[2] += 0.0571f; proba[3] += 0.0143f; proba[4] += 0.0000f; return; }
  node_30: { proba[0] += 0.8256f; proba[1] += 0.0930f; proba[2] += 0.0233f; proba[3] += 0.0581f; proba[4] += 0.0000f; return; }
  node_31: if (x[6] <= 22045.898438f) goto node_32; else goto node_33;
  node_32: { proba[0] += 1.0000f; proba[1] += 0.0000f; proba[2] += 0.0000f; proba[3] += 0.0000f; proba[4] += 0.0000f; return; }
  node_33: if (x[9] <= 3658.815308f) goto node_34; else goto node_41;
  node_34: if (x[6] <= 22094.726562f) goto node_35; else goto node_38;
  node_35: if (x[1] <= 2.423500f) goto node_36; else goto node_37;
  node_36: { proba[0] += 0.0000f; proba[1] += 0.0000f; proba[2] += 0.0000f; proba[3] += 1.0000f; proba[4] += 0.0000f; return; }
  node_37: { proba[0] += 0.0000f; proba[1] += 0.8933f; proba[2] += 0.0000f; proba[3] += 0.1067f; proba[4] += 0.0000f; return; }
  node_38: if (x[11] <= 2383.503784f) goto node_39; else goto node_40;
  node_39: { proba[0] += 0.0000f; proba[1] += 0.0000f; proba[2] += 0.0160f; proba[3] += 0.9840f; proba[4] += 0.0000f; return; }
  node_40: { proba[0] += 0.0000f; proba[1] += 0.0000f; proba[2] += 0.1250f; proba[3] += 0.8750f; proba[4] += 0.0000f; return; }
  node_41: if (x[11] <= 2576.644531f) goto node_42; else goto node_43;
  node_42: { proba[0] += 0.0000f; proba[1] += 0.0000f; proba[2] += 0.0000f; proba[3] += 1.0000f; proba[4] += 0.0000f; return; }
  node_43: if (x[3] <= -0.792699f) goto node_44; else goto node_45;
  node_44: { proba[0] += 0.0000f; proba[1] += 0.3636f; proba[2] += 0.0000f; proba[3] += 0.6364f; proba[4] += 0.0000f; return; }
  node_45: { proba[0] += 0.0000f; proba[1] += 0.0000f; proba[2] += 0.0000f; proba[3] += 1.0000f; proba[4] += 0.0000f; return; }
  node_46: { proba[0] += 0.0000f; proba[1] += 0.0000f; proba[2] += 0.0000f; proba[3] += 0.0000f; proba[4] += 1.0000f; return; }
}

static void rf_tree_1(const float *x, float *proba) {
  node_0: if (x[5] <= 1.904341f) goto node_1; else goto node_50;
  node_1: if (x[9] <= 2649.309814f) goto node_2; else goto node_27;
  node_2: if (x[6] <= 22094.726562f) goto node_3; else goto node_12;
  node_3: if (x[6] <= 22045.898438f) goto node_4; else goto node_5;
  node_4: { proba[0] += 1.0000f; proba[1] += 0.0000f; proba[2] += 0.0000f; proba[3] += 0.0000f; proba[4] += 0.0000f; return; }
  node_5: if (x[2] <= 2.100445f) goto node_6; else goto node_9;
  node_6: if (x[2] <= 2.094513f) goto node_7; else goto node_8;
  node_7: { proba[0] += 0.0000f; proba[1] += 1.0000f; proba[2] += 0.0000f; proba[3] += 0.0000f; proba[4] += 0.0000f; return; }
  node_8: { proba[0] += 0.0000f; proba[1] += 0.0000f; proba[2] += 0.0000f; proba[3] += 1.0000f; proba[4] += 0.0000f; return; }
  node_9: if (x[9] <= 2636.678345f) goto node_10; else goto node_11;
  node_10: { proba[0] += 0.0000f; proba[1] += 0.9945f; proba[2] += 0.0000f; proba[3] += 0.0055f; proba[4] += 0.0000f; return; }
  node_11: { proba[0] += 0.0000f; proba[1] += 0.8571f; proba[2] += 0.0000f; proba[3] += 0.1429f; proba[4] += 0.0000f; return; }
  node_12: if (x[0] <= 1.161677f) goto node_13; else goto node_20;
  node_13: if (x[3] <= -0.875404f) goto node_14; else goto node_17;
  node_14: if (x[7] <= 195131.726562f) goto node_15; else goto node_16;
  node_15: { proba[0] += 0.0000f; proba[1] += 0.0000f; proba[2] += 0.9912f; proba[3] += 0.0088f; proba[4] += 0.0000f; return; }
  node_16: { proba[0] += 0.0000f; proba[1] += 0.0000f; proba[2] += 0.6667f; proba[3] += 0.3333f; proba[4] += 0.0000f; return; }
  node_17: if (x[3] <= -0.874498f) goto node_18; else goto node_19;
  node_18: { proba[0] += 0.0000f; proba[1] += 0.0000f; proba[2] += 0.0000f; proba[3] += 1.0000f; proba[4] += 0.0000f; return; }
  node_19: { proba[0] += 0.0000f; proba[1] += 0.0000f; proba[2] += 0.9179f; proba[3] += 0.0821f; proba[4] += 0.0000f; return; }
  node_20: if (x[10] <= 1505.374390f) goto node_21; else goto node_24;
  node_21: if (x[7] <= 83862.644531f) goto node_22; else goto node_23;
  node_22: { proba[0] += 0.0000f; proba[1] += 0.0000f; proba[2] += 0.8333f; proba[3] += 0.1667f; proba[4] += 0.0000f; return; }
  node_23: { proba[0] += 0.0000f; proba[1] += 0.0000f; proba[2] += 0.9643f; proba[3] += 0.0357f; proba[4] += 0.0000f; return; }
  node_24: if (x[1] <= 3.863250f) goto node_25; else goto node_26;
  node_25: { proba[0] += 0.0000f; proba[1] += 0.0000f; proba[2] += 0.1111f; proba[3] += 0.8889f; proba[4] += 0.0000f; return; }
  node_26: { proba[0] += 0.0000f; proba[1] += 0.0000f; proba[2] += 0.7500f; proba[3] += 0.2500f; proba[4] += 0.0000f; return; }
  node_27: if (x[11] <= 2665.532349f) goto node_28; else goto node_43;
  node_28: if (x[9] <= 2768.452637f) goto node_29; else goto node_36;
  node_29: if (x[0] <= 1.143637f) goto node_30; else goto node_33;
  node_30: if (x[10] <= 1965.062378f) goto node_31; else goto node_32;
  node_31: { proba[0] += 0.0000f; proba[1] += 0.0000f; proba[2] += 0.0000f; proba[3] += 1.0000f; proba[4] += 0.0000f; return; }
  node_32: { proba[0] += 0.0000f; proba[1] += 1.0000f; proba[2] += 0.0000f; proba[3] += 0.0000f; proba[4] += 0.0000f; return; }
  node_33: if (x[4] <= -0.430803f) goto node_34; else goto node_35;
  node_34: { proba[0] += 1.0000f; proba[1] += 0.0000f; proba[2] += 0.0000f; proba[3] += 0.0000f; proba[4] += 0.0000f; return; }
  node_35: { proba[0] += 0.3333f; proba[1] += 0.0000f; proba[2] += 0.0000f; proba[3] += 0.6667f; proba[4] += 0.0000f; return; }
  node_36: if (x[11] <= 2495.669189f) goto node_37; else goto node_40;
  node_37: if (x[11] <= 2094.960693f) goto node_38; else goto node_39;
  node_38: { proba[0] += 0.0000f; proba[1] += 0.0000f; proba[2] += 0.0039f; proba[3] += 0.9961f; proba[4] += 0.0000f; return; }
  node_39: { proba[0] += 0.0442f; proba[1] += 0.0442f; proba[2] += 0.0000f; proba[3] += 0.9115f; proba[4] += 0.0000f; return; }
  node_40: if (x[6] <= 22094.726562f) goto node_41; else goto node_42;
  node_41: { proba[0] += 0.2222f; proba[1] += 0.7778f; proba[2] += 0.0000f; proba[3] += 0.0000f; proba[4] += 0.0000f; return; }
  node_42: { proba[0] += 0.0000f; proba[1] += 0.0000f; proba[2] += 0.0000f; proba[3] += 1.0000f; proba[4] += 0.0000f; return; }
  node_43: if (x[6] <= 22045.898438f) goto node_44; else goto node_45;
  node_44: { proba[0] += 1.0000f; proba[1] += 0.0000f; proba[2] += 0.0000f; proba[3] += 0.0000f; proba[4] += 0.0000f; return; }
  node_45: if (x[11] <= 3019.680664f) goto node_46; else goto node_49;
  node_46: if (x[6] <= 22094.726562f) goto node_47; else goto node_48;
  node_47: { proba[0] += 0.0000f; proba[1] += 1.0000f; proba[2] += 0.0000f; proba[3] += 0.0000f; proba[4] += 0.0000f; return; }
  node_48: { proba[0] += 0.0000f; proba[1] += 0.0000f; proba[2] += 0.0000f; proba[3] += 1.0000f; proba[4] += 0.0000f; return; }
  node_49: { proba[0] += 0.0000f; proba[1] += 1.0000f; proba[2] += 0.0000f; proba[3] += 0.0000f; proba[4] += 0.0000f; return; }
  node_50: { proba[0] += 0.0000f; proba[1] += 0.0000f; proba[2] += 0.0000f; proba[3] += 0.0000f; proba[4] += 1.0000f; return; }
}

static void rf_tree_2(const float *x, float *proba) {
  node_0: if (x[5] <= 1.820635f) goto node_1; else goto node_52;
  node_1: if (x[5] <= 1.204024f) goto node_2; else goto node_25;
  node_2: if (x[6] <= 22094.726562f) goto node_3; else goto node_16;
  node_3: if (x[4] <= -0.231832f) goto node_4; else goto node_11;
  node_4: if (x[2] <= 2.093529f) goto node_5; else goto node_8;
  node_5: if (x[10] <= 1468.017456f) goto node_6; else goto node_7;
  node_6: { proba[0] += 1.0000f; proba[1] += 0.0000f; proba[2] += 0.0000f; proba[3] += 0.0000f; proba[4] += 0.0000f; return; }
  node_7: { proba[0] += 0.2000f; proba[1] += 0.6000f; proba[2] += 0.0000f; proba[3] += 0.2000f; proba[4] += 0.0000f; return; }
  node_8: if (x[3] <= -1.064032f) goto node_9; else goto node_10;
  node_9: { proba[0] += 0.2241f; proba[1] += 0.7759f; proba[2] += 0.0000f; proba[3] += 0.0000f; proba[4] += 0.0000f; return; }
  node_10: { proba[0] += 0.1562f; proba[1] += 0.5567f; proba[2] += 0.0000f; proba[3] += 0.2872f; proba[4] += 0.0000f; return; }
  node_11: if (x[3] <= -1.158566f) goto node_12; else goto node_13;
  node_12: { proba[0] += 1.0000f; proba[1] += 0.0000f; proba[2] += 0.0000f; proba[3] += 0.0000f; proba[4] += 0.0000f; return; }
  node_13: if (x[9] <= 3854.344727f) goto node_14; else goto node_15;
  node_14: { proba[0] += 0.1667f; proba[1] += 0.8333f; proba[2] += 0.0000f; proba[3] += 0.0000f; proba[4] += 0.0000f; return; }
  node_15: { proba[0] += 0.0000f; proba[1] += 0.0000f; proba[2] += 0.0000f; proba[3] += 1.0000f; proba[4] += 0.0000f; return; }
  node_16: if (x[0] <= 0.910659f) goto node_17; else goto node_18;
  node_17: { proba[0] += 0.0000f; proba[1] += 0.0000f; proba[2] += 1.0000f; proba[3] += 0.0000f; proba[4] += 0.0000f; return; }
  node_18: if (x[10] <= 1202.123535f) goto node_19; else goto node_22;
  node_19: if (x[8] <= 8142.869385f) goto node_20; else goto node_21;
  node_20: { proba[0] += 0.0000f; proba[1] += 0.0000f; proba[2] += 0.9111f; proba[3] += 0.0889f; proba[4] += 0.0000f; return; }
  node_21: { proba[0] += 0.0000f; proba[1] += 0.0000f; proba[2] += 0.6486f; proba[3] += 0.3514f; proba[4] += 0.0000f; return; }
  node_22: if (x[9] <= 2251.283813f) goto node_23; else goto node_24;
  node_23: { proba[0] += 0.0000f; proba[1] += 0.0000f; proba[2] += 0.9375f; proba[3] += 0.0625f; proba[4] += 0.0000f; return; }
  node_24: { proba[0] += 0.0000f; proba[1] += 0.0000f; proba[2] += 0.0493f; proba[3] += 0.9507f; proba[4] += 0.0000f; return; }
  node_25: if (x[1] <= 2.985450f) goto node_26; else goto node_39;
  node_26: if (x[7] <= 79235.957031f) goto node_27; else goto node_34;
  node_27: if (x[1] <= 2.876200f) goto node_28; else goto node_31;
  node_28: if (x[1] <= 2.603500f) goto node_29; else goto node_30;
  node_29: { proba[0] += 0.0000f; proba[1] += 1.0000f; proba[2] += 0.0000f; proba[3] += 0.0000f; proba[4] += 0.0000f; return; }
  node_30: { proba[0] += 0.9375f; proba[1] += 0.0625f; proba[2] += 0.0000f; proba[3] += 0.0000f; proba[4] += 0.0000f; return; }
  node_31: if (x[6] <= 22045.898438f) goto node_32; else goto node_33;
  node_32: { proba[0] += 1.0000f; proba[1] += 0.0000f; proba[2] += 0.0000f; proba[3] += 0.0000f; proba[4] += 0.0000f; return; }
  node_33: { proba[0] += 0.0000f; proba[1] += 0.8571f; proba[2] += 0.0000f; proba[3] += 0.1429f; proba[4] += 0.0000f; return; }
  node_34: if (x[11] <= 1965.151245f) goto node_35; else goto node_36;
  node_35: { proba[0] += 0.0000f; proba[1] += 0.0000f; proba[2] += 0.0000f; proba[3] += 1.0000f; proba[4] += 0.0000f; return; }
  node_36: if (x[4] <= -0.461822f) goto node_37; else goto node_38;
  node_37: { proba[0] += 0.7778f; proba[1] += 0.2222f; proba[2] += 0.0000f; proba[3] += 0.0000f; proba[4] += 0.0000f; return; }
  node_38: { proba[0] += 1.0000f; proba[1] += 0.0000f; proba[2] += 0.0000f; proba[3] += 0.0000f; proba[4] += 0.0000f; return; }
  node_39: if (x[10] <= 1365.090088f) goto node_40; else goto node_45;
  node_40: if (x[7] <= 153297.609375f) goto node_41; else goto node_44;
  node_41: if (x[3] <= -0.900223f) goto node_42; else goto node_43;
  node_42: { proba[0] += 0.0606f; proba[1] += 0.0606f; proba[2] += 0.7273f; proba[3] += 0.1515f; proba[4] += 0.0000f; return; }
  node_43: { proba[0] += 0.0000f; proba[1] += 0.0000f; proba[2] += 0.0000f; proba[3] += 1.0000f; proba[4] += 0.0000f; return; }
  node_44: { proba[0] += 0.0000f; proba[1] += 0.0000f; proba[2] += 1.0000f; proba[3] += 0.0000f; proba[4] += 0.0000f; return; }
  node_45: if (x[9] <= 3675.997437f) goto node_46; else goto node_49;
  node_46: if (x[3] <= -1.055807f) goto node_47; else goto node_48;
  node_47: { proba[0] += 0.7092f; proba[1] += 0.2602f; proba[2] += 0.0000f; proba[3] += 0.0306f; proba[4] += 0.0000f; return; }
  node_48: { proba[0] += 0.3595f; proba[1] += 0.3268f; proba[2] += 0.0588f; proba[3] += 0.2549f; proba[4] += 0.0000f; return; }
  node_49: if (x[11] <= 2795.149170f) goto node_50; else goto node_51;
  node_50: { proba[0] += 0.0000f; proba[1] += 0.0435f; proba[2] += 0.0000f; proba[3] += 0.9565f; proba[4] += 0.0000f; return; }
  node_51: { proba[0] += 0.2778f; proba[1] += 0.3889f; proba[2] += 0.0000f; proba[3] += 0.3333f; proba[4] += 0.0000f; return; }
  node_52: { proba[0] += 0.0000f; proba[1] += 0.0000f; proba[2] += 0.0000f; proba[3] += 0.0000f; proba[4] += 1.0000f; return; }
}

static void rf_tree_3(const float *x, float *proba) {
  node_0: if (x[5] <= 1.826228f) goto node_1; else goto node_30;
  node_1: if (x[5] <= 1.307273f) goto node_2; else goto node_17;
  node_2: if (x[6] <= 22045.898438f) goto node_3; else goto node_4;
  node_3: { proba[0] += 1.0000f; proba[1] += 0.0000f; proba[2] += 0.0000f; proba[3] += 0.0000f; proba[4] += 0.0000f; return; }
  node_4: if (x[8] <= 7530.426758f) goto node_5; else goto node_10;
  node_5: if (x[5] <= 0.819692f) goto node_6; else goto node_7;
  node_6: { proba[0] += 0.0000f; proba[1] += 0.0000f; proba[2] += 1.0000f; proba[3] += 0.0000f; proba[4] += 0.0000f; return; }
  node_7: if (x[9] <= 2650.143921f) goto node_8; else goto node_9;
  node_8: { proba[0] += 0.0000f; proba[1] += 0.4633f; proba[2] += 0.4959f; proba[3] += 0.0408f; proba[4] += 0.0000f; return; }
  node_9: { proba[0] += 0.0000f; proba[1] += 0.1944f; proba[2] += 0.0139f; proba[3] += 0.7917f; proba[4] += 0.0000f; return; }
  node_10: if (x[5] <= 1.132451f) goto node_11; else goto node_14;
  node_11: if (x[11] <= 2112.500977f) goto node_12; else goto node_13;
  node_12: { proba[0] += 0.0000f; proba[1] += 0.0556f; proba[2] += 0.1019f; proba[3] += 0.8426f; proba[4] += 0.0000f; return; }
  node_13: { proba[0] += 0.0000f; proba[1] += 0.3817f; proba[2] += 0.3664f; proba[3] += 0.2519f; proba[4] += 0.0000f; return; }
  node_14: if (x[11] <= 2766.532104f) goto node_15; else goto node_16;
  node_15: { proba[0] += 0.0000f; proba[1] += 0.1230f; proba[2] += 0.2620f; proba[3] += 0.6150f; proba[4] += 0.0000f; return; }
  node_16: { proba[0] += 0.0000f; proba[1] += 0.8592f; proba[2] += 0.0352f; proba[3] += 0.1056f; proba[4] += 0.0000f; return; }
  node_17: if (x[0] <= 1.464940f) goto node_18; else goto node_27;
  node_18: if (x[6] <= 22045.898438f) goto node_19; else goto node_20;
  node_19: { proba[0] += 1.0000f; proba[1] += 0.0000f; proba[2] += 0.0000f; proba[3] += 0.0000f; proba[4] += 0.0000f; return; }
  node_20: if (x[10] <= 1797.435425f) goto node_21; else goto node_24;
  node_21: if (x[0] <= 1.397612f) goto node_22; else goto node_23;
  node_22: { proba[0] += 0.0000f; proba[1] += 0.0000f; proba[2] += 0.1333f; proba[3] += 0.8667f; proba[4] += 0.0000f; return; }
  node_23: { proba[0] += 0.0000f; proba[1] += 0.2000f; proba[2] += 0.8000f; proba[3] += 0.0000f; proba[4] += 0.0000f; return; }
  node_24: if (x[10] <= 2173.862549f) goto node_25; else goto node_26;
  node_25: { proba[0] += 0.0000f; proba[1] += 0.6000f; proba[2] += 0.0000f; proba[3] += 0.4000f; proba[4] += 0.0000f; return; }
  node_26: { proba[0] += 0.0000f; proba[1] += 1.0000f; proba[2] += 0.0000f; proba[3] += 0.0000f; proba[4] += 0.0000f; return; }
  node_27: if (x[3] <= -1.202814f) goto node_28; else goto node_29;
  node_28: { proba[0] += 1.0000f; proba[1] += 0.0000f; proba[2] += 0.0000f; proba[3] += 0.0000f; proba[4] += 0.0000f; return; }
  node_29: { proba[0] += 0.0000f; proba[1] += 0.0000f; proba[2] += 1.0000f; proba[3] += 0.0000f; proba[4] += 0.0000f; return; }
  node_30: { proba[0] += 0.0000f; proba[1] += 0.0000f; proba[2] += 0.0000f; proba[3] += 0.0000f; proba[4] += 1.0000f; return; }
}

static void rf_tree_4(const float *x, float *proba) {
  node_0: if (x[6] <= 22045.898438f) goto node_1; else goto node_4;
  node_1: if (x[8] <= 62566.875977f) goto node_2; else goto node_3;
  node_2: { proba[0] += 1.0000f; proba[1] += 0.0000f; proba[2] += 0.0000f; proba[3] += 0.0000f; proba[4] += 0.0000f; return; }
  node_3: { proba[0] += 0.0000f; proba[1] += 0.0000f; proba[2] += 0.0000f; proba[3] += 0.0000f; proba[4] += 1.0000f; return; }
  node_4: if (x[6] <= 22094.726562f) goto node_5; else goto node_24;
  node_5: if (x[5] <= 1.159470f) goto node_6; else goto node_19;
  node_6: if (x[9] <= 3003.824097f) goto node_7; else goto node_14;
  node_7: if (x[1] <= 2.270950f) goto node_8; else goto node_11;
  node_8: if (x[2] <= 2.197649f) goto node_9; else goto node_10;
  node_9: { proba[0] += 0.0000f; proba[1] += 0.3333f; proba[2] += 0.0000f; proba[3] += 0.6667f; proba[4] += 0.0000f; return; }
  node_10: { proba[0] += 0.0000f; proba[1] += 0.8788f; proba[2] += 0.0000f; proba[3] += 0.1212f; proba[4] += 0.0000f; return; }
  node_11: if (x[5] <= 0.941307f) goto node_12; else goto node_13;
  node_12: { proba[0] += 0.0000f; proba[1] += 0.9756f; proba[2] += 0.0000f; proba[3] += 0.0244f; proba[4] += 0.0000f; return; }
  node_13: { proba[0] += 0.0000f; proba[1] += 1.0000f; proba[2] += 0.0000f; proba[3] += 0.0000f; proba[4] += 0.0000f; return; }
  node_14: if (x[9] <= 3418.587402f) goto node_15; else goto node_18;
  node_15: if (x[7] <= 130798.417969f) goto node_16; else goto node_17;
  node_16: { proba[0] += 0.0000f; proba[1] += 0.1111f; proba[2] += 0.0000f; proba[3] += 0.8889f; proba[4] += 0.0000f; return; }
  node_17: { proba[0] += 0.0000f; proba[1] += 1.0000f; proba[2] += 0.0000f; proba[3] += 0.0000f; proba[4] += 0.0000f; return; }
  node_18: { proba[0] += 0.0000f; proba[1] += 0.0000f; proba[2] += 0.0000f; proba[3] += 1.0000f; proba[4] += 0.0000f; return; }
  node_19: if (x[7] <= 150532.898438f) goto node_20; else goto node_21;
  node_20: { proba[0] += 0.0000f; proba[1] += 1.0000f; proba[2] += 0.0000f; proba[3] += 0.0000f; proba[4] += 0.0000f; return; }
  node_21: if (x[9] <= 4026.320190f) goto node_22; else goto node_23;
  node_22: { proba[0] += 0.0000f; proba[1] += 1.0000f; proba[2] += 0.0000f; proba[3] += 0.0000f; proba[4] += 0.0000f; return; }
  node_23: { proba[0] += 0.0000f; proba[1] += 0.0000f; proba[2] += 0.0000f; proba[3] += 1.0000f; proba[4] += 0.0000f; return; }
  node_24: if (x[8] <= 8306.678223f) goto node_25; else goto node_38;
  node_25: if (x[10] <= 1539.568176f) goto node_26; else goto node_33;
  node_26: if (x[0] <= 1.098082f) goto node_27; else goto node_30;
  node_27: if (x[8] <= 4999.805420f) goto node_28; else goto node_29;
  node_28: { proba[0] += 0.0000f; proba[1] += 0.0000f; proba[2] += 1.0000f; proba[3] += 0.0000f; proba[4] += 0.0000f; return; }
  node_29: { proba[0] += 0.0000f; proba[1] += 0.0000f; proba[2] += 0.8424f; proba[3] += 0.1576f; proba[4] += 0.0000f; return; }
  node_30: if (x[9] <= 2189.928223f) goto node_31; else goto node_32;
  node_31: { proba[0] += 0.0000f; proba[1] += 0.0000f; proba[2] += 0.9068f; proba[3] += 0.0932f; proba[4] += 0.0000f; return; }
  node_32: { proba[0] += 0.0000f; proba[1] += 0.0000f; proba[2] += 0.0312f; proba[3] += 0.9688f; proba[4] += 0.0000f; return; }
  node_33: if (x[4] <= -0.308394f) goto node_34; else goto node_37;
  node_34: if (x[3] <= -1.056176f) goto node_35; else goto node_36;
  node_35: { proba[0] += 0.0000f; proba[1] += 0.0000f; proba[2] += 1.0000f; proba[3] += 0.0000f; proba[4] += 0.0000f; return; }
  node_36: { proba[0] += 0.0000f; proba[1] += 0.0000f; proba[2] += 0.3333f; proba[3] += 0.6667f; proba[4] += 0.0000f; return; }
  node_37: { proba[0] += 0.0000f; proba[1] += 0.0000f; proba[2] += 0.0000f; proba[3] += 1.0000f; proba[4] += 0.0000f; return; }
  node_38: if (x[11] <= 2867.233765f) goto node_39; else goto node_44;
  node_39: if (x[5] <= 1.408376f) goto node_40; else goto node_43;
  node_40: if (x[9] <= 2251.283813f) goto node_41; else goto node_42;
  node_41: { proba[0] += 0.0000f; proba[1] += 0.0000f; proba[2] += 0.9861f; proba[3] += 0.0139f; proba[4] += 0.0000f; return; }
  node_42: { proba[0] += 0.0000f; proba[1] += 0.0000f; proba[2] += 0.0279f; proba[3] += 0.9721f; proba[4] += 0.0000f; return; }
  node_43: { proba[0] += 0.0000f; proba[1] += 0.0000f; proba[2] += 1.0000f; proba[3] += 0.0000f; proba[4] += 0.0000f; return; }
  node_44: { proba[0] += 0.0000f; proba[1] += 0.0000f; proba[2] += 1.0000f; proba[3] += 0.0000f; proba[4] += 0.0000f; return; }
}

static void rf_tree_5(const float *x, float *proba) {
  node_0: if (x[7] <= 1351973.062500f) goto node_1; else goto node_54;
  node_1: if (x[11] <= 2684.542603f) goto node_2; else goto node_27;
  node_2: if (x[9] <= 2550.548828f) goto node_3; else goto node_18;
  node_3: if (x[6] <= 22094.726562f) goto node_4; else goto node_11;
  node_4: if (x[0] <= 1.144139f) goto node_5; else goto node_8;
  node_5: if (x[3] <= -1.153514f) goto node_6; else goto node_7;
  node_6: { proba[0] += 0.7429f; proba[1] += 0.2571f; proba[2] += 0.0000f; proba[3] += 0.0000f; proba[4] += 0.0000f; return; }
  node_7: { proba[0] += 0.2023f; proba[1] += 0.7824f; proba[2] += 0.0000f; proba[3] += 0.0153f; proba[4] += 0.0000f; return; }
  node_8: if (x[6] <= 22045.898438f) goto node_9; else goto node_10;
  node_9: { proba[0] += 1.0000f; proba[1] += 0.0000f; proba[2] += 0.0000f; proba[3] += 0.0000f; proba[4] += 0.0000f; return; }
  node_10: { proba[0] += 0.0000f; proba[1] += 1.0000f; proba[2] += 0.0000f; proba[3] += 0.0000f; proba[4] += 0.0000f; return; }
  node_11: if (x[1] <= 2.966900f) goto node_12; else goto node_15;
  node_12: if (x[8] <= 6546.877686f) goto node_13; else goto node_14;
  node_13: { proba[0] += 0.0000f; proba[1] += 0.0000f; proba[2] += 0.9808f; proba[3] += 0.0192f; proba[4] += 0.0000f; return; }
  node_14: { proba[0] += 0.0000f; proba[1] += 0.0000f; proba[2] += 0.6951f; proba[3] += 0.3049f; proba[4] += 0.0000f; return; }
  node_15: if (x[9] <= 1972.931519f) goto node_16; else goto node_17;
  node_16: { proba[0] += 0.0000f; proba[1] += 0.0000f; proba[2] += 0.9874f; proba[3] += 0.0126f; proba[4] += 0.0000f; return; }
  node_17: { proba[0] += 0.0000f; proba[1] += 0.0000f; proba[2] += 0.7059f; proba[3] += 0.2941f; proba[4] += 0.0000f; return; }
  node_18: if (x[10] <= 2626.009033f) goto node_19; else goto node_26;
  node_19: if (x[0] <= 1.166202f) goto node_20; else goto node_23;
  node_20: if (x[6] <= 22045.898438f) goto node_21; else goto node_22;
  node_21: { proba[0] += 1.0000f; proba[1] += 0.0000f; proba[2] += 0.0000f; proba[3] += 0.0000f; proba[4] += 0.0000f; return; }
  node_22: { proba[0] += 0.0000f; proba[1] += 0.0072f; proba[2] += 0.0108f; proba[3] += 0.9821f; proba[4] += 0.0000f; return; }
  node_23: if (x[8] <= 6542.010742f) goto node_24; else goto node_25;
  node_24: { proba[0] += 0.0000f; proba[1] += 0.6667f; proba[2] += 0.0000f; proba[3] += 0.3333f; proba[4] += 0.0000f; return; }
  node_25: { proba[0] += 0.1280f; proba[1] += 0.0560f; proba[2] += 0.0000f; proba[3] += 0.8160f; proba[4] += 0.0000f; return; }
  node_26: { proba[0] += 1.0000f; proba[1] += 0.0000f; proba[2] += 0.0000f; proba[3] += 0.0000f; proba[4] += 0.0000f; return; }
  node_27: if (x[2] <= 2.592358f) goto node_28; else goto node_43;
  node_28: if (x[5] <= 1.169939f) goto node_29; else goto node_36;
  node_29: if (x[0] <= 1.128425f) goto node_30; else goto node_33;
  node_30: if (x[6] <= 22094.726562f) goto node_31; else goto node_32;
  node_31: { proba[0] += 0.0000f; proba[1] += 1.0000f; proba[2] += 0.0000f; proba[3] += 0.0000f; proba[4] += 0.0000f; return; }
  node_32: { proba[0] += 0.0000f; proba[1] += 0.0000f; proba[2] += 1.0000f; proba[3] += 0.0000f; proba[4] += 0.0000f; return; }
  node_33: if (x[1] <= 2.934650f) goto node_34; else goto node_35;
  node_34: { proba[0] += 0.0000f; proba[1] += 1.0000f; proba[2] += 0.0000f; proba[3] += 0.0000f; proba[4] += 0.0000f; return; }
  node_35: { proba[0] += 0.5714f; proba[1] += 0.4286f; proba[2] += 0.0000f; proba[3] += 0.0000f; proba[4] += 0.0000f; return; }
  node_36: if (x[10] <= 2259.259521f) goto node_37; else goto node_40;
  node_37: if (x[8] <= 9299.602539f) goto node_38; else goto node_39;
  node_38: { proba[0] += 0.1905f; proba[1] += 0.8095f; proba[2] += 0.0000f; proba[3] += 0.0000f; proba[4] += 0.0000f; return; }
  node_39: { proba[0] += 0.6750f; proba[1] += 0.2750f; proba[2] += 0.0250f; proba[3] += 0.0250f; proba[4] += 0.0000f; return; }
  node_40: if (x[9] <= 1650.350220f) goto node_41; else goto node_42;
  node_41: { proba[0] += 0.3333f; proba[1] += 0.6667f; proba[2] += 0.0000f; proba[3] += 0.0000f; proba[4] += 0.0000f; return; }
  node_42: { proba[0] += 0.9718f; proba[1] += 0.0282f; proba[2] += 0.0000f; proba[3] += 0.0000f; proba[4] += 0.0000f; return; }
  node_43: if (x[5] <= 1.296166f) goto node_44; else goto node_51;
  node_44: if (x[0] <= 1.210910f) goto node_45; else goto node_48;
  node_45: if (x[6] <= 22094.726562f) goto node_46; else goto node_47;
  node_46: { proba[0] += 0.0000f; proba[1] += 1.0000f; proba[2] += 0.0000f; proba[3] += 0.0000f; proba[4] += 0.0000f; return; }
  node_47: { proba[0] += 0.0000f; proba[1] += 0.0000f; proba[2] += 0.8000f; proba[3] += 0.2000f; proba[4] += 0.0000f; return; }
  node_48: if (x[6] <= 22045.898438f) goto node_49; else goto node_50;
  node_49: { proba[0] += 1.0000f; proba[1] += 0.0000f; proba[2] += 0.0000f; proba[3] += 0.0000f; proba[4] += 0.0000f; return; }
  node_50: { proba[0] += 0.0000f; proba[1] += 0.8068f; proba[2] += 0.0341f; proba[3] += 0.1591f; proba[4] += 0.0000f; return; }
  node_51: if (x[6] <= 22045.898438f) goto node_52; else goto node_53;
  node_52: { proba[0] += 1.0000f; proba[1] += 0.0000f; proba[2] += 0.0000f; proba[3] += 0.0000f; proba[4] += 0.0000f; return; }
  node_53: { proba[0] += 0.0000f; proba[1] += 1.0000f; proba[2] += 0.0000f; proba[3] += 0.0000f; proba[4] += 0.0000f; return; }
  node_54: { proba[0] += 0.0000f; proba[1] += 0.0000f; proba[2] += 0.0000f; proba[3] += 0.0000f; proba[4] += 1.0000f; return; }
}

static void rf_tree_6(const float *x, float *proba) {
  node_0: if (x[7] <= 1330863.875000f) goto node_1; else goto node_30;
  node_1: if (x[6] <= 22045.898438f) goto node_2; else goto node_3;
  node_2: { proba[0] += 1.0000f; proba[1] += 0.0000f; proba[2] += 0.0000f; proba[3] += 0.0000f; proba[4] += 0.0000f; return; }
  node_3: if (x[8] <= 7534.340820f) goto node_4; else goto node_15;
  node_4: if (x[6] <= 22094.726562f) goto node_5; else goto node_10;
  node_5: if (x[1] <= 2.704900f) goto node_6; else goto node_9;
  node_6: if (x[4] <= -0.491871f) goto node_7; else goto node_8;
  node_7: { proba[0] += 0.0000f; proba[1] += 0.0000f; proba[2] += 0.0000f; proba[3] += 1.0000f; proba[4] += 0.0000f; return; }
  node_8: { proba[0] += 0.0000f; proba[1] += 0.9474f; proba[2] += 0.0000f; proba[3] += 0.0526f; proba[4] += 0.0000f; return; }
  node_9: { proba[0] += 0.0000f; proba[1] += 1.0000f; proba[2] += 0.0000f; proba[3] += 0.0000f; proba[4] += 0.0000f; return; }
  node_10: if (x[5] <= 0.892817f) goto node_11; else goto node_12;
  node_11: { proba[0] += 0.0000f; proba[1] += 0.0000f; proba[2] += 1.0000f; proba[3] += 0.0000f; proba[4] += 0.0000f; return; }
  node_12: if (x[9] <= 2204.725708f) goto node_13; else goto node_14;
  node_13: { proba[0] += 0.0000f; proba[1] += 0.0000f; proba[2] += 0.9637f; proba[3] += 0.0363f; proba[4] += 0.0000f; return; }
  node_14: { proba[0] += 0.0000f; proba[1] += 0.0000f; proba[2] += 0.0364f; proba[3] += 0.9636f; proba[4] += 0.0000f; return; }
  node_15: if (x[0] <= 1.139679f) goto node_16; else goto node_23;
  node_16: if (x[1] <= 3.459050f) goto node_17; else goto node_20;
  node_17: if (x[8] <= 10069.089355f) goto node_18; else goto node_19;
  node_18: { proba[0] += 0.0000f; proba[1] += 0.2515f; proba[2] += 0.2275f; proba[3] += 0.5210f; proba[4] += 0.0000f; return; }
  node_19: { proba[0] += 0.0000f; proba[1] += 0.0703f; proba[2] += 0.0469f; proba[3] += 0.8828f; proba[4] += 0.0000f; return; }
  node_20: if (x[6] <= 22094.726562f) goto node_21; else goto node_22;
  node_21: { proba[0] += 0.0000f; proba[1] += 0.1818f; proba[2] += 0.0000f; proba[3] += 0.8182f; proba[4] += 0.0000f; return; }
  node_22: { proba[0] += 0.0000f; proba[1] += 0.0000f; proba[2] += 0.8636f; proba[3] += 0.1364f; proba[4] += 0.0000f; return; }
  node_23: if (x[6] <= 22094.726562f) goto node_24; else goto node_27;
  node_24: if (x[9] <= 4404.361572f) goto node_25; else goto node_26;
  node_25: { proba[0] += 0.0000f; proba[1] += 1.0000f; proba[2] += 0.0000f; proba[3] += 0.0000f; proba[4] += 0.0000f; return; }
  node_26: { proba[0] += 0.0000f; proba[1] += 0.1111f; proba[2] += 0.0000f; proba[3] += 0.8889f; proba[4] += 0.0000f; return; }
  node_27: if (x[9] <= 2431.494385f) goto node_28; else goto node_29;
  node_28: { proba[0] += 0.0000f; proba[1] += 0.0000f; proba[2] += 0.9615f; proba[3] += 0.0385f; proba[4] += 0.0000f; return; }
  node_29: { proba[0] += 0.0000f; proba[1] += 0.0000f; proba[2] += 0.0154f; proba[3] += 0.9846f; proba[4] += 0.0000f; return; }
  node_30: { proba[0] += 0.0000f; proba[1] += 0.0000f; proba[2] += 0.0000f; proba[3] += 0.0000f; proba[4] += 1.0000f; return; }
}

static void rf_tree_7(const float *x, float *proba) {
  node_0: if (x[0] <= 1.900116f) goto node_1; else goto node_52;
  node_1: if (x[9] <= 2768.592163f) goto node_2; else goto node_29;
  node_2: if (x[11] <= 2959.930176f) goto node_3; else goto node_16;
  node_3: if (x[4] <= -0.220619f) goto node_4; else goto node_11;
  node_4: if (x[7] <= 225648.468750f) goto node_5; else goto node_8;
  node_5: if (x[6] <= 22094.726562f) goto node_6; else goto node_7;
  node_6: { proba[0] += 0.3976f; proba[1] += 0.5822f; proba[2] += 0.0000f; proba[3] += 0.0203f; proba[4] += 0.0000f; return; }
  node_7: { proba[0] += 0.0000f; proba[1] += 0.0000f; proba[2] += 0.8966f; proba[3] += 0.1034f; proba[4] += 0.0000f; return; }
  node_8: if (x[4] <= -0.295821f) goto node_9; else goto node_10;
  node_9: { proba[0] += 0.0000f; proba[1] += 0.0000f; proba[2] += 1.0000f; proba[3] += 0.0000f; proba[4] += 0.0000f; return; }
  node_10: { proba[0] += 0.0000f; proba[1] += 0.0000f; proba[2] += 0.7500f; proba[3] += 0.2500f; proba[4] += 0.0000f; return; }
  node_11: if (x[6] <= 22045.898438f) goto node_12; else goto node_13;
  node_12: { proba[0] += 1.0000f; proba[1] += 0.0000f; proba[2] += 0.0000f; proba[3] += 0.0000f; proba[4] += 0.0000f; return; }
  node_13: if (x[10] <= 1439.468445f) goto node_14; else goto node_15;
  node_14: { proba[0] += 0.0000f; proba[1] += 0.0556f; proba[2] += 0.9444f; proba[3] += 0.0000f; proba[4] += 0.0000f; return; }
  node_15: { proba[0] += 0.0000f; proba[1] += 0.6667f; proba[2] += 0.0000f; proba[3] += 0.3333f; proba[4] += 0.0000f; return; }
  node_16: if (x[8] <= 9404.136230f) goto node_17; else goto node_22;
  node_17: if (x[0] <= 1.141411f) goto node_18; else goto node_19;
  node_18: { proba[0] += 0.0000f; proba[1] += 0.0000f; proba[2] += 1.0000f; proba[3] += 0.0000f; proba[4] += 0.0000f; return; }
  node_19: if (x[0] <= 1.306085f) goto node_20; else goto node_21;
  node_20: { proba[0] += 0.2333f; proba[1] += 0.7333f; proba[2] += 0.0333f; proba[3] += 0.0000f; proba[4] += 0.0000f; return; }
  node_21: { proba[0] += 0.7895f; proba[1] += 0.1579f; proba[2] += 0.0526f; proba[3] += 0.0000f; proba[4] += 0.0000f; return; }
  node_22: if (x[8] <= 12609.291504f) goto node_23; else goto node_26;
  node_23: if (x[6] <= 22045.898438f) goto node_24; else goto node_25;
  node_24: { proba[0] += 1.0000f; proba[1] += 0.0000f; proba[2] += 0.0000f; proba[3] += 0.0000f; proba[4] += 0.0000f; return; }
  node_25: { proba[0] += 0.0000f; proba[1] += 0.8250f; proba[2] += 0.1750f; proba[3] += 0.0000f; proba[4] += 0.0000f; return; }
  node_26: if (x[0] <= 1.234708f) goto node_27; else goto node_28;
  node_27: { proba[0] += 0.0000f; proba[1] += 0.2500f; proba[2] += 0.7500f; proba[3] += 0.0000f; proba[4] += 0.0000f; return; }
  node_28: { proba[0] += 0.9348f; proba[1] += 0.0652f; proba[2] += 0.0000f; proba[3] += 0.0000f; proba[4] += 0.0000f; return; }
  node_29: if (x[9] <= 3527.689453f) goto node_30; else goto node_45;
  node_30: if (x[10] <= 1976.260437f) goto node_31; else goto node_38;
  node_31: if (x[6] <= 22094.726562f) goto node_32; else goto node_35;
  node_32: if (x[1] <= 2.556550f) goto node_33; else goto node_34;
  node_33: { proba[0] += 0.0000f; proba[1] += 0.0000f; proba[2] += 0.0000f; proba[3] += 1.0000f; proba[4] += 0.0000f; return; }
  node_34: { proba[0] += 0.1250f; proba[1] += 0.8750f; proba[2] += 0.0000f; proba[3] += 0.0000f; proba[4] += 0.0000f; return; }
  node_35: if (x[8] <= 5115.042969f) goto node_36; else goto node_37;
  node_36: { proba[0] += 0.0000f; proba[1] += 0.0000f; proba[2] += 0.7500f; proba[3] += 0.2500f; proba[4] += 0.0000f; return; }
  node_37: { proba[0] += 0.0000f; proba[1] += 0.0000f; proba[2] += 0.0099f; proba[3] += 0.9901f; proba[4] += 0.0000f; return; }
  node_38: if (x[0] <= 1.233280f) goto node_39; else goto node_42;
  node_39: if (x[5] <= 1.119718f) goto node_40; else goto node_41;
  node_40: { proba[0] += 0.5000f; proba[1] += 0.0000f; proba[2] += 0.0000f; proba[3] += 0.5000f; proba[4] += 0.0000f; return; }
  node_41: { proba[0] += 0.1852f; proba[1] += 0.7407f; proba[2] += 0.0000f; proba[3] += 0.0741f; proba[4] += 0.0000f; return; }
  node_42: if (x[6] <= 22045.898438f) goto node_43; else goto node_44;
  node_43: { proba[0] += 1.0000f; proba[1] += 0.0000f; proba[2] += 0.0000f; proba[3] += 0.0000f; proba[4] += 0.0000f; return; }
  node_44: { proba[0] += 0.0000f; proba[1] += 0.8182f; proba[2] += 0.0000f; proba[3] += 0.1818f; proba[4] += 0.0000f; return; }
  node_45: if (x[6] <= 22045.898438f) goto node_46; else goto node_47;
  node_46: { proba[0] += 1.0000f; proba[1] += 0.0000f; proba[2] += 0.0000f; proba[3] += 0.0000f; proba[4] += 0.0000f; return; }
  node_47: if (x[6] <= 22094.726562f) goto node_48; else goto node_51;
  node_48: if (x[0] <= 1.219784f) goto node_49; else goto node_50;
  node_49: { proba[0] += 0.0000f; proba[1] += 0.0309f; proba[2] += 0.0000f; proba[3] += 0.9691f; proba[4] += 0.0000f; return; }
  node_50: { proba[0] += 0.0000f; proba[1] += 1.0000f; proba[2] += 0.0000f; proba[3] += 0.0000f; proba[4] += 0.0000f; return; }
  node_51: { proba[0] += 0.0000f; proba[1] += 0.0000f; proba[2] += 0.0000f; proba[3] += 1.0000f; proba[4] += 0.0000f; return; }
  node_52: { proba[0] += 0.0000f; proba[1] += 0.0000f; proba[2] += 0.0000f; proba[3] += 0.0000f; proba[4] += 1.0000f; return; }
}

static void rf_tree_8(const float *x, float *proba) {
  node_0: if (x[8] <= 56271.476562f) goto node_1; else goto node_30;
  node_1: if (x[6] <= 22045.898438f) goto node_2; else goto node_3;
  node_2: { proba[0] += 1.0000f; proba[1] += 0.0000f; proba[2] += 0.0000f; proba[3] += 0.0000f; proba[4] += 0.0000f; return; }
  node_3: if (x[6] <= 22094.726562f) goto node_4; else goto node_17;
  node_4: if (x[9] <= 3595.234619f) goto node_5; else goto node_12;
  node_5: if (x[9] <= 2690.561646f) goto node_6; else goto node_9;
  node_6: if (x[1] <= 2.439950f) goto node_7; else goto node_8;
  node_7: { proba[0] += 0.0000f; proba[1] += 0.9452f; proba[2] += 0.0000f; proba[3] += 0.0548f; proba[4] += 0.0000f; return; }
  node_8: { proba[0] += 0.0000f; proba[1] += 1.0000f; proba[2] += 0.0000f; proba[3] += 0.0000f; proba[4] += 0.0000f; return; }
  node_9: if (x[0] <= 1.113137f) goto node_10; else goto node_11;
  node_10: { proba[0] += 0.0000f; proba[1] += 0.0455f; proba[2] += 0.0000f; proba[3] += 0.9545f; proba[4] += 0.0000f; return; }
  node_11: { proba[0] += 0.0000f; proba[1] += 1.0000f; proba[2] += 0.0000f; proba[3] += 0.0000f; proba[4] += 0.0000f; return; }
  node_12: if (x[5] <= 1.206449f) goto node_13; else goto node_16;
  node_13: if (x[11] <= 2476.049194f) goto node_14; else goto node_15;
  node_14: { proba[0] += 0.0000f; proba[1] += 0.0000f; proba[2] += 0.0000f; proba[3] += 1.0000f; proba[4] += 0.0000f; return; }
  node_15: { proba[0] += 0.0000f; proba[1] += 1.0000f; proba[2] += 0.0000f; proba[3] += 0.0000f; proba[4] += 0.0000f; return; }
  node_16: { proba[0] += 0.0000f; proba[1] += 1.0000f; proba[2] += 0.0000f; proba[3] += 0.0000f; proba[4] += 0.0000f; return; }
  node_17: if (x[9] <= 2321.806641f) goto node_18; else goto node_25;
  node_18: if (x[7] <= 114883.453125f) goto node_19; else goto node_22;
  node_19: if (x[7] <= 78181.742188f) goto node_20; else goto node_21;
  node_20: { proba[0] += 0.0000f; proba[1] += 0.0000f; proba[2] += 1.0000f; proba[3] += 0.0000f; proba[4] += 0.0000f; return; }
  node_21: { proba[0] += 0.0000f; proba[1] += 0.0000f; proba[2] += 0.9600f; proba[3] += 0.0400f; proba[4] += 0.0000f; return; }
  node_22: if (x[10] <= 1705.535156f) goto node_23; else goto node_24;
  node_23: { proba[0] += 0.0000f; proba[1] += 0.0000f; proba[2] += 0.9204f; proba[3] += 0.0796f; proba[4] += 0.0000f; return; }
  node_24: { proba[0] += 0.0000f; proba[1] += 0.0000f; proba[2] += 0.0000f; proba[3] += 1.0000f; proba[4] += 0.0000f; return; }
  node_25: if (x[0] <= 1.451278f) goto node_26; else goto node_29;
  node_26: if (x[9] <= 2809.999390f) goto node_27; else goto node_28;
  node_27: { proba[0] += 0.0000f; proba[1] += 0.0000f; proba[2] += 0.2281f; proba[3] += 0.7719f; proba[4] += 0.0000f; return; }
  node_28: { proba[0] += 0.0000f; proba[1] += 0.0000f; proba[2] += 0.0000f; proba[3] += 1.0000f; proba[4] += 0.0000f; return; }
  node_29: { proba[0] += 0.0000f; proba[1] += 0.0000f; proba[2] += 1.0000f; proba[3] += 0.0000f; proba[4] += 0.0000f; return; }
  node_30: { proba[0] += 0.0000f; proba[1] += 0.0000f; proba[2] += 0.0000f; proba[3] += 0.0000f; proba[4] += 1.0000f; return; }
}

static void rf_tree_9(const float *x, float *proba) {
  node_0: if (x[9] <= 11486.053711f) goto node_1; else goto node_30;
  node_1: if (x[6] <= 22045.898438f) goto node_2; else goto node_5;
  node_2: if (x[9] <= 5148.665039f) goto node_3; else goto node_4;
  node_3: { proba[0] += 1.0000f; proba[1] += 0.0000f; proba[2] += 0.0000f; proba[3] += 0.0000f; proba[4] += 0.0000f; return; }
  node_4: { proba[0] += 0.0000f; proba[1] += 0.0000f; proba[2] += 0.0000f; proba[3] += 0.0000f; proba[4] += 1.0000f; return; }
  node_5: if (x[10] <= 1200.337769f) goto node_6; else goto node_21;
  node_6: if (x[0] <= 1.069165f) goto node_7; else goto node_14;
  node_7: if (x[10] <= 1014.133667f) goto node_8; else goto node_11;
  node_8: if (x[10] <= 831.528748f) goto node_9; else goto node_10;
  node_9: { proba[0] += 0.0000f; proba[1] += 0.0000f; proba[2] += 1.0000f; proba[3] += 0.0000f; proba[4] += 0.0000f; return; }
  node_10: { proba[0] += 0.0000f; proba[1] += 0.2376f; proba[2] += 0.7327f; proba[3] += 0.0297f; proba[4] += 0.0000f; return; }
  node_11: if (x[6] <= 22094.726562f) goto node_12; else goto node_13;
  node_12: { proba[0] += 0.0000f; proba[1] += 0.9821f; proba[2] += 0.0000f; proba[3] += 0.0179f; proba[4] += 0.0000f; return; }
  node_13: { proba[0] += 0.0000f; proba[1] += 0.0000f; proba[2] += 0.9138f; proba[3] += 0.0862f; proba[4] += 0.0000f; return; }
  node_14: if (x[9] <= 2059.661865f) goto node_15; else goto node_18;
  node_15: if (x[2] <= 2.351162f) goto node_16; else goto node_17;
  node_16: { proba[0] += 0.0000f; proba[1] += 0.0000f; proba[2] += 0.7917f; proba[3] += 0.2083f; proba[4] += 0.0000f; return; }
  node_17: { proba[0] += 0.0000f; proba[1] += 0.0000f; proba[2] += 1.0000f; proba[3] += 0.0000f; proba[4] += 0.0000f; return; }
  node_18: if (x[5] <= 1.129550f) goto node_19; else goto node_20;
  node_19: { proba[0] += 0.0000f; proba[1] += 0.0000f; proba[2] += 0.4000f; proba[3] += 0.6000f; proba[4] += 0.0000f; return; }
  node_20: { proba[0] += 0.0000f; proba[1] += 0.0000f; proba[2] += 0.0000f; proba[3] += 1.0000f; proba[4] += 0.0000f; return; }
  node_21: if (x[7] <= 235367.320312f) goto node_22; else goto node_29;
  node_22: if (x[11] <= 2923.936890f) goto node_23; else goto node_26;
  node_23: if (x[11] <= 2156.439697f) goto node_24; else goto node_25;
  node_24: { proba[0] += 0.0000f; proba[1] += 0.2220f; proba[2] += 0.0955f; proba[3] += 0.6826f; proba[4] += 0.0000f; return; }
  node_25: { proba[0] += 0.0000f; proba[1] += 0.4225f; proba[2] += 0.2492f; proba[3] += 0.3283f; proba[4] += 0.0000f; return; }
  node_26: if (x[6] <= 22094.726562f) goto node_27; else goto node_28;
  node_27: { proba[0] += 0.0000f; proba[1] += 1.0000f; proba[2] += 0.0000f; proba[3] += 0.0000f; proba[4] += 0.0000f; return; }
  node_28: { proba[0] += 0.0000f; proba[1] += 0.0000f; proba[2] += 0.2500f; proba[3] += 0.7500f; proba[4] += 0.0000f; return; }
  node_29: { proba[0] += 0.0000f; proba[1] += 0.0000f; proba[2] += 1.0000f; proba[3] += 0.0000f; proba[4] += 0.0000f; return; }
  node_30: { proba[0] += 0.0000f; proba[1] += 0.0000f; proba[2] += 0.0000f; proba[3] += 0.0000f; proba[4] += 1.0000f; return; }
}

static int rf_predict(const float *x, float *proba) {
  for (int c = 0; c < 5; c++) proba[c] = 0;
  rf_tree_0(x, proba);
  rf_tree_1(x, proba);
  rf_tree_2(x, proba);
  rf_tree_3(x, proba);
  rf_tree_4(x, proba);
  rf_tree_5(x, proba);
  rf_tree_6(x, proba);
  rf_tree_7(x, proba);
  rf_tree_8(x, proba);
  rf_tree_9(x, proba);
  int best = 0;
  for (int c = 0; c < 5; c++) { proba[c] /= 10.0f; if (proba[c] > proba[best]) best = c; }
  return best;
}