/*
 * SPDX-FileCopyrightText: 2025 mhl6829
 * SPDX-License-Identifier: MIT
 * File: [gt1000_param.h] - GT1000 memory model emulation
 */


#ifndef _GT1000_PARAM_H
#define _GT1000_PARAM_H

#include "freertos/FreeRTOS.h"
#include "gt1000.h"

#define EFFECT_BLOCK_SIZE           0x100
#define EFFECT_BLOCK_ALIGN          0x100

#define ALIGNED_EFFECT_BLOCK\
    struct __attribute__((packed, aligned(EFFECT_BLOCK_ALIGN)))

typedef ALIGNED_EFFECT_BLOCK {
    bool sw;                            // 0x00: SW, OFF/ON (0-1)
    uint8_t type;                       // 0x01: TYPE (0-5) BOSS COMP, X-COMP, D-COMP, ORANGE, STEREO COMP, X-BASS COMP
    uint8_t sustain;                    // 0x02: SUSTAIN (0-100)
    uint8_t attack;                     // 0x03: ATTACK (0-100)
    uint8_t level;                      // 0x04: LEVEL (0-100)
    uint8_t tone;                       // 0x05: TONE (-50 to 50, stored as 14-114)
    uint8_t ratio;                      // 0x06: RATIO (0-17), 1:1 ~ INF:1
    uint8_t direct_mix;                 // 0x07: DIRECT MIX (0-100)
    uint8_t threshold;                  // 0x08: THRESHOLD (0-100)
} gt1000_comp_t;

typedef ALIGNED_EFFECT_BLOCK {
    bool sw;                            // 0x00: SW, OFF/ON (0-1)
    uint8_t type;                       // 0x01: TYPE (0-36), various Dist/OD types
    uint8_t drive;                      // 0x02: DRIVE (0-120)
    uint8_t tone;                       // 0x03: TONE (-50 to 50, stored as 14-114)
    uint8_t level;                      // 0x04: LEVEL (0-100)
    uint8_t bottom;                     // 0x05: BOTTOM (-50 to 50, stored as 14-114)
    uint8_t direct_mix;                 // 0x06: DIRECT MIX (0-100)
    bool solo_sw;                       // 0x07: SOLO SW, OFF/ON (0-1)
    uint8_t solo_level;                 // 0x08: SOLO LEVEL (0-100)
} gt1000_dist_t;

typedef ALIGNED_EFFECT_BLOCK {
    bool sw;                            // 0x00: SW, OFF/ON (0-1)
    uint8_t type;                       // 0x01: TYPE (0-31), various amp types
    uint8_t gain;                       // 0x02: GAIN (0-120)
    uint8_t sag;                        // 0x03: SAG (-10 to 10, stored as 6-26)
    uint8_t resonance;                  // 0x04: RESONANCE (-10 to 10, stored as 6-26)
    uint8_t level;                      // 0x05: LEVEL (0-100)
    uint8_t bass;                       // 0x06: BASS (0-100)
    uint8_t middle;                     // 0x07: MIDDLE (0-100)
    uint8_t treble;                     // 0x08: TREBLE (0-100)
    uint8_t presence;                   // 0x09: PRESENCE (0-100)
    bool bright;                        // 0x0A: BRIGHT, OFF/ON (0-1)
    uint8_t gain_sw;                    // 0x0B: GAIN SW (0-2), LOW/MIDDLE/HIGH
    bool solo_sw;                       // 0x0C: SOLO SW, OFF/ON (0-1)
    uint8_t solo_level;                 // 0x0D: SOLO LEVEL (0-100)
} gt1000_preamp_t;

typedef ALIGNED_EFFECT_BLOCK {
    bool sw;                            // 0x00: SW, OFF/ON (0-1)
    uint8_t threshold;                  // 0x01: THRESHOLD (0-100)
    uint8_t release;                    // 0x02: RELEASE (0-100)
    uint8_t detect;                     // 0x03: DETECT (0-2), INPUT/NS INPUT/FV OUT
} gt1000_ns_t;

typedef ALIGNED_EFFECT_BLOCK {
    bool sw;                            // 0x00: SW, OFF/ON (0-1)
    uint8_t type;                       // 0x01: TYPE (0-1), PARAMETRIC or GRAPHIC
    uint8_t low_gain;                   // 0x02: LOW GAIN (-20 to 20 dB, stored as 12-52)
    uint8_t high_gain;                  // 0x03: HIGH GAIN (-20 to 20 dB, stored as 12-52)
    uint8_t level;                      // 0x04: LEVEL (-20 to 20 dB, stored as 12-52)
    uint8_t low_mid_freq;               // 0x05: LOW-MID FREQ (0-29)
    uint8_t low_mid_q;                  // 0x06: LOW-MID Q (0-5)
    uint8_t low_mid_gain;               // 0x07: LOW-MID GAIN (-20 to 20 dB, stored as 12-52)
    uint8_t high_mid_freq;              // 0x08: HIGH-MID FREQ (0-29)
    uint8_t high_mid_q;                 // 0x09: HIGH-MID Q (0-5)
    uint8_t high_mid_gain;              // 0x0A: HIGH-MID GAIN (-20 to 20 dB, stored as 12-52)
    uint8_t low_cut;                    // 0x0B: LOW CUT (0-31)
    uint8_t high_cut;                   // 0x0C: HIGH CUT (0-31)
    uint8_t graphic_level;              // 0x0D: LEVEL (-20 to 20 dB, stored as 12-52)
    uint8_t band_31hz;                  // 0x0E: 31.5Hz (-20 to 20 dB, stored as 12-52)
    uint8_t band_63hz;                  // 0x0F: 63Hz (-20 to 20 dB, stored as 12-52)
    uint8_t band_125hz;                 // 0x10: 125Hz (-20 to 20 dB, stored as 12-52)
    uint8_t band_250hz;                 // 0x11: 250Hz (-20 to 20 dB, stored as 12-52)
    uint8_t band_500hz;                 // 0x12: 500Hz (-20 to 20 dB, stored as 12-52)
    uint8_t band_1k;                    // 0x13: 1kHz (-20 to 20 dB, stored as 12-52)
    uint8_t band_2k;                    // 0x14: 2kHz (-20 to 20 dB, stored as 12-52)
    uint8_t band_4k;                    // 0x15: 4kHz (-20 to 20 dB, stored as 12-52)
    uint8_t band_8k;                    // 0x16: 8kHz (-20 to 20 dB, stored as 12-52)
    uint8_t band_16k;                   // 0x17: 16kHz (-20 to 20 dB, stored as 12-52)
} gt1000_eq_t;

