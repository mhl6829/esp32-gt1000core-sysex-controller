/*
 * SPDX-FileCopyrightText: 2025 mhl6829
 * SPDX-License-Identifier: MIT
 * File: [gt1000_param.c] - GT1000 parameter definition and metadata
 */


#include "freertos/FreeRTOS.h"

#include <string.h>
#include <stddef.h>

#include "esp_log.h"

#include "gt1000.h"
#include "gt1000_param.h"

#define EFFECT_BLOCK_MASK         0xFFFFFF00

#define DEFINE_EFFECT_BLOCK(block, block_name, len, ...) \
{ \
    .name = #block_name, \
    .size = sizeof(block), \
    .param_len = len, \
    .params = { __VA_ARGS__ } \
}

#define DEFINE_PARAM(block, param_name) \
    { .name = #param_name, .size = sizeof(((block*)0)->param_name), .offset = offsetof(block, param_name) }

typedef enum
{
    COMP,
    DIST,
    PREAMP,
    NS,
    EQ,
    DELAY,
    MSTDELAY,
    CHORUS,
    FX,
    FX_AGSIM,
    FX_ACRESO,
    FX_AWAH,
    FX_CHORUS,
    FX_CVIBE,
    FX_COMP,
    FX_DEFRETTER,
    FX_FEEDBACKER,
    FX_FLANGER,
    FX_HARMONIST,
    FX_HUMANIZER,
    FX_OCTAVE,
    FX_OVERTONE,
    FX_PAN,
    FX_PHASER,
    FX_PITCHSHIFT,
    FX_RINGMOD,
    FX_ROTARY,
    FX_SITARSIM,
    FX_SLICER,
    FX_SLOWGEAR,
    FX_SOUNDHOLD,
    FX_SBEND,
    FX_TREMOLO,
    FX_TWAH,
    FX_VIBRATO,
    REVERB,
    PEDALFX
} effect_block_type_t;

typedef struct
{
    char *name;
    effect_block_type_t type;
} effect_block_t;

typedef struct
{
    char *name;
    uint8_t size;
    size_t offset;
} effect_block_parameter_t;

typedef struct
{
    char *name;
    uint16_t size;
    uint8_t param_len;
    effect_block_parameter_t params[64];
} effect_block_metadata_t;

static const effect_block_metadata_t effect_block_metadata_list[] = {
    
        DEFINE_EFFECT_BLOCK(gt1000_comp_t, COMP, 9,
            DEFINE_PARAM(gt1000_comp_t, sw),
            DEFINE_PARAM(gt1000_comp_t, type),
            DEFINE_PARAM(gt1000_comp_t, sustain),
            DEFINE_PARAM(gt1000_comp_t, attack),
            DEFINE_PARAM(gt1000_comp_t, level),
            DEFINE_PARAM(gt1000_comp_t, tone),
            DEFINE_PARAM(gt1000_comp_t, ratio),
            DEFINE_PARAM(gt1000_comp_t, direct_mix),
            DEFINE_PARAM(gt1000_comp_t, threshold)
        ),
    
        DEFINE_EFFECT_BLOCK(gt1000_dist_t, DIST, 9,
            DEFINE_PARAM(gt1000_dist_t, sw),
            DEFINE_PARAM(gt1000_dist_t, type),
            DEFINE_PARAM(gt1000_dist_t, drive),
            DEFINE_PARAM(gt1000_dist_t, tone),
            DEFINE_PARAM(gt1000_dist_t, level),
            DEFINE_PARAM(gt1000_dist_t, bottom),
            DEFINE_PARAM(gt1000_dist_t, direct_mix),
            DEFINE_PARAM(gt1000_dist_t, solo_sw),
            DEFINE_PARAM(gt1000_dist_t, solo_level)
        ),
    
        DEFINE_EFFECT_BLOCK(gt1000_preamp_t, PREAMP, 14,
            DEFINE_PARAM(gt1000_preamp_t, sw),
            DEFINE_PARAM(gt1000_preamp_t, type),
            DEFINE_PARAM(gt1000_preamp_t, gain),
            DEFINE_PARAM(gt1000_preamp_t, sag),
            DEFINE_PARAM(gt1000_preamp_t, resonance),
            DEFINE_PARAM(gt1000_preamp_t, level),
            DEFINE_PARAM(gt1000_preamp_t, bass),
            DEFINE_PARAM(gt1000_preamp_t, middle),
            DEFINE_PARAM(gt1000_preamp_t, treble),
            DEFINE_PARAM(gt1000_preamp_t, presence),
            DEFINE_PARAM(gt1000_preamp_t, bright),
            DEFINE_PARAM(gt1000_preamp_t, gain_sw),
            DEFINE_PARAM(gt1000_preamp_t, solo_sw),
            DEFINE_PARAM(gt1000_preamp_t, solo_level)
        ),
    
        DEFINE_EFFECT_BLOCK(gt1000_ns_t, NS, 4,
            DEFINE_PARAM(gt1000_ns_t, sw),
            DEFINE_PARAM(gt1000_ns_t, threshold),
            DEFINE_PARAM(gt1000_ns_t, release),
            DEFINE_PARAM(gt1000_ns_t, detect)
        ),
    
        DEFINE_EFFECT_BLOCK(gt1000_eq_t, EQ, 24,
            DEFINE_PARAM(gt1000_eq_t, sw),
            DEFINE_PARAM(gt1000_eq_t, type),
            DEFINE_PARAM(gt1000_eq_t, low_gain),
            DEFINE_PARAM(gt1000_eq_t, high_gain),
            DEFINE_PARAM(gt1000_eq_t, level),
            DEFINE_PARAM(gt1000_eq_t, low_mid_freq),
            DEFINE_PARAM(gt1000_eq_t, low_mid_q),
            DEFINE_PARAM(gt1000_eq_t, low_mid_gain),
            DEFINE_PARAM(gt1000_eq_t, high_mid_freq),
            DEFINE_PARAM(gt1000_eq_t, high_mid_q),
            DEFINE_PARAM(gt1000_eq_t, high_mid_gain),
            DEFINE_PARAM(gt1000_eq_t, low_cut),
            DEFINE_PARAM(gt1000_eq_t, high_cut),
            DEFINE_PARAM(gt1000_eq_t, graphic_level),
            DEFINE_PARAM(gt1000_eq_t, band_31hz),
            DEFINE_PARAM(gt1000_eq_t, band_63hz),
            DEFINE_PARAM(gt1000_eq_t, band_125hz),
            DEFINE_PARAM(gt1000_eq_t, band_250hz),
            DEFINE_PARAM(gt1000_eq_t, band_500hz),
            DEFINE_PARAM(gt1000_eq_t, band_1k),
            DEFINE_PARAM(gt1000_eq_t, band_2k),
            DEFINE_PARAM(gt1000_eq_t, band_4k),
            DEFINE_PARAM(gt1000_eq_t, band_8k),
            DEFINE_PARAM(gt1000_eq_t, band_16k)
        ),
    
        DEFINE_EFFECT_BLOCK(gt1000_delay_t, DELAY, 6,
            DEFINE_PARAM(gt1000_delay_t, sw),
            DEFINE_PARAM(gt1000_delay_t, time),
            DEFINE_PARAM(gt1000_delay_t, feedback),
            DEFINE_PARAM(gt1000_delay_t, high_cut),
            DEFINE_PARAM(gt1000_delay_t, effect_level),
            DEFINE_PARAM(gt1000_delay_t, direct_level)
        ),
    
        DEFINE_EFFECT_BLOCK(gt1000_mstdelay_t, MSTDELAY, 41,
            DEFINE_PARAM(gt1000_mstdelay_t, sw),
            DEFINE_PARAM(gt1000_mstdelay_t, type),
            DEFINE_PARAM(gt1000_mstdelay_t, time),
            DEFINE_PARAM(gt1000_mstdelay_t, feedback),
            DEFINE_PARAM(gt1000_mstdelay_t, high_cut),
            DEFINE_PARAM(gt1000_mstdelay_t, effect_level),
            DEFINE_PARAM(gt1000_mstdelay_t, mod_rate),
            DEFINE_PARAM(gt1000_mstdelay_t, mod_depth),
            DEFINE_PARAM(gt1000_mstdelay_t, duck_sens),
            DEFINE_PARAM(gt1000_mstdelay_t, duck_pre_depth),
            DEFINE_PARAM(gt1000_mstdelay_t, duck_post_depth),
            DEFINE_PARAM(gt1000_mstdelay_t, direct_level),
            DEFINE_PARAM(gt1000_mstdelay_t, pitch),
            DEFINE_PARAM(gt1000_mstdelay_t, pitch_bal),
            DEFINE_PARAM(gt1000_mstdelay_t, pitch_feedback),
            DEFINE_PARAM(gt1000_mstdelay_t, dual_mode),
            DEFINE_PARAM(gt1000_mstdelay_t, d1_type),
            DEFINE_PARAM(gt1000_mstdelay_t, d1_time),
            DEFINE_PARAM(gt1000_mstdelay_t, d1_feedback),
            DEFINE_PARAM(gt1000_mstdelay_t, d1_high_cut),
            DEFINE_PARAM(gt1000_mstdelay_t, d1_effect_level),
            DEFINE_PARAM(gt1000_mstdelay_t, d2_type),
            DEFINE_PARAM(gt1000_mstdelay_t, d2_time),
            DEFINE_PARAM(gt1000_mstdelay_t, d2_feedback),
            DEFINE_PARAM(gt1000_mstdelay_t, d2_high_cut),
            DEFINE_PARAM(gt1000_mstdelay_t, d2_effect_level),
            DEFINE_PARAM(gt1000_mstdelay_t, twist_mode),
            DEFINE_PARAM(gt1000_mstdelay_t, trigger),
            DEFINE_PARAM(gt1000_mstdelay_t, rise_time),
            DEFINE_PARAM(gt1000_mstdelay_t, fall_time),
            DEFINE_PARAM(gt1000_mstdelay_t, level),
            DEFINE_PARAM(gt1000_mstdelay_t, stage),
            DEFINE_PARAM(gt1000_mstdelay_t, head),
            DEFINE_PARAM(gt1000_mstdelay_t, fade_time),
            DEFINE_PARAM(gt1000_mstdelay_t, tap_time),
            DEFINE_PARAM(gt1000_mstdelay_t, wow_flutter),
            DEFINE_PARAM(gt1000_mstdelay_t, drum_echo_head),
            DEFINE_PARAM(gt1000_mstdelay_t, selector),
            DEFINE_PARAM(gt1000_mstdelay_t, auto_trigger),
            DEFINE_PARAM(gt1000_mstdelay_t, duty),
            DEFINE_PARAM(gt1000_mstdelay_t, tone)
        ),
    
        DEFINE_EFFECT_BLOCK(gt1000_chorus_t, CHORUS, 25,
            DEFINE_PARAM(gt1000_chorus_t, sw),
            DEFINE_PARAM(gt1000_chorus_t, type),
            DEFINE_PARAM(gt1000_chorus_t, rate),
            DEFINE_PARAM(gt1000_chorus_t, depth),
            DEFINE_PARAM(gt1000_chorus_t, pre_delay),
            DEFINE_PARAM(gt1000_chorus_t, effect_level),
            DEFINE_PARAM(gt1000_chorus_t, waveform),
            DEFINE_PARAM(gt1000_chorus_t, low_cut),
            DEFINE_PARAM(gt1000_chorus_t, high_cut),
            DEFINE_PARAM(gt1000_chorus_t, rate1),
            DEFINE_PARAM(gt1000_chorus_t, depth1),
            DEFINE_PARAM(gt1000_chorus_t, pre_delay1),
            DEFINE_PARAM(gt1000_chorus_t, effect_level1),
            DEFINE_PARAM(gt1000_chorus_t, waveform1),
            DEFINE_PARAM(gt1000_chorus_t, low_cut1),
            DEFINE_PARAM(gt1000_chorus_t, high_cut1),
            DEFINE_PARAM(gt1000_chorus_t, rate2),
            DEFINE_PARAM(gt1000_chorus_t, depth2),
            DEFINE_PARAM(gt1000_chorus_t, pre_delay2),
            DEFINE_PARAM(gt1000_chorus_t, effect_level2),
            DEFINE_PARAM(gt1000_chorus_t, waveform2),
            DEFINE_PARAM(gt1000_chorus_t, low_cut2),
            DEFINE_PARAM(gt1000_chorus_t, high_cut2),
            DEFINE_PARAM(gt1000_chorus_t, direct_level),
            DEFINE_PARAM(gt1000_chorus_t, output_mode)
        ),
    
        DEFINE_EFFECT_BLOCK(gt1000_fx_t, FX, 2,
            DEFINE_PARAM(gt1000_fx_t, fx_sw),
            DEFINE_PARAM(gt1000_fx_t, fx_type)
        ),
    
        DEFINE_EFFECT_BLOCK(gt1000_fx_agsim_t, FX_AGSIM, 4,
            DEFINE_PARAM(gt1000_fx_agsim_t, body),
            DEFINE_PARAM(gt1000_fx_agsim_t, low),
            DEFINE_PARAM(gt1000_fx_agsim_t, high),
            DEFINE_PARAM(gt1000_fx_agsim_t, level)
        ),
    
        DEFINE_EFFECT_BLOCK(gt1000_fx_acreso_t, FX_ACRESO, 4,
            DEFINE_PARAM(gt1000_fx_acreso_t, type),
            DEFINE_PARAM(gt1000_fx_acreso_t, resonance),
            DEFINE_PARAM(gt1000_fx_acreso_t, tone),
            DEFINE_PARAM(gt1000_fx_acreso_t, level)
        ),
    
        DEFINE_EFFECT_BLOCK(gt1000_fx_awah_t, FX_AWAH, 8,
            DEFINE_PARAM(gt1000_fx_awah_t, filter_mode),
            DEFINE_PARAM(gt1000_fx_awah_t, rate),
            DEFINE_PARAM(gt1000_fx_awah_t, depth),
            DEFINE_PARAM(gt1000_fx_awah_t, effect_level),
            DEFINE_PARAM(gt1000_fx_awah_t, frequency),
            DEFINE_PARAM(gt1000_fx_awah_t, resonance),
            DEFINE_PARAM(gt1000_fx_awah_t, waveform),
            DEFINE_PARAM(gt1000_fx_awah_t, direct_mix)
        ),
    
        DEFINE_EFFECT_BLOCK(gt1000_fx_chorus_t, FX_CHORUS, 29,
            DEFINE_PARAM(gt1000_fx_chorus_t, type),
            DEFINE_PARAM(gt1000_fx_chorus_t, direct_level),
            DEFINE_PARAM(gt1000_fx_chorus_t, output_mode),
            DEFINE_PARAM(gt1000_fx_chorus_t, sweetness),
            DEFINE_PARAM(gt1000_fx_chorus_t, bell),
            DEFINE_PARAM(gt1000_fx_chorus_t, preamp_sw),
            DEFINE_PARAM(gt1000_fx_chorus_t, preamp_gain),
            DEFINE_PARAM(gt1000_fx_chorus_t, preamp_level),
            DEFINE_PARAM(gt1000_fx_chorus_t, rate),
            DEFINE_PARAM(gt1000_fx_chorus_t, depth),
            DEFINE_PARAM(gt1000_fx_chorus_t, pre_delay),
            DEFINE_PARAM(gt1000_fx_chorus_t, effect_level),
            DEFINE_PARAM(gt1000_fx_chorus_t, waveform),
            DEFINE_PARAM(gt1000_fx_chorus_t, low_cut),
            DEFINE_PARAM(gt1000_fx_chorus_t, high_cut),
            DEFINE_PARAM(gt1000_fx_chorus_t, rate1),
            DEFINE_PARAM(gt1000_fx_chorus_t, depth1),
            DEFINE_PARAM(gt1000_fx_chorus_t, pre_delay1),
            DEFINE_PARAM(gt1000_fx_chorus_t, effect_level1),
            DEFINE_PARAM(gt1000_fx_chorus_t, waveform1),
            DEFINE_PARAM(gt1000_fx_chorus_t, low_cut1),
            DEFINE_PARAM(gt1000_fx_chorus_t, high_cut1),
            DEFINE_PARAM(gt1000_fx_chorus_t, rate2),
            DEFINE_PARAM(gt1000_fx_chorus_t, depth2),
            DEFINE_PARAM(gt1000_fx_chorus_t, pre_delay2),
            DEFINE_PARAM(gt1000_fx_chorus_t, effect_level2),
            DEFINE_PARAM(gt1000_fx_chorus_t, waveform2),
            DEFINE_PARAM(gt1000_fx_chorus_t, low_cut2),
            DEFINE_PARAM(gt1000_fx_chorus_t, high_cut2)
        ),
    
        DEFINE_EFFECT_BLOCK(gt1000_fx_cvibe_t, FX_CVIBE, 4,
            DEFINE_PARAM(gt1000_fx_cvibe_t, mode),
            DEFINE_PARAM(gt1000_fx_cvibe_t, rate),
            DEFINE_PARAM(gt1000_fx_cvibe_t, depth),
            DEFINE_PARAM(gt1000_fx_cvibe_t, effect_level)
        ),
    
        DEFINE_EFFECT_BLOCK(gt1000_fx_comp_t, FX_COMP, 8,
            DEFINE_PARAM(gt1000_fx_comp_t, type),
            DEFINE_PARAM(gt1000_fx_comp_t, sustain),
            DEFINE_PARAM(gt1000_fx_comp_t, attack),
            DEFINE_PARAM(gt1000_fx_comp_t, level),
            DEFINE_PARAM(gt1000_fx_comp_t, tone),
            DEFINE_PARAM(gt1000_fx_comp_t, ratio),
            DEFINE_PARAM(gt1000_fx_comp_t, direct_mix),
            DEFINE_PARAM(gt1000_fx_comp_t, threshold)
        ),
    
        DEFINE_EFFECT_BLOCK(gt1000_fx_defretter_t, FX_DEFRETTER, 12,
            DEFINE_PARAM(gt1000_fx_defretter_t, sens),
            DEFINE_PARAM(gt1000_fx_defretter_t, depth),
            DEFINE_PARAM(gt1000_fx_defretter_t, tone),
            DEFINE_PARAM(gt1000_fx_defretter_t, effect_level),
            DEFINE_PARAM(gt1000_fx_defretter_t, attack),
            DEFINE_PARAM(gt1000_fx_defretter_t, resonance),
            DEFINE_PARAM(gt1000_fx_defretter_t, direct_mix),
            DEFINE_PARAM(gt1000_fx_defretter_t, sens_bass),
            DEFINE_PARAM(gt1000_fx_defretter_t, attack_bass),
            DEFINE_PARAM(gt1000_fx_defretter_t, tone_bass),
            DEFINE_PARAM(gt1000_fx_defretter_t, effect_level_bass),
            DEFINE_PARAM(gt1000_fx_defretter_t, direct_mix_bass)
        ),
    
        DEFINE_EFFECT_BLOCK(gt1000_fx_feedbacker_t, FX_FEEDBACKER, 9,
            DEFINE_PARAM(gt1000_fx_feedbacker_t, mode),
            DEFINE_PARAM(gt1000_fx_feedbacker_t, trigger),
            DEFINE_PARAM(gt1000_fx_feedbacker_t, depth),
            DEFINE_PARAM(gt1000_fx_feedbacker_t, rise_time),
            DEFINE_PARAM(gt1000_fx_feedbacker_t, oct_rise_time),
            DEFINE_PARAM(gt1000_fx_feedbacker_t, feedback),
            DEFINE_PARAM(gt1000_fx_feedbacker_t, oct_feedback),
            DEFINE_PARAM(gt1000_fx_feedbacker_t, vib_rate),
            DEFINE_PARAM(gt1000_fx_feedbacker_t, vib_depth)
        ),
    
        DEFINE_EFFECT_BLOCK(gt1000_fx_flanger_t, FX_FLANGER, 14,
            DEFINE_PARAM(gt1000_fx_flanger_t, rate),
            DEFINE_PARAM(gt1000_fx_flanger_t, depth),
            DEFINE_PARAM(gt1000_fx_flanger_t, resonance),
            DEFINE_PARAM(gt1000_fx_flanger_t, manual),
            DEFINE_PARAM(gt1000_fx_flanger_t, turbo),
            DEFINE_PARAM(gt1000_fx_flanger_t, waveform),
            DEFINE_PARAM(gt1000_fx_flanger_t, step_rate),
            DEFINE_PARAM(gt1000_fx_flanger_t, separation),
            DEFINE_PARAM(gt1000_fx_flanger_t, effect_level),
            DEFINE_PARAM(gt1000_fx_flanger_t, low_damp),
            DEFINE_PARAM(gt1000_fx_flanger_t, high_damp),
            DEFINE_PARAM(gt1000_fx_flanger_t, low_cut),
            DEFINE_PARAM(gt1000_fx_flanger_t, high_cut),
            DEFINE_PARAM(gt1000_fx_flanger_t, direct_mix)
        ),
    
        DEFINE_EFFECT_BLOCK(gt1000_fx_harmonist_t, FX_HARMONIST, 33,
            DEFINE_PARAM(gt1000_fx_harmonist_t, voice),
            DEFINE_PARAM(gt1000_fx_harmonist_t, hr1_harmony),
            DEFINE_PARAM(gt1000_fx_harmonist_t, hr2_harmony),
            DEFINE_PARAM(gt1000_fx_harmonist_t, hr1_level),
            DEFINE_PARAM(gt1000_fx_harmonist_t, hr1_pre_delay),
            DEFINE_PARAM(gt1000_fx_harmonist_t, hr1_feedback),
            DEFINE_PARAM(gt1000_fx_harmonist_t, direct_level),
            DEFINE_PARAM(gt1000_fx_harmonist_t, hr2_level),
            DEFINE_PARAM(gt1000_fx_harmonist_t, hr2_pre_delay),
            DEFINE_PARAM(gt1000_fx_harmonist_t, hr1_C),
            DEFINE_PARAM(gt1000_fx_harmonist_t, hr1_Db),
            DEFINE_PARAM(gt1000_fx_harmonist_t, hr1_D),
            DEFINE_PARAM(gt1000_fx_harmonist_t, hr1_Eb),
            DEFINE_PARAM(gt1000_fx_harmonist_t, hr1_E),
            DEFINE_PARAM(gt1000_fx_harmonist_t, hr1_F),
            DEFINE_PARAM(gt1000_fx_harmonist_t, hr1_Fs),
            DEFINE_PARAM(gt1000_fx_harmonist_t, hr1_G),
            DEFINE_PARAM(gt1000_fx_harmonist_t, hr1_Ab),
            DEFINE_PARAM(gt1000_fx_harmonist_t, hr1_A),
            DEFINE_PARAM(gt1000_fx_harmonist_t, hr1_Bb),
            DEFINE_PARAM(gt1000_fx_harmonist_t, hr1_B),
            DEFINE_PARAM(gt1000_fx_harmonist_t, hr2_C),
            DEFINE_PARAM(gt1000_fx_harmonist_t, hr2_Db),
            DEFINE_PARAM(gt1000_fx_harmonist_t, hr2_D),
            DEFINE_PARAM(gt1000_fx_harmonist_t, hr2_Eb),
            DEFINE_PARAM(gt1000_fx_harmonist_t, hr2_E),
            DEFINE_PARAM(gt1000_fx_harmonist_t, hr2_F),
            DEFINE_PARAM(gt1000_fx_harmonist_t, hr2_Fs),
            DEFINE_PARAM(gt1000_fx_harmonist_t, hr2_G),
            DEFINE_PARAM(gt1000_fx_harmonist_t, hr2_Ab),
            DEFINE_PARAM(gt1000_fx_harmonist_t, hr2_A),
            DEFINE_PARAM(gt1000_fx_harmonist_t, hr2_Bb),
            DEFINE_PARAM(gt1000_fx_harmonist_t, hr2_B)
        ),
    
        DEFINE_EFFECT_BLOCK(gt1000_fx_humanizer_t, FX_HUMANIZER, 8,
            DEFINE_PARAM(gt1000_fx_humanizer_t, mode),
            DEFINE_PARAM(gt1000_fx_humanizer_t, vowel1),
            DEFINE_PARAM(gt1000_fx_humanizer_t, vowel2),
            DEFINE_PARAM(gt1000_fx_humanizer_t, sens),
            DEFINE_PARAM(gt1000_fx_humanizer_t, rate),
            DEFINE_PARAM(gt1000_fx_humanizer_t, depth),
            DEFINE_PARAM(gt1000_fx_humanizer_t, manual),
            DEFINE_PARAM(gt1000_fx_humanizer_t, level)
        ),
    
        DEFINE_EFFECT_BLOCK(gt1000_fx_octave_t, FX_OCTAVE, 9,
            DEFINE_PARAM(gt1000_fx_octave_t, type),
            DEFINE_PARAM(gt1000_fx_octave_t, minus_2oct),
            DEFINE_PARAM(gt1000_fx_octave_t, minus_1oct),
            DEFINE_PARAM(gt1000_fx_octave_t, direct_level),
            DEFINE_PARAM(gt1000_fx_octave_t, range),
            DEFINE_PARAM(gt1000_fx_octave_t, octave_level),
            DEFINE_PARAM(gt1000_fx_octave_t, minus_2oct_bass),
            DEFINE_PARAM(gt1000_fx_octave_t, minus_1oct_bass),
            DEFINE_PARAM(gt1000_fx_octave_t, direct_level_bass)
        ),
    
        DEFINE_EFFECT_BLOCK(gt1000_fx_overtone_t, FX_OVERTONE, 8,
            DEFINE_PARAM(gt1000_fx_overtone_t, lower_level),
            DEFINE_PARAM(gt1000_fx_overtone_t, upper_level),
            DEFINE_PARAM(gt1000_fx_overtone_t, unison_level),
            DEFINE_PARAM(gt1000_fx_overtone_t, direct_level),
            DEFINE_PARAM(gt1000_fx_overtone_t, detune),
            DEFINE_PARAM(gt1000_fx_overtone_t, output_mode),
            DEFINE_PARAM(gt1000_fx_overtone_t, low),
            DEFINE_PARAM(gt1000_fx_overtone_t, high)
        ),
    
        DEFINE_EFFECT_BLOCK(gt1000_fx_pan_t, FX_PAN, 5,
            DEFINE_PARAM(gt1000_fx_pan_t, rate),
            DEFINE_PARAM(gt1000_fx_pan_t, depth),
            DEFINE_PARAM(gt1000_fx_pan_t, waveform),
            DEFINE_PARAM(gt1000_fx_pan_t, effect_level),
            DEFINE_PARAM(gt1000_fx_pan_t, direct_mix)
        ),
    
        DEFINE_EFFECT_BLOCK(gt1000_fx_phaser_t, FX_PHASER, 16,
            DEFINE_PARAM(gt1000_fx_phaser_t, type),
            DEFINE_PARAM(gt1000_fx_phaser_t, stage),
            DEFINE_PARAM(gt1000_fx_phaser_t, rate),
            DEFINE_PARAM(gt1000_fx_phaser_t, depth),
            DEFINE_PARAM(gt1000_fx_phaser_t, resonance),
            DEFINE_PARAM(gt1000_fx_phaser_t, manual),
            DEFINE_PARAM(gt1000_fx_phaser_t, waveform),
            DEFINE_PARAM(gt1000_fx_phaser_t, step_rate),
            DEFINE_PARAM(gt1000_fx_phaser_t, biphase),
            DEFINE_PARAM(gt1000_fx_phaser_t, separation),
            DEFINE_PARAM(gt1000_fx_phaser_t, low_damp),
            DEFINE_PARAM(gt1000_fx_phaser_t, high_damp),
            DEFINE_PARAM(gt1000_fx_phaser_t, low_cut),
            DEFINE_PARAM(gt1000_fx_phaser_t, high_cut),
            DEFINE_PARAM(gt1000_fx_phaser_t, effect_level),
            DEFINE_PARAM(gt1000_fx_phaser_t, direct_mix)
        ),
    
        DEFINE_EFFECT_BLOCK(gt1000_fx_pitchshift_t, FX_PITCHSHIFT, 13,
            DEFINE_PARAM(gt1000_fx_pitchshift_t, voice),
            DEFINE_PARAM(gt1000_fx_pitchshift_t, ps1_pitch),
            DEFINE_PARAM(gt1000_fx_pitchshift_t, ps2_pitch),
            DEFINE_PARAM(gt1000_fx_pitchshift_t, direct_level),
            DEFINE_PARAM(gt1000_fx_pitchshift_t, ps1_mode),
            DEFINE_PARAM(gt1000_fx_pitchshift_t, ps1_fine),
            DEFINE_PARAM(gt1000_fx_pitchshift_t, ps1_pre_delay),
            DEFINE_PARAM(gt1000_fx_pitchshift_t, ps1_level),
            DEFINE_PARAM(gt1000_fx_pitchshift_t, ps1_feedback),
            DEFINE_PARAM(gt1000_fx_pitchshift_t, ps2_mode),
            DEFINE_PARAM(gt1000_fx_pitchshift_t, ps2_fine),
            DEFINE_PARAM(gt1000_fx_pitchshift_t, ps2_pre_delay),
            DEFINE_PARAM(gt1000_fx_pitchshift_t, ps2_level)
        ),
    
        DEFINE_EFFECT_BLOCK(gt1000_fx_ringmod_t, FX_RINGMOD, 6,
            DEFINE_PARAM(gt1000_fx_ringmod_t, intelligent),
            DEFINE_PARAM(gt1000_fx_ringmod_t, frequency),
            DEFINE_PARAM(gt1000_fx_ringmod_t, freq_mod_rate),
            DEFINE_PARAM(gt1000_fx_ringmod_t, freq_mod_depth),
            DEFINE_PARAM(gt1000_fx_ringmod_t, effect_level),
            DEFINE_PARAM(gt1000_fx_ringmod_t, direct_mix)
        ),
    
        DEFINE_EFFECT_BLOCK(gt1000_fx_rotary_t, FX_ROTARY, 10,
            DEFINE_PARAM(gt1000_fx_rotary_t, speed_select),
            DEFINE_PARAM(gt1000_fx_rotary_t, slow_rate),
            DEFINE_PARAM(gt1000_fx_rotary_t, fast_rate),
            DEFINE_PARAM(gt1000_fx_rotary_t, effect_level),
            DEFINE_PARAM(gt1000_fx_rotary_t, rise_time),
            DEFINE_PARAM(gt1000_fx_rotary_t, fall_time),
            DEFINE_PARAM(gt1000_fx_rotary_t, mic_distance),
            DEFINE_PARAM(gt1000_fx_rotary_t, rotor_horn),
            DEFINE_PARAM(gt1000_fx_rotary_t, drive),
            DEFINE_PARAM(gt1000_fx_rotary_t, direct_mix)
        ),
    
        DEFINE_EFFECT_BLOCK(gt1000_fx_sitarsim_t, FX_SITARSIM, 7,
            DEFINE_PARAM(gt1000_fx_sitarsim_t, sens),
            DEFINE_PARAM(gt1000_fx_sitarsim_t, depth),
            DEFINE_PARAM(gt1000_fx_sitarsim_t, tone),
            DEFINE_PARAM(gt1000_fx_sitarsim_t, effect_level),
            DEFINE_PARAM(gt1000_fx_sitarsim_t, resonance),
            DEFINE_PARAM(gt1000_fx_sitarsim_t, buzz),
            DEFINE_PARAM(gt1000_fx_sitarsim_t, direct_mix)
        ),
    
        DEFINE_EFFECT_BLOCK(gt1000_fx_slicer_t, FX_SLICER, 7,
            DEFINE_PARAM(gt1000_fx_slicer_t, pattern),
            DEFINE_PARAM(gt1000_fx_slicer_t, rate),
            DEFINE_PARAM(gt1000_fx_slicer_t, trigger),
            DEFINE_PARAM(gt1000_fx_slicer_t, effect_level),
            DEFINE_PARAM(gt1000_fx_slicer_t, attack),
            DEFINE_PARAM(gt1000_fx_slicer_t, duty),
            DEFINE_PARAM(gt1000_fx_slicer_t, direct_mix)
        ),
    
        DEFINE_EFFECT_BLOCK(gt1000_fx_slowgear_t, FX_SLOWGEAR, 6,
            DEFINE_PARAM(gt1000_fx_slowgear_t, sens),
            DEFINE_PARAM(gt1000_fx_slowgear_t, rise_time),
            DEFINE_PARAM(gt1000_fx_slowgear_t, level),
            DEFINE_PARAM(gt1000_fx_slowgear_t, sens_bass),
            DEFINE_PARAM(gt1000_fx_slowgear_t, rise_time_bass),
            DEFINE_PARAM(gt1000_fx_slowgear_t, level_bass)
        ),
    
        DEFINE_EFFECT_BLOCK(gt1000_fx_soundhold_t, FX_SOUNDHOLD, 3,
            DEFINE_PARAM(gt1000_fx_soundhold_t, trigger),
            DEFINE_PARAM(gt1000_fx_soundhold_t, rise_time),
            DEFINE_PARAM(gt1000_fx_soundhold_t, effect_level)
        ),
    
        DEFINE_EFFECT_BLOCK(gt1000_fx_sbend_t, FX_SBEND, 4,
            DEFINE_PARAM(gt1000_fx_sbend_t, trigger),
            DEFINE_PARAM(gt1000_fx_sbend_t, pitch),
            DEFINE_PARAM(gt1000_fx_sbend_t, rise_time),
            DEFINE_PARAM(gt1000_fx_sbend_t, fall_time)
        ),
    
        DEFINE_EFFECT_BLOCK(gt1000_fx_twah_t, FX_TWAH, 16,
            DEFINE_PARAM(gt1000_fx_twah_t, filter_mode),
            DEFINE_PARAM(gt1000_fx_twah_t, polarity),
            DEFINE_PARAM(gt1000_fx_twah_t, sens),
            DEFINE_PARAM(gt1000_fx_twah_t, frequency),
            DEFINE_PARAM(gt1000_fx_twah_t, resonance),
            DEFINE_PARAM(gt1000_fx_twah_t, decay),
            DEFINE_PARAM(gt1000_fx_twah_t, effect_level),
            DEFINE_PARAM(gt1000_fx_twah_t, direct_mix),
            DEFINE_PARAM(gt1000_fx_twah_t, filter_mode_bass),
            DEFINE_PARAM(gt1000_fx_twah_t, polarity_bass),
            DEFINE_PARAM(gt1000_fx_twah_t, sens_bass),
            DEFINE_PARAM(gt1000_fx_twah_t, frequency_bass),
            DEFINE_PARAM(gt1000_fx_twah_t, resonance_bass),
            DEFINE_PARAM(gt1000_fx_twah_t, decay_bass),
            DEFINE_PARAM(gt1000_fx_twah_t, effect_level_bass),
            DEFINE_PARAM(gt1000_fx_twah_t, direct_mix_bass)
        ),
    
        DEFINE_EFFECT_BLOCK(gt1000_fx_tremolo_t, FX_TREMOLO, 7,
            DEFINE_PARAM(gt1000_fx_tremolo_t, rate),
            DEFINE_PARAM(gt1000_fx_tremolo_t, depth),
            DEFINE_PARAM(gt1000_fx_tremolo_t, waveform),
            DEFINE_PARAM(gt1000_fx_tremolo_t, effect_level),
            DEFINE_PARAM(gt1000_fx_tremolo_t, trigger),
            DEFINE_PARAM(gt1000_fx_tremolo_t, rise_time),
            DEFINE_PARAM(gt1000_fx_tremolo_t, direct_mix)
        ),
    
        DEFINE_EFFECT_BLOCK(gt1000_fx_vibrato_t, FX_VIBRATO, 7,
            DEFINE_PARAM(gt1000_fx_vibrato_t, rate),
            DEFINE_PARAM(gt1000_fx_vibrato_t, depth),
            DEFINE_PARAM(gt1000_fx_vibrato_t, color),
            DEFINE_PARAM(gt1000_fx_vibrato_t, effect_level),
            DEFINE_PARAM(gt1000_fx_vibrato_t, trigger),
            DEFINE_PARAM(gt1000_fx_vibrato_t, rise_time),
            DEFINE_PARAM(gt1000_fx_vibrato_t, direct_mix)
        ),
    
        DEFINE_EFFECT_BLOCK(gt1000_reverb_t, REVERB, 41,
            DEFINE_PARAM(gt1000_reverb_t, sw),
            DEFINE_PARAM(gt1000_reverb_t, type),
            DEFINE_PARAM(gt1000_reverb_t, direct_level),
            DEFINE_PARAM(gt1000_reverb_t, low_damp),
            DEFINE_PARAM(gt1000_reverb_t, high_damp),
            DEFINE_PARAM(gt1000_reverb_t, mod_rate),
            DEFINE_PARAM(gt1000_reverb_t, mod_depth),
            DEFINE_PARAM(gt1000_reverb_t, duck_sens),
            DEFINE_PARAM(gt1000_reverb_t, duck_pre),
            DEFINE_PARAM(gt1000_reverb_t, duck_post),
            DEFINE_PARAM(gt1000_reverb_t, time),
            DEFINE_PARAM(gt1000_reverb_t, tone),
            DEFINE_PARAM(gt1000_reverb_t, effect_level),
            DEFINE_PARAM(gt1000_reverb_t, density),
            DEFINE_PARAM(gt1000_reverb_t, pre_delay),
            DEFINE_PARAM(gt1000_reverb_t, low_cut),
            DEFINE_PARAM(gt1000_reverb_t, high_cut),
            DEFINE_PARAM(gt1000_reverb_t, pitch1),
            DEFINE_PARAM(gt1000_reverb_t, level1),
            DEFINE_PARAM(gt1000_reverb_t, type1),
            DEFINE_PARAM(gt1000_reverb_t, time1),
            DEFINE_PARAM(gt1000_reverb_t, tone1),
            DEFINE_PARAM(gt1000_reverb_t, effect_level1),
            DEFINE_PARAM(gt1000_reverb_t, density1),
            DEFINE_PARAM(gt1000_reverb_t, pre_delay1),
            DEFINE_PARAM(gt1000_reverb_t, low_cut1),
            DEFINE_PARAM(gt1000_reverb_t, high_cut1),
            DEFINE_PARAM(gt1000_reverb_t, pitch2),
            DEFINE_PARAM(gt1000_reverb_t, level2),
            DEFINE_PARAM(gt1000_reverb_t, type2),
            DEFINE_PARAM(gt1000_reverb_t, time2),
            DEFINE_PARAM(gt1000_reverb_t, tone2),
            DEFINE_PARAM(gt1000_reverb_t, effect_level2),
            DEFINE_PARAM(gt1000_reverb_t, density2),
            DEFINE_PARAM(gt1000_reverb_t, pre_delay2),
            DEFINE_PARAM(gt1000_reverb_t, low_cut2),
            DEFINE_PARAM(gt1000_reverb_t, high_cut2),
            DEFINE_PARAM(gt1000_reverb_t, mode),
            DEFINE_PARAM(gt1000_reverb_t, spread_time),
            DEFINE_PARAM(gt1000_reverb_t, feedback),
            DEFINE_PARAM(gt1000_reverb_t, trigger)
        ),
    
        DEFINE_EFFECT_BLOCK(gt1000_pedalfx_t, PEDALFX, 11,
            DEFINE_PARAM(gt1000_pedalfx_t, sw),
            DEFINE_PARAM(gt1000_pedalfx_t, type),
            DEFINE_PARAM(gt1000_pedalfx_t, pitch_max),
            DEFINE_PARAM(gt1000_pedalfx_t, effect_level),
            DEFINE_PARAM(gt1000_pedalfx_t, direct_mix),
            DEFINE_PARAM(gt1000_pedalfx_t, wah_type),
            DEFINE_PARAM(gt1000_pedalfx_t, pedal_min),
            DEFINE_PARAM(gt1000_pedalfx_t, pedal_max),
            DEFINE_PARAM(gt1000_pedalfx_t, wah_pedal_position),
            DEFINE_PARAM(gt1000_pedalfx_t, pedalbend_pedal_position),
            DEFINE_PARAM(gt1000_pedalfx_t, pitch_min)
        ),
};

const size_t effect_block_list_len = 100;
const effect_block_t effect_block_list[] = {
    {"COMP", COMP},
    {"DIST1", DIST},
    {"DIST2", DIST},
    {"PREAMP1", PREAMP},
    {"PREAMP2", PREAMP},
    {"NS1", NS},
    {"NS2", NS},
    {"EQ1", EQ},
    {"EQ2", EQ},
    {"EQ3", EQ},
    {"EQ4", EQ},
    {"DELAY1", DELAY},
    {"DELAY2", DELAY},
    {"DELAY3", DELAY},
    {"DELAY4", DELAY},
    {"MSTDELAY", MSTDELAY},
    {"CHORUS", CHORUS},
    {"FX1", FX},
    {"FX1_AGSIM", FX_AGSIM},
    {"FX1_ACRESO", FX_ACRESO},
    {"FX1_AWAH", FX_AWAH},
    {"FX1_CHORUS", FX_CHORUS},
    {"FX1_CVIBE", FX_CVIBE},
    {"FX1_COMP", FX_COMP},
    {"FX1_DEFRETTER", FX_DEFRETTER},
    {"FX1_FEEDBACKER", FX_FEEDBACKER},
    {"FX1_FLANGER", FX_FLANGER},
    {"FX1_HARMONIST", FX_HARMONIST},
    {"FX1_HUMANIZER", FX_HUMANIZER},
    {"FX1_OCTAVE", FX_OCTAVE},
    {"FX1_OVERTONE", FX_OVERTONE},
    {"FX1_PAN", FX_PAN},
    {"FX1_PHASER", FX_PHASER},
    {"FX1_PITCHSHIFT", FX_PITCHSHIFT},
    {"FX1_RINGMOD", FX_RINGMOD},
    {"FX1_ROTARY", FX_ROTARY},
    {"FX1_SITARSIM", FX_SITARSIM},
    {"FX1_SLICER", FX_SLICER},
    {"FX1_SLOWGEAR", FX_SLOWGEAR},
    {"FX1_SOUNDHOLD", FX_SOUNDHOLD},
    {"FX1_SBEND", FX_SBEND},
    {"FX1_TREMOLO", FX_TREMOLO},
    {"FX1_TWAH", FX_TWAH},
    {"FX1_VIBRATO", FX_VIBRATO},
    {"FX2", FX},
    {"FX2_AGSIM", FX_AGSIM},
    {"FX2_ACRESO", FX_ACRESO},
    {"FX2_AWAH", FX_AWAH},
    {"FX2_CHORUS", FX_CHORUS},
    {"FX2_CVIBE", FX_CVIBE},
    {"FX2_COMP", FX_COMP},
    {"FX2_DEFRETTER", FX_DEFRETTER},
    {"FX2_FEEDBACKER", FX_FEEDBACKER},
    {"FX2_FLANGER", FX_FLANGER},
    {"FX2_HARMONIST", FX_HARMONIST},
    {"FX2_HUMANIZER", FX_HUMANIZER},
    {"FX2_OCTAVE", FX_OCTAVE},
    {"FX2_OVERTONE", FX_OVERTONE},
    {"FX2_PAN", FX_PAN},
    {"FX2_PHASER", FX_PHASER},
    {"FX2_PITCHSHIFT", FX_PITCHSHIFT},
    {"FX2_RINGMOD", FX_RINGMOD},
    {"FX2_ROTARY", FX_ROTARY},
    {"FX2_SITARSIM", FX_SITARSIM},
    {"FX2_SLICER", FX_SLICER},
    {"FX2_SLOWGEAR", FX_SLOWGEAR},
    {"FX2_SOUNDHOLD", FX_SOUNDHOLD},
    {"FX2_SBEND", FX_SBEND},
    {"FX2_TREMOLO", FX_TREMOLO},
    {"FX2_TWAH", FX_TWAH},
    {"FX2_VIBRATO", FX_VIBRATO},
    {"FX3", FX},
    {"FX3_AGSIM", FX_AGSIM},
    {"FX3_ACRESO", FX_ACRESO},
    {"FX3_AWAH", FX_AWAH},
    {"FX3_CHORUS", FX_CHORUS},
    {"FX3_CVIBE", FX_CVIBE},
    {"FX3_COMP", FX_COMP},
    {"FX3_DEFRETTER", FX_DEFRETTER},
    {"FX3_FEEDBACKER", FX_FEEDBACKER},
    {"FX3_FLANGER", FX_FLANGER},
    {"FX3_HARMONIST", FX_HARMONIST},
    {"FX3_HUMANIZER", FX_HUMANIZER},
    {"FX3_OCTAVE", FX_OCTAVE},
    {"FX3_OVERTONE", FX_OVERTONE},
    {"FX3_PAN", FX_PAN},
    {"FX3_PHASER", FX_PHASER},
    {"FX3_PITCHSHIFT", FX_PITCHSHIFT},
    {"FX3_RINGMOD", FX_RINGMOD},
    {"FX3_ROTARY", FX_ROTARY},
    {"FX3_SITARSIM", FX_SITARSIM},
    {"FX3_SLICER", FX_SLICER},
    {"FX3_SLOWGEAR", FX_SLOWGEAR},
    {"FX3_SOUNDHOLD", FX_SOUNDHOLD},
    {"FX3_SBEND", FX_SBEND},
    {"FX3_TREMOLO", FX_TREMOLO},
    {"FX3_TWAH", FX_TWAH},
    {"FX3_VIBRATO", FX_VIBRATO},
    {"REVERB", REVERB},
    {"PEDALFX", PEDALFX},
};

static inline uint8_t gt1000_get_effect_block_index(const gt1000_effect_t *base, const gt1000_param_addr_t parameter) {
    return (((uint32_t)parameter - (uint32_t)base) >> 8) & 0xFF;
}

static inline uint8_t gt1000_get_parameter_offset(const gt1000_effect_t *base, const gt1000_param_addr_t parameter) {
    return ((uint32_t) parameter - (uint32_t) base) & 0xFF;
}


bool gt1000_get_parameter_info(gt1000_param_t *param, gt1000_param_addr_t parameter) {
    *param = (gt1000_param_t){0};

    const gt1000_t *device = gt1000_get_device();
    const gt1000_effect_t *base = &(device->effect);
    const uint8_t effect_block_index = gt1000_get_effect_block_index(base, parameter);
    const uint8_t parameter_offset = gt1000_get_parameter_offset(base, parameter);


    const effect_block_t *block = &effect_block_list[effect_block_index];
    const effect_block_metadata_t *metadata = &effect_block_metadata_list[block->type];

    param->effect_block_name = block->name;

    for (int i = 0; i < metadata->param_len; ++i) {
        if (metadata->params[i].offset == parameter_offset) {
            param->parameter_name = metadata->params[i].name;
            param->size = metadata->params[i].size;
            memcpy(&param->value, parameter, metadata->params[i].size);
            return true;
        }
    };

    return false;
}