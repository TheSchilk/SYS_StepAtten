# SYS_StepAtten


Design files for Connector Riser board:
https://github.com/TheSchilk/3.5mm_RiserPCB/releases/tag/v0.0

## UI Modes/Switches

The SYS is normally fitted with a toggle switch (ALPS SPUN191400), but
a momentary version of that switch also exists (ALPS SPUN190900).

Both can be used, and there are different UI modes that should support
either switch. The UI mode is selected using the small config jumpers
located inside the unit right next to the switch.

|      *Mode*     | *CFG1 Setting* | *CFG2 Setting* |
|:---------------:|:--------------:|:--------------:|
|      Cycle      |        0       |        0       |
| Cycle on toggle |        1       |        0       |
|      Select     |        0       |        1       | 
I personally prefer using the momentary switch (ALPS SPUN190900) in the
*Cycle* mode.

### Mode: Cycle
In this mode the unit cycles through the different inputs everytime
the switch is depressed. This works well with the momentary switch.
The unit will mute for around 1 second when changing inputs.

### Modei: Cycle on toggle
In this mode the unit cycles through the different inputs everytime
the switch changes position. This works well with the toggle switch.
The unit will mute for around 1 second when changing inputs.

### Mode: Select
In this mode the input is selected using the potentiometer while the button
is pressed in. The lamp will change color while the potentiometer is turned
to indicate which input is currently selected. Once the switch is returned
to the forward position, the currently selected input will be used. To prevent
volume spikes the unit will not unmute until the potentiometer is returned
to 0 volume. This is indicated by a flashing LED.