typedef ALIGNED_EFFECT_BLOCK {
    bool sw;                            // 0x00: SW, OFF/ON (0-1)
    // 4 Byte value
    uint32_t time;                      // 0x01: TIME (1-2018), 1ms-2000ms or note values
    uint8_t feedback;                   // 0x05: FEEDBACK (0-100)
    uint8_t high_cut;                   // 0x06: HIGH CUT (0-31)
    uint8_t effect_level;               // 0x07: EFFECT LEVEL (0-120)
    uint8_t direct_level;               // 0x08: DIRECT LEVEL (0-100)
} gt1000_delay_t;

typedef ALIGNED_EFFECT_BLOCK {
    bool sw;                            // 0x00: SW, OFF/ON (0-1)
    uint8_t type;                       // 0x01: TYPE (0-19), MONO/PAN/STEREO/... 
    // 4 Byte value
    uint32_t time;                      // 0x02-0x05: TIME (1-4018), 1ms-4000ms or note values
    uint8_t feedback;                   // 0x06: FEEDBACK (0-100)
    uint8_t high_cut;                   // 0x07: HIGH CUT (0-31)
    uint8_t effect_level;               // 0x08: EFFECT LEVEL (0-120)
    uint8_t mod_rate;                   // 0x09: MOD RATE (0-100)
    uint8_t mod_depth;                  // 0x0A: MOD DEPTH (0-100)
    uint8_t duck_sens;                  // 0x0B: DUCK SENS (0-100)
    uint8_t duck_pre_depth;             // 0x0C: DUCK PRE DEPTH (0-100)
    uint8_t duck_post_depth;            // 0x0D: DUCK POST DEPTH (0-100)
    uint8_t direct_level;               // 0x0E: DIRECT LEVEL (0-100)
    uint8_t pitch;                      // 0x0F: PITCH (-24 to 24, stored 8-56)
    uint8_t pitch_bal;                  // 0x10: PITCH BAL (0-100)
    uint8_t pitch_feedback;             // 0x11: PITCH FEEDBACK (0-100)
    uint8_t dual_mode;                  // 0x12: DUAL:MODE (0-2), SERIES/PARALLEL/LR
    uint8_t d1_type;                    // 0x13: D1 TYPE (0-3), MONO/PAN/ANALOG/TAPE
    // 4 Byte value
    uint32_t d1_time;                   // 0x14-0x17: D1 TIME (1-2018)
    uint8_t d1_feedback;                // 0x18: D1 FEEDBACK (0-100)
    uint8_t d1_high_cut;                // 0x19: D1 HIGH CUT (0-31)
    uint8_t d1_effect_level;            // 0x1A: D1 EFFECT LEVEL (0-120)
    uint8_t d2_type;                    // 0x1B: D2 TYPE (0-3), MONO/PAN/ANALOG/TAPE
    // 4 Byte value
    uint32_t d2_time;                   // 0x1C-0x1F: D2 TIME (1-2018)
    uint8_t d2_feedback;                // 0x20: D2 FEEDBACK (0-100)
    uint8_t d2_high_cut;                // 0x21: D2 HIGH CUT (0-31)
    uint8_t d2_effect_level;            // 0x22: D2 EFFECT LEVEL (0-120)
    uint8_t twist_mode;                 // 0x23: TWIST:MODE (0-1)
    bool trigger;                       // 0x24: TRIGGER (0-1)
    uint8_t rise_time;                  // 0x25: RISE TIME (0-100)
    uint8_t fall_time;                  // 0x26: FALL TIME (0-100)
    uint8_t level;                      // 0x27: LEVEL (0-100)
    uint8_t stage;                      // 0x28: STAGE (0-2)
    uint8_t head;                       // 0x29: HEAD (0-4)
    uint8_t fade_time;                  // 0x2A: FADE TIME (0-100)
    uint8_t tap_time;                   // 0x2B: TAP TIME (0-100)
    uint8_t wow_flutter;                // 0x2C: WOW & FLUTTER (0-100)
    uint8_t drum_echo_head;             // 0x2D: DRUM ECHO:HEAD (0-11)
    uint8_t selector;                   // 0x2E: SELECTOR (0-2), ECHO/REPEAT/SWELL
    bool auto_trigger;                  // 0x2F: AUTO TRIGGER (0-1)
    uint8_t duty;                       // 0x30: DUTY (0-100)
    uint8_t tone;                       // 0x31: TONE (0-100)
} gt1000_mstdelay_t;


typedef ALIGNED_EFFECT_BLOCK {
    bool sw;                            // 0x00: SW (0-1)
    uint8_t type;                       // 0x01: TYPE (0-3), MONO/STEREO1/STEREO2/DUAL
    uint8_t rate;                       // 0x02: RATE (0-118), 0-100 or note values
    uint8_t depth;                      // 0x03: DEPTH (0-100)
    uint8_t pre_delay;                  // 0x04: PRE-DELAY (0-80)
    uint8_t effect_level;               // 0x05: EFFECT LEVEL (0-100)
    uint8_t waveform;                   // 0x06: WAVEFORM (0-1), TRI/SINE
    uint8_t low_cut;                    // 0x07: LOW CUT (0-31)
    uint8_t high_cut;                   // 0x08: HIGH CUT (0-31)
    uint8_t rate1;                      // 0x09: RATE 1 (0-118)
    uint8_t depth1;                     // 0x0A: DEPTH 1 (0-100)
    uint8_t pre_delay1;                 // 0x0B: PRE-DELAY 1 (0-80)
    uint8_t effect_level1;              // 0x0C: EFFECT LEVEL 1 (0-100)
    uint8_t waveform1;                  // 0x0D: WAVEFORM 1 (0-1)
    uint8_t low_cut1;                   // 0x0E: LOW CUT 1 (0-31)
    uint8_t high_cut1;                  // 0x0F: HIGH CUT 1 (0-31)
    uint8_t rate2;                      // 0x10: RATE 2 (0-118)
    uint8_t depth2;                     // 0x11: DEPTH 2 (0-100)
    uint8_t pre_delay2;                 // 0x12: PRE-DELAY 2 (0-80)
    uint8_t effect_level2;              // 0x13: EFFECT LEVEL 2 (0-100)
    uint8_t waveform2;                  // 0x14: WAVEFORM 2 (0-1)
    uint8_t low_cut2;                   // 0x15: LOW CUT 2 (0-31)
    uint8_t high_cut2;                  // 0x16: HIGH CUT 2 (0-31)
    uint8_t direct_level;               // 0x17: DIRECT LEVEL (0-100)
    uint8_t output_mode;                // 0x18: OUTPUT MODE (0-1), MONO/STEREO
} gt1000_chorus_t;

