# NIGHTWAVE HAS BEEN REPLACED BY [MIDNIGHT](https://github.com/RazerFrFr/Midnight) A MORE STABLE EXPERIENCE FOR OG FORTNITE MOBILE!!!!!!!!!!!!

# Nightwave Android

Nightwave Android is a Fortnite Android redirect and SSL bypass tool, designed to work with Fortnite versions between **5.21 – 18ish**. It hooks into network requests and redirects epicgames URLs to custom backends, allowing controlled access or modifications.

# Branches
  - **Android:** Default redirect for 5.21-18.20/30.
  - **Android-UE:** Default redirect for 18.40/Latest.
  - **Android-EOS:** EOS redirect (needed for s22+).

# Offsets for 18.40+
```
18.40
ProcessRequest = 0x95FB1C4
GetUrl = 0x70
SetUrl = 0x95F8108

19.10
ProcessRequest = 0xB98C690
GetUrl = 0x70
SetUrl = 0xB98A238

20.40
ProcessRequest = 0x88C178C
GetUrl = 0x70
SetUrl = 0x88BF314

21.00
ProcessRequest = 0x8A6AE30
GetUrl = 0x70
SetUrl = 0x8A689B8

22.40
ProcessRequest = 0x8FE0F9C
GetUrl = 0x70
SetUrl = 0x8FDEC60

EOS 22.40
ProcessRequest = 0xFA881C
GetUrl = 0x178
SetUrl = 0xFA51D0

23.50
ProcessRequest = 0x6EFF15C
GetUrl = 0x70
SetUrl = 0x72511AC

EOS 23.50
ProcessRequest = 0xF869F4
GetUrl = 0x178
SetUrl = 0xF84A98

24.20
ProcessRequest = 0x72DA320
GetUrl = 0x70
SetUrl = 0x75E7F78

EOS 24.20
ProcessRequest = 0xFB2DFC
GetUrl = 0x178
SetUrl = 0xFAF7B0

26.30
ProcessRequest = 0x7B346F0
GetUrl = 0x78
SetUrl = 0x80F82A8

EOS 26.30
ProcessRequest = 0x104F0B4
GetUrl = 0x178
SetUrl = 0x104BA68

27.11
ProcessRequest = 0x8F3F580
GetUrl = 0x88
SetUrl = 0x93527E4

EOS 27.11
ProcessRequest = 0x1052524
GetUrl = 0x178
SetUrl = 0x104EED8

28.30
ProcessRequest = 0x9914B88
GetUrl = 0x160
SetUrl = 0x9D11EF8

EOS 28.30
ProcessRequest = 0x1105254
GetUrl = 0x178
SetUrl = 0x1101C08
```

# Credits
  - **Andrew/Tiva:** For helping with finding offsets and teaching me how to use ida.
