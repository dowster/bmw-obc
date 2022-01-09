# Stock On-Board Computer Pinout

List of the pins on the OBC and their purposes, also instructions for hooking up the OBC on a test
bench.

## OBC Connector Pinout (1986)

| Pin | Description                                                 | Notes                                                                                                                                               |
| --: | ----------------------------------------------------------- | --------------------------------------------------------------------------------------------------------------------------------------------------- |
|   1 | Horn trigger to the OBC relay box, might be current sinking |
|   2 | Trigger output                                              | Connects to the chime module                                                                                                                        |
|   4 | Combination Switch                                          | Switch on the end of the turn signal stalk, used to cycle through the OBC's displays.                                                               |
|   5 | Hot in Accy, Run, or Start                                  |
|   6 | KL30 Input                                                  | KL30 is hot at all times                                                                                                                            |
|   7 | Hot in Run / Start                                          | Comes from C5.23 on instrument cluster, whihch comes from Fuse 6 in PDU. Also drives service interval indicator.                                    |
|   8 | J8K?                                                        | Wire points to "Dash Lights" so maybe its for keypad and backlight?                                                                                 |
|   9 | Dash Lights                                                 |
|  11 | Code Relay                                                  | Code relay controls power to the Main relay                                                                                                         |
|  12 | Vehicle Speed Input                                         | Comes from C5.21 on instrument cluster                                                                                                              |
|  14 | Marked as "Not Used"                                        |
|  15 | Fuel Tank Level Input                                       | Instrument cluster provides this directly split from its own input. Sensor connects to cluster on C2.20, and is output from cluster to OBC on c5.4. |
|  16 | Outside Temperature Input                                   | Outside temperature sensor marked as NTC styule.                                                                                                    |
|  17 | Fuel Rate Input                                             |
|  18 | Fuel Tank Low Level Input                                   | Limit swithc triggered when fuel reaches "Low". Comes from C5.9 on the cluster which is directly connected to C2.9, whcih connects to the switch.   |
|  22 | Start Input                                                 | Comes from C5.1 on the instrument cluster, alos connected to the SII. Cluster get sthis from the ignition switch, marked as "Hot In Start"          |

## Wiring for bench setup

| Pin | Connection |
| --- | ---------- |
| 5   | +12v       |
| 6   | +12v       |
| 7   | +12v       |
| 8   | +12v       |
| 9   | GND        |
| 10  | GND        |
