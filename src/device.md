mumo-v2-001

DevEUI : 70B3D57ED005BE9F
DevAddr : 260B3B92
AppsKey : 398F459FE521152FD5B014EA44428AC2
NwkSKey : 680AB79064FD273E52FBBF4FC6349B13


mumo-v2-005

DevEUI : 70B3D57ED005E7D3
DevAddr : 260B161E
AppsKey : 5C8EEE782B1CCACF5B830112AB4ABE08
NwkSKey : C73542F1C559AD24DAEEB49B0B97D7D7

mumo-v2-006

DevEUI : 70B3D57ED005E7DA
DevAddr : 260B9911
AppsKey : E54E4411F4FE8955904197C8D824BC2C
NwkSKey : 8DD6FEB76D7754A78538BA5089A834AA


mumo-v2-007

DevEUI : 70B3D57ED005E7DC
DevAddr : 260B1763
AppsKey : 171708449B2A5F65EC1D50AE33F342F1
NwkSKey : 2DED7CA2EDD5048B57CE9DE08F3A1638


mumo-v2-008

DevEUI : 70B3D57ED005E7F2
DevAddr : 260B9ADD
AppsKey : DEF2560D43B889B44E451FDC904108C1
NwkSKey : E087F74D59963579AD27BACA81CB5461


mumo-v2-009

DevEUI : 70B3D57ED005E7F5
DevAddr : 260B416C
AppsKey : C16A77060CD95C73857D0141F8F05782
NwkSKey : 172D898DCA3F527443C6B6FB48A6FF7A


mumo-v2-test-endpoint

DevEUI : 70B3D57ED005E878
DevAddr : 260BB8CA
AppsKey : 18217B0F9765621CE45346344364AB99
NwkSKey : B4ABBF24AD9B0427862E6F91762CD096


-*-*-*-*-*-*-*-*-*-*-*-*-*-*-

mumo-v2-001 : enterprise application

DevEUI : 70B3D57ED800159D
DevAddr : 27FE980D
AppsKey : 1422E5991BF660580299A7E32FFE13E5
NwkSKey : 5CB6F799266E0284ABC49539011807FE






SF9 for Rx2

*************************************************
APP_VERSION:        V1.1.0
MW_LORAWAN_VERSION: V2.3.0
MW_RADIO_VERSION:   V1.1.0

WR [0923]:02
0s007:Set Standby : C [80]:00
0s010:Set TCXO Mode : C [97]:01:00:00:40
WR [0911]:00
0s010:Calibrate : C [89]:7F

0s015:Set PacketType : C [8A]:01
WR [0740]:14 ?? wrong syncword
WR [0741]:24

0s015:Set RegulatorMode : C [96]:01
0s015:Set Buffer Base Address : C [8F]:00:00
0s016:Set PA Config : C [95]:04:00:01:01
0s016:Set REG_OCP : WR [08E7]:18
0s016:Set TX Parameters : C [8E]:00:04
C [08]:FF:FF:FF:FF:00:00:00:00
WR [0923]:02
0s017:Set Sleep : C [84]:04
###### OTAA ######
###### AppKey:      39:8F:45:9F:E5:21:15:2F:D5:B0:14:EA:44:42:8A:C2
###### NwkKey:      68:0A:B7:90:64:FD:27:3E:52:FB:BF:4F:C6:34:9B:13
###### ABP  ######
###### AppSKey:     39:8F:45:9F:E5:21:15:2F:D5:B0:14:EA:44:42:8A:C2
###### NwkSKey:     68:0A:B7:90:64:FD:27:3E:52:FB:BF:4F:C6:34:9B:13
0s020:Set PacketType : C [8A]:01
C [08]:00:00:00:00:00:00:00:00
WR [08E2]:0C
WR [08E5]:20
0s024:Start Rx : C [82]:FF:FF:FF
0s025:Set Standby : C [80]:00
WR [08E2]:0C
WR [08E5]:20
0s025:Set PacketType : C [8A]:01
WR [0740]:34
WR [0741]:44
WR [0923]:02
0s026:Set Sleep : C [84]:04
###### DevEui:  70:B3:D5:7E:D0:05:BE:9F


***********************************************************

0s028:Set PacketType : 180s034:temp= 16
180s035:VDDA= 254