typedef ALIGNED_EFFECT_BLOCK {
    bool sw;                            // 0x00: SW (0-1)
    uint8_t type;                       // 0x01: TYPE (0-9)
    uint8_t direct_level;               // 0x02: DIRECT LEVEL (0-100)
    uint8_t low_damp;                   // 0x03: LOW DAMP (14-114, -50 to 50)
    uint8_t high_damp;                  // 0x04: HIGH DAMP (14-114, -50 to 50)
    uint8_t mod_rate;                   // 0x05: MOD RATE (0-100)
    uint8_t mod_depth;                  // 0x06: MOD DEPTH (0-100)
    uint8_t duck_sens;                  // 0x07: DUCK SENS (0-100)
    uint8_t duck_pre;                   // 0x08: DUCK PRE DEPTH (0-100)
    uint8_t duck_post;                  // 0x09: DUCK POST DEPTH (0-100)
    uint8_t time;                       // 0x0A: TIME (1-100, 0.1s - 10.0s)
    uint8_t tone;                       // 0x0B: TONE (14-114, -50 to 50)
    uint8_t effect_level;               // 0x0C: EFFECT LEVEL (0-100)
    uint8_t density;                    // 0x0D: DENSITY (1-10)
    // 2 Byte value
    uint16_t pre_delay;                 // 0x0E-0x0F: PRE-DELAY (0-200 ms)
    uint8_t low_cut;                    // 0x10: LOW CUT (0-31)
    uint8_t high_cut;                   // 0x11: HIGH CUT (0-31)
    uint8_t pitch1;                     // 0x12: PITCH 1 (8-56, -24 to 24)
    uint8_t level1;                     // 0x13: LEVEL 1 (0-100)
    uint8_t type1;                      // 0x14: TYPE 1 (0-2), HALL/PLATE/ROOM
    uint8_t time1;                      // 0x15: TIME 1 (1-100)
    uint8_t tone1;                      // 0x16: TONE 1 (14-114)
    uint8_t effect_level1;              // 0x17: EFFECT LEVEL 1 (0-100)
    uint8_t density1;                   // 0x18: DENSITY 1 (1-10)
    // 2 Byte value
    uint16_t pre_delay1;                // 0x1-0x1A: PRE-DELAY 1 (0-200 ms)
    uint8_t low_cut1;                   // 0x1B: LOW CUT 1 (0-31)
    uint8_t high_cut1;                  // 0x1C: HIGH CUT 1 (0-31)
    uint8_t pitch2;                     // 0x1D: PITCH 2 (8-56, -24 to 24)
    uint8_t level2;                     // 0x1E: LEVEL 2 (0-100)
    uint8_t type2;                      // 0x1F: TYPE 2 (0-2), HALL/PLATE/ROOM
    uint8_t time2;                      // 0x20: TIME 2 (1-100)
    uint8_t tone2;                      // 0x21: TONE 2 (14-114)
    uint8_t effect_level2;              // 0x22: EFFECT LEVEL 2 (0-100)
    uint8_t density2;                   // 0x23: DENSITY 2 (1-10)
    // 2 Byte value
    uint16_t pre_delay2;                // 0x24-0x25: PRE-DELAY 2 (0-200 ms)
    uint8_t low_cut2;                   // 0x26: LOW CUT 2 (0-31)
    uint8_t high_cut2;                  // 0x27: HIGH CUT 2 (0-31)
    uint8_t mode;                       // 0x28: MODE (0-2), MONO/STEREO1/STEREO2
    uint8_t spread_time;                // 0x29: SPREAD TIME (0-100)
    uint8_t feedback;                   // 0x2A: FEEDBACK (0-100)
    bool trigger;                       // 0x2B: TRIGGER (0-1)
} gt1000_reverb_t;


typedef ALIGNED_EFFECT_BLOCK {
    bool sw;                            // 0x00: SW (0-1), OFF/ON
    bool type;                          // 0x01: TYPE (0-1), PEDAL BEND / WAH
    uint8_t pitch_max;                  // 0x02: PITCH MAX (8-56, -24 to 24)
    uint8_t effect_level;               // 0x03: EFFECT LEVEL (0-100)
    uint8_t direct_mix;                 // 0x04: DIRECT MIX (0-100)
    uint8_t wah_type;                   // 0x05: WAH TYPE (0-6)
    // 4 Byte value
    uint16_t pedal_min;                 // 0x06-0x09: PEDAL MIN (0-1000)
    // 4 Byte value
    uint16_t pedal_max;                 // 0x0A-0x0D: PEDAL MAX (0-1000)
    // 4 Byte value
    uint16_t wah_pedal_position;        // 0x0E-0x11: WAH:PEDAL POSITION (0-1000)
    // 4 Byte value
    uint16_t pedalbend_pedal_position;  // 0x12-0x15: PEDAL BEND:PEDAL POSITION (0-1000)
    uint8_t pitch_min;                  // 0x16: PITCH MIN (8-56, -24 to 24)
} gt1000_pedalfx_t;

typedef ALIGNED_EFFECT_BLOCK {
    bool fx_sw;                         // 0x00: FX SW (0-1), OFF/ON
    uint8_t fx_type;                    // 0x01: FX TYPE (0-33)
} gt1000_fx_t;

//
//
// FX BLOCKS
//
//


typedef ALIGNED_EFFECT_BLOCK {
    uint8_t body;                       // 0x00: BODY (0-100)
    uint8_t low;                        // 0x01: LOW (0-100, -50..+50)
    uint8_t high;                       // 0x02: HIGH (0-100, -50..+50)
    uint8_t level;                      // 0x03: LEVEL (0-100)
} gt1000_fx_agsim_t;

