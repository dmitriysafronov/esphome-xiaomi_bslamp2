#pragma once

#include <array>
#include <cmath>

#include "../common.h"
#include "light_modes.h" 
#include "gpio_outputs.h"

namespace esphome {
namespace xiaomi {
namespace bslamp2 {

struct RGB {
    float red;
    float green;
    float blue;
};

struct RGBPoint {
    RGB low;
    RGB high;
};

using RGBRing = std::array<RGBPoint, 24>;

using RGBCircle = std::array<RGBRing, 7>;

/**
 * The following table contains GPIO PWM duty cycles as used for driving the
 * LEDs in the device in RGB mode.
 *
 * The base for this table are measurements against the original device
 * firmware, using the RGB color circle as used in Home Assistant as the
 * color space model.
 *
 * This circle has 7 colored rings around a white center point. The outer
 * ring, with the highest saturation, is numbered as 0. The inner ring
 * around the white center point is numbered as 6. The white center point
 * itself is numbered as 7, although this one cannot really be called "a
 * ring".
 *
 * For each ring, there are 24 color positions, starting at the color red
 * (0°), going around the circle clockwise via green (120°) and blue (240°).
 *
 * For each color position, two duty cycle measurements are registered:
 * - one defining the duty cycles at 1% brightness
 * - one defining the duty cycles at 100% brightness Duty cycles for
 *   in-between brightnesses can be derived from these values by means of
 *   linear interpolation.
 */
static const RGBCircle rgb_circle_ {{
    // Ring 0, min value RGB component value = 0
    {{ 
        {{ 0.8998, 0.9997, 0.9997 }, { 0.0000, 0.9997, 0.9997 }}, // 0°   [255,0,0] (red)
        {{ 0.8727, 0.9404, 0.9682 }, { 0.0000, 0.6758, 0.9539 }}, // 15°  [255,0,63]
        {{ 0.8727, 0.8967, 0.9677 }, { 0.0000, 0.2389, 0.9506 }}, // 30°  [255,0,126]
        {{ 0.9030, 0.8727, 0.9682 }, { 0.3040, 0.0000, 0.9536 }}, // 45°  [255,0,190]
        {{ 0.9270, 0.8727, 0.9685 }, { 0.5426, 0.0000, 0.9570 }}, // 60°  [255,0,255]
        {{ 0.9404, 0.8727, 0.9687 }, { 0.6753, 0.0000, 0.9590 }}, // 75°  [190,0,255]
        {{ 0.9491, 0.8727, 0.9687 }, { 0.7638, 0.0000, 0.9601 }}, // 90°  [126,0,255]
        {{ 0.9539, 0.8727, 0.9689 }, { 0.8115, 0.0000, 0.9609 }}, // 105° [63,0,255]
        {{ 0.9997, 0.8998, 0.9997 }, { 0.9997, 0.0000, 0.9997 }}, // 120° [0,0,255] (green)
        {{ 0.9553, 0.8727, 0.9672 }, { 0.8264, 0.0000, 0.9452 }}, // 135° [0,63,255]
        {{ 0.9555, 0.8727, 0.9621 }, { 0.8266, 0.0000, 0.8937 }}, // 150° [0,126,255]
        {{ 0.9555, 0.8727, 0.9524 }, { 0.8273, 0.0000, 0.7964 }}, // 165° [0,190,255]
        {{ 0.9555, 0.8727, 0.9375 }, { 0.8285, 0.0000, 0.6469 }}, // 180° [0,255,255]
        {{ 0.9557, 0.8727, 0.9091 }, { 0.8301, 0.0000, 0.3648 }}, // 195° [0,255,190]
        {{ 0.9606, 0.9037, 0.8727 }, { 0.8782, 0.3091, 0.0000 }}, // 210° [0,255,126]
        {{ 0.9677, 0.9514, 0.8727 }, { 0.9486, 0.7856, 0.0000 }}, // 225° [0,255,63]
        {{ 0.9997, 0.9997, 0.8998 }, { 0.9997, 0.9997, 0.0000 }}, // 240° [0,255,0] (blue)
        {{ 0.9652, 0.9652, 0.8727 }, { 0.9245, 0.9252, 0.0000 }}, // 255° [63,255,0]
        {{ 0.9501, 0.9631, 0.8727 }, { 0.7746, 0.9029, 0.0000 }}, // 270° [126,255,0]
        {{ 0.9219, 0.9587, 0.8727 }, { 0.4919, 0.8601, 0.0000 }}, // 285° [190,255,0]
        {{ 0.8786, 0.9521, 0.8727 }, { 0.0584, 0.7946, 0.0000 }}, // 300° [255,255,0]
        {{ 0.8727, 0.9531, 0.9152 }, { 0.0000, 0.8022, 0.4250 }}, // 315° [255,190,0]
        {{ 0.8727, 0.9542, 0.9467 }, { 0.0000, 0.8145, 0.7889 }}, // 330° [255,126,0]
        {{ 0.8728, 0.9547, 0.9631 }, { 0.0000, 0.8207, 0.9044 }}  // 345° [255,63,0]
    }},
    // Ring 1, min value RGB component value = 35
    {{
        {{ 0.8727, 0.9499, 0.9660 }, { 0.0000, 0.7714, 0.9337 }}, // 0°   [255,35,39] (red)
        {{ 0.8727, 0.9255, 0.9665 }, { 0.0000, 0.5268, 0.9365 }}, // 15°  [255,35,90]
        {{ 0.8727, 0.8793, 0.9662 }, { 0.0000, 0.0664, 0.9345 }}, // 30°  [255,35,145]
        {{ 0.9079, 0.8727, 0.9672 }, { 0.3515, 0.0000, 0.9455 }}, // 45°  [255,35,200]
        {{ 0.9270, 0.8727, 0.9680 }, { 0.5429, 0.0000, 0.9519 }}, // 60°  [255,35,255]
        {{ 0.9386, 0.8727, 0.9680 }, { 0.6593, 0.0000, 0.9534 }}, // 75°  [200,35,255]
        {{ 0.9470, 0.8727, 0.9682 }, { 0.7433, 0.0000, 0.9546 }}, // 90°  [145,35,255]
        {{ 0.9524, 0.8727, 0.9682 }, { 0.7961, 0.0000, 0.9552 }}, // 105° [90,35,255]
        {{ 0.9547, 0.8727, 0.9682 }, { 0.8212, 0.0000, 0.9555 }}, // 120° [39,35,255] (green)
        {{ 0.9547, 0.8727, 0.9655 }, { 0.8215, 0.0000, 0.9284 }}, // 135° [35,90,255]
        {{ 0.9550, 0.8727, 0.9600 }, { 0.8217, 0.0000, 0.8712 }}, // 150° [35,145,255]
        {{ 0.9550, 0.8727, 0.9506 }, { 0.8225, 0.0000, 0.7788 }}, // 165° [35,200,255]
        {{ 0.9551, 0.8727, 0.9375 }, { 0.8235, 0.0000, 0.6469 }}, // 180° [35,255,255]
        {{ 0.9547, 0.8727, 0.9150 }, { 0.8212, 0.0000, 0.4226 }}, // 195° [35,255,200]
        {{ 0.9560, 0.8828, 0.8727 }, { 0.8322, 0.1003, 0.0000 }}, // 210° [35,255,145]
        {{ 0.9639, 0.9357, 0.8727 }, { 0.9104, 0.6289, 0.0000 }}, // 225° [35,255,90]
        {{ 0.9675, 0.9610, 0.8727 }, { 0.9475, 0.8803, 0.0000 }}, // 240° [35,255,35] (blue)
        {{ 0.9596, 0.9597, 0.8727 }, { 0.8686, 0.8686, 0.0000 }}, // 255° [90,255,35]
        {{ 0.9430, 0.9570, 0.8727 }, { 0.7023, 0.8435, 0.0000 }}, // 270° [145,255,35]
        {{ 0.9160, 0.9530, 0.8727 }, { 0.4332, 0.8032, 0.0000 }}, // 285° [200,255,35]
        {{ 0.8780, 0.9472, 0.8728 }, { 0.0518, 0.7458, 0.0000 }}, // 300° [255,255,35]
        {{ 0.8727, 0.9477, 0.9099 }, { 0.0000, 0.7497, 0.3717 }}, // 315° [255,200,35]
        {{ 0.8727, 0.9490, 0.9396 }, { 0.0000, 0.7612, 0.6689 }}, // 330° [255,145,35]
        {{ 0.8727, 0.9496, 0.9580 }, { 0.0000, 0.7683, 0.8512 }}  // 345° [255,90,35]
    }},
    // Ring 2, min value RGB component value = 73
    {{
        {{ 0.8727, 0.9352, 0.9609 }, { 0.0000, 0.6244, 0.8822 }}, // 0°   [255,73,76] (red)
        {{ 0.8727, 0.9035, 0.9616 }, { 0.0000, 0.3068, 0.8888 }}, // 15°  [255,73,119]
        {{ 0.8847, 0.8727, 0.9629 }, { 0.1189, 0.0000, 0.9004 }}, // 30°  [255,73,164]
        {{ 0.9121, 0.8727, 0.9650 }, { 0.3936, 0.0000, 0.9237 }}, // 45°  [255,73,209]
        {{ 0.9270, 0.8727, 0.9665 }, { 0.5431, 0.0000, 0.9367 }}, // 60°  [255,73,255]
        {{ 0.9370, 0.8727, 0.9665 }, { 0.6428, 0.0000, 0.9377 }}, // 75°  [209,73,255]
        {{ 0.9445, 0.8727, 0.9665 }, { 0.7182, 0.0000, 0.9386 }}, // 90°  [164,73,255]
        {{ 0.9499, 0.8727, 0.9667 }, { 0.7722, 0.0000, 0.9391 }}, // 105° [119,73,255]
        {{ 0.9534, 0.8727, 0.9667 }, { 0.8066, 0.0000, 0.9396 }}, // 120° [73,73,255] (green)
        {{ 0.9534, 0.8727, 0.9631 }, { 0.8068, 0.0000, 0.9024 }}, // 135° [73,119,255]
        {{ 0.9536, 0.8727, 0.9570 }, { 0.8073, 0.0000, 0.8435 }}, // 150° [73,164,255]
        {{ 0.9536, 0.8727, 0.9487 }, { 0.8081, 0.0000, 0.7602 }}, // 165° [73,209,255]
        {{ 0.9536, 0.8727, 0.9375 }, { 0.8088, 0.0000, 0.6474 }}, // 180° [73,255,255]
        {{ 0.9526, 0.8727, 0.9201 }, { 0.7990, 0.0000, 0.4736 }}, // 195° [73,255,209]
        {{ 0.9509, 0.8727, 0.8866 }, { 0.7804, 0.0000, 0.1383 }}, // 210° [73,255,164]
        {{ 0.9557, 0.9109, 0.8727 }, { 0.8291, 0.3813, 0.0000 }}, // 225° [73,255,119]
        {{ 0.9606, 0.9455, 0.8727 }, { 0.8798, 0.7268, 0.0000 }}, // 240° [73,255,73] (blue)
        {{ 0.9499, 0.9439, 0.8727 }, { 0.7714, 0.7107, 0.0000 }}, // 255° [119,255,73]
        {{ 0.9329, 0.9414, 0.8727 }, { 0.6005, 0.6855, 0.0000 }}, // 270° [164,255,73]
        {{ 0.9086, 0.9377, 0.8727 }, { 0.5381, 0.6496, 0.0000 }}, // 285° [209,255,73]
        {{ 0.8760, 0.9329, 0.8727 }, { 0.0314, 0.6013, 0.0000 }}, // 300° [255,255,73]
        {{ 0.8727, 0.9331, 0.9052 }, { 0.0000, 0.6027, 0.3248 }}, // 315° [255,209,73]
        {{ 0.8727, 0.9340, 0.9316 }, { 0.0000, 0.6130, 0.5876 }}, // 330° [255,164,73]
        {{ 0.8727, 0.9347, 0.9499 }, { 0.0000, 0.6202, 0.7717 }}  // 345° [255,119,73]
    }},
    // Ring 3, min value RGB component value = 109
    {{
        {{ 0.8727, 0.9114, 0.9526 }, { 0.0000, 0.3850, 0.7983 }}, // 0°   [255,109,112] (red)
        {{ 0.8727, 0.8788, 0.9541 }, { 0.0000, 0.0615, 0.8125 }}, // 15°  [255,109,145]
        {{ 0.8979, 0.8727, 0.9587 }, { 0.2526, 0.0000, 0.8586 }}, // 30°  [255,109,182]
        {{ 0.9160, 0.8727, 0.9619 }, { 0.4311, 0.0000, 0.8906 }}, // 45°  [255,109,218]
        {{ 0.9272, 0.8727, 0.9639 }, { 0.6254, 0.0000, 0.9111 }}, // 60°  [255,109,255]
        {{ 0.9352, 0.8727, 0.9639 }, { 0.6254, 0.0000, 0.9111 }}, // 75°  [218,109,255]
        {{ 0.9419, 0.8727, 0.9639 }, { 0.6909, 0.0000, 0.9116 }}, // 90°  [182,109,255]
        {{ 0.9472, 0.8727, 0.9639 }, { 0.7438, 0.0000, 0.9119 }}, // 105° [145,109,255]
        {{ 0.9509, 0.8727, 0.9640 }, { 0.7814, 0.0000, 0.9121 }}, // 120° [109,109,255] (green)
        {{ 0.9509, 0.7827, 0.9599 }, { 0.7819, 0.0000, 0.7813 }}, // 135° [109,145,255]
        {{ 0.9509, 0.7827, 0.9541 }, { 0.7824, 0.0000, 0.8135 }}, // 150° [109,182,255]
        {{ 0.9511, 0.7827, 0.9467 }, { 0.7829, 0.0000, 0.7409 }}, // 165° [109,218,255]
        {{ 0.9512, 0.7827, 0.9376 }, { 0.7837, 0.0000, 0.6484 }}, // 180° [109,255,255]
        {{ 0.9494, 0.7827, 0.9245 }, { 0.7661, 0.0000, 0.5188 }}, // 195° [109,255,218]
        {{ 0.9465, 0.7827, 0.9035 }, { 0.7373, 0.0000, 0.3073 }}, // 210° [109,255,182]
        {{ 0.9435, 0.8810, 0.8727 }, { 0.7077, 0.0083, 0.0000 }}, // 225° [109,255,145]
        {{ 0.9491, 0.9190, 0.8727 }, { 0.7633, 0.4624, 0.0000 }}, // 240° [109,255,109] (blue)
        {{ 0.9372, 0.9171, 0.8727 }, { 0.6443, 0.4450, 0.0000 }}, // 255° [145,255,109]
        {{ 0.9204, 0.9147, 0.8727 }, { 0.4760, 0.4209, 0.0000 }}, // 270° [182,255,109]
        {{ 0.8991, 0.9119, 0.8727 }, { 0.2651, 0.3906, 0.0000 }}, // 285° [218,255,109]
        {{ 0.8727, 0.9081, 0.8730 }, { 0.0000, 0.3540, 0.0034 }}, // 300° [255,255,109]
        {{ 0.8727, 0.9094, 0.9016 }, { 0.0000, 0.3652, 0.2871 }}, // 315° [255,218,109]
        {{ 0.8727, 0.9101, 0.9236 }, { 0.0000, 0.3737, 0.5083 }}, // 330° [255,182,109]
        {{ 0.8727, 0.9109, 0.9411 }, { 0.0000, 0.3805, 0.6833 }}  // 345° [255,145,109]
    }},
    // Ring 4, min value RGB component value = 145
    {{
        {{ 0.8727, 0.8783, 0.9416 }, { 0.0000, 0.0566, 0.6879 }}, // 0°   [255,145,147] (red)
        {{ 0.8916, 0.8727, 0.9484 }, { 0.1869, 0.0000, 0.7575 }}, // 15°  [255,145,172]
        {{ 0.9081, 0.8727, 0.9539 }, { 0.3550, 0.0000, 0.8110 }}, // 30°  [255,145,200]
        {{ 0.9191, 0.8727, 0.9572 }, { 0.4646, 0.0000, 0.8460 }}, // 45°  [255,145,227]
        {{ 0.9272, 0.8727, 0.9600 }, { 0.5444, 0.0000, 0.8712 }}, // 60°  [255,145,255]
        {{ 0.9336, 0.8727, 0.9600 }, { 0.6074, 0.0000, 0.8713 }}, // 75°  [227,145,255]
        {{ 0.9387, 0.8727, 0.9600 }, { 0.6603, 0.0000, 0.8712 }}, // 90°  [200,145,255]
        {{ 0.9435, 0.8727, 0.9600 }, { 0.7071, 0.0000, 0.8712 }}, // 105° [172,145,255]
        {{ 0.9472, 0.8727, 0.9600 }, { 0.7442, 0.0000, 0.8713 }}, // 120° [145,145,255] (green)
        {{ 0.9472, 0.8727, 0.9557 }, { 0.7446, 0.0000, 0.8311 }}, // 135° [145,172,255]
        {{ 0.9472, 0.8727, 0.9506 }, { 0.7451, 0.0000, 0.7798 }}, // 150° [145,200,255]
        {{ 0.9472, 0.8727, 0.9447 }, { 0.7456, 0.0000, 0.7208 }}, // 165° [145,227,255]
        {{ 0.9475, 0.8727, 0.9377 }, { 0.7463, 0.0000, 0.6498 }}, // 180° [145,255,255]
        {{ 0.9450, 0.8727, 0.9286 }, { 0.7228, 0.0000, 0.5588 }}, // 195° [145,255,227]
        {{ 0.9419, 0.8727, 0.9160 }, { 0.6904, 0.0000, 0.4319 }}, // 210° [145,255,200]
        {{ 0.9367, 0.8727, 0.8960 }, { 0.6393, 0.0000, 0.2328 }}, // 225° [145,255,172]
        {{ 0.9316, 0.8793, 0.8727 }, { 0.5890, 0.0666, 0.0000 }}, // 240° [145,255,145] (blue)
        {{ 0.9199, 0.8778, 0.8727 }, { 0.4716, 0.0505, 0.0000 }}, // 255° [172,255,145]
        {{ 0.9050, 0.8757, 0.8727 }, { 0.3227, 0.0003, 0.0000 }}, // 270° [200,255,145]
        {{ 0.8878, 0.8735, 0.8727 }, { 0.1513, 0.0064, 0.0000 }}, // 285° [227,255,145]
        {{ 0.8727, 0.8760, 0.8781 }, { 0.0000, 0.0316, 0.0528 }}, // 300° [255,255,145]
        {{ 0.8727, 0.8766, 0.8990 }, { 0.0000, 0.0398, 0.2607 }}, // 315° [255,227,145]
        {{ 0.8727, 0.8775, 0.9160 }, { 0.0000, 0.0465, 0.4316 }}, // 330° [255,200,145]
        {{ 0.8727, 0.8778, 0.9306 }, { 0.0000, 0.0523, 0.5798 }}  // 345° [255,172,145]
    }},
    // Ring 5, min value RGB component value = 181
    {{
        {{ 0.8980, 0.8727, 0.9391 }, { 0.5784, 0.4409, 0.8030 }}, // 0°   [255,181,182] (red)
        {{ 0.9079, 0.8727, 0.9445 }, { 0.6330, 0.4409, 0.8327 }}, // 15°  [255,181,199]
        {{ 0.9162, 0.8727, 0.9486 }, { 0.6774, 0.4409, 0.8556 }}, // 30°  [255,181,218]
        {{ 0.9221, 0.8727, 0.9519 }, { 0.7104, 0.4409, 0.8725 }}, // 45°  [255,181,236]
        {{ 0.9272, 0.8727, 0.9545 }, { 0.7382, 0.4409, 0.8866 }}, // 60°  [255,181,255]
        {{ 0.9316, 0.8727, 0.9545 }, { 0.7618, 0.4409, 0.8863 }}, // 75°  [236,181,255]
        {{ 0.9355, 0.8727, 0.9544 }, { 0.7827, 0.4409, 0.8861 }}, // 90°  [218,181,255]
        {{ 0.9391, 0.8727, 0.9542 }, { 0.8025, 0.4409, 0.8859 }}, // 105° [199,181,255]
        {{ 0.9421, 0.8727, 0.9542 }, { 0.8191, 0.4409, 0.8859 }}, // 120° [181,181,255] (green)
        {{ 0.9421, 0.8727, 0.9509 }, { 0.8193, 0.4409, 0.8676 }}, // 135° [181,199,255]
        {{ 0.9421, 0.8727, 0.9470 }, { 0.8195, 0.4409, 0.8458 }}, // 150° [181,218,255]
        {{ 0.9424, 0.8727, 0.9429 }, { 0.8197, 0.4409, 0.8230 }}, // 165° [181,236,255]
        {{ 0.9424, 0.8727, 0.9380 }, { 0.8202, 0.4409, 0.7961 }}, // 180° [181,255,255]
        {{ 0.9399, 0.8727, 0.9321 }, { 0.8071, 0.4409, 0.7651 }}, // 195° [181,255,236]
        {{ 0.9370, 0.8727, 0.9252 }, { 0.7912, 0.4409, 0.7274 }}, // 210° [181,255,218]
        {{ 0.9331, 0.8727, 0.9160 }, { 0.7697, 0.4409, 0.6767 }}, // 225° [181,255,199]
        {{ 0.9282, 0.8727, 0.9045 }, { 0.7431, 0.4409, 0.6135 }}, // 240° [181,255,181] (blue)
        {{ 0.9221, 0.8727, 0.9050 }, { 0.7094, 0.4409, 0.6164 }}, // 255° [199,255,181]
        {{ 0.9147, 0.8727, 0.9057 }, { 0.6700, 0.4409, 0.6200 }}, // 270° [218,255,181]
        {{ 0.9074, 0.8727, 0.9062 }, { 0.6296, 0.4409, 0.6240 }}, // 285° [236,255,181]
        {{ 0.8988, 0.8727, 0.9072 }, { 0.5834, 0.4409, 0.6282 }}, // 300° [255,255,181]
        {{ 0.8986, 0.8727, 0.9166 }, { 0.5820, 0.4409, 0.6807 }}, // 315° [255,236,181]
        {{ 0.8985, 0.8727, 0.9250 }, { 0.5805, 0.4409, 0.7258 }}, // 330° [255,218,181]
        {{ 0.8981, 0.8727, 0.9329 }, { 0.5793, 0.4409, 0.7687 }}  // 345° [255,199,181]
    }},
    // Ring 6, min value RGB component value = 219
    {{
        {{ 0.9167, 0.8727, 0.9389 }, { 0.4399, 0.0000, 0.6606 }}, // 0°   [255,219,219] (red)
        {{ 0.9199, 0.8727, 0.9414 }, { 0.4711, 0.0000, 0.6850 }}, // 15°  [255,219,228]
        {{ 0.9226, 0.8727, 0.9432 }, { 0.4992, 0.0000, 0.7068 }}, // 30°  [255,219,237]
        {{ 0.9252, 0.8727, 0.9452 }, { 0.5241, 0.0000, 0.7262 }}, // 45°  [255,219,246]
        {{ 0.9275, 0.8727, 0.9472 }, { 0.5465, 0.0000, 0.7437 }}, // 60°  [255,219,255]
        {{ 0.9294, 0.8727, 0.9470 }, { 0.5675, 0.0000, 0.7432 }}, // 75°  [246,219,255]
        {{ 0.9316, 0.8727, 0.9470 }, { 0.5879, 0.0000, 0.7427 }}, // 90°  [237,219,255]
        {{ 0.9335, 0.8727, 0.9470 }, { 0.6074, 0.0000, 0.7424 }}, // 105° [228,219,255]
        {{ 0.9352, 0.8727, 0.9470 }, { 0.6259, 0.0000, 0.7419 }}, // 120° [219,219,255] (green)
        {{ 0.9352, 0.8727, 0.9450 }, { 0.6262, 0.0000, 0.7217 }}, // 135° [219,228,255]
        {{ 0.9355, 0.8727, 0.9429 }, { 0.6264, 0.0000, 0.7002 }}, // 150° [219,237,255]
        {{ 0.9355, 0.8727, 0.9406 }, { 0.6269, 0.0000, 0.6777 }}, // 165° [219,246,255]
        {{ 0.9355, 0.8727, 0.9381 }, { 0.6272, 0.0000, 0.6540 }}, // 180° [219,255,255]
        {{ 0.9339, 0.8727, 0.9357 }, { 0.6111, 0.0000, 0.6291 }}, // 195° [219,255,246]
        {{ 0.9321, 0.8727, 0.9329 }, { 0.5929, 0.0000, 0.6013 }}, // 210° [219,255,237]
        {{ 0.9301, 0.8727, 0.9299 }, { 0.5729, 0.0000, 0.5703 }}, // 225° [219,255,228]
        {{ 0.9277, 0.8727, 0.9263 }, { 0.5503, 0.0000, 0.5354 }}, // 240° [219,255,219] (blue)
        {{ 0.9252, 0.8727, 0.9265 }, { 0.5249, 0.0000, 0.5368 }}, // 255° [228,255,219]
        {{ 0.9226, 0.8727, 0.9265 }, { 0.4985, 0.0000, 0.5383 }}, // 270° [237,255,219]
        {{ 0.9199, 0.8727, 0.9267 }, { 0.4711, 0.0000, 0.5399 }}, // 285° [246,255,219]
        {{ 0.9170, 0.8727, 0.9270 }, { 0.4426, 0.0000, 0.5414 }}, // 300° [255,255,219]
        {{ 0.9170, 0.8727, 0.9301 }, { 0.4419, 0.0000, 0.5734 }}, // 315° [255,246,219]
        {{ 0.9170, 0.8727, 0.9331 }, { 0.4411, 0.0000, 0.6039 }}, // 330° [255,237,219]
        {{ 0.9167, 0.8727, 0.9360 }, { 0.4406, 0.0000, 0.6330 }}  // 345° [255,228,219]
    }}
}};

/**
 * This class can handle the GPIO outputs for the RGB light mode,
 * based on RGB color values + brightness.
 */
class ColorRGBLight : public GPIOOutputs {
public:
    bool set_light_color_values(light::LightColorValues v) {
        light_mode = LIGHT_MODE_RGB;

        if (v.get_white() > 0.0f) {
            return false;
        }

        // Determine the ring level for the color. This is a value between 0
        // and 7, determining in what ring of the RGB circle the requested
        // color resides.
        auto rgb_min = min(min(v.get_red(), v.get_green()), v.get_blue());
        auto level = 7.0f * rgb_min;

        // While the default color circle in Home Assistant presents only a
        // subset of colors, it is possible to request colors outside this
        // subset as well. Therefore, the ring level might contain a
        // fractional value instead of a plain integer. To accomodate for
        // this, interpolation will be done to get the final outputs.

        // Determine duty cycle measurements for the outer ring.
        auto level_a = floor(level);
        set_duty_cycles_(
            &rgbp_a_, level_a, v.get_red(), v.get_green(), v.get_blue(),
            v.get_brightness(), &rgb_a_);

        // Determine duty cycle measurements for the inner ring.
        auto level_b = ceil(level);
        set_duty_cycles_(
            &rgbp_b_, level_a, v.get_red(), v.get_green(), v.get_blue(),
            v.get_brightness(), &rgb_b_);

        // Almost there! We now have the correct duty cycles for the
        // two rings that we were looking at. In this last step, the
        // two values are interpolated based on the ring level.
        auto d = level - level_a;
        red = esphome::lerp(d, rgb_a_.red, rgb_b_.red);
        green = esphome::lerp(d, rgb_a_.green, rgb_b_.green);
        blue = esphome::lerp(d, rgb_a_.blue, rgb_b_.blue);

        // The white output channel will always be 0 for RGB.
        white = 0.0f;

        return true;
    }