180s035:Calibrate Image : C [98]:D7:DB
180s041:Set RF Frequency : C [86]:36:44:CC:CC
180s041:Set Standby : C [80]:00
180s041:Set PacketType : C [8A]:01
180s041:Set Modulation Parameters : C [8B]:07:04:01:00
180s041:Set Packet Parameters : C [8C]:00:08:00:FF:01:00
WR [08D8]:1E : Better Resistance of the SX1262 Tx to Antenna Mismatch
180s042:Set PA Config : C [95]:04:07:00:01
180s042:Set REG_OCP : WR [08E7]:38
180s042:Set TX Parameters : C [8E]:0D:02
180s042:TX on freq 868300000 Hz at DR 5
180s042:Set Packet Parameters : C [8C]:00:08:00:1D:01:00
C [08]:02:01:02:01:00:00:00:00 : Set DIOIrq Parameters
180s044:ConfigRFSwitch : [03]
WR [0889]:24 : Section 15 : Modulation Quality
180s044:Set Packet Parameters : C [8C]:00:08:00:1D:01:00 : Payload Length = 1D = 29
WB [00]:40:92:3B:0B:26:00:01:00:18:28:45:D7:FC:0F:78:0A:6E:51:13:5E:62:7A:27:D5:D2:B9:47:13:D6
180s045:Sart Tx : C [83]:00:00:00 : Set Tx - no timeout
180s047:SEND REQUEST
180s114:Set Standby : C [80]:00
180s114:MAC txDone

****************************************************

WR [0923]:02 : Sub-GHz radio SMPS control 2 register : 0x2: 60 mA
180s115:Set Sleep : C [84]:04
181s094:Set Standby : C [80]:00
181s097:Set RF Frequency : C [86]:36:44:CC:CC
C [9F]:00 : StopTimerOnPreamble
181s098:Set Standby : C [80]:00
181s098:Set PacketType : C [8A]:01
181s098:Set Modulation Parameters : C [8B]:07:04:01:00
181s098:Set Packet Parameters : C [8C]:00:08:00:FF:00:01
C [A0]:2C : SetLoRaSymbNumTimeout
WR [0736]:09 : IQ Polarity Setup 0x0736 0x0D Optimize the inverted IQ operation
181s099:Set Packet Parameters : C [8C]:00:08:00:FF:00:01
181s099:RX_1 on freq 868300000 Hz at DR 5
C [08]:02:62:02:62:00:00:00:00 : Set DIOIrq Parameters
181s099:ConfigRFSwitch : [01]
181s099:Start Rx : C [82]:3F:FF:C0
181s146:IRQ_RX_TX_TIMEOUT
181s146:Set Standby : C [80]:00
181s146:MAC rxTimeOut
WR [0923]:02
181s146:Set Sleep : C [84]:04
182s145:Set Standby : C [80]:00
182s148:Set RF Frequency : C [86]:36:58:66:66
C [9F]:00
182s149:Set Standby : C [80]:00
182s149:Set PacketType : C [8A]:01
182s149:Set Modulation Parameters : C [8B]:0C:04:01:01
182s149:Set Packet Parameters : C [8C]:00:08:00:FF:00:01
C [A0]:06
WR [0736]:09 : Optimizing the Inverted IQ Operation
182s150:Set Packet Parameters : C [8C]:00:08:00:40:00:01
182s150:RX_2 on freq 869525000 Hz at DR 0
C [08]:02:62:02:62:00:00:00:00
182s150:ConfigRFSwitch : [01]
182s150:Start Rx : C [82]:3F:FF:C0
182s348:IRQ_RX_TX_TIMEOUT
182s348:Set Standby : C [80]:00
182s348:MAC rxTimeOut
WR [0923]:02
182s349:Set Sleep : C [84]:04