typedef ALIGNED_EFFECT_BLOCK {
    uint8_t type;                       // 0x00: TYPE (0-2), NATURAL/WIDE/BRIGHT
    uint8_t resonance;                  // 0x01: RESONANCE (0-100)
    uint8_t tone;                       // 0x02: TONE (14-114, -50..+50)
    uint8_t level;                      // 0x03: LEVEL (0-100)
} gt1000_fx_acreso_t;

typedef ALIGNED_EFFECT_BLOCK {
    uint8_t filter_mode;                // 0x00: FILTER MODE (0-2), LPF/BPF/HPF
    uint8_t rate;                       // 0x01: RATE (0-118)
    uint8_t depth;                      // 0x02: DEPTH (0-100)
    uint8_t effect_level;               // 0x03: EFFECT LEVEL (0-100)
    uint8_t frequency;                  // 0x04: FREQUENCY (0-100)
    uint8_t resonance;                  // 0x05: RESONANCE (0-100)
    uint8_t waveform;                   // 0x06: WAVEFORM (0-1), TRI/SINE
    uint8_t direct_mix;                 // 0x07: DIRECT MIX (0-100)
} gt1000_fx_awah_t;

typedef ALIGNED_EFFECT_BLOCK {
    uint8_t type;                       // 0x00: TYPE (0-6), MONO/STEREO1/STEREO2/DUAL/PRIME/CE-1 CHORUS/CE-1 VIBRATO
    uint8_t direct_level;               // 0x01: DIRECT LEVEL (0-100)
    uint8_t output_mode;                // 0x02: OUTPUT MODE (0-1), MONO/STEREO
    uint8_t sweetness;                  // 0x03: SWEETNESS (0-100)
    uint8_t bell;                       // 0x04: BELL (0-100)
    uint8_t preamp_sw;                  // 0x05: PREAMP SW (0-1), OFF/ON
    uint8_t preamp_gain;                // 0x06: PREAMP GAIN (0-100)
    uint8_t preamp_level;               // 0x07: PREAMP LEVEL (0-100)
    uint8_t rate;                       // 0x08: RATE (0-118)
    uint8_t depth;                      // 0x09: DEPTH (0-100)
    uint8_t pre_delay;                  // 0x0A: PRE-DELAY (0-80)
    uint8_t effect_level;               // 0x0B: EFFECT LEVEL (0-100)
    uint8_t waveform;                   // 0x0C: WAVEFORM (0-1), TRI/SINE
    uint8_t low_cut;                    // 0x0D: LOW CUT (0-31)
    uint8_t high_cut;                   // 0x0E: HIGH CUT (0-31)
    uint8_t rate1;                      // 0x0F: RATE 1 (0-118)
    uint8_t depth1;                     // 0x10: DEPTH 1 (0-100)
    uint8_t pre_delay1;                 // 0x11: PRE-DELAY 1 (0-80)
    uint8_t effect_level1;              // 0x12: EFFECT LEVEL 1 (0-100)
    uint8_t waveform1;                  // 0x13: WAVEFORM 1 (0-1)
    uint8_t low_cut1;                   // 0x14: LOW CUT 1 (0-31)
    uint8_t high_cut1;                  // 0x15: HIGH CUT 1 (0-31)
    uint8_t rate2;                      // 0x16: RATE 2 (0-118)
    uint8_t depth2;                     // 0x17: DEPTH 2 (0-100)
    uint8_t pre_delay2;                 // 0x18: PRE-DELAY 2 (0-80)
    uint8_t effect_level2;              // 0x19: EFFECT LEVEL 2 (0-100)
    uint8_t waveform2;                  // 0x1A: WAVEFORM 2 (0-1)
    uint8_t low_cut2;                   // 0x1B: LOW CUT 2 (0-31)
    uint8_t high_cut2;                  // 0x1C: HIGH CUT 2 (0-31)
} gt1000_fx_chorus_t;

typedef ALIGNED_EFFECT_BLOCK {
    uint8_t mode;                       // 0x00: MODE (0-1), CHORUS/VIBRATO
    uint8_t rate;                       // 0x01: RATE (0-118)
    uint8_t depth;                      // 0x02: DEPTH (0-100)
    uint8_t effect_level;               // 0x03: EFFECT LEVEL (0-100)
} gt1000_fx_cvibe_t;

typedef ALIGNED_EFFECT_BLOCK {
    uint8_t type;                       // 0x00: TYPE (0-5), BOSS COMP / X-COMP / ...
    uint8_t sustain;                    // 0x01: SUSTAIN (0-100)
    uint8_t attack;                     // 0x02: ATTACK (0-100)
    uint8_t level;                      // 0x03: LEVEL (0-100)
    uint8_t tone;                       // 0x04: TONE (14-114, -50~50)
    uint8_t ratio;                      // 0x05: RATIO (0-17)
    uint8_t direct_mix;                 // 0x06: DIRECT MIX (0-100)
    uint8_t threshold;                  // 0x07: THRESHOLD (0-100)
} gt1000_fx_comp_t;

// Bass effector combined

typedef ALIGNED_EFFECT_BLOCK {
    uint8_t sens;                       // 0x00: SENS (0-100)
    uint8_t depth;                      // 0x01: DEPTH (0-100)
    uint8_t tone;                       // 0x02: TONE (14-114, -50~50)
    uint8_t effect_level;               // 0x03: EFFECT LEVEL (0-100)
    uint8_t attack;                     // 0x04: ATTACK (0-100)
    uint8_t resonance;                  // 0x05: RESONANCE (0-100)
    uint8_t direct_mix;                 // 0x06: DIRECT MIX (0-100)
    // Bass
    uint8_t sens_bass;                       // 0x07: SENS (0-100)
    uint8_t attack_bass;                     // 0x08: ATTACK (0-100)
    uint8_t tone_bass;                       // 0x09: TONE (14-114, -50~50)
    uint8_t effect_level_bass;               // 0x0A: EFFECT LEVEL (0-100)
    uint8_t direct_mix_bass;                 // 0x0B: DIRECT MIX (0-100)
} gt1000_fx_defretter_t;

typedef ALIGNED_EFFECT_BLOCK {
    uint8_t sens;                       // 0x07: SENS (0-100)
    uint8_t attack;                     // 0x08: ATTACK (0-100)
    uint8_t tone;                       // 0x09: TONE (14-114, -50~50)
    uint8_t effect_level;               // 0x0A: EFFECT LEVEL (0-100)
    uint8_t direct_mix;                 // 0x0B: DIRECT MIX (0-100)
} gt1000_fx_defretterbass_t;

