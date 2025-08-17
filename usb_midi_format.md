# USB-MIDI Message Format

USB-MIDI messages are **4 bytes each**:

| Byte | Description |
|------|-------------|
| 0    | **CIN & Cable Number**: Upper 4 bits = Code Index Number (CIN), Lower 4 bits = Cable Number |
| 1    | **Status Byte**: Upper 4 bits = Message Type, Lower 4 bits = Channel (0–15) |
| 2    | **Data 1**: Note number or CC/Program number |
| 3    | **Data 2**: Velocity or CC value (Program Change does not use this byte) |

---

## Message Types

| MIDI Message | CIN | Status Byte Example | Data 1 | Data 2 |
|--------------|-----|------------------|--------|--------|
| Note Off     | 0x8 | 0x8n             | Note # | Velocity |
| Note On      | 0x9 | 0x9n             | Note # | Velocity |
| Control Change (CC) | 0xB | 0xBn       | CC #   | Value    |
| Program Change (PC) | 0xC | 0xCn       | Program # | -      |

  > n = Channel number (0–15)


### Example: Note On (Channel 1, Cable 0)

| Byte | Value | Description |
|------|-------|-------------|
| 0    | 0x90  | CIN=0x9 (Note On), Cable=0 |
| 1    | 0x90  | Status: Note On, Channel 1 |
| 2    | 60    | MIDI note number (C4) |
| 3    | 100   | Velocity |