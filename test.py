import pigpio
import time

GPIO_PIN = 19  # <-- Hier deine BCM-Pin-Nummer eintragen!
BAUD = 19200

def crc16_blheli(data: bytes) -> bytes:
    crc = 0
    for b in data:
        crc ^= b
        for _ in range(8):
            crc = ((crc >> 1) ^ 0xA001) if (crc & 1) else (crc >> 1)
    return crc.to_bytes(2, "little")

def xfer(pi: pigpio.pi, pin: int, payload: bytes, resp_len: int, timeout: float = 0.08) -> tuple[bytes, int]:
    pi.bb_serial_read(pin)  # RX-Puffer leeren

    pi.wave_clear()
    pi.wave_add_serial(pin, BAUD, payload)
    wid = pi.wave_create()
    pi.wave_send_once(wid)
    while pi.wave_tx_busy():
        pass  # Ohne sleep warten, damit der Pin sofort frei wird
    pi.set_mode(pin, pigpio.INPUT)
    pi.set_pull_up_down(pin, pigpio.PUD_UP)
    pi.wave_delete(wid)

    buf = bytearray()
    deadline = time.monotonic() + timeout
    while time.monotonic() < deadline and len(buf) < len(payload) + resp_len:
        count, data = pi.bb_serial_read(pin)
        if count > 0:
            buf.extend(data)
        else:
            time.sleep(0.001)

    return bytes(buf[len(payload):]), len(buf)

def read_flash(pi: pigpio.pi, pin: int, addr: int, length: int) -> bytes:
    cmd_addr = b"\xFF\x00" + addr.to_bytes(2, "big")
    resp_addr, _ = xfer(pi, pin, cmd_addr + crc16_blheli(cmd_addr), 1, 0.2)
    if resp_addr != b"\x30":
        return b""
    cmd_read = bytes([0x03, length])
    resp, _ = xfer(pi, pin, cmd_read + crc16_blheli(cmd_read), length + 3, 0.3)
    return resp[:length] if len(resp) == length + 3 and resp[-1] == 0x30 else b""

pi = pigpio.pi()
if not pi.connected:
    raise RuntimeError("Keine Verbindung zu pigpiod (läuft 'sudo pigpiod'?)")

try:
    try:
        pi.bb_serial_read_close(GPIO_PIN)
    except pigpio.error:
        pass

    pi.set_mode(GPIO_PIN, pigpio.INPUT)
    pi.set_pull_up_down(GPIO_PIN, pigpio.PUD_UP)
    time.sleep(0.05)

    idle_level = pi.read(GPIO_PIN)
    print(f"Ruhepegel auf GPIO {GPIO_PIN}: {idle_level} (sollte 1 sein)")
    if idle_level == 0:
        print("⚠️ Leitung wird auf LOW gezogen! Evtl. 1kΩ–4.7kΩ Pull-Up von 3.3V (Pin 1) auf Signal nötig.")

    pi.bb_serial_read_open(GPIO_PIN, BAUD, 8)
    magic = b"\x0D" + b"BLHeli"
    init_pkt = (b"\x00" * 12) + magic + crc16_blheli(magic)

    print("Jetzt den LiPo an den ESC anstecken (Skript sucht 15 Sekunden lang)...")
    boot = b""
    last_total_bytes = 0
    deadline = time.monotonic() + 15.0

    while time.monotonic() < deadline:
        resp, total_bytes = xfer(pi, GPIO_PIN, init_pkt, 9, timeout=0.08)
        last_total_bytes = total_bytes
        if len(resp) >= 6:
            boot = resp
            break

    if boot.startswith(b"471"):
        mcu = boot[4:6]
        if mcu[0] == 0xE8:  # Silabs EFM8 -> BLHeli_S / Bluejay
            info = read_flash(pi, GPIO_PIN, 0x1A00, 0x70)
            fw = info[0x60:0x70].decode("ascii", "ignore").strip("\x00 ")
            target = info[0x40:0x50].decode("ascii", "ignore").strip("\x00 ")
            print(f"✅ Firmware: {fw or 'BLHeli_S'} | Version: {info[0]}.{info[1]} | Target: {target}")
        else:  # STM32 / AT32 -> AM32
            info = read_flash(pi, GPIO_PIN, 0x7C00, 16)
            if not info or info[0] not in (0x01, 0x02):
                info = read_flash(pi, GPIO_PIN, 0xF800, 16)
            fw = info[4:16].decode("ascii", "ignore").strip("\x00 ")
            print(f"✅ Firmware: {fw or 'AM32'} | Version: {info[2]}.{info[3]}")
    elif b"BL32" in boot:
        idx = boot.index(b"BL32")
        print(f"✅ Firmware: BLHeli_32 | Version: {boot[idx+4]}.{boot[idx+5]}")
    else:
        print(f"❌ Keine Bootloader-Antwort (Gelesene Gesamtbytes inkl. Echo: {last_total_bytes}, Rest: {boot.hex(' ')})")
finally:
    pi.bb_serial_read_close(GPIO_PIN)
    pi.stop()