typedef ALIGNED_EFFECT_BLOCK {
    uint8_t mode;                       // 0x00: MODE (0-1), NORMAL / OSC
    bool trigger;                       // 0x01: TRIGGER (0-1), OFF / ON
    uint8_t depth;                      // 0x02: DEPTH (0-100)
    uint8_t rise_time;                  // 0x03: RISE TIME (0-100)
    uint8_t oct_rise_time;              // 0x04: OCT RISE TIME (0-100)
    uint8_t feedback;                   // 0x05: FEEDBACK (0-100)
    uint8_t oct_feedback;               // 0x06: OCT FEEDBACK (0-100)
    uint8_t vib_rate;                   // 0x07: VIB RATE (0-100)
    uint8_t vib_depth;                  // 0x08: VIB DEPTH (0-100)
} gt1000_fx_feedbacker_t;

typedef ALIGNED_EFFECT_BLOCK {
    uint8_t rate;                       // 0x00: RATE (0-118) [0-100 + note values]
    uint8_t depth;                      // 0x01: DEPTH (0-100)
    uint8_t resonance;                  // 0x02: RESONANCE (0-100)
    uint8_t manual;                     // 0x03: MANUAL (0-100)
    bool turbo;                         // 0x04: TURBO (0-1), OFF/ON
    uint8_t waveform;                   // 0x05: WAVEFORM (0-1), TRI/SINE
    uint8_t step_rate;                  // 0x06: STEP RATE (0-119) [OFF, 0-100 + note values]
    uint8_t separation;                 // 0x07: SEPARATION (0-12), (0, 15–180)
    uint8_t effect_level;               // 0x08: EFFECT LEVEL (0-100)
    // 2 Byte value
    uint16_t low_damp;                  // 0x09-0x0A: LOW DAMP (28-128, -100–0)
    // 2 Byte value
    uint16_t high_damp;                 // 0x0B-0x0C: HIGH DAMP (28-128, -100–0)
    uint8_t low_cut;                    // 0x0D: LOW CUT (0-31), FLAT–20kHz
    uint8_t high_cut;                   // 0x0E: HIGH CUT (0-31), 20Hz–FLAT
    uint8_t direct_mix;                 // 0x0F: DIRECT MIX (0-100)
} gt1000_fx_flanger_t;

typedef ALIGNED_EFFECT_BLOCK {
    uint8_t voice;                      // 0x00: VOICE (0-2), 1VOICE / 2MONO / 2STEREO
    uint8_t hr1_harmony;                // 0x01: HR1:HARMONY (0-29)
    uint8_t hr2_harmony;                // 0x02: HR2:HARMONY (0-29)
    uint8_t hr1_level;                  // 0x03: HR1:LEVEL (0-100)
    uint32_t hr1_pre_delay;             // 0x04-0x07: HR1:PRE-DELAY (0-318, ms/note values)
    uint8_t hr1_feedback;               // 0x08: HR1:FEEDBACK (0-100)
    uint8_t direct_level;               // 0x09: DIRECT LEVEL (0-100)
    uint8_t hr2_level;                  // 0x0A: HR2:LEVEL (0-100)
    uint32_t hr2_pre_delay;             // 0x0B-0x0E: HR2:PRE-DELAY (0-318, ms/note values)

    // HR1 scale tables (per semitone, 0-48)
    uint8_t hr1_C;                      // 0x0F
    uint8_t hr1_Db;                     // 0x10
    uint8_t hr1_D;                      // 0x11
    uint8_t hr1_Eb;                     // 0x12
    uint8_t hr1_E;                      // 0x13
    uint8_t hr1_F;                      // 0x14
    uint8_t hr1_Fs;                     // 0x15
    uint8_t hr1_G;                      // 0x16
    uint8_t hr1_Ab;                     // 0x17
    uint8_t hr1_A;                      // 0x18
    uint8_t hr1_Bb;                     // 0x19
    uint8_t hr1_B;                      // 0x1A

    // HR2 scale tables (per semitone, 0-48)
    uint8_t hr2_C;                      // 0x1B
    uint8_t hr2_Db;                     // 0x1C
    uint8_t hr2_D;                      // 0x1D
    uint8_t hr2_Eb;                     // 0x1E
    uint8_t hr2_E;                      // 0x1F
    uint8_t hr2_F;                      // 0x20
    uint8_t hr2_Fs;                     // 0x21
    uint8_t hr2_G;                      // 0x22
    uint8_t hr2_Ab;                     // 0x23
    uint8_t hr2_A;                      // 0x24
    uint8_t hr2_Bb;                     // 0x25
    uint8_t hr2_B;                      // 0x26
} gt1000_fx_harmonist_t;

typedef ALIGNED_EFFECT_BLOCK {
    uint8_t mode;                       // 0x00: MODE (0-1) [PICKING, AUTO]
    uint8_t vowel1;                     // 0x01: VOWEL1 (0-4) [a, e, i, o, u]
    uint8_t vowel2;                     // 0x02: VOWEL2 (0-4) [a, e, i, o, u]
    uint8_t sens;                       // 0x03: SENS (0-100)
    uint8_t rate;                       // 0x04: RATE (0-118)
                                        // [0-100, DoubleWholeNote, DottedWholeNote, WholeNote,
                                        //  DottedHalfNote, TripletWholeNote, HalfNote,
                                        //  DottedQuarterNote, TripletHalfNote, QuarterNote,
                                        //  Dotted8thNote, TripletQuarterNote, 8thNote,
                                        //  Dotted16thNote, Triplet8thNote, 16thNote,
                                        //  Dotted32ndNote, Triplet16thNote, 32ndNote]
    uint8_t depth;                      // 0x05: DEPTH (0-100)
    uint8_t manual;                     // 0x06: MANUAL (0-100)
    uint8_t level;                      // 0x07: LEVEL (0-100)
} gt1000_fx_humanizer_t;

// Bass effector combined

