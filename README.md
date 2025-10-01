# ESP32-based MIDI SysEx Controller for BOSS GT-1000 Core with State Synchronization

This project details the development of a custom MIDI controller for the BOSS GT-1000 Core using an ESP32. Bidirectional state synchronization is achieved by reverse-engineering the SysEx implementation and mirroring the GT-1000 Core's internal memory model.
![main](/documents/image1.png)

## The Problem with Existing MIDI Controllers

1. **Limited Control** <br>
   Most conventional MIDI controllers can only send generic MIDI messages (CC, PC, Note, etc.). This is insufficient for adjusting the thousands of parameters available on the GT-1000.
2. **One-Way Communication** <br>
   They typically operate on a "fire and forget" basis, unilaterally sending control messages. This means there is no parameter state management, so they cannot provide visual feedback (e.g., via LEDs) to reflect the device's current status.

## This Project's Solution

1. **SysEx (System Exclusive Message)** <br>
   The GT-1000 supports SysEx (System Exclusive) messages in addition to generic MIDI. These extended messages unlock control over far more functions. The usage of these messages is detailed in the official GT-1000 MIDI Implementation document provided by Boss.
2. **Enabling Bidirectional Sync** <br>
   While the official documentation describes the SysEx implementation, it primarily outlines a polling method where the host must inquire the device for data. However, through analysis, I discovered an undocumented command that activates a "streaming" mode, causing the GT-1000 to automatically send parameter changes to the host. This feature is the key to achieving true bidirectional state synchronization.

## Project Details

### Hardware

- BOSS GT-1000 Core <br> (Note: This project was developed with the GT-1000 Core, but for simplicity, it will be referred to as the GT-1000 hereafter.)
- ESP32-C3
- SSD1306 OLED Display
- 6N137 Optocoupler (for MIDI connection)
- Buttons and LEDs

### Software and Dependencies

- ESP-IDF
- FreeRTOS
- lvgl
- espressif/button component

### Demo Video

