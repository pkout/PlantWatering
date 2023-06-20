# Plant Watering

# Component List

| Component | Model | URL | Notes |
| --- | --- | --- | --- |
| Transistor | 2N2222A | https://www.st.com/resource/en/datasheet/cd00003223.pdf <br/><br/> Tutorial: https://www.onsemi.com/pdf/datasheet/p2n2222a-d.pdf |  |
| Voltage regulator | LM 1117T 3.3V | https://www.ti.com/lit/ds/symlink/lm1117.pdf?HQS=dis-dk-null-digikeymode-dsf-pf-null-wwe&ts=1655102092366 |  |
| Microcontroller | ESP8266 WeMos D1 | Tutorial: https://diyi0t.com/esp8266-wemos-d1-mini-tutorial/ <br/><br/> Wifi Programming: https://www.instructables.com/Programming-a-HTTP-Server-on-ESP-8266-12E/ <br/><br/>Docs: https://www.wemos.cc/en/latest/d1/d1_mini.html | Can supply only 12mA per pin, not enough to run the pump. |
| Submersible water pump | N/A | https://www.amazon.com/d/B09185Y8BN?psc=1&ref=ppx_yo2ov_dt_b_product_details | Runs on 3-5V. Consumes up to 25mA at 3.5V and loaded. At lower voltage, the current decreases. |
| Stronger submersible water pump | N/A | https://www.amazon.com/dp/B01CG2YE6K?psc=1&ref=ppx_yo2ov_dt_b_product_details | USB powered. Consumes up to 130mA at 5V stalled. |

# Useful Links

- Picking the correct base resistor:
    - [https://electronics.stackexchange.com/questions/91307/which-resistor-for-npn-transistor-base#:~:text=on this post.-,Current,this from the data sheets](https://electronics.stackexchange.com/questions/91307/which-resistor-for-npn-transistor-base#:~:text=on%20this%20post.-,Current,this%20from%20the%20data%20sheets).
- Getting date and time from NTP Server with ESP8266 MCU:
    - https://lastminuteengineers.com/esp8266-ntp-server-date-time-tutorial/p