typedef ALIGNED_EFFECT_BLOCK {
    uint8_t type;                       // 0x00: TYPE (0-1), MONO/POLY
    uint8_t minus_2oct;                 // 0x01: -2OCT (0-100)
    uint8_t minus_1oct;                 // 0x02: -1OCT (0-100)
    uint8_t direct_level;               // 0x03: DIRECT LEVEL (0-100)
    uint8_t range;                      // 0x04: RANGE (0-100)
    uint8_t octave_level;               // 0x05: OCTAVE LEVEL (0-100)
    // Bass
    uint8_t minus_2oct_bass;                 // 0x06: -2OCT (0-100)
    uint8_t minus_1oct_bass;                 // 0x07: -1OCT (0-100)
    uint8_t direct_level_bass;               // 0x08: DIRECT LEVEL (0-100)
} gt1000_fx_octave_t;


typedef ALIGNED_EFFECT_BLOCK {
    uint8_t minus_2oct;                 // 0x06: -2OCT (0-100)
    uint8_t minus_1oct;                 // 0x07: -1OCT (0-100)
    uint8_t direct_level;               // 0x08: DIRECT LEVEL (0-100)
} gt1000_fx_octave_bass_t;

typedef ALIGNED_EFFECT_BLOCK {
    uint8_t lower_level;                // 0x00: LOWER LEVEL (0-100)
    uint8_t upper_level;                // 0x01: UPPER LEVEL (0-100)
    uint8_t unison_level;               // 0x02: UNISON LEVEL (0-100)
    uint8_t direct_level;               // 0x03: DIRECT LEVEL (0-100)
    uint8_t detune;                     // 0x04: DETUNE (0-100)
    uint8_t output_mode;                // 0x05: OUTPUT MODE (0-1), MONO/STEREO
    uint8_t low;                        // 0x06: LOW (14-114, -50–50)
    uint8_t high;                       // 0x07: HIGH (14-114, -50–50)
} gt1000_fx_overtone_t;

typedef ALIGNED_EFFECT_BLOCK {
    uint8_t rate;                       // 0x00: RATE (0-118) [0-100 + note values]
    uint8_t depth;                      // 0x01: DEPTH (0-100)
    uint8_t waveform;                   // 0x02: WAVEFORM (0-100)
    uint8_t effect_level;               // 0x03: EFFECT LEVEL (0-100)
    uint8_t direct_mix;                 // 0x04: DIRECT MIX (0-100)
} gt1000_fx_pan_t;

typedef ALIGNED_EFFECT_BLOCK {
    uint8_t type;                       // 0x00: TYPE (0-1), PRIME/SCRIPT
    uint8_t stage;                      // 0x01: STAGE (0-4), 2/4/8/16/24 STAGE
    uint8_t rate;                       // 0x02: RATE (0-118) [0-100 + note values]
    uint8_t depth;                      // 0x03: DEPTH (0-100)
    uint8_t resonance;                  // 0x04: RESONANCE (0-100)
    uint8_t manual;                     // 0x05: MANUAL (0-100)
    uint8_t waveform;                   // 0x06: WAVEFORM (0-1), TRI/SINE
    uint8_t step_rate;                  // 0x07: STEP RATE (0-119) [OFF, 0-100 + note values]
    bool biphase;                       // 0x08: BI-PHASE (0-1), OFF/ON
    uint8_t separation;                 // 0x09: SEPARATION (0-12), (0, 15–180)
    // 2 Byte value
    uint16_t low_damp;                  // 0x0A-0x0B: LOW DAMP (28-128, -100–0)
    // 2 Byte value
    uint16_t high_damp;                 // 0x0C-0x0D: HIGH DAMP (28-128, -100–0)
    uint8_t low_cut;                    // 0x0E: LOW CUT (0-31), FLAT–20kHz
    uint8_t high_cut;                   // 0x0F: HIGH CUT (0-31), 20Hz–FLAT
    uint8_t effect_level;               // 0x10: EFFECT LEVEL (0-100)
    uint8_t direct_mix;                 // 0x11: DIRECT MIX (0-100)
} gt1000_fx_phaser_t;

typedef ALIGNED_EFFECT_BLOCK {
    uint8_t voice;                      // 0x00: VOICE (0-2) [1VOICE, 2MONO, 2STEREO]
    uint8_t ps1_pitch;                  // 0x01: PS1 PITCH (8-56, -24–24 semitones)
    uint8_t ps2_pitch;                  // 0x02: PS2 PITCH (8-56, -24–24 semitones)
    uint8_t direct_level;               // 0x03: DIRECT LEVEL (0-100)
    uint8_t ps1_mode;                   // 0x04: PS1 MODE (0-3) [FAST, MEDIUM, SLOW, MONO]
    uint8_t ps1_fine;                   // 0x05: PS1 FINE (14-114, -50–50 cents)
    // 4 Byte value
    uint8_t ps1_pre_delay;              // 0x06-0x09: PS1 PRE-DELAY (0-318, 0–300ms or note values)
    uint8_t ps1_level;                  // 0x0A: PS1 LEVEL (0-100)
    uint8_t ps1_feedback;               // 0x0B: PS1 FEEDBACK (0-100)
    uint8_t ps2_mode;                   // 0x0C: PS2 MODE (0-3) [FAST, MEDIUM, SLOW, MONO]
    uint8_t ps2_fine;                   // 0x0D: PS2 FINE (14-114, -50–50 cents)
    // 4 Byte value
    uint8_t ps2_pre_delay;              // 0x0E-0x11: PS2 PRE-DELAY (0-318, 0–300ms or note values)
    uint8_t ps2_level;                  // 0x12: PS2 LEVEL (0-100)
} gt1000_fx_pitchshift_t;

typedef ALIGNED_EFFECT_BLOCK {
    bool intelligent;                   // 0x00: INTELLIGENT (0-1), OFF/ON
    uint8_t frequency;                  // 0x01: FREQUENCY (0-100)
    uint8_t freq_mod_rate;              // 0x02: FREQ MOD RATE (0-118) [0-100 + note values]
    uint8_t freq_mod_depth;             // 0x03: FREQ MOD DEPTH (0-100)
    uint8_t effect_level;               // 0x04: EFFECT LEVEL (0-100)
    uint8_t direct_mix;                 // 0x05: DIRECT MIX (0-100)
} gt1000_fx_ringmod_t;