[Youtube Link](https://www.youtube.com/watch?v=kOhcvgjPvE4)

## Technical Details

### Achieving Bidirectional State Sync with SysEx

The GT-1000's SysEx implementation is described in detail in the official document. [GT-1000 MIDI Implementation](https://static.roland.com/assets/media/pdf/GT-1000_GT-1000CORE_MIDI_Imple_eng11_W.pdf).

The document shows two primary methods for data exchange:

**Method A: Data Request/Receive**

```
Host --------> GT-1000   (RQ1: Request Data)
Host <-------- GT-1000   (DT1: Data Set)
```

**Method B: Data Set/Confirm**

```
Host --------> GT-1000   (DT1: Data Set)
Host <-------- GT-1000   (DT1: Data Set, as confirmation)
```

While both methods allow the host to control the device, neither allows the host to become aware of changes made directly on the GT-1000's hardware interface. However, the official [Boss Tone Studio](https://bosstonecentral.com/about/) PC editor _does_ achieve this seamless bidirectional sync.

To understand how, I dumped USB packets between Boss Tone Studio and the GT-1000. Boss Tone Studio communicates via SysEx over USB MIDI. (This is why Tone Studio is so slow.) Therefore, by analyzing the USB packets, it is possible to see how Tone Studio implements bidirectional synchronization with the GT-1000 via SysEx.

By capturing and analyzing the USB packets exchanged between the GT-1000 and Boss Tone Studio with [Wireshark](https://www.wireshark.org/download.html), I confirmed that they exchange message sequences that are not documented in the official manuals.

1. When Boss Tone Studio connects, it first sends a device inquiry (Identity Request) to get the device ID. It then uses a DT1 message to **write a value of `1` to the address `0x7F000001`**. (`F0 41 10 00 00 00 4F 12 7F 00 00 01 01 7F F7` )
   ![packet_capture1](/documents/image2.png)

2. When Boss Tone Studio disconnects, it **writes a value of `0` to the address `0x7F000001`**. (`F0 41 10 00 00 00 4F 12 7F 00 00 01 00 00 F7` )
   ![packet_capture2](/documents/image3.png)

(The bytes `04h`–`07h` in the middle of the message are the SysEx CIN bytes of USB MIDI.)

I hypothesized that this sequence enables a flag for streaming parameter changes from the device to the host. Testing confirmed this: with the flag enabled, any parameter change made on the GT-1000 was immediately sent to the host as a SysEx DT1 message.

With this hidden feature, bidirectional state synchronization could be implemented.

> I searched extensively to see if this feature was documented or known anywhere else but found nothing. If you have any specifications or documents regarding this feature, I would be very grateful if you could share them.

### SysEx Message Handling

The ESP32 receives MIDI signals via a UART peripheral set to a baud rate of 31250 (the MIDI standard of 31.25 kbps). A 6N137 optocoupler was used to isolate MIDI input circuit.

The project's firmware separates message handling into distinct layers:

```
[uart.c] --> [byte queue] --> [sysex.c] --> [message queue] --> [gt1000.c]
```

- **`uart.c`**: Receives raw UART events and enqueues them into the queue byte by byte.
- **`sysex.c`**: Reads from the byte queue, detects the SysEx start (`F0h`) and end (`F7h`) bytes, and assembles a complete SysEx message. It also handles filtering out _MIDI Active Sensing messages_ (`FEh`) and processing synchronous messages like _Identity Requests_ before passing the valid SysEx message to the GT1000 message queue.
- **`gt1000.c`**: Parses the finalized SysEx message and acts upon the data.

### Mirroring the GT-1000 Memory Model

The SysEx protocol is fundamentally based on device memory addresses. To manipulate parameters, the host controller needs an `address-parameter` map. To interpret incoming data, it also needs the reverse mapping.

To simplify these conversions, I modeled the GT-1000's parameter memory structure directly in C code. According to the documentation, all parameters are 1-byte aligned, and effect blocks within a patch are aligned to `0x100` (256) byte boundaries.

Using compiler-specific attributes (`__attribute__((packed, aligned(0x100)))`), I defined C `struct`s that exactly match the GT-1000's memory layout (see `gt1000_param.h`).

```c
#define EFFECT_BLOCK_ALIGN 0x100

#define ALIGNED_EFFECT_BLOCK\
	struct __attribute__((packed, aligned(EFFECT_BLOCK_ALIGN)))

typedef ALIGNED_EFFECT_BLOCK {
	bool sw; // 0x00: SW, OFF/ON (0-1)
	uint8_t type; // 0x01: TYPE (0-5) BOSS COMP, X-COMP, D-COMP, ORANGE, STEREO COMP, X-BASS COMP
	uint8_t sustain; // 0x02: SUSTAIN (0-100)
	uint8_t attack; // 0x03: ATTACK (0-100)
	uint8_t level; // 0x04: LEVEL (0-100)
	uint8_t tone; // 0x05: TONE (-50 to 50, stored as 14-114)
	uint8_t ratio; // 0x06: RATIO (0-17), 1:1 ~ INF:1
	uint8_t direct_mix; // 0x07: DIRECT MIX (0-100)
	uint8_t threshold; // 0x08: THRESHOLD (0-100)
} gt1000_comp_t;
```

For runtime lookups, parameter metadata (size, name, parameter offset) is defined using macros, creating a static map (see `gt1000_param.c`).

```c
#define DEFINE_EFFECT_BLOCK(block, block_name, len, ...) \
{ \
	.name = #block_name, \
	.size = sizeof(block), \
	.param_len = len, \
	.params = { __VA_ARGS__ } \
}

#define DEFINE_PARAM(block, param_name) \
	{ .name = #param_name, .size = sizeof(((block*)0)->param_name), .offset = offsetof(block, param_name) }

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
	...
};
```

This approach makes address translation trivial:

- **Host to Device:** `device_parameter_addr = GT1000_BASE_ADDRESS + (host_struct_member_addr - host_struct_base_addr)`
- **Device to Host:** `host_struct_member_addr = host_struct_base_addr + (device_parameter_addr - GT1000_BASE_ADDRESS)`

This architecture dramatically simplifies processing incoming DT1 messages. The data payload from the device can be copied directly into the correct location in the local `struct` using a simple `memcpy`, instantly synchronizing the state.

Furthermore, the uniform alignment of parameter blocks allows for easy identification using bitmasking. For example, the _compressor block_ in the current patch spans addresses `0x10001200` to `0x10001208`. If a DT1 message arrives for address `0x10001205` (the compressor's tone parameter), applying a `0xFFFFFF00` mask immediately yields the block's base address (`0x10001200`). From there, simple address arithmetic can be used to find the block's type and look up its metadata. (see `gt1000_param.c`.)

```c
static inline uint8_t gt1000_get_effect_block_index(const gt1000_effect_t *base, const gt1000_param_addr_t parameter) {
	return (((uint32_t)parameter - (uint32_t)base) >> 8) & 0xFF;
}

static inline uint8_t gt1000_get_parameter_offset(const gt1000_effect_t *base, const gt1000_param_addr_t parameter) {
	return ((uint32_t) parameter - (uint32_t) base) & 0xFF;
}
```

### Demo Video Explanation

For the demonstration video, an [Ampero Control](https://www.hotone.com/products/controlers/Ampero%20Control) was modified. This controller is ideal for modding because its main control module and footswitch module are physically separate. Simply by disconnecting the jumper cable and connecting wires directly, the footswitch module can be utilized.
![ampero1](/documents/image4.png)

While the enclosure is mostly metal, the plastic cover over the front 7-segment display (where the BLE module sits) is not shielded for its bluetooth functionality. Removing the black tape under this plastic cover reveals a perfect cavity for a 128x32 ssd1306 OLED display and custom LED indicators.
![ampero2](documents/image5.png)
In the demo video, the LVGL graphics library is used to render the current preset name on this newly installed OLED display.
