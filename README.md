# SYS_StepAtten

## Switch Selection

The SYS is normally fitted with a toggle switch (ALPS SPUN191400), but
a momentary version of that switch also exists (ALPS SPUN190900).

The momentary switch is probably the better choice, but just in case
one wants to harvest as many parts as possible from an original SYS,
both are supported by the firmware.

The firmware supports both kinds of switches, and the internal jumper
CFG1 is used to indicate which kind is installed.

|           *Switch*          | *CFG1 Setting* |
|:---------------------------:|:--------------:|
| Momentary (ALPS SPUN190900) |        0       |
|   Toggle (ALPS SPUN191400)  |        1       |

Note that this setting is only updated during powerup.