typedef ALIGNED_EFFECT_BLOCK {
    uint8_t speed_select;               // 0x00: SPEED SELECT (0-1), SLOW/FAST
    uint8_t slow_rate;                  // 0x01: SLOW RATE (0-118) [0-100 + note values]
    uint8_t fast_rate;                  // 0x02: FAST RATE (0-118) [0-100 + note values]
    uint8_t effect_level;               // 0x03: EFFECT LEVEL (0-100)
    uint8_t rise_time;                  // 0x04: RISE TIME (0-100)
    uint8_t fall_time;                  // 0x05: FALL TIME (0-100)
    uint8_t mic_distance;               // 0x06: MIC DISTANCE (0-100)
    uint8_t rotor_horn;                 // 0x07: ROTOR/HORN (0-100) [100:0 ~ 0:100]
    uint8_t drive;                      // 0x08: DRIVE (0-100)
    uint8_t direct_mix;                 // 0x09: DIRECT MIX (0-100)
} gt1000_fx_rotary_t;

typedef ALIGNED_EFFECT_BLOCK {
    uint8_t sens;                       // 0x00: SENS (0-100)
    uint8_t depth;                      // 0x01: DEPTH (0-100)
    uint8_t tone;                       // 0x02: TONE (14-114) [-50 ~ 50]
    uint8_t effect_level;               // 0x03: EFFECT LEVEL (0-100)
    uint8_t resonance;                  // 0x04: RESONANCE (0-100)
    uint8_t buzz;                       // 0x05: BUZZ (0-100)
    uint8_t direct_mix;                 // 0x06: DIRECT MIX (0-100)
} gt1000_fx_sitarsim_t;

typedef ALIGNED_EFFECT_BLOCK {
    uint8_t pattern;                    // 0x00: PATTERN (0-19) P1-P20
    uint8_t rate;                       // 0x01: RATE (0-118) [0-100 + note values]
    bool trigger;                       // 0x02: TRIGGER (0-1), OFF/ON
    uint8_t effect_level;               // 0x03: EFFECT LEVEL (0-100)
    uint8_t attack;                     // 0x04: ATTACK (0-100)
    uint8_t duty;                       // 0x05: DUTY (1-99)
    uint8_t direct_mix;                 // 0x06: DIRECT MIX (0-100)
} gt1000_fx_slicer_t;

// Bass effector combined

typedef ALIGNED_EFFECT_BLOCK {
    uint8_t sens;                       // 0x00: SENS (0-100)
    uint8_t rise_time;                  // 0x01: RISE TIME (0-100)
    uint8_t level;                      // 0x02: LEVEL (0-100)
    // Bass
    uint8_t sens_bass;                       // 0x03: SENS (0-100)
    uint8_t rise_time_bass;                  // 0x04: RISE TIME (0-100)
    uint8_t level_bass;                      // 0x05: LEVEL (0-100)
} gt1000_fx_slowgear_t;

typedef ALIGNED_EFFECT_BLOCK {
    uint8_t sens;                       // 0x03: SENS (0-100)
    uint8_t rise_time;                  // 0x04: RISE TIME (0-100)
    uint8_t level;                      // 0x05: LEVEL (0-100)
} gt1000_fx_slowgearbass_t;

typedef ALIGNED_EFFECT_BLOCK {
    bool trigger;                       // 0x00: TRIGGER (0-1), OFF/ON
    uint8_t rise_time;                  // 0x01: RISE TIME (0-100)
    uint8_t effect_level;               // 0x02: EFFECT LEVEL (0-120)
} gt1000_fx_soundhold_t;

typedef ALIGNED_EFFECT_BLOCK {
    bool trigger;                       // 0x00: TRIGGER (0-1), OFF/ON
    uint8_t pitch;                      // 0x01: PITCH (0-6), -3OCT ~ +4OCT
    uint8_t rise_time;                  // 0x02: RISE TIME (0-100)
    uint8_t fall_time;                  // 0x03: FALL TIME (0-100)
} gt1000_fx_sbend_t;

// Bass effector combined

typedef ALIGNED_EFFECT_BLOCK {
    uint8_t filter_mode;                // 0x00: FILTER MODE (0-2), LPF/BPF/HPF
    uint8_t polarity;                   // 0x01: POLARITY (0-1), DOWN/UP
    uint8_t sens;                       // 0x02: SENS (0-100)
    uint8_t frequency;                  // 0x03: FREQUENCY (0-100)
    uint8_t resonance;                  // 0x04: RESONANCE (0-100)
    uint8_t decay;                      // 0x05: DECAY (0-100)
    uint8_t effect_level;               // 0x06: EFFECT LEVEL (0-100)
    uint8_t direct_mix;                 // 0x07: DIRECT MIX (0-100)
    // Bass
    uint8_t filter_mode_bass;                // 0x08: FILTER MODE (0-1), LPF/BPF
    uint8_t polarity_bass;                   // 0x09: POLARITY (0-1), DOWN/UP
    uint8_t sens_bass;                       // 0x0A: SENS (0-100)
    uint8_t frequency_bass;                  // 0x0B: FREQUENCY (0-100)
    uint8_t resonance_bass;                  // 0x0C: RESONANCE (0-100)
    uint8_t decay_bass;                      // 0x0D: DECAY (0-100)
    uint8_t effect_level_bass;               // 0x0E: EFFECT LEVEL (0-100)
    uint8_t direct_mix_bass;                 // 0x0F: DIRECT MIX (0-100)
} gt1000_fx_twah_t;

typedef ALIGNED_EFFECT_BLOCK {
    uint8_t rate;                       // 0x00: RATE (0-118) [0-100 + note values]
    uint8_t depth;                      // 0x01: DEPTH (0-100)
    uint8_t waveform;                   // 0x02: WAVEFORM (0-100)
    uint8_t effect_level;               // 0x03: EFFECT LEVEL (0-100)
    bool trigger;                       // 0x04: TRIGGER (0-1), OFF/ON
    uint8_t rise_time;                  // 0x05: RISE TIME (0-100)
    uint8_t direct_mix;                 // 0x06: DIRECT MIX (0-100)
} gt1000_fx_tremolo_t;

typedef ALIGNED_EFFECT_BLOCK {
    uint8_t rate;                       // 0x00: RATE (0-118) [0-100 + note values]
    uint8_t depth;                      // 0x01: DEPTH (0-100)
    uint8_t color;                      // 0x02: COLOR (0-100)
    uint8_t effect_level;               // 0x03: EFFECT LEVEL (0-100)
    bool trigger;                       // 0x04: TRIGGER (0-1), OFF/ON
    uint8_t rise_time;                  // 0x05: RISE TIME (0-100)
    uint8_t direct_mix;                 // 0x06: DIRECT MIX (0-100)
} gt1000_fx_vibrato_t;