###### ========== MCPS-Confirm =============
###### U/L FRAME:0001 | PORT:24 | DR:5 | PWR:0 | MSG TYPE:UNCONFIRMED
APP_VERSION:        V1.1.0
MW_LORAWAN_VERSION: V2.3.0
MW_RADIO_VERSION:   V1.1.0
WR [0923]:02
0s007:Set Standby : C [80]:00
0s010:Set TCXO Mode : C [97]:01:00:00:40
WR [0911]:00
0s010:Calibrate : C [89]:7F
0s015:Set PacketType : C [8A]:01
WR [0740]:14
WR [0741]:24
0s015:Set RegulatorMode : C [96]:01
0s015:Set Buffer Base Address : C [8F]:00:00
0s016:Set PA Config : C [95]:04:00:01:01
0s016:Set REG_OCP : WR [08E7]:18
0s016:Set TX Parameters : C [8E]:00:04
C [08]:FF:FF:FF:FF:00:00:00:00
WR [0923]:02
0s017:Set Sleep : C [84]:04
###### OTAA ######
###### AppKey:      39:8F:45:9F:E5:21:15:2F:D5:B0:14:EA:44:42:8A:C2
###### NwkKey:      68:0A:B7:90:64:FD:27:3E:52:FB:BF:4F:C6:34:9B:13
###### ABP  ######
###### AppSKey:     39:8F:45:9F:E5:21:15:2F:D5:B0:14:EA:44:42:8A:C2
###### NwkSKey:     68:0A:B7:90:64:FD:27:3E:52:FB:BF:4F:C6:34:9B:13
0s020:Set PacketType : C [8A]:01
C [08]:00:00:00:00:00:00:00:00
WR [08E2]:0C
WR [08E5]:20
0s024:Start Rx : C [82]:FF:FF:FF
0s025:Set Standby : C [80]:00
WR [08E2]:0C
WR [08E5]:20
0s025:Set PacketType : C [8A]:01
WR [0740]:34
WR [0741]:44
WR [0923]:02
0s026:Set Sleep : C [84]:04
###### DevEui:  70:B3:D5:7E:D0:05:BE:9F
0s028:Set PacketType : 180s034:temp= 16

180s035:VDDA= 254
180s035:Calibrate Image : C [98]:D7:DB
180s041:Set RF Frequency : C [86]:36:44:CC:CC
180s041:Set Standby : C [80]:00
180s041:Set PacketType : C [8A]:01
180s041:Set Modulation Parameters : C [8B]:07:04:01:00
180s041:Set Packet Parameters : C [8C]:00:08:00:FF:01:00
WR [08D8]:1E
180s042:Set PA Config : C [95]:04:07:00:01
180s042:Set REG_OCP : WR [08E7]:38
180s042:Set TX Parameters : C [8E]:0D:02
180s042:TX on freq 868300000 Hz at DR 5
180s042:Set Packet Parameters : C [8C]:00:08:00:1D:01:00
C [08]:02:01:02:01:00:00:00:00
180s044:ConfigRFSwitch : [03]
WR [0889]:24
180s044:Set Packet Parameters : C [8C]:00:08:00:1D:01:00
WB [00]:40:92:3B:0B:26:00:01:00:18:28:45:D7:FC:0F:78:0A:6E:51:13:5E:62:7A:27:D5:D2:B9:47:13:D6
180s045:Sart Tx : C [83]:00:00:00
180s047:SEND REQUEST
180s114:Set Standby : C [80]:00
180s114:MAC txDone
WR [0923]:02
180s115:Set Sleep : C [84]:04
181s094:Set Standby : C [80]:00
181s097:Set RF Frequency : C [86]:36:44:CC:CC
C [9F]:00
181s098:Set Standby : C [80]:00
181s098:Set PacketType : C [8A]:01
181s098:Set Modulation Parameters : C [8B]:07:04:01:00
181s098:Set Packet Parameters : C [8C]:00:08:00:FF:00:01
C [A0]:2C
WR [0736]:09
181s099:Set Packet Parameters : C [8C]:00:08:00:FF:00:01
181s099:RX_1 on freq 868300000 Hz at DR 5
C [08]:02:62:02:62:00:00:00:00
181s099:ConfigRFSwitch : [01]
181s099:Start Rx : C [82]:3F:FF:C0
181s146:IRQ_RX_TX_TIMEOUT
181s146:Set Standby : C [80]:00
181s146:MAC rxTimeOut
WR [0923]:02
181s146:Set Sleep : C [84]:04
182s145:Set Standby : C [80]:00
182s148:Set RF Frequency : C [86]:36:58:66:66
C [9F]:00
182s149:Set Standby : C [80]:00
182s149:Set PacketType : C [8A]:01
182s149:Set Modulation Parameters : C [8B]:0C:04:01:01
182s149:Set Packet Parameters : C [8C]:00:08:00:FF:00:01
C [A0]:06
WR [0736]:09
182s150:Set Packet Parameters : C [8C]:00:08:00:40:00:01
182s150:RX_2 on freq 869525000 Hz at DR 0
C [08]:02:62:02:62:00:00:00:00
182s150:ConfigRFSwitch : [01]
182s150:Start Rx : C [82]:3F:FF:C0
182s348:IRQ_RX_TX_TIMEOUT
182s348:Set Standby : C [80]:00
182s348:MAC rxTimeOut
WR [0923]:02
182s349:Set Sleep : C [84]:04

###### ========== MCPS-Confirm =============
###### U/L FRAME:0001 | PORT:24 | DR:5 | PWR:0 | MSG TYPE:UNCONFIRMED
