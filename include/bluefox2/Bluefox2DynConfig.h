#ifndef BLUEFOX_2_DYN_CONFIG_H_
#define BLUEFOX_2_DYN_CONFIG_H_

struct Bluefox2DynConfig {
  // Area of Intreset
  int width; int height;
  // Pixel Format
  int idpf;
  // Binning
  int cbm;
  // Gain
  bool agc; double gain_db;
  // Expose
  bool aec; int expose_us;
  // Auto Controller
  int acs; int des_grey_value;
  // Auto exposure upper limit us
  /*int acs;*/ int expose_upper_limit_us;
  // White Balance
  int wbp; double r_gain; double g_gain; double b_gain;
  // High Dynamic Range
  bool hdr;
  // Dark Current Filter
  int dcfm;
  // Pixel Clock
  int cpc;
  // Trigger Mode
  int ctm;
  // Trigger Source
  int cts;
  // Request
  int max_expose_jump;
  // Expose Jump
  int request;
  int fps;
};

const int Bluefox2Dyn_idpf_auto = 0;
const int Bluefox2Dyn_idpf_raw = 1;
const int Bluefox2Dyn_idpf_mono8 = 2;
const int Bluefox2Dyn_idpf_mono16 = 9;
const int Bluefox2Dyn_idpf_rgb888_packed = 10;
const int Bluefox2Dyn_idpf_bgr888_packed = 22;
const int Bluefox2Dyn_cbm_off = 0;
const int Bluefox2Dyn_cbm_binning_h = 1;
const int Bluefox2Dyn_cbm_binning_v = 2;
const int Bluefox2Dyn_cbm_binning_hv = 3;
const int Bluefox2Dyn_acs_unavailable = -1;
const int Bluefox2Dyn_acs_slow = 0;
const int Bluefox2Dyn_acs_medium = 1;
const int Bluefox2Dyn_acs_fast = 2;
const int Bluefox2Dyn_dcfm_off = 0;
const int Bluefox2Dyn_dcfm_on = 1;
const int Bluefox2Dyn_dcfm_calibrate = 2;
const int Bluefox2Dyn_correction_image = 3;
const int Bluefox2Dyn_cpc_12000 = 12000;
const int Bluefox2Dyn_cpc_20000 = 20000;
const int Bluefox2Dyn_cpc_24000 = 24000;
const int Bluefox2Dyn_cpc_27000 = 27000;
const int Bluefox2Dyn_cpc_32000 = 32000;
const int Bluefox2Dyn_cpc_40000 = 40000;
const int Bluefox2Dyn_cpc_50000 = 50000;
const int Bluefox2Dyn_ctm_continuous = 0;
const int Bluefox2Dyn_ctm_on_demand = 1;
const int Bluefox2Dyn_ctm_on_low_level = 2;
const int Bluefox2Dyn_ctm_on_high_level = 3;
const int Bluefox2Dyn_ctm_on_falling_edge = 4;
const int Bluefox2Dyn_ctm_on_rising_edge = 5;
const int Bluefox2Dyn_hard_sync = -1;
const int Bluefox2Dyn_cts_unavailable = -1;
const int Bluefox2Dyn_cts_dig_in_0 = 0;
const int Bluefox2Dyn_cts_dig_in_1 = 1;
const int Bluefox2Dyn_wbp_unavailable = -1;
const int Bluefox2Dyn_wbp_tungsten = 0;
const int Bluefox2Dyn_wbp_halogen = 1;
const int Bluefox2Dyn_wbp_fluorescent = 2;
const int Bluefox2Dyn_wbp_daylight = 3;
const int Bluefox2Dyn_wbp_photolight = 4;
const int Bluefox2Dyn_wbp_bluesky = 5;
const int Bluefox2Dyn_wbp_user1 = 6;
const int Bluefox2Dyn_wbp_calibrate = 10;
const int Bluefox2Dyn_mm_off = 0;
const int Bluefox2Dyn_mm_topdown = 1;
const int Bluefox2Dyn_mm_leftright = 2;
const int Bluefox2Dyn_mm_topdown_and_leftright = 3;

#endif // BLUEFOX_2_DYN_CONFIG_H_