typedef ALIGNED_EFFECT_BLOCK {
    gt1000_comp_t comp;
    gt1000_dist_t dist1;
    gt1000_dist_t dist2;
    gt1000_preamp_t preamp1;
    gt1000_preamp_t preamp2;
    gt1000_ns_t ns1;
    gt1000_ns_t ns2;
    gt1000_eq_t eq1;
    gt1000_eq_t eq2;
    gt1000_eq_t eq3;
    gt1000_eq_t eq4;
    gt1000_delay_t delay1;
    gt1000_delay_t delay2;
    gt1000_delay_t delay3;
    gt1000_delay_t delay4;
    gt1000_mstdelay_t mstdelay;
    gt1000_chorus_t chorus;
    // FX1 start
    gt1000_fx_t fx1;
    gt1000_fx_agsim_t fx1_agsim;
    gt1000_fx_acreso_t fx1_acreso;
    gt1000_fx_awah_t fx1_awah;
    gt1000_fx_chorus_t fx1_chorus;
    gt1000_fx_cvibe_t fx1_cvibe;
    gt1000_fx_comp_t fx1_comp;
    gt1000_fx_defretter_t fx1_defretter;
    gt1000_fx_feedbacker_t fx1_feedbacker;
    gt1000_fx_flanger_t fx1_flanger;
    gt1000_fx_harmonist_t fx1_harmonist;
    gt1000_fx_humanizer_t fx1_humanizer;
    gt1000_fx_octave_t fx1_octave;
    gt1000_fx_overtone_t fx1_overtone;
    gt1000_fx_pan_t fx1_pan;
    gt1000_fx_phaser_t fx1_phaser;
    gt1000_fx_pitchshift_t fx1_pitchshift;
    gt1000_fx_ringmod_t fx1_ringmod;
    gt1000_fx_rotary_t fx1_rotary;
    gt1000_fx_sitarsim_t fx1_sitarsim;
    gt1000_fx_slicer_t fx1_slicer;
    gt1000_fx_slowgear_t fx1_slowgear;
    gt1000_fx_soundhold_t fx1_soundhold;
    gt1000_fx_sbend_t fx1_sbend;
    gt1000_fx_tremolo_t fx1_tremolo;
    gt1000_fx_twah_t fx1_twah;
    gt1000_fx_vibrato_t fx1_vibrato;
    // FX2 start
    gt1000_fx_t fx2;
    gt1000_fx_agsim_t fx2_agsim;
    gt1000_fx_acreso_t fx2_acreso;
    gt1000_fx_awah_t fx2_awah;
    gt1000_fx_chorus_t fx2_chorus;
    gt1000_fx_cvibe_t fx2_cvibe;
    gt1000_fx_comp_t fx2_comp;
    gt1000_fx_defretter_t fx2_defretter;
    gt1000_fx_feedbacker_t fx2_feedbacker;
    gt1000_fx_flanger_t fx2_flanger;
    gt1000_fx_harmonist_t fx2_harmonist;
    gt1000_fx_humanizer_t fx2_humanizer;
    gt1000_fx_octave_t fx2_octave;
    gt1000_fx_overtone_t fx2_overtone;
    gt1000_fx_pan_t fx2_pan;
    gt1000_fx_phaser_t fx2_phaser;
    gt1000_fx_pitchshift_t fx2_pitchshift;
    gt1000_fx_ringmod_t fx2_ringmod;
    gt1000_fx_rotary_t fx2_rotary;
    gt1000_fx_sitarsim_t fx2_sitarsim;
    gt1000_fx_slicer_t fx2_slicer;
    gt1000_fx_slowgear_t fx2_slowgear;
    gt1000_fx_soundhold_t fx2_soundhold;
    gt1000_fx_sbend_t fx2_sbend;
    gt1000_fx_tremolo_t fx2_tremolo;
    gt1000_fx_twah_t fx2_twah;
    gt1000_fx_vibrato_t fx2_vibrato;
    // FX2 start
    gt1000_fx_t fx3;
    gt1000_fx_agsim_t fx3_agsim;
    gt1000_fx_acreso_t fx3_acreso;
    gt1000_fx_awah_t fx3_awah;
    gt1000_fx_chorus_t fx3_chorus;
    gt1000_fx_cvibe_t fx3_cvibe;
    gt1000_fx_comp_t fx3_comp;
    gt1000_fx_defretter_t fx3_defretter;
    gt1000_fx_feedbacker_t fx3_feedbacker;
    gt1000_fx_flanger_t fx3_flanger;
    gt1000_fx_harmonist_t fx3_harmonist;
    gt1000_fx_humanizer_t fx3_humanizer;
    gt1000_fx_octave_t fx3_octave;
    gt1000_fx_overtone_t fx3_overtone;
    gt1000_fx_pan_t fx3_pan;
    gt1000_fx_phaser_t fx3_phaser;
    gt1000_fx_pitchshift_t fx3_pitchshift;
    gt1000_fx_ringmod_t fx3_ringmod;
    gt1000_fx_rotary_t fx3_rotary;
    gt1000_fx_sitarsim_t fx3_sitarsim;
    gt1000_fx_slicer_t fx3_slicer;
    gt1000_fx_slowgear_t fx3_slowgear;
    gt1000_fx_soundhold_t fx3_soundhold;
    gt1000_fx_sbend_t fx3_sbend;
    gt1000_fx_tremolo_t fx3_tremolo;
    gt1000_fx_twah_t fx3_twah;
    gt1000_fx_vibrato_t fx3_vibrato;
    // FX end
    gt1000_reverb_t reverb;
    gt1000_pedalfx_t pedalfx;
} gt1000_effect_t;

typedef struct {
    char patch_name[16];
    uint32_t patch_number;
    gt1000_effect_t effect;
} gt1000_t;

typedef void *gt1000_param_addr_t;

typedef struct {
    const char *effect_block_name;
    const char *parameter_name;
    size_t size;
    uint32_t value;
} gt1000_param_t;

bool gt1000_get_parameter_info(gt1000_param_t *param, gt1000_param_addr_t parameter);

#endif