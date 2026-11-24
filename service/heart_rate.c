/*
 * Beat detection with adaptive threshold (peak/trough tracking),
 * IBI = inter-beat interval, BPM averaged over the last 4 beats.
 */
#include "heart_rate.h"
#include "../inc/config.h"

static uint32_t last_beat_ms;
static uint16_t ibi = 600;
static uint16_t thresh = 512, peak = 512, trough = 512;
static uint16_t bpm;
static uint16_t rate[4];
static bool pulse, first_beat = true, second_beat;

static void reset_state(uint32_t now)
{
    thresh = peak = trough = 512;
    last_beat_ms = now;
    ibi = 600;
    bpm = 0;
    pulse = false;
    first_beat = true;
    second_beat = false;
}

void heart_rate_init(void)
{
    reset_state(0);
}

bool heart_rate_update(uint16_t s, uint32_t now)
{
    bool beat = false;
    uint32_t n = now - last_beat_ms;
    uint32_t guard = (uint32_t)(ibi / 5) * 3;

    if (s < thresh && n > guard && s < trough) trough = s;
    if (s > thresh && s > peak)                peak = s;

    if (n > 250 && s > thresh && !pulse && n > guard) {
        pulse = true;
        ibi = (uint16_t)n;
        last_beat_ms = now;

        if (second_beat) {
            second_beat = false;
            for (uint8_t i = 0; i < 4; i++) rate[i] = ibi;
        }
        if (first_beat) {
            first_beat = false;
            second_beat = true;
            return false;
        }

        uint32_t sum = 0;
        for (uint8_t i = 0; i < 3; i++) {
            rate[i] = rate[i + 1];
            sum += rate[i];
        }
        rate[3] = ibi;
        sum += ibi;
        bpm = (uint16_t)(60000UL / (sum / 4UL));
        beat = true;
    }

    if (s < thresh && pulse) {
        pulse = false;
        uint16_t amp = (peak > trough) ? (peak - trough) : 0;
        thresh = amp / 2 + trough;
        peak = trough = thresh;
    }

    if (n > 2500) {              /* signal lost */
        reset_state(now);
    }
    return beat;
}

uint16_t heart_rate_get_bpm(void)     { return bpm; }
uint16_t heart_rate_get_ibi(void)     { return ibi; }
bool     heart_rate_pulse_active(void){ return pulse; }

hr_status_t heart_rate_get_status(void)
{
    if (bpm == 0)                return HR_NO_SIGNAL;
    if (bpm < BPM_LOW_LIMIT)     return HR_LOW;
    if (bpm > BPM_HIGH_LIMIT)    return HR_HIGH;
    return HR_NORMAL;
}

const char *heart_rate_status_str(hr_status_t s)
{
    switch (s) {
    case HR_LOW:    return "LOW";
    case HR_NORMAL: return "NORMAL";
    case HR_HIGH:   return "HIGH";
    default:        return "NO SIGNAL";
    }
}