    RGBPoint rgbp_a_;
    RGBPoint rgbp_b_;
    RGB rgb_a_;
    RGB rgb_b_;

    void set_duty_cycles_(RGBPoint *p, int ring_level,
        float r, float g, float b, float brightness, RGB *rgb) {

        // Ring level 7 = white light center. The duty cycles for this level
        // can be computed using a few basic functions.
        if (ring_level == 7) {
            rgb->red = 0.932101 - 0.383377 * brightness;
            rgb->green = 0.883185 - 0.881623 * brightness;
            rgb->blue = 0.94188 - 0.284498 * brightness;
            return;
        }

        // Other ring levels are more complex. Start by retrieving the duty
        // cycle measurement data for the ring at hand.
        auto ring = rgb_circle_[ring_level];

        // Because we only have a subset of all colors in the RGB ring
        // available in the configuration table, some interpolation will
        // have to be done.
        // First, compute the position on the ring for the requested RGB
        // color. This is basically a hue representation of the requested
        // color. It is expressed as a number of degrees around the ring,
        // starting with red (at 0°).
        auto pos = ring_pos_(r, g, b) / 15.0f;

        // Since there are 24 measurements for each ring, each measurement
        // covers 360°/24 = 15°. Using that knowledge, the measurements to
        // use for interpolation can be picked from the ring data.
        auto pos_x = floor(pos);
        auto x = ring[pos_x];
        auto pos_y = ceil(pos);
        auto y = ring[pos_y > 23 ? 0 : pos_y];

        // Interpolate based on the ring position.
        auto d = pos - pos_x;
        p->low.red = esphome::lerp(d, x.low.red, y.low.red);
        p->low.green = esphome::lerp(d, x.low.green, y.low.green);
        p->low.blue = esphome::lerp(d, x.low.blue, y.low.blue);
        p->high.red = esphome::lerp(d, x.high.red, y.high.red);
        p->high.green = esphome::lerp(d, x.high.green, y.high.green);
        p->high.blue = esphome::lerp(d, x.high.blue, y.high.blue);

        // Interpolate based on brightness level.
        apply_brightness_(p, brightness, rgb);
    }

protected:
    /**
     * Returns the position on an RGB ring in degrees (0 - 359).
     */
    float ring_pos_(float red, float green, float blue) {
        auto rgb_min = min(min(red, green), blue);
        auto rgb_max = max(max(red, green), blue);
        auto delta = rgb_max - rgb_min;
        float pos; 
        if (delta == 0.0f)
            pos = 0.0f;
        else if (red == rgb_max)
            pos = 60.0f * fmod((green - blue) / delta, 6);
        else if (green == rgb_max)
            pos = 60.0f * ((blue - red) / delta + 2.0f);
        else
            pos = 60.0f * ((red - green) / delta + 4.0f);
        if (pos < 0)
            pos = pos + 360;
        return pos;
    }

    /**
     * Apply brightness interpolation to the duty cycle measurements. We
     * have the low (0.01) and high (1.00) brightness measurements in the
     * data. Brightness can be applied by means of linear interpolation.
     */
    void apply_brightness_(RGBPoint *p, float brightness, RGB *rgb) {
        auto d = brightness - 0.01f;
        rgb->red = esphome::lerp(d, p->low.red, p->high.red);
        rgb->green = esphome::lerp(d, p->low.green, p->high.green);
        rgb->blue = esphome::lerp(d, p->low.blue, p->high.blue);
    }
};


} // namespace bslamp2
} // namespace xiaomi
} // namespace esphome
